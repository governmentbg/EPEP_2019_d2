#include "cac.h"

#define write32(s, value, len) write3x((s), (value), (len), 32)

void subpoena_legal_name(char *name, char *tRequestId, const TRCDKey *key, char status, long requestId, const char *subdir)
{
	mstr m;

	// TODO: store requestId bit 31 in type if needed
	*tRequestId = '\0';
	write32(tRequestId, COURT_CODE, 2);
	strcach(tRequestId, key->type);
	write32(tRequestId, key->no, 4);
	scprintf(tRequestId, "%02ld", key->year - YEAR_MIN);
	write32(tRequestId, requestId, 6);

	if (subdir)
		m.printf("%s\\", subdir);
	m.printf("%c%s.xml", status, tRequestId);
	build_fn(XFER_LEGAL_AID_PATH, str(m), name);
}

void legalaid_rename(TSubpoena *subpoena, char oldStatus, char newStatus)
{
	char requestId[SIZE_OF_LEGAL_AID_ID];
	char name[SIZE_OF_PATH];

	subpoena_legal_name(name, requestId, &subpoena->key, oldStatus, subpoena->requestId);

	if (exist(name))
	{
		char tName[SIZE_OF_PATH];
		mstr m;
		TDummyContainer container;

		subpoena_legal_name(tName, requestId, &subpoena->key, newStatus, subpoena->requestId);
		container << subpoena;

		if (MoveFileEx(name, tName, MOVEFILE_REPLACE_EXISTING))
		{
			m.printf("legalaid: %s", basename(tName));
			log(LOG_TRANSFER, 0, &container, str(m));
		}
		else
		{
			DWORD lastError = GETLASTERROR;

			error("%s: грешка %lu при преименуване.", tName, lastError);
			m.printf("legalaid: грешка %lu при преименуване", lastError);
			log(LOG_TRANSFER, 0, &container, str(m));
		}
	}
}

void create_lawer_college(TFirm *firm)
{
	msql m;

	Court->Seek(COURT_AREA);
	sprintf(firm->name, "АДВОКАТСКА КОЛЕГИЯ %s", UCLP->Seek(Court->uclp));
	firm->ucnType = UCN_FIRM_CODE;
	m.Begin(firm->Name);
	m.Add(firm, "F_NAME, F_UCN_TYPE");

	TQuery q(str(m));

	if (q.Read())
		*firm << q;
	else
		firm->Insert();
}

const char *legal_aid_status(TSubpoena *subpoena)
{
	enum { LAID_STATUSES = 3 };
	constant subdirs[LAID_STATUSES] = { NULL, "sent", "received" };
	static constant statuses[LAID_STATUSES] = { "в процес на изпращане към АК", "Изпратено към АК", "Обработено от АК" };

	for (int i = 0; i < LAID_STATUSES; i++)
	{
		char requestId[SIZE_OF_LEGAL_AID_ID];
		char name[SIZE_OF_PATH];

		subpoena_legal_name(name, requestId, &subpoena->key, LEGAL_AID_KILLED, subpoena->requestId, subdirs[i]);

		if (exist(name))
			return NULL;

		subpoena_legal_name(name, requestId, &subpoena->key, LEGAL_AID_QUEUED, subpoena->requestId, subdirs[i]);

		if (exist(name))
			return statuses[i];
	}

	return NULL;
}

// ----- TAppointedLawer -----------------------------------------------------
class TAppointedLawer : public TCitizen
{
public:
	TAppointedLawer() { }

	char docType[SIZE_OF_EXT];
	TGArray<TSubpoena> litigants;
};

class TLegalResponse : public TGroup, public TUserFile
{
public:
	TLegalResponse();

	virtual void Print(mstr &m);
	void Read(const char *tName);

	TRCDKey key;
	char requestId[SIZE_OF_LEGAL_AID_ID];
	long subpoenaKind;
	char involvement;
	TGArray<TAppointedLawer> lawers;

protected:
	bool Try(const char *elem, char *output, int minLength, int bufferSize);
	void Get(const char *elem, char *output, int minLength, int bufferSize);

	virtual bool Try(const char *fields) { return TGroup::Try(fields); }
	virtual bool Get(const char *fields) { return TGroup::Get(fields); }

	char buffer[0x1000];
	bool empty;
};

TLegalResponse::TLegalResponse()
	: TGroup("T_LEGAL_AID", NULL), TUserFile("ini"), empty(true)
{
}

#define read32(s, len) read3x((s), (len), 32, true)

void TLegalResponse::Read(const char *name)
{
	char tName[SIZE_OF_PATH];

	build_fn(XFER_LEGAL_AID_PATH, "received", tName);
	build_fn(tName, name, tName);
	f = Open(tName, "r");

	if (!f)
		failure();

	long tRequestId;
	char instance[SIZE_OF_COURT_CODE];

	Get("RequestID", requestId, LENGTH_OF_LEGAL_AID_ID, sizeof requestId);
	if (strncmpi(name + 1, requestId, LENGTH_OF_LEGAL_AID_ID))
		fatal("%s: грешен RequestID.", name);

	// TODO: read tRequestId bit 31 from type if needed
	key.type = requestId[2];
	key.no = read32(requestId + 3, 4);
	sscanf(requestId + 7, "%2ld", &key.year);
	tRequestId = read32(requestId + 9, 6);
	key.year += YEAR_MIN;
	Get("InstanceIdentifier", instance, 1, sizeof instance);

	if (atoi(instance) != COURT_CODE)
		fatal("%s: грешен код на съда.", name);

	// 2014:184 LRQ: FIX: check access
	mstr cause;		// to suppress messages
	TRCDKeyContainer *const container = can_access_basic(key, true, false, &cause);

	if (!container)
		failure();

	TVirtPtr VP(container);
	TSubpoena subpoena;
	msql m(subpoena.Name, "F_SUBPOENA_KIND");

	subpoena.key = key;
	subpoena.requestId = tRequestId;
	m.Add(&subpoena, "F_NO, F_YEAR, F_TYPE, F_REQUEST_ID");

	TQuery q(str(m));
	if (!q.Read())
		fatal("%s: няма такава заявка.", name);

	subpoena << q;
	subpoenaKind = subpoena.subpoenaKind;
	involvement = subpoenaKind == SUBPOENA_KIND_LEGAL_AID_TYPED ? INVOLVEMENT_LAWER : INVOLVEMENT_SPECPRESENT;

	char lawerName[SIZE_OF_LAWER_NAME];

	while (Try("LawyerName", lawerName, 3, sizeof lawerName))
	{
		TAppointedLawer *lawer = new TAppointedLawer();
		char nprrNo[SIZE_OF_LAWER_NRPP_NO];
		char litigantId[SIZE_OF_LITIGANT_ID];
		TKnownPerson known;

		Get("PersonalNumber", known.personalNo, LENGTH_OF_LAWER_PERSONAL_NO, sizeof known.personalNo);
		Get("NRPPNumber", nprrNo, 1, sizeof nprrNo);

		m.Begin(known.Name);
		known.involvement = INVOLVEMENT_LEGALESE;
		m.Add(&known, "F_INVOLVEMENT, F_PERSONAL_NO");
		TQuery q(str(m));

		if (q.Read())
		{
			known << q;
			*lawer << known;
			if (!lawer->Get())
				failure();
		}
		else
		{
			strimall(lawerName);
			compress(lawerName);
			strupr(lawerName);	// 2014:315 LPR: FIX

			char *reName = "", *reFamily = "", *s;
			char *family = strchr(lawerName, ' ');

			if (!family)
				fatal("%s: %s: невалидни имена на адвокат", name, lawerName);

			*family++ = '\0';

			if ((s = strchr(family, ' ')) != NULL)
			{
				reName = family;
				family = s;
				*family++ = '\0';
			}

			if ((s = strchr(family, ' ')) != NULL || (s = strchr(family, '-')) != NULL)
			{
				if (s == family)
					fatal("%s: %s: невалидна фамилия на адвокат", name, family);

				reFamily = s;
				*reFamily++ = '\0';
			}

			strzcpy(lawer->name, lawerName, SIZE_OF_CITIZEN_NAME);
			strzcpy(lawer->reName, reName, SIZE_OF_CITIZEN_RENAME);
			strzcpy(lawer->family, family, SIZE_OF_CITIZEN_FAMILY);
			strzcpy(lawer->reFamily, reFamily, SIZE_OF_CITIZEN_REFAMILY);

			// 2014:316 try to find by name 1:1
			bool found = false;

			if (any(reName))
			{
				m.Begin("T_KNOWN_PERSON K JOIN T_CITIZEN C ON C.F_UCN = K.F_UCN AND C.F_UCN_TYPE = K.F_UCN_TYPE");
				m.Add(&known, "K.F_INVOLVEMENT");
				// reFamily must be NULL if missing
				// 2015:008 FIX: don't Add . fields
				//m.Add(lawer, "C.F_NAME, C.F_FAMILY, C.F_RENAME, C.F_REFAMILY");
				m.AddString("C.F_NAME", lawer->name);
				m.AddString("C.F_FAMILY", lawer->family);
				m.AddString("C.F_RENAME", lawer->reName);
				m.AddString("C.F_REFAMILY", lawer->reFamily);

				TQuery p(str(m));
				if (p.Read())
				{
					if (p.Read())
						break;

					*lawer << p;
					known << lawer;
					known.Update("F_PERSONAL_NO");
					found = true;
				}
			}

			if (!found)
			{
				lawer->ucnType = UCN_CITIZEN_CODE;
				*lawer->ucn = '\0';
				if (!lawer->Insert())
					failure();

				known << lawer;
				known.involvement = INVOLVEMENT_LEGALESE;
				if (!known.Insert())
					failure();
			}
		}

		while (Try("LitigantID", litigantId, LENGTH_OF_UCN + 1, sizeof litigantId))
		{
			strcpy(subpoena.ucn, litigantId + 1);
			subpoena.ucnType = *litigantId;

			m.Begin(subpoena.Name);
			m.Add(&subpoena, "F_NO, F_YEAR, F_TYPE, F_REQUEST_ID, F_UCN, F_UCN_TYPE");
			TQuery q(str(m));

			if (q.Read())
			{
				TSubpoena *tSubpoena = new TSubpoena;
				*tSubpoena << q;
				lawer->litigants.Add(tSubpoena);
			}
			else
				fatal("%s: %s: няма такава заявка.", name, litigantId);
		}

		Get("Type", lawer->docType, LENGTH_OF_LEGAL_AID_TYPE, SIZE_OF_LEGAL_AID_TYPE);
		Get("UrlFile", NULL, 1, SIZE_OF_LEGAL_AID_URL);
		Get("UrlPreview", NULL, 1, SIZE_OF_LEGAL_AID_URL);
		lawers.Add(lawer);
	}

	if (!empty)
		fatal("%s: %s: очаква се елемент LawyerName или край на файла.", name, buffer);

	Close();
}

bool TLegalResponse::Try(const char *elem, char *output, int minLength, int bufferSize)
{
	if (empty)
	{
		if (!fgets(buffer, sizeof buffer, f))
			return false;

		if (!*buffer || buffer[strlen(buffer) - 1] != '\n')
			fatal("%s: %s: невалиден ред.");

		buffer[strlen(buffer) - 1] = '\0';
	}

	char *s;

	if ((s = strstr(buffer, " = ")) == NULL)
		fatal("%s: %s: невалиден формат.", name, buffer);

	if (s - buffer != strlen(elem) || strncmp(elem, buffer, s - buffer))
		return empty = false;

	s += 3;

	if (strlen(s) < minLength || strlen(s) >= bufferSize)
		fatal("%s: %s: невалидна дължина на %s.", name, buffer, elem);

	if (output)
		strcpy(output, s);

	return empty = true;
}

void TLegalResponse::Get(const char *elem, char *output, int minLength, int bufferSize)
{
	if (!Try(elem, output, minLength, bufferSize))
		fatal("%s: %s: очаква се елемент %s.", name, empty ? "край на файла" : buffer, elem);
}

void TLegalResponse::Print(mstr &m)
{
	m.printf("%s %ld/%ld - ", Type->Seek(key.type), key.no, key.year);

	for (int i = 0; i < lawers.Count(); i++)
	{
		if (i)
			m.cat(", ");

		m.printf("%s ", Involvement->Seek(involvement));

		size_t len = m.len();

		lawers[i]->Print(m);
		mbk_remove(&m, len, SIZE_OF_UCN_TYPE_UCN);
	}
}

static bool legalaid_import(TWindow *, TLegalResponse *response)
{
	TSideWind sideWind;
	msql m;
	char name[SIZE_OF_PATH];
	bool imported = false;
	mstr surText;

	for (int index = 0; index < response->lawers.Count(); index++)
	{
		TAppointedLawer *tLawer = response->lawers[index];
		bool surrounded = false;

		for (int n = 0; n < tLawer->litigants.Count(); n++)
		{
			TSubpoena *tLitigant = tLawer->litigants[n];

			m.Begin(sideWind.Name);		// was tLitigant->Name == T_SUBPOENA
			m.Add(tLitigant, "F_NO, F_YEAR, F_TYPE, F_UCN, F_UCN_TYPE, F_INVOLVEMENT");
			m.Add("F_DATE");
			m.printf(" >= %ld", tLitigant->date.Value());

			TQuery q(str(m));
			while (q.Read())
			{
				sideWind << q;
				sideWind.ucnType = tLawer->ucnType;
				strcpy(sideWind.ucn, tLawer->ucn);
				sideWind.involvement = response->involvement;

				// 2014:057 FIX: we may receive a lawer 2+ times
				//if (sideWind.Try() || !sideWind.Insert())
				if (!sideWind.Try() && !sideWind.Insert())
					continue;

				TLinkWind linkWind;

				linkWind << q;
				strcpy(linkWind.proxyUcn, sideWind.ucn);
				linkWind.proxyUcnType = sideWind.ucnType;
				linkWind.proxyInvolvement = response->involvement;
				linkWind.proxyKind = SIDE_MARK_LIKE_LAWYER;
				linkWind.Insert();

				if (surrounded)
					continue;

				TSurround surround;

				surround.kind = KIND_ANOTHER_SURROUND;
				surround.date = Today;
				surround.college = response->key.type;
				surround.lawsuit = response->key;
				strcpy(surround.text, "Уведомително писмо правна помощ");

				if (!surround.Insert())
					continue;

				surText.sep(", ");
				surText.printf("%ld/%ld", surround.key.no, surround.key.year);

				sprintf(name, "%s\\received\\%c%s_%03d.%s", XFER_LEGAL_AID_PATH, LEGAL_AID_QUEUED,
					response->requestId, index, tLawer->docType);

				if (exist(name))
				{
					char tName[SIZE_OF_PATH];

					electric_name(&surround, tName, TYPE_IMAGE);
					strcpy(file_ext(tName), tLawer->docType);
					copy_overead_file(name, tName, false, true);
				}

				TFirm firm;
				TSideWind sideWind;

				create_lawer_college(&firm);
				sideWind << surround;
				strcpy(sideWind.ucn, firm.ucn);
				sideWind.ucnType = firm.ucnType;
				sideWind.involvement = INVOLVEMENT_SENDER;
				sideWind.Insert();

				surrounded = true;
				imported = true;
			}
		}
	}

	if (imported)
	{
		sprintf(name, "%s\\received\\%c%s.ini", XFER_LEGAL_AID_PATH, LEGAL_AID_QUEUED, response->requestId);
		remove_file(name, "текстов файл от обмена със \"Служебни защити\".");

		m.clear();
		m.printf("legalaid: %s.ini", response->requestId);
		log(LOG_IMPORT, 0, &response->key, str(m));
		message("Данните са извлечени, съпровождащи документи: %s.", str(surText));
		return true;
	}

	return error("Грешка при извличането на данните.");
}

static void legalaid_view(TWindow *, TLegalResponse *response)
{
	for (int index = 0; index < response->lawers.Count(); index++)
	{
		TAppointedLawer *tLawer = response->lawers[index];
		char name[SIZE_OF_PATH];

		sprintf(name, "%s\\received\\%c%s_%03d.%s", XFER_LEGAL_AID_PATH, LEGAL_AID_QUEUED, response->requestId, index,
			tLawer->docType);

		if (exist(name))
			electric_action(name, TYPE_IMAGE, ACTION_VIEW);
	}
}

#include "LegalResponseListWindow.h"
#include "LegalResponseListWindow.cpp"

#include "LegalResponseDialog.h"
#include "LegalResponseDialog.cpp"

void legalaid_import(TWindow *parent)
{
	char name[SIZE_OF_TWICE_BASE];
	char tName[SIZE_OF_PATH];
	WIN32_FIND_DATA find;
	HANDLE hFind;
	TGArray<TLegalResponse> responses;

	strcpy(name, "received\\q");
	write32(name, COURT_CODE, 2);
	strcat(name, "?????????????.ini");
	build_fn(XFER_LEGAL_AID_PATH, name, tName);

	if ((hFind = FindFirstFile(tName, &find)) != INVALID_HANDLE_VALUE)
	{
		do
		{
			TLegalResponse *response = new TLegalResponse();

			try
			{
				response->Read(find.cFileName);
				responses.Add(response);
			}
			catch (...)
			{
				delete response;
			}
		} while (FindNextFile(hFind, &find));
	}

	unsigned long dwError = GETLASTERROR;

	if (dwError != ERROR_FILE_NOT_FOUND && dwError != ERROR_NO_MORE_FILES && dwError != ERROR_PATH_NOT_FOUND)
		error("%s: грешка %lu при търсене на файлове.", tName, GETLASTERROR);
	if (hFind != INVALID_HANDLE_VALUE)
		FindClose(hFind);

	if (responses.Count())
		TLegalResponseDialog(parent, &responses).Execute();
	else
		error("Няма намерени данни за импорт.", responses.Count());
}
