#if COURT_TYPE == COURT_ADMIN
// ----- CSV const -----------------------------------------------------------
#define ADMC_YEAR_MIN 2007

static const CDate ADMC_DATE_MIN(1, 1, ADMC_YEAR_MIN);
static const CDate ADMC_DATE_MAX(31, 3, 2017);

// ----- TVolUCLP ------------------------------------------------------------
class TVolUCLP : public TUCLP
{
public:
	virtual bool Try(const char *fields = NULL);

private:
	static const long internals[];
};

bool TVolUCLP::Try(const char *fields)
{
	for (int i = 0; internals[i]; i++)
	{
		if (internals[i] == uclp)
		{
			area = 21;
			region = 46;
			uclp = 68134;
			strcpy(name, "ГР.СОФИЯ");
			return true;
		}
	}

	return TUCLP::Try(fields);
}

const long TVolUCLP::internals[] = { 44912, 49669, 54571, 70832, 70857, 81949, 41201, 72000, 0 };

TVolUCLP *VCLP = NULL;
//TUCLP *VCLP;

// ----- TExportedPerson -----------------------------------------------------
struct TExportedPerson
{
	TExportedPerson(char tUCNType, const char *tUCN);

	char ucnType;
	char ucn[SIZE_OF_UCN];
	long addresses;		// non-comparable

	bool operator<=(const TExportedPerson &that);
};

TExportedPerson::TExportedPerson(char tUCNType, const char *tUCN)
	: ucnType(tUCNType), addresses(0)
{
	strcpy(ucn, tUCN);
}

bool TExportedPerson::operator<=(const TExportedPerson &that)
{
	return ucnType != that.ucnType ? ucnType < that.ucnType : strcmp(ucn, that.ucn) <= 0;
}

// ----- TUpdateCSV ----------------------------------------------------------
class TUpdateCSV : public TUpdate
{
public:
	TUpdateCSV(TWindow *parent) : TUpdate(parent, CM_ADMIN_EXPORT_CSV, "данни към CSV", "Експорт", WAIT_MORE, "Грешки") { }

	void eprintf(const char *format, ...);
	virtual bool HumanReadable() = 0;
	virtual bool ShowWarnings() = 0;
};

void TUpdateCSV::eprintf(const char *format, ...)
{
	bool error = *format != '(';

	if (error || ShowWarnings())
	{
		va_list ap;

		va_start(ap, format);
		vdprintf(format, ap);
		va_end(ap);
		dprintf(")\n" + error);
	}

	wrong += error;
}

// ----- TExportedAddress ----------------------------------------------------
struct TExportedAddress
{
	TExportedAddress(TUpdateCSV *update, const TSubpoena &subpoena);

	char ucnType;
	char ucn[SIZE_OF_UCN];
	long address;		// non-comparable
	long uclp;
	char streetName[SIZE_OF_STREET_NAME];
	char adrNo[LENGTH_OF_ADR_NO + SIZE_OF_SUB_NO];
	char blockNo[SIZE_OF_BLOCK_NO];
	char entrance[SIZE_OF_ENTRANCE];
	char floor[SIZE_OF_FLOOR];
	char apartment[SIZE_OF_APARTMENT];
	char foreign[SIZE_OF_FOREIGN_ADDRESS];
	char remark[SIZE_OF_ADDRESS_REMARK];

	bool operator<=(const TExportedAddress &that);
};

TExportedAddress::TExportedAddress(TUpdateCSV *update, const TSubpoena &subpoena)
	: ucnType(subpoena.ucnType), uclp(subpoena.uclp)
{
	strcpy(ucn, subpoena.ucn);
	*streetName = '\0';

	if (uclp && subpoena.street)
	{
		Street->uclp = uclp;
		Street->street = subpoena.street;

		if (Street->Try())
			strcpy(streetName, Street->name);
		else
			update->eprintf("%ld/%ld: липсваща улица", uclp, subpoena.street);
	}

	sprintf(adrNo, "%s%s", subpoena.adrNo, subpoena.subNo);
	strcpy(blockNo, subpoena.blockNo);
	strcpy(entrance, subpoena.entrance);
	strcpy(floor, subpoena.floor);
	strcpy(apartment, subpoena.apartment);
	strcpy(foreign, subpoena.foreign);
	strcpy(remark, subpoena.remark);
}

bool TExportedAddress::operator<=(const TExportedAddress &that)
{
	return ucnType != that.ucnType ? ucnType < that.ucnType :
		strcmp(ucn, that.ucn) ? strcmp(ucn, that.ucn) < 0 :
		uclp != that.uclp ? uclp < that.uclp :
		strcmp(streetName, that.streetName) ? strcmp(streetName, that.streetName) < 0 :
		strcmp(adrNo, that.adrNo) ? strcmp(adrNo, that.adrNo) < 0 :
		strcmp(blockNo, that.blockNo) ? strcmp(blockNo, that.blockNo) < 0 :
		strcmp(entrance, that.entrance) ? strcmp(entrance, that.entrance) < 0 :
		strcmp(floor, that.floor) ? strcmp(floor, that.floor) < 0 :
		strcmp(apartment, that.apartment) ? strcmp(apartment, that.apartment) < 0 :
		strcmp(foreign, that.foreign) ? strcmp(foreign, that.foreign) < 0 :
		strcmp(remark, that.remark) <= 0;
}

// ----- csv_uuid ------------------------------------------------------------
class csv_uuid
{
public:
	csv_uuid(const TRCDKeyContainer &container, int special = 0);
	csv_uuid(const char *ucn, char ucnType);
	csv_uuid(const TSubpoena &subpoena);
	csv_uuid(const TUCNGroup *ucnGroup);
	csv_uuid(const TExportedPerson *person);
	csv_uuid(const TExportedAddress *address);

	const char *str() const { return uuid.str(); }
	const char *newstr() const { return uuid.newstr(); }

protected:
	mstr uuid;

	void print(const TRCDKey &key);
	void print(char kind, const CDate &date, char type, int special = 0);
	void print(char involvement, char ucnType, const char *ucn);
	void print_person(char ucnType, const char *ucn);
	void print_address(long address);
	void print(char status, long subpoenaNo);
	void finish();
};

csv_uuid::csv_uuid(const TRCDKeyContainer &container, int special)
{
	print(container.key);
	print(container.kind, container.date, container.key.type, special);
	finish();
}

csv_uuid::csv_uuid(const char *ucn, char ucnType)
{
	print_person(ucnType, ucn);
	finish();
}

csv_uuid::csv_uuid(const TSubpoena &subpoena)
{
	print(subpoena.key);
	print(subpoena.kind, subpoena.date, subpoena.key.type);
	print(subpoena.involvement, subpoena.ucnType, subpoena.ucn);
	print(subpoena.eventStatus, subpoena.subpoenaNo);
	finish();
}

csv_uuid::csv_uuid(const TUCNGroup *ucnGroup)
{
	print_person(ucnGroup->ucnType, ucnGroup->ucn);
	finish();
}

csv_uuid::csv_uuid(const TExportedPerson *person)
{
	print_person(person->ucnType, person->ucn);
	finish();
}

csv_uuid::csv_uuid(const TExportedAddress *address)
{
	print_person(address->ucnType, address->ucn);
	print_address(address->address);
	finish();
}

void csv_uuid::print(const TRCDKey &key)
{
	uuid.printf("%03d%05X-%1X%1X", COURT_CODE, key.no, key.year - ADMC_YEAR_MIN, strindex("ractsioVj", key.type));
}

void csv_uuid::print(char kind, const CDate &date, char type, int special)
{
	if (type == TYPE_REQUEST || strchr(KIND_LAWSUITS, kind) || type == TYPE_SURROUND || type == TYPE_INREG ||
		type == TYPE_OUTREG || type == TYPE_RETURNED || type == TYPE_JURISD)
	{
		uuid.cat("00-0000");
	}
	else if (strchr(KIND_SESSIONS, kind) || strchr(KIND_ENDOCS, kind))
	{
		uuid.printf("%02X-%1X%1X%02d", kind, date.year - ADMC_YEAR_MIN, date.month, date.day);

		if (special)
			uuid.printf("-%02X", special);
	}
	else
		fatal("%d/%d: видовете не се поддържат (%s %D)", type, kind, uuid.str(), &date);
}

// 2015:268 +I; 2016:305 +TN; 2017:052 +b for request lowers
static constant UCN_ADMC_EXPORTS = "cCfFsJpkKLAUITN" "b";

void csv_uuid::print(char involvement, char ucnType, const char *ucn)
{
	uuid.printf("-%02X%01X%c-%08lX", involvement, strindex(UCN_ADMC_EXPORTS, ucnType), ucn[0], atoi(ucn + 1));
}

void csv_uuid::print_person(char ucnType, const char *ucn)
{
	uuid.printf("%03d", COURT_CODE);
	uuid.cat("00000-0000-0000");
	print('\0', ucnType, ucn);
}

#define ADDRESS_LIMIT 0xFFFF

void csv_uuid::print_address(long address)
{
	if (address > ADDRESS_LIMIT)
		fatal("%ld: прекалено голям номер на адрес.", address);

	uuid.printf("%04lX", address);
}

#define SUBPOENA_NO_LIMIT 0x8000

void csv_uuid::print(char status, long subpoenaNo)
{
	if (subpoenaNo >= SUBPOENA_NO_LIMIT)
		fatal("%ld: прекалено голям номер на призовка.");

	uuid.printf("%04lX", subpoenaNo + (status == RESULT_FIXED ? 0 : SUBPOENA_NO_LIMIT));
}

void csv_uuid::finish()
{
	static constant templ = "00000000-0000-0000-0000-000000000000";
	uuid.cat(templ + uuid.len());
}

// ----- csv_line ------------------------------------------------------------
class csv_line : protected mstr
{
public:
	csv_line(TUpdateCSV *tUpdate);

	void put(const char *field, const char *s);
	void put(const char *field, const csv_uuid &uuid) { put_raw(field, uuid.str()); }
	void put(const char *field, long i);
	void put_flag(const char *field, int i);
	void put(const char *field, const CDate &date);
	void put(const char *field, const CTime &time);
	void put_money(const char *field, double d);
	void put_date(const char *field, char gop, const TRCDKey &key, char kind, const CDate &date);
	void put_user(const char *field, char gop, const TRCDKey &key, char kind, const CDate *date);
	void put_type(const char *field, const TConnect &connect);
	void put_court(const char *field, const TConnect &connect);
	void put_court(const char *field, const TOutReg *outReg);
	void put_name(const char *field, const char *ucn);
	void put_raw(const char *field, const char *s);
	void sep(const char *field);

	const char *header() { return head.str(); }
	void assert_count(int tCount);

protected:
	TUpdateCSV *update;
	int count;
	mstr head;

	bool get(TLog &log, char gop, const TRCDKey &key, char kind, const CDate *date, const char *fields);
	void put_court(const char *field, const TRCDKey &key, char ucnType, const char *ucn);
	void print_error(mstr &m, char gop, const TRCDKey &key, char kind, const CDate *date);
};

csv_line::csv_line(TUpdateCSV *tUpdate)
	: update(tUpdate), count(0)
{
}

void csv_line::put(const char *field, const char *s)
{
	sep(field);
	cach('"');

	while (*s)
	{
		char c = *s;

		if (c == '"')
			cach('"');

	#if 20150257
		utf8_write(c, *this);
	#else  // 20150257
		cach(c);
	#endif  // 20150257
		s++;
	}

	cach('"');
}

void csv_line::put(const char *field, long i)
{
	sep(field);
	itom(i, *this);
}

void csv_line::put_flag(const char *field, int i)
{
	sep(field);
	printf("%d", i);
}

void csv_line::put(const char *field, const CDate &date)
{
	sep(field);

	if (!date.Empty())
		printf("%04ld-%02d-%02d", date.year, date.month, date.day);
}

void csv_line::put(const char *field, const CTime &time)
{
	sep(field);

	if (!time.Empty())
		printf("%02d:%02d", time.hour, time.minute);
}

void csv_line::put_money(const char *field, double d)
{
	sep(field);

	if (d != 0.0)
		printf("%.2lf", d);
}

bool csv_line::get(TLog &log, char gop, const TRCDKey &key, char kind, const CDate *date, const char *fields)
{
	msql m(log.Name, fields);

	log.key = key;
	log.gop = gop;
	m.Add(&log, "F_NO, F_YEAR, F_TYPE, F_GOP");

	if (date)
		m.AddDate("F_DATE", *date);

	if (kind)
		m.AddChar("F_KIND", kind);

	m.AddOrder("F_STAMP DESC");

	{
		TQuery q(m.str());
		if (q.Read())
		{
			log << q;
			return true;
		}
	}

	if (date && !strchr(KIND_OPEN_SESSIONS, kind))	// match kind '\0' as well
	{
		m.Begin(log.Name, fields);
		m.Add(&log, "F_NO, F_YEAR, F_TYPE, F_GOP");
		m.AddRange("F_STAMP", CDateTime(date->day, date->month, date->year, 0, 0),
			CDateTime(date->day, date->month, date->year, 23, 59));
		m.AddChar("F_KIND", kind);

		TQuery q(m.str());
		if (q.Read())
		{
			log << q;
			return true;
		}
	}

	return false;
}

void csv_line::print_error(mstr &m, char gop, const TRCDKey &key, char kind, const CDate *date)
{
	m.printf("%s %ld/%ld: %s за %s", Type->Seek(key.type), key.no, key.year,
		date && *date >= FULL_KIND_DATE ? "не е намерен журналния запис" : "в журнала няма достатъчно данни",
		GOP->Seek(gop));

	if (kind)
		m.printf(" на %s", Kind->Seek(kind));

	if (date)
		m.printf(" от %D", date);
}

void csv_line::put_date(const char *field, char gop, const TRCDKey &key, char kind, const CDate &date)
{
	TLog log;
	const CDate *tDate = kind ? &date : NULL;

	if (get(log, gop, key, kind, tDate, "F_STAMP"))
		put(field, (CDate &) log.stamp);
	else
	{
		mstr m;

		print_error(m, gop, key, kind, tDate);
		update->eprintf("(%s, за %s ще се използва подразбиращата се дата", m.str(), field);
		put(field, date);
	}
}

void csv_line::put_user(const char *field, char gop, const TRCDKey &key, char kind, const CDate *date)
{
	TLog log;

	if (get(log, gop, key, kind, date, "F_UCN"))
		put_name(field, log.ucn);
	else
	{
		mstr m;

		print_error(m, gop, key, kind, date);
		update->eprintf("(%s, за %s ще се експортира празен стринг", m.str(), field);
		put(field, "");
	}
}

void csv_line::put_type(const char *field, const TConnect &connect)
{
	if (connect.connectKind)
	{
		ConnectKind->Seek(connect.connectKind);
		put(field, ConnectKind->name);
	}
	else
	{
		update->eprintf("%s %ld/%ld: данните за първоинстанционно дело не съдържат основния вид "
			"на делото", Type->Seek(connect.key.type), connect.key.no, connect.key.year);
		put(field, "");
	}
}

void csv_line::put_court(const char *field, const TRCDKey &key, char ucnType, const char *ucn)
{
	TSender sender;

	sender.ucnType = ucnType;
	strcpy(sender.ucn, ucn);

	if (sender.Try())
		put(field, sender.court);
	else
	{
		update->eprintf("%s %ld/%ld: адресанта липсва в адресантите", Type->Seek(key.type), key.no, key.year);
		sep(field);
	}
}

void csv_line::put_court(const char *field, const TConnect &connect)
{
	put_court(field, connect.key, connect.ucnType, connect.ucn);
}

void csv_line::put_court(const char *field, const TOutReg *outReg)
{
	put_court(field, outReg->key, outReg->receiverType, outReg->receiver);
}

void csv_line::put_name(const char *field, const char *ucn)
{
	mstr m;

	if (atob(ucn))
	{
		TCitizen citizen;

		citizen.ucnType = UCN_CITIZEN_UCN;
		strcpy(citizen.ucn, ucn);

		if (citizen.Try())
		{
			m.cat(citizen.name);

			if (any(citizen.reName))
				m.printf(" %s", citizen.reName);

			m.printf(" %s", citizen.family);

			if (any(citizen.reFamily))
				m.printf("-%s", citizen.reFamily);
		}
		else
		{
			update->eprintf("%s %s: липсва в базата данни", UCNType->Seek(citizen.ucnType), citizen.ucn);
			m.cat("");
		}
	}

	put(field, m.str());
}

void csv_line::put_raw(const char *field, const char *s)
{
	sep(field);
	cat(s);
}

void csv_line::sep(const char *field)
{
	if (count)
	{
		cat(update->HumanReadable() ? "|\n" : "|");
		head.cat("|");
	}

	head.cat(field);
	count++;

	if (update->HumanReadable())
		printf("%d. %s = ", count, field);
}

void csv_line::assert_count(int tCount)
{
	if (tCount != count)
		fatal("%s: грешка: %d полета вместо %d", str(), count, tCount);
}

// ----- TCSVFile ------------------------------------------------------------
class TCSVFile : protected TUserFile
{
public:
	TCSVFile();

	void Open(const char *prefix);
	void Write(csv_line &l);
	void Close();

protected:
	bool firstLine;
};

TCSVFile::TCSVFile() :
	TUserFile("CSV"), firstLine(true)
{
}

void TCSVFile::Open(const char *prefix)
{
	build_fn(EXPORT_CSV_PATH, prefix, temp);
	scprintf(temp, "_%ld.csv", COURT_CODE);

	if (!OpenFile(temp, "wb"))
		failure();
}

void TCSVFile::Write(csv_line &l)
{
	if (firstLine)
	{
		fprintf(f, "%s\r\n", l.header());
		firstLine = false;
	}

	fprintf(f, "%s\r\n", str(l));
}

void TCSVFile::Close()
{
	if (!TUserFile::Close())
		failure();
}

// ----- TExportCSVGroup -----------------------------------------------------
class TExportCSVGroup : public TGroup
{
public:
	TExportCSVGroup();

	CDate minRequestDate;
	CDate maxRequestDate;
	long exportRequests;

	CDate minLawsuitDate;
	CDate maxLawsuitDate;
	long exportLawsuits;

	long humanReadable;
	long showWarnings;
};

#include "ExportCSVDialog.h"
#include "ExportCSVDialog.cpp"

TExportCSVGroup::TExportCSVGroup()
	: TGroup("T_EXPORT_CSV", NULL)
{
	Add(
		new TDate("F_MIN_REQUEST_DATE", &minRequestDate, false),
		new TDate("F_MAX_REQUEST_DATE", &maxRequestDate, false),
		new TCheck("F_EXPORT_REQUESTS", &exportRequests, TRUE),
		new TDate("F_MIN_LAWSUIT_DATE", &minLawsuitDate, false),
		new TDate("F_MAX_LAWSUIT_DATE", &maxLawsuitDate, false),
		new TCheck("F_EXPORT_LAWSUITS", &exportLawsuits, TRUE),
		new TCheck("F_HUMAN_READABLE", &humanReadable, TRUE),
		new TCheck("F_SHOW_WARNINGS", &showWarnings, TRUE),
		NULL
	);

	exportRequests = TRUE;
	exportLawsuits = TRUE;
}

// ----- TExportCSVSide ------------------------------------------------------
class TExportCSVSide : public TSideWind
{
public:
	TExportCSVSide(const TSideWind *sideWind, TGArray<TLinkWind> &links);
	TExportCSVSide() { }

	TStringArray relations;

	void GetRelation(mstr &relation) const;
};

TExportCSVSide::TExportCSVSide(const TSideWind *sideWind, TGArray<TLinkWind> &links)
{
	*this << sideWind;

	for (int i = 0; i < links.Count(); i++)
	{
		const TLinkWind *linkWind = links[i];

		if (SameWind(linkWind))
		{
			relations.Add(csv_uuid(linkWind->proxyUcn, linkWind->proxyUcnType).newstr());
		}
		else if (atob(linkWind->redirectUcn))
		{
			TSideWind proxy;
			linkWind->GetProxy(proxy);

			if (SameWind(proxy))
				relations.Add(csv_uuid(linkWind->redirectUcn, linkWind->redirectUcnType).newstr());
		}
	}
}

void TExportCSVSide::GetRelation(mstr &relation) const
{
	for (int i = 0; i < relations.Count(); i++)
	{
		relation.sep(",");
		relation.cat(relations[i]);
	}
}

// ----- TExportCSVReturnData ------------------------------------------------
class TExportCSVReturnData
{
public:
	TExportCSVReturnData(TUpdateCSV *update, const TOutReg *outReg, const TInReg *inReg);

	const char *resultName;
	char connectType;
	bool connectFound;
	TConnect connect;
	const char *remark;
};

TExportCSVReturnData::TExportCSVReturnData(TUpdateCSV *update, const TOutReg *outReg, const TInReg *inReg)
{
	if (outReg->resultII != RESOLUTION_FAKE)
	{
		resultName = Resolution->Seek(outReg->resultII);
		connectType = CONNECT_TYPE_VZ_LAWSUIT;
		remark = str(outReg->remarkVZ);
	}
	else if (outReg->resultKS != RESOLUTION_FAKE)
	{
		resultName = Resolution->Seek(outReg->resultKS);
		connectType = CONNECT_TYPE_KS_LAWSUIT;
		remark = str(outReg->remarkKS);
	}
	else if (outReg->resultI != RESOLUTION_FAKE)
	{
		resultName = Resolution->Seek(outReg->resultI);
		connectType = CONNECT_TYPE_VZ_LAWSUIT;
		remark = str(outReg->remarkVZ);
	}
	else if (inReg->kind == KIND_INREG_SLOWDOWN || inReg->kind == KIND_INREG_RESCIND ||
		inReg->kind == KIND_INREG_REVIEW)
	{
		resultName = Result->Seek(outReg->result);
		connectType = CONNECT_TYPE_KS_LAWSUIT;
		remark = str(outReg->remarkKS);
	}
	else
	{
		update->eprintf("%s %ld/%ld: има дата на връщане, но липсва поне един резултат от по-висша "
			"инстанция", Type->Seek(outReg->key.type), outReg->key.no, outReg->key.year);
		resultName = "";
		connectType = '\0';
		remark = "";
	}

	if (connectType)
		connectFound = connect.TryBy(outReg, connectType, "F_DECISION_KIND, F_DECISION_DATE");
	else
		connectFound = false;
}

// ----- TExportCSV ----------------------------------------------------------
class TExportCSV : public TUpdateCSV
{
public:
	TExportCSV(TWindow *parent, TExportCSVGroup &tGroup);

	virtual void Execute();
	virtual bool HumanReadable() { return group.humanReadable; }
	virtual bool ShowWarnings() { return group.showWarnings; }

protected:
	TExportCSVGroup &group;

	TCSVFile lawsuits_file;
	TCSVFile requests_file;
	TCSVFile persons_file;
	TCSVFile addresses_file;
	TCSVFile lawsuit_sides_file;
	TCSVFile request_sides_file;
	TCSVFile sessions_file;
	TCSVFile session_sides_file;
	TCSVFile subpoenas_file;
	TCSVFile acts_file;
	TCSVFile documents_file;
	TCSVFile document_sides_file;
	TCSVFile appeals_file;
	TCSVFile appeal_sides_file;
	TCSVFile electrics_file;
	TCSVFile history_file;
	TCSVFile return_file;
	TCSVFile movement_file;
	TCSVFile resolutions_file;

	void UpdateCount();

	TISArray<TExportedPerson> exportedPersons;
	TISArray<TExportedAddress> exportedAddresses;

	enum
	{
		SIDE_LAWSUIT,
		SIDE_REQUEST,
		SIDE_SESSION,
		SIDE_DOCUMENT,
		SIDE_APPEAL
	};

	void ExportSubpoenas(const TRCDKeyContainer &container, const TSideWind &sideWind, const csv_uuid &container_uuid,
		const csv_uuid &lawsuit_uuid, TExportedPerson *person);
	TExportedPerson *ExportPerson(char ucnType, const char *ucn);
	TExportedPerson *ExportSideITV(csv_line &side_line, const TSideWind &sideWind);
	TExportedPerson *ExportSideITV_R(csv_line &side_line, const TExportCSVSide &sideWind);
	TExportedPerson *ExportSideDCV(csv_line &side_line, const TExportCSVSide &sideWind, const csv_uuid &container_uuid);
	void ExportSideWind(TCSVFile &sides_file, const TRCDKeyContainer &container, const TExportCSVSide &sideWind,
		const csv_uuid &container_uuid, const csv_uuid *lawsuit_uuid, int sideType);
	void ExportSideWinds(TCSVFile &sides_file, const TRCDKeyContainer &container, const csv_uuid &container_uuid,
		const csv_uuid *lawsuit_uuid, int sideType);
	void ExportEDocs(const TRCDKeyContainer &container, const csv_uuid &uuid, const char *table, const char *description,
		int extraNo = -1);
	void ExportHistory(const TRCDKeyContainer &container, const csv_uuid &container_uuid, const char *table);
	void ExportDecision(const TDecision *decision, const csv_uuid &lawsuit_uuid, const csv_uuid &session_uuid,
		const TSession *finalizer, bool minOrder, char gop = '\0');		// non-zero for virtuals
	void ExportSession(const TGSArray<TRCDKeyContainer> &array, int session_index, const csv_uuid &lawsuit_uuid,
		char college, const TSession *finalizer, TGArray<TDecision> &virtuals);
	void ExportMovement(const TRCDKeyContainer &container, const csv_uuid &container_uuid);
	void ExportDocument(const TDatedContainer *container, const csv_uuid &lawsuit_uuid);
	void ExportAppeal(TInReg *inReg, const csv_uuid &lawsuit_uuid, const TGArray<TDecision> &virtuals);
	void ExportReturn(const TOutReg *outReg, const TLawsuit &lawsuit, const TGSArray<TRCDKeyContainer> &array);
	void ExportRequest(const TRequest &request);
	void ExportRequests();
	void ExportLawsuit(const TLawsuit &lawsuit);
	void ExportLawsuits();
};

TExportCSV::TExportCSV(TWindow *parent, TExportCSVGroup &tGroup) :
	TUpdateCSV(parent), group(tGroup)
{
	lawsuits_file.Open("cases");
	requests_file.Open("indocs");
	persons_file.Open("contragents");
	addresses_file.Open("addresses");
	lawsuit_sides_file.Open("case_contr");
	request_sides_file.Open("indoc_contr");
	sessions_file.Open("sessions");
	session_sides_file.Open("session_contr");
	subpoenas_file.Open("subpoenas");
	acts_file.Open("acts");
	documents_file.Open("documents");
	document_sides_file.Open("document_contr");
	appeals_file.Open("appeals");
	appeal_sides_file.Open("appeal_contr");
	electrics_file.Open("edocuments");
	history_file.Open("history");
	return_file.Open("resultappeals");
	movement_file.Open("documentlocations");
	resolutions_file.Open("resolutions"); 
}

static TAnyAddress *csv_get_address(TUCNGroup *ucnGroup, bool foreign)
{
	TAnyAddress *address = TAddress::NewGroup(foreign);
	msql m;

	*address << ucnGroup;
	address->FloatCriteria(m);

	// 2015:258
	if (!foreign)
	{
		m.Add("F_UCLP");
		m.cat("> 0");
	}

	m.AddOrder(foreign ? "F_ADDRESS DESC" : "F_ADDRESS");

	TQuery q(str(m));
	if (q.Read())
		*address << q;
	else if (!foreign)
	{
		delete address;
		address = NULL;
	}

	return address;
}

void TExportCSV::ExportSubpoenas(const TRCDKeyContainer &container, const TSideWind &sideWind,
	const csv_uuid &container_uuid, const csv_uuid &lawsuit_uuid, TExportedPerson *person)
{
	csv_uuid person_uuid(person);
	TSubpoena subpoena;
	TBaseWind baseWind("T_SUBPOENA", NULL);
	msql m;

	baseWind << sideWind;
	baseWind.ExactCriteria(m);
	TQuery q(str(m));

	while (q.Read())
	{
		subpoena << q;
		SubpoenaKind->Seek(subpoena.subpoenaKind);

		if (!SubpoenaKind->genericKind)
			continue;

		// 2015:258			
		if (subpoena.uclp < 0)
		{
			eprintf("%ld: призовка с отрицателно населено място", subpoena.uclp);
			continue;
		}

		VCLP->uclp = subpoena.uclp;

		if (subpoena.uclp && !VCLP->Try())
		{
			eprintf("%ld: липсващо населено място", subpoena.uclp);
			continue;
		}

		csv_line sub_line(this);
		csv_uuid sub_uuid(subpoena);

		sub_line.put("DOCUMENT_ID", sub_uuid);
		sub_line.put("E_SESSION_ID", container_uuid);
		sub_line.put("E_CASE_ID", lawsuit_uuid);
		sub_line.put("CONTARGENT_ID", person_uuid);

		TExportedAddress *address = new TExportedAddress(this, subpoena);
		int index = exportedAddresses.Find(address);

		if (index == INT_MAX)
		{
			csv_line adr_line(this);

			address->address = ++person->addresses;
			adr_line.put("CONTRAGENT_ID", person_uuid);
			adr_line.put("ADDRESS_ID", csv_uuid(address));

			Area->area = VCLP->area;

			if (subpoena.uclp && Area->Try())
				adr_line.put("REGION", Area->name);
			else
			{
				adr_line.put("REGION", "");

				if (subpoena.uclp)
					eprintf("%ld: липсваща област от населено място %ld", VCLP->area, VCLP->uclp);
			}

			Region->area = VCLP->area;
			Region->region = VCLP->region;

			if (subpoena.uclp && Region->Try())
				adr_line.put("MUNICIPALITY", Region->name);
			else
			{
				adr_line.put("MUNICIPALITY", "");

				if (subpoena.uclp)
					eprintf("%ld/%ld: липсваща община от населено място %ld", VCLP->area, VCLP->region, VCLP->uclp);
			}

			adr_line.put("PLACE", VCLP->uclp);
			adr_line.put("STREET_NAME", address->streetName);
			adr_line.put("ADR_NO", address->adrNo);
			adr_line.put("BLOCK_NO", address->blockNo);
			adr_line.put("ENTRANCE", address->entrance);
			adr_line.put("FLOOR", address->floor);
			adr_line.put("APARTMENT", address->apartment);
			adr_line.put("ADDRESS", address->foreign);
			adr_line.put("REMARK", address->remark);
			adr_line.assert_count(0xD);
			addresses_file.Write(adr_line);
			exportedAddresses.Add(address);
		}
		else
		{
			delete address;
			address = exportedAddresses[index];
		}

		sub_line.put("ADDRESS_ID", csv_uuid(address));
		sub_line.put("REG_DATE", subpoena.subpoenaDate);
		sub_line.put("REG_NO", subpoena.subpoenaNo);
		sub_line.put("DOCUMENTTYPE_NAME", SubpoenaKind->Seek(subpoena.subpoenaKind));
		sub_line.put_flag("DELIVERED_F", subpoena.receivedStatus == SRS_RECEIVED);
		if (subpoena.receivedStatus == SRS_RECEIVED)
			sub_line.put("DELIVERED_DATE", subpoena.subpoenaReceived);
		else
			sub_line.sep("DELIVERED_DATE");
		sub_line.put_flag("RETURNED_F", !subpoena.subpoenaReturned.Empty());
		sub_line.put("RETURNED_DATE", subpoena.subpoenaReturned);
		sub_line.put("REMARK", subpoena.receivedText);
		// 2017:051 spec 2.7
		//sub_line.assert_count(0xD);
		sub_line.put("TYPE", Involvement->Seek(sideWind.involvement));
		sub_line.assert_count(14);
		subpoenas_file.Write(sub_line);
		ExportEDocs(container, sub_uuid, "Subpoena", SubpoenaKind->Seek(subpoena.subpoenaKind), subpoena.subpoenaNo);
	}
}

TExportedPerson *TExportCSV::ExportPerson(char ucnType, const char *ucn)
{
	// must be before subpoena addresses
	TExportedPerson *person = new TExportedPerson(ucnType, ucn);
	int index = exportedPersons.Find(person);

	if (!strchr(UCN_ADMC_EXPORTS, ucnType))
		fatal("%s %s: вида на лицето не се поддържа\n", UCNType->Seek(ucnType), ucn);

	if (exportedPersons.Find(person) == INT_MAX)
	{
		TUCNGroup *const ucnGroup = TNamedGroup::NewGroup(ucnType);
		TVirtPtr VP(ucnGroup);
		csv_line pers_line(this);

		strcpy(ucnGroup->ucn, ucn);

		if (!ucnGroup->Try())
			eprintf("%s %s: липсва в базата данни", UCNType->Seek(ucnType), ucn);

		pers_line.put("CONTRAGENT_ID", csv_uuid(person));

		if (strchr(UCN_CITIZENS, ucnType))
		{
			const TCitizen *citizen = (const TCitizen *) ucnGroup;

			pers_line.put("TYPE", "ФЛ");
			pers_line.put_raw("EIKEGN", citizen->ucnType == UCN_CITIZEN_UCN ? citizen->ucn : "");
			pers_line.put("NAME", "");
			pers_line.put("FIRSTNAME", citizen->name);
			pers_line.put("SECONDNAME", citizen->reName);

			{
				mstr m(citizen->family);

				if (any(citizen->reFamily))
					m.printf("-%s", citizen->reFamily);

				pers_line.put("THIRDNAME", str(m));
			}

			//pers_line.put("COUNTRY", citizen->country);
		}
		else if (strchr(UCN_FIRMS, ucnType))
		{
			const TFirm *firm = (const TFirm *) ucnGroup;

			pers_line.put("TYPE", "ЮЛ");
			pers_line.put_raw("EIKEGN", firm->bulstat);
			pers_line.put("NAME", firm->name);
			pers_line.put("FIRSTNAME", "");
			pers_line.put("SECONDNAME", "");
			pers_line.put("THIRDNAME", "");
			//pers_line.put("COUNTRY", firm->country);
		}
		else
		{
			const TSender *sender = (const TSender *) ucnGroup;

			pers_line.put("TYPE", "ЮЛ");
			pers_line.put_raw("EIKEGN", "");
			pers_line.put("NAME", sender->name);
			pers_line.put("FIRSTNAME", "");
			pers_line.put("SECONDNAME", "");
			pers_line.put("THIRDNAME", "");
			//pers_line.put("COUNTRY", "BG");
		}

		TAnyAddress *anyAddress = csv_get_address(ucnGroup, false);

		if (anyAddress)
		{
			const TAddress *address = (const TAddress *) anyAddress;

			pers_line.put("COUNTRY", BUILDIN_COUNTRY);
			VCLP->uclp = address->uclp;

			if (VCLP->Try())
			{
				Area->area = VCLP->area;

				if (Area->Try())
					pers_line.put("REGION", Area->name);
				else
				{
					pers_line.put("REGION", "");
					eprintf("%ld: липсваща област от населено място %ld", VCLP->area, VCLP->uclp);
				}

				Region->area = VCLP->area;
				Region->region = VCLP->region;

				if (Region->Try())
					pers_line.put("MUNICIPALITY", Region->name);
				else
				{
					pers_line.put("MUNICIPALITY", "");
					eprintf("%ld/%ld: липсваща община от населено място %ld", VCLP->area, VCLP->region, VCLP->uclp);
				}

				pers_line.put("PLACE", VCLP->uclp);

				if (address->street)
				{
					Street->uclp = address->uclp;
					Street->street = address->street;

					if (Street->Try())
						pers_line.put("STREET_NAME", Street->name);
					else
					{
						eprintf("%ld/%ld: липсваща улица", address->uclp, address->street);
						pers_line.put("STREET_NAME", "");
					}
				}
				else
					pers_line.put("STREET_NAME", "");
			}
			else
			{
				eprintf("%ld: липсващо населено място", address->uclp);
				pers_line.put("REGION", "");
				pers_line.put("MUNICIPALITY", "");
				pers_line.put("PLACE", VCLP->uclp);
				pers_line.put("STREET_NAME", "");
			}

			{
				mstr m(address->adrNo);
				m.cat(address->subNo);
				pers_line.put("ADR_NO", str(m));
			}

			pers_line.put("BLOCK_NO", address->blockNo);
			pers_line.put("ENTRANCE", address->entrance);
			pers_line.put("FLOOR", address->floor);
			pers_line.put("APARTMENT", address->apartment);
			pers_line.put("ADDRESS", "");
			pers_line.put("REMARK", address->remark);
			pers_line.put("PHONE", address->phone);
			pers_line.put("FAX", address->fax);
			pers_line.put("EMAIL", address->email);
		}
		else
		{
			anyAddress = csv_get_address(ucnGroup, true);
			const TForeignAddress *foreign = (const TForeignAddress *) anyAddress;

			pers_line.put("COUNTRY", foreign->country);
			pers_line.put("REGION", "");
			pers_line.put("MUNICIPALITY", "");
			pers_line.sep("PLACE");
			pers_line.put("STREET_NAME", "");
			pers_line.put("ADR_NO", "");
			pers_line.put("BLOCK_NO", "");
			pers_line.put("ENTRANCE", "");
			pers_line.put("FLOOR", "");
			pers_line.put("APARTMENT", "");
			pers_line.put("ADDRESS", foreign->foreign);
			pers_line.put("REMARK", "");
			pers_line.put("PHONE", "");
			pers_line.put("FAX", "");
			pers_line.put("EMAIL", "");
		}

		pers_line.assert_count(22);
		persons_file.Write(pers_line);
		delete anyAddress;
		exportedPersons.Add(person);
	}
	else
	{
		delete person;
		person = exportedPersons[index];
	}

	return person;
}

TExportedPerson *TExportCSV::ExportSideITV(csv_line &side_line, const TSideWind &sideWind)
{
	TExportedPerson *person = ExportPerson(sideWind.ucnType, sideWind.ucn);

	side_line.put("CONTRAGENT_ID", csv_uuid(person));
	side_line.put("CONTRAGENT_TYPE_NAME", Involvement->Seek(sideWind.involvement));
	side_line.put("VIEW_ORDER", sideWind.Key());
	return person;
}

TExportedPerson *TExportCSV::ExportSideITV_R(csv_line &side_line, const TExportCSVSide &sideWind)
{
	TExportedPerson *person = ExportSideITV(side_line, sideWind);
	mstr relation;

	sideWind.GetRelation(relation);
	side_line.put_raw("RELATION", str(relation));
	return person;
}

TExportedPerson *TExportCSV::ExportSideDCV(csv_line &side_line, const TExportCSVSide &sideWind, const csv_uuid &container_uuid)
{
	TExportedPerson *person = ExportPerson(sideWind.ucnType, sideWind.ucn);

	side_line.put("DOCUMENT_ID", container_uuid);
	side_line.put("CONTRAGENT_ID", csv_uuid(person));
	side_line.put("VIEW_ORDER", sideWind.Key());
	return person;
}

void TExportCSV::ExportSideWind(TCSVFile &sides_file, const TRCDKeyContainer &container, const TExportCSVSide &sideWind,
	const csv_uuid &container_uuid, const csv_uuid *lawsuit_uuid, int sideType)
{
	csv_line side_line(this);
	TExportedPerson *person;

	switch (sideType)
	{
		case SIDE_LAWSUIT :
		{
			side_line.put("E_CASE_ID", container_uuid);
			person = ExportSideITV_R(side_line, sideWind);
			break;
		}
		case SIDE_REQUEST :
		{
			side_line.put("CASE_ID", container_uuid);
			person = ExportSideITV_R(side_line, sideWind);
			break;
		}
		case SIDE_SESSION :
		{
			side_line.put("E_SESSION_ID", container_uuid);
			person = ExportSideITV_R(side_line, sideWind);
			break;
		}
		case SIDE_DOCUMENT :
		{
			person = ExportSideDCV(side_line, sideWind, container_uuid);
			break;
		}
		case SIDE_APPEAL :
		{
			person = ExportSideDCV(side_line, sideWind, container_uuid);
			side_line.put("TYPE", Involvement->Seek(sideWind.involvement));
			break;
		}
		default : fatal("%d: invalid CSV side type", sideType);
	}

	sides_file.Write(side_line);

	if (lawsuit_uuid)
		ExportSubpoenas(container, sideWind, container_uuid, *lawsuit_uuid, person);
}

void TExportCSV::ExportSideWinds(TCSVFile &sides_file, const TRCDKeyContainer &container, const csv_uuid &container_uuid,
	const csv_uuid *lawsuit_uuid, int sideType)
{
	// TODO? make local to sideWind loader
	TGArray<TLinkWind> links;
	{
		TLinkWind linkWind;
		linkWind << container;
		linkWind.LoadFloat(links);
	}

	TGArray<TExportCSVSide> sideWinds;
	{
		TGArray<TSideWind> tSideWinds;
		TSideWind tSideWind;

		tSideWind << container;
		tSideWind.LoadFloat(tSideWinds);
		sweep_invalid_link_winds(tSideWinds, links);

		for (int i = 0; i < tSideWinds.Count(); i++)
			sideWinds.Add(new TExportCSVSide(tSideWinds[i], links));
	}

	int tCount;
	TStringArray exported(true);

	while ((tCount = sideWinds.Count()) != 0)
	{
		for (int i = 0; i < sideWinds.Count(); )
		{
			const TExportCSVSide *sideWind = sideWinds[i];
			int n;

			for (n = 0; n < sideWind->relations.Count(); n++)
				if (exported.Find(sideWind->relations[n]) == INT_MAX)
					break;

			if (n == sideWind->relations.Count())
			{
				ExportSideWind(sides_file, container, *sideWind, container_uuid, lawsuit_uuid, sideType);
				exported.Add(csv_uuid(sideWind).newstr());
				sideWinds.Remove(i);
			}
			else
			{
				i++;
				//eprintf("%c%s is blocked by %s at this point", sideWind->ucnType, sideWind->ucn,
				//	sideWind->relstr[n]);
			}
		}

		if (tCount == sideWinds.Count())
		{
			eprintf("%s %ld/%ld %s %D: взаимно свързани страни!", Type->Seek(container.key.type),
				container.key.no, container.key.year, Kind->Seek(container.kind), &container.date);
			break;
		}
	}

	for (int i = 0; i < tCount; i++)
	{
		const TExportCSVSide *sideWind = sideWinds[i];
		mstr relation;

		sideWind->GetRelation(relation);
		eprintf("\t\t%s %s, страна %s, връзки %s", UCNType->Seek(sideWind->ucnType), sideWind->ucn,
			csv_uuid(sideWind).str(), str(relation));
		ExportSideWind(sides_file, container, *sideWind, container_uuid, lawsuit_uuid, sideType);
	}
}

void TExportCSV::ExportEDocs(const TRCDKeyContainer &container, const csv_uuid &uuid, const char *table,
	const char *description, int extraNo)
{
	for (int type = 0; type < TROUBLE_COUNT; type++)
	{
		char name[SIZE_OF_PATH];
		electric_name(&container, name, type, extraNo);

		if (exist(name))
		{
			csv_line elec_line(this);

			elec_line.put("RECORD_TABLE", table);
			elec_line.put("RECORD_ID", uuid);
			elec_line.put("NAME", basename(name));

			HANDLE handle = CreateFileA(name, 0, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
			bool gotInfo = false;
			BY_HANDLE_FILE_INFORMATION fileInfo;

			if (handle == INVALID_HANDLE_VALUE)
				eprintf("%s: грешка при отваряне", name);
			else
			{
				if (GetFileInformationByHandle(handle, &fileInfo))
					gotInfo = true;
				else
					eprintf("%s: грешка при получаване на информация", name);

				CloseHandle(handle);
			}

			if (gotInfo)
				elec_line.put("SIZE", fileInfo.nFileSizeLow);
			else
				elec_line.sep("SIZE");

			elec_line.put("DESCRIPTION", description);

			if (gotInfo)
			{
			#if 20180702
				CDate date;

				if (fileTime2CDate(&fileInfo.ftCreationTime, date))
					elec_line.put("CREATED_DATE", date);
			#else  // 20180702
				FILETIME localTime;
				SYSTEMTIME systemTime;

				if (FileTimeToLocalFileTime(&fileInfo.ftCreationTime, &localTime) &&
					FileTimeToSystemTime(&localTime, &systemTime))
				{
					CDate date(systemTime.wDay, systemTime.wMonth, systemTime.wYear);
					elec_line.put("CREATED_DATE", date);
				}
			#endif  // 20180702
				else
				{
					eprintf("%s: грешка %ld при конверсия на датата", name, GETLASTERROR);
					elec_line.sep("CREATED_DATE");
				}
			}
			else
				elec_line.sep("CREATED_DATE");

			*strrchr(name, '\\') = '\0';
			elec_line.put("ATT_PATH", name);
			electrics_file.Write(elec_line);
		}
	}
}

void TExportCSV::ExportHistory(const TRCDKeyContainer &container, const csv_uuid &container_uuid, const char *table)
{
	TLog log;
	msql m(log.Name);

	m.Add(&container, "F_NO, F_YEAR, F_TYPE, F_KIND");
	if (strchr(KIND_SESSIONS, container.kind) || strchr(KIND_ENDOCS, container.kind))
		m.AddDate("F_DATE", container.date);
	m.AddRange("F_OPC", 10000, 19990);	// primary actions only
	TQuery q(m.str());

	while (q.Read())
	{
		csv_line log_line(this);

		log << q;
		log_line.put("RECORD_TABLE", table);
		log_line.put("RECORD_ID", container_uuid);
		log_line.put("DATE", (CDate &) log.stamp);
		log_line.put_name("ORG_NAME", log.ucn);
		log_line.put("ACTION", GOP->Seek(log.gop));
		log_line.put("ACTION_INFO", log.text);
		log_line.assert_count(6);
		history_file.Write(log_line);
	}
}

void TExportCSV::ExportRequest(const TRequest &request)
{
	csv_uuid req_uuid(request);
	csv_line req_line(this);

	req_line.put("CASE_ID", req_uuid);
	req_line.put("REG_DATE", request.date);
	req_line.put("REG_NUMBER", request.key.no);
	req_line.put_date("CREATED_DATE", LOG_NEW_DOC, request.key, '\0', request.date);
	req_line.put("DOCUMENTTYPE_NAME", Kind->Seek(request.kind));
	req_line.put("DESCRIPTION", request.text);

	TConnect connect;
	bool first_instance = strchr(KIND_1ST_INDOCS, request.kind);

	if (!first_instance && connect.TryLower(&request))
	{
		req_line.put_raw("TO_CASE_NUMBER", connect.connectNo);
		req_line.put("TO_CASE_YEAR", connect.connectYear);
		req_line.put_type("TO_CASE_TYPE", connect);
		req_line.put_court("TO_COURT_ID", connect);
		req_line.put("TO_ACT_TYPE_NAME", connect.decisionKind ? Kind->Seek(connect.decisionKind) : "");
		req_line.sep("TO_ACT_NUMBER");
		req_line.put("TO_ACT_DATE", connect.decisionDate);
		req_line.put("TO_ACT_YEAR", connect.decisionDate.year);
	}
	else
	{
		if (!first_instance)
		{
			eprintf("%s %ld/%ld: липсват данни за дело на подчинен съд", Type->Seek(request.key.type),
				request.key.no, request.key.year);
		}

		req_line.sep("TO_CASE_NUMBER");
		req_line.sep("TO_CASE_YEAR");
		req_line.put("TO_CASE_TYPE", "");
		req_line.sep("TO_COURT_ID");
		req_line.put("TO_ACT_TYPE_NAME", "");
		req_line.sep("TO_ACT_NUMBER");
		req_line.sep("TO_ACT_DATE");
		req_line.sep("TO_ACT_YEAR");
	}

	bool jurisdiction = connect.TryBy(&request, CONNECT_TYPE_JURISDICTION);

	req_line.put_flag("JURISDICTION_F", jurisdiction);

	if (jurisdiction)
		req_line.put_court("JURISDICTION_COURT_ID", connect);
	else
		req_line.sep("JURISDICTION_COURT_ID");

	TExpense expense;
	msql m(expense.Name);

	m.Add(&request, "F_NO, F_YEAR, F_TYPE");
	m.AddChar("F_SUM_TYPE", SUM_TYPE_TAX);
	m.AddChar("F_SUM_STATE", SUM_STATE_PAID);

	TQuery r(str(m));
	int taxes = 0;

	if (r.Read())
	{
		expense << r;
		taxes = 1 + r.Read();
	}

	if (taxes == 1)
	{
		req_line.put_money("TAX_AMOUNT", expense.sum1);
		req_line.put("TAX_STATUS", SumState->Seek(expense.sumState));
		req_line.put("TAX_PAYMENT_DATE", expense.paidDate);
		req_line.put("TAX_DOCUMENT", expense.paidNote);
	}
	else
	{
		if (taxes == 2)
		{
			eprintf("%s %ld/%ld: две или повече държавни такси", Type->Seek(request.key.type),
				request.key.no, request.key.year);
		}

		req_line.sep("TAX_AMOUNT");
		req_line.put("TAX_STATUS", "");
		req_line.sep("TAX_PAYMENT_DATE");
		req_line.put("TAX_DOCUMENT", "");
	}

	{	// 2017:054 separate scope for resolution
		TRequestResolution *resolution = request.LastResolution("F_RESOLUTION, F_TEXT, F_DATE");

		if (resolution)
		{
			Resolution->Seek(resolution->resolution);
			req_line.put("RESOLUTION_TYPE", Resolution->name);
			req_line.put("RESOLUTION_TEXT", str(resolution->text));
			req_line.put("RESOLUTION_DATE", resolution->date);
			delete resolution;
		}
		else
		{
			req_line.put("RESOLUTION_TYPE", "");
			req_line.put("RESOLUTION_TEXT", "");
			req_line.sep("RESOLUTION_DATE");
		}
	}

	TLawsuit lawsuit;

	if (lawsuit.TryBy(&request, "F_NO, F_YEAR, F_TYPE"))
		req_line.put("E_CASE_ID", csv_uuid(lawsuit));
	else
		req_line.sep("E_CASE_ID");

	// 2017:051 spec 2.7
	// req_line.assert_count(24);
	if (!first_instance && connect.TryLower(&request))
	{
		TExportedPerson *person = ExportPerson(connect.ucnType, connect.ucn);
		req_line.put("TO_CONTRAGENT_ID", csv_uuid(person));
	}
	else
	{
		req_line.sep("TO_CONTRAGENT_ID");
		// error message handled in TO_CASE above
	}

	req_line.assert_count(25);
	requests_file.Write(req_line);
	ExportEDocs(request, req_uuid, "Indoc", Kind->Seek(request.kind));
	ExportHistory(request, req_uuid, "Indoc");
	// 2017:052 spec 2.7 new format
	ExportSideWinds(request_sides_file, request, req_uuid, NULL, SIDE_REQUEST);
	// 2017:054 spec 2.7
	ExportMovement(request, req_uuid);
	// 2017:054 spec 2.7
	TGSArray<TRCDKeyContainer> resolutions;
	{
		TRequestResolution resolution;
		resolution.key = request.key;
		container_events(&resolution, &resolutions);
	}

	for (int i = 0; i < resolutions.Count(); i++)
	{
		const TRequestResolution *resolution = (const TRequestResolution *) resolutions[i];

		if (strchr(RESOLUTION_RESOLUTIONS, resolution->resolution))
		{
			csv_line res_line(this);

			res_line.put("DOCUMENT_ID", req_uuid);
			res_line.put("DATE", resolution->date);
			res_line.put("TYPE", Resolution->Seek(resolution->resolution));
			res_line.put("TEXT", str(resolution->text));

			if (atob(resolution->judge))
				res_line.put_name("ORG_NAME", resolution->judge);
			else
				res_line.sep("ORG_NAME");

			res_line.assert_count(5);
			resolutions_file.Write(res_line);
		}
	}
}

void TExportCSV::ExportRequests()
{
	for (int year = group.minRequestDate.year; year <= group.maxRequestDate.year; year++)
	{
		msql m;
		TRequest request;

		m.printf("Входящи документи от %dг...", year);
		wait->SetText(str(m));

		m.Begin(request.Name);
		m.AddLong("F_YEAR", year);
		m.AddRange("F_DATE", group.minRequestDate, group.maxRequestDate);

		TQuery q(str(m));
		while (q.Read())
		{
			request << q;
			ExportRequest(request);
			wait->TickCount(++count, "обработени входящи документа");
		}
	}
}

static bool csv_act_matches(const TSession *session, const TRCDKeyContainer *container)
{
	return strchr(KIND_ENDOCS, container->kind) && container->date == session->date &&
		((const TDecision *) container)->sessionKind == session->kind;
}

void TExportCSV::ExportDecision(const TDecision *decision, const csv_uuid &lawsuit_uuid, const csv_uuid &session_uuid,
	const TSession *finalizer, bool minOrder, char gop)
{
	csv_uuid act_uuid(*decision, gop != '\0');
	csv_line act_line(this);

	act_line.put("ACT_ID", act_uuid);
	act_line.put("E_CASE_ID", lawsuit_uuid);
	act_line.put("E_SESSION_ID", session_uuid);
	act_line.put("DOCUMENTTYPE_NAME", Kind->Seek(kind2VisibleKind(decision->kind)));
	act_line.put("REG_DATE", decision->fromDate);
	act_line.put("REG_NUMBER", PRINT_JUDGEMENTS ? decision->eventNo : 0);

	if (gop == '\0')	// normal decision
	{
		act_line.put_date("CREATED_DATE", LOG_ENTER, decision->key, decision->kind, decision->date);
		act_line.put_flag("FINAL_F", finalizer && minOrder && csv_act_matches(finalizer, decision));

		TLog log;
		msql m(log.Name, "F_DATE");

		m.Add(decision, "F_NO, F_YEAR, F_TYPE, F_KIND, F_DATE");
		m.AddChar("F_GOP", LOG_TRANSFER);
		m.AddLike("F_TEXT", "webp:подготвен за изпращане", false);
		m.AddOrder("F_STAMP DESC");
		TQuery q(str(m));
		bool prepared = q.Read();

		act_line.put_flag("PUBLISHED_F", prepared);

		if (prepared)
		{
			log << q;
			act_line.put("PUBLISHED_DATE", (CDate &) log.stamp);
		}
		else
			act_line.sep("PUBLISHED_DATE");

		act_line.put("DISPOSITIV", str(decision->text));
		ExportEDocs(*decision, act_uuid, "Act", Kind->Seek(kind2VisibleKind(decision->kind)));
		ExportHistory(*decision, act_uuid, "Act");
	}
	else	// virtual decision
	{
		act_line.put_date("CREATED_DATE", gop, decision->key, decision->sessionKind, decision->date);
		act_line.put_flag("FINAL_F", 0);
		act_line.put_flag("PUBLISHED_F", 0);
		act_line.sep("PUBLISHED_DATE");
		act_line.put("DISPOSITIV", "");
	}

	// 2017:051 spec 2.7
	act_line.put_flag("ENFORCEMENT_F", !decision->forceDate.Empty());
	if (!decision->forceDate.Empty())
		act_line.put("ENFORCEMENT_DATE", decision->date);
	else
		act_line.sep("ENFORCEMENT_DATE");
	act_line.assert_count(0xD);
	acts_file.Write(act_line);
}

struct TResults2Text
{
	const char *results;
	const char *text;
};

void TExportCSV::ExportSession(const TGSArray<TRCDKeyContainer> &array, int session_index, const csv_uuid &lawsuit_uuid,
	char college, const TSession *finalizer, TGArray<TDecision> &virtuals)
{
	// ----- RESULT_ -------------------------------------------------------
	static const TResults2Text Results2Text[] =
	{
		{ "hD",	"Без движение" },
		{ "АLIЎЫ",	"Отложено" },
		{ "P",	"Прекратено" },
		{ "†",	"Присъединено" },
		{ "Ѕd",	"С определение" },
		{ "І4",	"С разпореждане" },
		{ "с",	"С решение" },
		{ NULL,	NULL }
	};

	const TSession *session = (const TSession *) array[session_index];
	csv_uuid ses_uuid(*session);
	csv_line ses_line(this);
	bool open = strchr(KIND_OPEN_SESSIONS, session->kind);

	ses_line.put("E_SESSION_ID", ses_uuid);
	ses_line.put("E_CASE_ID", lawsuit_uuid);
	ses_line.put("DATE", session->date);
	ses_line.put("TYPE_NAME", open ? "Открито заседание" : "Закрито заседание");

	const char *resultName = Result->Seek(session->result);

	for (const TResults2Text *r2t = Results2Text; r2t->results; r2t++)
	{
		if (strchr(r2t->results, session->result))
		{
			resultName = r2t->text;
			break;
		}
	}

	ses_line.put("RESULT_NAME", resultName);
	ses_line.put("START_TIME", session->time);
	if (session->hall)
		ses_line.put("ROOM_NAME", Hall->Seek(session->hall));
	else
		ses_line.put("ROOM_NAME", "");
	ses_line.put("DEPARTMENT_NAME", College->Seek(college));
	ses_line.put("TEAM_NAME", PRINT_COMPOSITIONS ? Composition->Seek(session->composition) : "");
	ses_line.put_name("JUDGE_NAME", session->judge);
	ses_line.put_name("CHAIR_NAME", session->president);
	ses_line.put_name("SECRETARY_NAME", session->secretar);
	ses_line.put_name("PROSECUTOR_NAME", session->prosecutor);

	bool unlinked = strchr(RESULT_UNLINKEDS, session->result);

	ses_line.put_flag("CANCELED_F", unlinked);
	ses_line.put("CANCELED_INFO", unlinked ? str(session->text) : "");

	char gop;

	switch (session->result)
	{
		case RESULT_FIXED :
		case RESULT_ABORTED : gop = LOG_FIXED; break;
		case RESULT_UNFIXED : gop = LOG_UNSET; break;
		default : gop = LOG_ENTER;
	}

	ses_line.put_user("REGISTRATOR_NAME", gop, session->key, session->kind, &session->date);

	char virtKind = '\0';
	// 2015:238 compute virtual before REMARK/EDocs
	if (!strchr(RESULT_UNCARRIEDS, session->result))
	{
		int minActOrder = INT_MAX;

		for (int pass = 1; pass <= 2; pass++)
		{
			for (int index = 0; index < array.Count(); index++)
			{
				const TRCDKeyContainer *container = array[index];

				if (csv_act_matches(session, container))
				{
					const TDecision *decision = (const TDecision *) container;

					Kind->Seek(decision->kind);

					if (pass == 1)
					{
						if (Kind->Order() < minActOrder)
							minActOrder = Kind->Order();
					}
					else
						ExportDecision(decision, lawsuit_uuid, ses_uuid, finalizer, Kind->Order() == minActOrder);
				}
			}
		}

		if (strchr(RESULT_ENTERABLES, session->result))
		{
			if (minActOrder == INT_MAX)
			{
				int index;

				for (index = 0; index < array.Count(); index++)
				{
					const TRCDKeyContainer *container = array[index];

					if (strchr(KIND_SESSIONS, container->kind) &&
						(container->date > session->date ||
						(container->date == session->date && container->kind != session->kind)) &&
						(strchr(RESULT_FINALIZEDS, ((const TSession *) container)->result) ||
						strchr(RESULT_UNDOS, ((const TSession *) container)->result)))
					{
						break;
					}
				}

				if (index == array.Count())
				{
					eprintf("%s %ld/%ld: няма съдебен акт за %s от %D", Type->Seek(session->key.type),
						session->key.no, session->key.year, Kind->Seek(session->kind), &session->date);
				}
			}
		}
		else if (minActOrder == INT_MAX)
		{
			const TSession *tSession = (const TSession *) session;
			TDecision *tDecision = new TDecision;

			*tDecision << session;
			tDecision->kind = tSession->IndexKind();
			tDecision->sessionKind = session->kind;
			tDecision->fromDate = session->date;
			tDecision->forceDate = session->forceDate;
			ExportDecision(tDecision, lawsuit_uuid, ses_uuid, NULL, false, gop);
			virtuals.Add(tDecision);
			virtKind = tDecision->kind;
		}
	}

	ses_line.put("REMARK", unlinked || virtKind ? "" : str(session->text));
	// 2017:051 spec 2.7
	ses_line.put_name("MEMBER1_NAME", session->judgeX);
	ses_line.put_name("MEMBER2_NAME", session->reJudgeX);
	ses_line.assert_count(19);
	sessions_file.Write(ses_line);
	ExportEDocs(*session, ses_uuid, "Session", strchr(KIND_OPEN_SESSIONS, session->kind) ? "Протокол от О.С.З." :
		virtKind ? Kind->Seek(virtKind) : "Текст по З.З.");
	ExportEDocs(*session, ses_uuid, "Session", "Списък на призованите лица", EXTRA_NO_SUBPLIST_FIXED);
	ExportEDocs(*session, ses_uuid, "Session", "Списък на уведомените лица", EXTRA_NO_SUBPLIST_OTHER);
	ExportHistory(*session, ses_uuid, "Session");
	// 2017:051 spec 2.7 new format
	ExportSideWinds(session_sides_file, *session, ses_uuid, &lawsuit_uuid, SIDE_SESSION);
}

void TExportCSV::ExportMovement(const TRCDKeyContainer &container, const csv_uuid &container_uuid)
{
	TGArray<TMovement> movements;
	{
		TMovement movement;
		msql m;

		movement.key = container.key;
		movement.FloatCriteria(m);
		m.AddOrder("F_DATE, F_TIME");
		movement.LoadArray(movements, str(m));
	}

	for (int i = 0; i < movements.Count(); i++)
	{
		const TMovement *movement = movements[i];
		csv_line mov_line(this);
		mstr m;

		mov_line.put("DOCUMENT_ID", container_uuid);
		mov_line.put("DATE", movement->date);
		m.printf("%s: %s", Kind->Seek(movement->kind), movement->text);
		mov_line.put("PLACE", str(m));

		if (atob(movement->ucn))
			mov_line.put_name("ORG_NAME", movement->ucn);
		else
		{
			mov_line.put_user("ORG_NAME", movement->kind == KIND_OUT_MOVEMENT ? LOG_GIVE : LOG_RETURN,
				movement->key, movement->kind, &movement->date);
		}

		mov_line.assert_count(4);
		movement_file.Write(mov_line);
	}
}

void TExportCSV::ExportDocument(const TDatedContainer *container, const csv_uuid &lawsuit_uuid)
{
	csv_uuid doc_uuid(*container);
	csv_line doc_line(this);
	const TOutReg *outReg = container->key.type == TYPE_OUTREG ? (const TOutReg *) container : NULL;

	doc_line.put("DOCUMENT_ID", doc_uuid);
	doc_line.put("E_CASE_ID", lawsuit_uuid);
	doc_line.put("REG_DATE", container->date);
	doc_line.put("REG_NO", container->key.no);
	doc_line.put_date("CREATED_DATE", outReg ? LOG_SEND : LOG_NEW_DOC, container->key, '\0', container->date);
	doc_line.put("DIRECTION", outReg ? "Изходящ" : "Входящ");

	const char *text = outReg ? outReg->text.str() : ((const TSurround *) container)->text;
	bool otherText = strchr(KIND_ANY_OTHER_DOCUMENTS, container->kind) && any(text);

	doc_line.put("DOCUMENTTYPE_NAME", otherText ? text : Kind->Seek(container->kind));
	doc_line.put("REMARK", outReg ? outReg->remark.str() : otherText ? "" : text);
	// 2017:051 spec 2.7
	//doc_line.assert_count(8);
	if (outReg && strchr(UCN_RECEIVER_SENDERS, outReg->receiverType))
		doc_line.put_court("COURT", outReg);
	else
		doc_line.sep("COURT");
	doc_line.assert_count(9);
	documents_file.Write(doc_line);
	ExportEDocs(*container, doc_uuid, "Document", otherText ? text : Kind->name);
	ExportHistory(*container, doc_uuid, "Document");
	// 2017:052 spec 2.7 new format
	ExportSideWinds(document_sides_file, *container, doc_uuid, NULL, SIDE_DOCUMENT);

	if (outReg)
	{
		TExportCSVSide sideWind;

		sideWind << outReg;
		sideWind.ucnType = outReg->receiverType;
		strcpy(sideWind.ucn, outReg->receiver);
		sideWind.involvement = INVOLVEMENT_RECEIVER;

		if (sideWind.ucnType != UCN_INSTANCE_RESERVED && !sideWind.Try())
			ExportSideWind(document_sides_file, *container, sideWind, doc_uuid, NULL, SIDE_DOCUMENT);
	}
	// 2017:054 spec 2.7
	if (container->key.type == TYPE_SURROUND)
		ExportMovement(*container, doc_uuid);
}

void TExportCSV::ExportAppeal(TInReg *inReg, const csv_uuid &lawsuit_uuid, const TGArray<TDecision> &virtuals)
{
	csv_uuid app_uuid(*inReg);
	csv_line app_line(this);

	app_line.put("CASE_ID", app_uuid);
	app_line.put("E_CASE_ID", lawsuit_uuid);

	{
		TDecision decision;

		decision.key = inReg->lawsuit;
		decision.kind = inReg->decisionKind;
		decision.date = inReg->decisionDate;

		if (decision.Try("F_NO"))
		{
			csv_uuid act_uuid(decision);

			app_line.put("ACT_ID", act_uuid);
			inReg->AddExtraText(act_uuid.str());
		}
		else
		{
			// ----- KIND_ ---------------------------------------------
			static constant VIRTUAL_KINDS = "-мл";
			int i;

			if (!strchr(VIRTUAL_KINDS, decision.kind))
				i = virtuals.Count();
			else
			{
				for (i = 0; i < virtuals.Count(); i++)
				{
					const TDecision *tDecision = virtuals[i];

					if (tDecision->key == inReg->lawsuit && tDecision->kind == inReg->decisionKind &&
						tDecision->date == inReg->decisionDate)
					{
						break;
					}
				}
			}

			if (i < virtuals.Count())
			{
				csv_uuid act_uuid(decision, 1);

				app_line.put("ACT_ID", act_uuid);
				inReg->AddExtraText(act_uuid.str());
			}
			else
			{
				eprintf("%s %ld/%ld: липсват данни за обжалвания акт", Type->Seek(inReg->key.type), inReg->key.no,
					inReg->key.year);
				app_line.sep("ACT_ID");
			}
		}
	}

	app_line.put("REG_DATE", inReg->date);
	app_line.put("REG_NO", inReg->key.no);

	bool otherText = strchr(KIND_ANY_OTHER_DOCUMENTS, inReg->kind) && any(inReg->text);

	app_line.put("DOCUMENTTYPE_NAME", otherText ? inReg->text : Kind->Seek(inReg->kind));
	app_line.put("DESCRIPTION", otherText ? "" : inReg->text);
	app_line.assert_count(7);
	appeals_file.Write(app_line);
	ExportEDocs(*inReg, app_uuid, "Appeal", otherText ? inReg->text : Kind->name);
	ExportHistory(*inReg, app_uuid, "Appeal");
	// 2017:052 spec 2.7 new format
	ExportSideWinds(appeal_sides_file, *inReg, app_uuid, NULL, SIDE_APPEAL);
	// 2017:054 spec 2.7
	ExportMovement(*inReg, app_uuid);
}

void TExportCSV::ExportReturn(const TOutReg *outReg, const TLawsuit &lawsuit, const TGSArray<TRCDKeyContainer> &array)
{
	const TRCDKey inKey = { outReg->inNo, outReg->inYear, TYPE_INREG };
	const TInReg *inReg = NULL;
	const char *act_id = NULL;
	csv_line ret_line(this);

	for (int i = 0; i < array.Count(); i++)
	{
		const TRCDKeyContainer *container = (const TRCDKeyContainer *) array[i];

		if (strchr(KIND_IN_REG_DOCS, container->kind) && container->key == inKey)
		{
			TString *extraText = container->FindExtraText();
			inReg = (const TInReg *) container;

			if (extraText)
			{
				act_id = extraText->S();
				break;
			}
		}
	}

	if (!inReg)
	{
		// match ExportLawsuit()
		//eprintf("%s %ld/%ld: indoc missing", Type->Seek(outReg->key.type), outReg->key.no, outReg->key.year);
		return;
	}

	ret_line.put("E_CASE_ID", csv_uuid(lawsuit));

	if (act_id)
		ret_line.put_raw("DOCUMENT_ID", act_id);
	else
		ret_line.sep("DOCUMENT_ID");

	TExportCSVReturnData returnData(this, outReg, inReg);

	ret_line.put("RESULT", returnData.resultName);
	ret_line.put("RESULT_INFO", returnData.remark);
	ret_line.put("RESULT_TYPE", outReg->vssIndex);

	if (returnData.connectType)
	{
		bool fullData = true;

		if (returnData.connectFound && returnData.connect.decisionKind)
			ret_line.put("ACT_TYPE", Kind->Seek(returnData.connect.decisionKind));
		else
		{
			ret_line.sep("ACT_TYPE");
			fullData = false;
		}

		if (returnData.connectFound && !returnData.connect.decisionDate.Empty())
			ret_line.put("ACT_DATE", returnData.connect.decisionDate);
		else
		{
			ret_line.sep("ACT_DATE");
			fullData = false;
		}

		if (!fullData)
		{
			eprintf("%s %ld/%ld: има резултат от обжалване, но липсват пълните данни", Type->Seek(outReg->key.type),
				outReg->key.no, outReg->key.year);
		}
	}
	else
	{
		ret_line.sep("ACT_TYPE");
		ret_line.sep("ACT_DATE");
	}

	ret_line.assert_count(7);
	return_file.Write(ret_line);
}

static long cvs_get_priority(const TDecision *decision)
{
	Kind->Seek(decision->kind);
	return Kind->Order();
}

void TExportCSV::ExportLawsuit(const TLawsuit &lawsuit)
{
	bool first_instance = strchr(KIND_1ST_LAWSUITS, lawsuit.kind);
	csv_uuid law_uuid(lawsuit);
	csv_line law_line(this);

	law_line.put("E_CASE_ID", law_uuid);

	if (lawsuit.source.type == TYPE_REQUEST)
	{
		TRequest request;
		request.key = lawsuit.source;
		law_line.put("CASE_ID", csv_uuid(request));
	}
	else
	{
		TType tType;
		tType.Seek(lawsuit.source.type);

		eprintf("(%s %ld/%ld: образувано по '%s', за CASE_ID ще се експортира празен стринг",
			Type->Seek(lawsuit.key.type), lawsuit.key.no, lawsuit.key.year, tType.name);
		law_line.sep("CASE_ID");
	}

	law_line.put("REG_NO", lawsuit.key.no);
	law_line.put("REG_YEAR", lawsuit.key.year);
	law_line.put("REG_DATE", lawsuit.date);
	law_line.put_date("CREATED_DATE", LOG_NEW_LAW, lawsuit.key, '\0', lawsuit.date);
	law_line.put("INSTANCE_NAME", first_instance ? "Първа инстанция" : "Касационна инстанция");
	law_line.put("CHARACTER_NAME", Type->Seek(lawsuit.key.type));
	law_line.put("TYPE_NAME", Kind->Seek(lawsuit.kind));
	{
		mstr m;
		m.printf("%s %s", lawsuit.Code(), Subject->Seek(lawsuit.key.type, lawsuit.subject));
		law_line.put("MATTER_NAME", str(m));
	}

	{  // 2017:053 separate scope for connect
		TConnect connect;

		if (!first_instance && connect.TryBy(&lawsuit, CONNECT_TYPE_LOWER_INSTANCE))
		{
			law_line.put_raw("PREVIOUS_E_CASE_REG_NO", connect.connectNo);
			law_line.put("PREVIOUS_E_CASE_REG_YEAR", connect.connectYear);
			law_line.put_type("PREVIOUS_E_CASE_CHARACTER_NAME", connect);
			law_line.put_court("PREVIOUS_E_CASE_COURT", connect);
		}
		else
		{
			if (!first_instance)
			{
				eprintf("%s %ld/%ld: липсват данни за %s", Type->Seek(lawsuit.key.type), lawsuit.key.no,
					lawsuit.key.year, ConnectType->Seek(CONNECT_TYPE_LOWER_INSTANCE));
			}

			law_line.sep("PREVIOUS_E_CASE_REG_NO");
			law_line.sep("PREVIOUS_E_CASE_REG_YEAR");
			law_line.put("PREVIOUS_E_CASE_CHARACTER_NAME", "");
			law_line.sep("PREVIOUS_E_CASE_COURT");
		}
	}

	law_line.put_flag("RAPID_F", (lawsuit.quick & QUICK_QUICK) != 0);
	law_line.put_flag("SECRECY_LEVEL", (lawsuit.flags & LAWSUIT_LIMITED) != 0);
	law_line.put("DEPARTMENT_NAME", College->Seek(lawsuit.college));
	law_line.put("TEAM_NAME", PRINT_COMPOSITIONS ? Composition->Seek(lawsuit.composition) : "");
	law_line.put_name("JUDGE_NAME", lawsuit.judge);

	TGSArray<TRCDKeyContainer> array;
	query_events(&lawsuit, &array);

	const char *statusName;
	CDate statusDate;
	CDate finishDate = lawsuit.finished;	// 2017:017 LRQ: +related

	{
		TLawInterval interval;
		msql m;

		interval.key = lawsuit.key;
		interval.FloatCriteria(m, "F_FINISHED");
		m.AddIsNull("F_FINISHED", false);
		m.AddOrder("F_FINISHED DESC");
		TQuery q(str(m));

		if (q.Read())
		{
			interval << q;
			finishDate = interval.finished;
		}
	}

	const TSession *finalizer = NULL;
	const TDecision *decidizer = NULL;

	for (int index = 0; index < array.Count(); index++)
	{
		const TRCDKeyContainer *container = array[index];

		if (strchr(KIND_SESSIONS, container->kind))
		{
			const TSession *session = (const TSession *) container;

			if (!session->final.Empty())
			{
				finalizer = session;

				if (session->final == finishDate)
					break;
			}
		}
	}

	TArchive archive;
	archive.key = lawsuit.key;
	bool archived = archive.Try();

	if (finalizer)
	{
		statusDate = finalizer->final;
		bool cancelaw = false;

		for (int index = 0; index < array.Count(); index++)
		{
			const TRCDKeyContainer *container = array[index];

			if (csv_act_matches(finalizer, container))
			{
				const TDecision *decision = (const TDecision *) container;
				cancelaw = strchr(RESULT_CANCELAWS, decision->result);

				if (!decidizer || cvs_get_priority(decision) < cvs_get_priority(decidizer) ||
					cancelaw)
				{
					decidizer = decision;

					if (cancelaw)
						break;
				}
			}
		}

		if (!decidizer)
			statusName = "";
		else if (finalizer->result == RESULT_LAWSUIT_LINKED)
			statusName = "Присъединено";
		else
			statusName = cancelaw ? "Прекратено" : "Решено";
	}
	else if (archived)
	{
		if (archive.killDate.Empty())
		{
			statusName = "Архивирано";
			statusDate = archive.archiveDate;
		}
		else
		{
			statusName = "Унищожено";
			statusDate = archive.killDate;
		}
	}
	else
	{
		statusName = "Образувано";
		statusDate = lawsuit.date;

		for (int index = 0; index < array.Count(); index++)
		{
			const TRCDKeyContainer *container = array[index];

			if (strchr(KIND_SESSIONS, container->kind))
			{
				const TSession *session = (const TSession *) array[index];

				// ----- RESULT_ -------------------------------------------------------
				if (strchr("тJЅЗІЄАс", session->result))
					continue;

				statusName = Result->Seek(session->result);

				static const TResults2Text Results2Text[] =
				{
					{ "hD",	"Без движение" },
					{ "LIЎЫ",	"Отложено" },
					{ "p",	"За решаване" },
					{ "g",	"Определено" },
					{ NULL,	NULL }
				};

				for (const TResults2Text *r2t = Results2Text; r2t->results; r2t++)
				{
					if (strchr(r2t->results, session->result))
					{
						statusName = r2t->text;
						break;
					}
				}
			}
			else if (container->key.type == TYPE_INREG || container->key.type == TYPE_SURROUND)
				statusName = "За разпореждане";
		}
	}

	law_line.put("STATUS_NAME", statusName);
	law_line.put("STATUS_DATE", statusDate);

	TMovement *movement = lawsuit.LastMove();

	if (movement)
	{
		law_line.put("LOCATION_NAME", movement->text);
		law_line.put("LOCATION_DATE", movement->date);
		delete movement;
	}
	else
	{
		law_line.put("LOCATION_NAME", "");
		law_line.sep("LOCATION_DATE");
	}

	const TSession *session = NULL;

	for (int index = array.Count() - 1; index >= 0; index--)
	{
		const TRCDKeyContainer *container = array[index];

		if (strchr(KIND_OPEN_SESSIONS, container->kind) &&
			((const TSession *) container)->result == RESULT_PUBLIC_DECISION)
		{
			session = (const TSession *) container;
			break;
		}
	}

	if (session)
		law_line.put("WAITING_SOLUTION_DATE", session->date);
	else
		law_line.sep("WAITING_SOLUTION_DATE");

	if (finalizer)
	{
		law_line.put("CLOSED_SESSION_TYPE", strchr(KIND_OPEN_SESSIONS, finalizer->kind) ?
			"Открито заседание" : "Закрито заседание");

		law_line.put("CLOSED_DATE", finalizer->final);

		if (decidizer)
		{
			law_line.put("ACT_DATE", decidizer->fromDate);
			law_line.put("ACT_NO", PRINT_JUDGEMENTS ? decidizer->eventNo : 0);

			if (!strchr(RESULT_WITH_READY_ANYS, decidizer->result))
				law_line.put("RESULT_TYPE", Result->Seek(decidizer->result));
			else if (lawsuit.grant != GRANT_NONE_YET)
				law_line.put("RESULT_TYPE", Grant->Seek(lawsuit.grant));
			else
				law_line.put("RESULT_TYPE", "");
		}
		else
		{
			eprintf("%s %ld/%ld: няма съдебен акт за %s от %D",
				Type->Seek(finalizer->key.type), finalizer->key.no, finalizer->key.year,
				Kind->Seek(finalizer->kind), &finalizer->date);

			law_line.sep("ACT_DATE");
			law_line.sep("ACT_NO");
			law_line.put("RESULT_TYPE", "");
		}
	}
	else
	{
		law_line.put("CLOSED_SESSION_TYPE", "");
		law_line.sep("CLOSED_DATE");
		law_line.sep("ACT_DATE");
		law_line.sep("ACT_NO");
		law_line.put("RESULT_TYPE", "");
	}

	const TInReg *inReg = NULL;
	const TOutReg *outReg = NULL;

	for (int index = array.Count() - 1; index >= 0; index--)
	{
		const TRCDKeyContainer *container = array[index];

		if (container->key.type == TYPE_INREG)
		{
			inReg = (const TInReg *) container;
			break;
		}
	}

	law_line.put_flag("APPEAL_F", inReg != NULL);

	if (inReg)
	{
		for (int index = 0; index < array.Count(); index++)
		{
			const TRCDKeyContainer *container = array[index];

			if (container->key.type == TYPE_OUTREG &&
				((const TOutReg *) container)->inNo == inReg->key.no &&
				((const TOutReg *) container)->inYear == inReg->key.year)
			{
				outReg = (const TOutReg *) container;
			}
		}

		if (outReg && outReg->returned.Empty())
			outReg = NULL;
	}

	if (outReg)
	{
	#if 20170053
		TExportCSVReturnData returnData(this, outReg, inReg);

		law_line.put("APPEAL_RESULT", returnData.resultName);
		law_line.put("APPEAL_DATE", inReg->date);

		if (returnData.connectType)
		{
			if (returnData.connectFound && !returnData.connect.decisionDate.Empty())
				law_line.put("APPEAL_RESULT_DATE", returnData.connect.decisionDate);
			else
			{
				eprintf("%s %ld/%ld: има резултат от обжалване, но липсват пълните данни",
					Type->Seek(outReg->key.type), outReg->key.no, outReg->key.year);
				law_line.sep("APPEAL_RESULT_DATE");
			}
		}
		else
			law_line.sep("APPEAL_RESULT_DATE");
	#else
		const char *resultName;
		char connectType;

		if (outReg->resultII != RESOLUTION_FAKE)
		{
			resultName = Resolution->Seek(outReg->resultII);
			connectType = CONNECT_TYPE_VZ_LAWSUIT;
		}
		else if (outReg->resultKS != RESOLUTION_FAKE)
		{
			resultName = Resolution->Seek(outReg->resultKS);
			connectType = CONNECT_TYPE_KS_LAWSUIT;
		}
		else if (outReg->resultI != RESOLUTION_FAKE)
		{
			resultName = Resolution->Seek(outReg->resultI);
			connectType = CONNECT_TYPE_VZ_LAWSUIT;
		}
		else if (inReg->kind == KIND_INREG_SLOWDOWN || inReg->kind == KIND_INREG_RESCIND ||
			inReg->kind == KIND_INREG_REVIEW)
		{
			resultName = Result->Seek(outReg->result);
			connectType = CONNECT_TYPE_KS_LAWSUIT;
		}
		else
		{
			eprintf("%s %ld/%ld: има дата на връщане, но липсва поне един резултат от по-висша "
				"инстанция", Type->Seek(outReg->key.type), outReg->key.no, outReg->key.year);
			resultName = "";
			connectType = '\0';
		}

		law_line.put("APPEAL_RESULT", resultName);
		law_line.put("APPEAL_DATE", inReg->date);

		if (connectType)
		{
			TConnect connect;

			if (connect.TryBy(outReg, connectType, "F_DECISION_DATE"))
				law_line.put("APPEAL_RESULT_DATE", connect.decisionDate);
			else
			{
				eprintf("%s %ld/%ld: има резултат от обжалване, но липсват пълните данни",
					Type->Seek(outReg->key.type), outReg->key.no, outReg->key.year);
				law_line.sep("APPEAL_RESULT_DATE");
			}
		}
		else
			law_line.sep("APPEAL_RESULT_DATE");
	#endif
	}
	else
	{
		law_line.put("APPEAL_RESULT", "");
		if (inReg)
			law_line.put("APPEAL_DATE", inReg->date);
		else
			law_line.sep("APPEAL_DATE");
		law_line.sep("APPEAL_RESULT_DATE");
	}

	law_line.put_flag("RETURNED_F", outReg != NULL);

	if (outReg)
		law_line.put("RETURNED_DATE", outReg->returned);
	else
		law_line.sep("RETURNED_DATE");

	outReg = NULL;

	for (int index = 0; index < array.Count(); index++)
	{
		const TRCDKeyContainer *container = array[index];

		if (container->key.type == TYPE_OUTREG &&
			((const TOutReg *) container)->receiverType == UCN_INSTANCE_LOWER)
		{
			outReg = (const TOutReg *) container;
		}
	}

	law_line.put_flag("ACT_SENT_F", outReg != NULL);

	if (outReg)
	{
		law_line.put("ACT_SENT_DATE", outReg->date);
		law_line.put("ACT_SENT_INFO", str(outReg->text));
		law_line.put_court("ACT_SEND_COURT", outReg);
	}
	else
	{
		law_line.sep("ACT_SENT_DATE");
		law_line.put("ACT_SENT_INFO", "");
		law_line.sep("ACT_SENT_COURT");
	}

	law_line.put_flag("ARCHIVED_F", archived);

	if (archived)
	{
		law_line.put("ARCHIVE_INDEX", archive.archiveIndex);
		law_line.put("ARCHIVE_NO", archive.archiveNo);
		law_line.put_user("ARCHIVED_BY_NAME", LOG_ARCHIVE, archive.key, '\0', NULL);
		law_line.put("ARCHIVED_DATE", archive.archiveDate);
		law_line.put("ARCHIVE_YEAR", archive.archiveYear);
		//law_line.sep("ARCHIVE_VOLUME");	// 2017:051 spec 2.7:comout
		law_line.put("ARCHIVE_BUNCH", archive.linkNo);
	}
	else
	{
		law_line.sep("ARCHIVE_INDEX");
		law_line.sep("ARCHIVE_NO");
		law_line.put("ARCHIVED_BY_NAME", "");
		law_line.sep("ARCHIVED_DATE");
		law_line.sep("ARCHIVE_YEAR");
		//law_line.sep("ARCHIVE_VOLUME");	// 2017:051 spec 2.7:comout
		law_line.sep("ARCHIVE_BUNCH");
	}

	law_line.put("REMARK", lawsuit.text);

	// 2017:051 spec 2.7
	//law_line.assert_count(48);
	law_line.put_flag("ENFORCEMENT_F", !lawsuit.forceDate.Empty());

	if (!lawsuit.forceDate.Empty())
		law_line.put("ENFORCEMENT_DATE", lawsuit.forceDate);
	else
		law_line.sep("ENFORCEMENT_DATE");

	bool killed = archived && !archive.killDate.Empty();
	law_line.put_flag("DESTROYED_F", killed);

	if (killed)
	{
		law_line.put("DESTROYED_PROTOCOL", archive.killerNo);
		law_line.put("DESTROYED_DATE", archive.killDate);
		law_line.put("DESTROYED_INFO", str(archive.killRemark));
	}
	else
	{
		law_line.sep("DESTROYED_PROTOCOL");
		law_line.sep("DESTROYED_DATE");
		law_line.put("DESTROYED_INFO", "");
	}

	if (decidizer)
		law_line.put("ACT_ID", csv_uuid(*decidizer));
	else
		law_line.sep("ACT_ID");

	const TOutReg *sent = NULL;

	for (int index = array.Count() - 1; index >= 0; index--)
	{
		const TRCDKeyContainer *container = array[index];

		if (container->key.type == TYPE_OUTREG &&
			strchr(UCN_RECEIVER_SENDERS, ((const TOutReg *) container)->receiverType))
		{
			sent = (const TOutReg *) container;
			break;
		}
	}

	law_line.put_flag("SENT_F", sent != NULL);

	if (sent)
	{
		law_line.put("SENT_TYPE", send_type(sent));
		law_line.put_court("SENT_AC_CODE", sent);
		law_line.put("SENT_DATE", sent->date);
	}
	else
	{
		law_line.put("SENT_TYPE", "");
		law_line.sep("SENT_AC_CODE");
		law_line.sep("SENT_DATE");
	}

	law_line.assert_count(58);
	lawsuits_file.Write(law_line);
	ExportEDocs(lawsuit, law_uuid, "Case", "Дело");
	ExportHistory(lawsuit, law_uuid, "Case");

	// 2017:052 spec 2.7 new format
	ExportSideWinds(lawsuit_sides_file, lawsuit, law_uuid, NULL, SIDE_LAWSUIT);

	TGArray<TDecision> virtuals;

	for (int index = 0; index < array.Count(); index++)
	{
		const TRCDKeyContainer *container = array[index];

		if (strchr(KIND_SESSIONS, container->kind))
		{
			ExportSession(array, index, law_uuid, lawsuit.college, finalizer, virtuals);
		}
		else if (container->key.type == TYPE_SURROUND || container->key.type == TYPE_OUTREG)
		{
			ExportDocument((const TDatedContainer *) container, law_uuid);
			// 2017:053 spec 2.7
			if (container->key.type == TYPE_OUTREG)
			{
				const TOutReg *outReg = (const TOutReg *) container;

				if (outReg->lawsuit.type && strchr(TYPE_LAWSUITS, outReg->lawsuit.type) && !outReg->returned.Empty())
					ExportReturn(outReg, lawsuit, array);
			}
		}
		else if (container->key.type == TYPE_INREG)
		{
			TInReg *inReg = (TInReg *) container;

			if (strchr(TYPE_LAWSUITS, inReg->lawsuit.type) && inReg->sessionKind &&
				strchr(KIND_ENDOCS, inReg->decisionKind))
			{
				ExportAppeal(inReg, law_uuid, virtuals);
			}
		}
	}

	// 2017:054 spec 2.7
	ExportMovement(lawsuit, law_uuid);
}

void TExportCSV::ExportLawsuits()
{
	for (int year = group.minLawsuitDate.year; year <= group.maxLawsuitDate.year; year++)
	{
		msql m;
		int final;
		TLawsuit lawsuit;

		m.printf("Дела от %dг...", year);
		wait->SetText(str(m));

		for (int start = 1; start < NO_MAX; start = final + 1)
		{
			final = start == 99900 ? NO_MAX : start + 99;
			//final = start == 99950 ? NO_MAX : start + 49;
			//final = start == 99000 ? NO_MAX : start + 999;

			m.Begin(lawsuit.Name);
			m.AddRange("F_NO", start, final);
			m.AddLong("F_YEAR", year);
			m.AddRange("F_DATE", group.minLawsuitDate, group.maxLawsuitDate);

			TGArray<TLawsuit> lawsuits;
			lawsuit.LoadArray(lawsuits, str(m));

			for (int i = 0; i < lawsuits.Count(); i++)
			{
				lawsuit << lawsuits[i];
				ExportLawsuit(lawsuit);
				wait->TickCount(++count, "обработени дела");
			}
		}
	}
}

void TExportCSV::Execute()
{
	if (group.exportRequests)
	{
		if (group.minRequestDate.Empty())
			group.minRequestDate = ADMC_DATE_MIN;

		if (group.maxRequestDate.Empty())
			group.maxRequestDate = ADMC_DATE_MAX;

		mstr m("входящи документи ");
		m.printf("постъпили в периода от %D до %D", &group.minRequestDate, &group.maxRequestDate);

		items = str(m);
		UpdateStart();
		ExportRequests();
		UpdateCount();
		UpdateFinal();
	}

	if (group.exportLawsuits)
	{
		if (group.exportRequests)
			dprintf("\n\n\n");

		if (group.minLawsuitDate.Empty())
			group.minLawsuitDate = ADMC_DATE_MIN;

		if (group.maxLawsuitDate.Empty())
			group.maxLawsuitDate = ADMC_DATE_MAX;

		mstr m("дела ");
		m.printf("образувани в периода от %D до %D", &group.minLawsuitDate, &group.maxLawsuitDate);

		UpdateStart();
		items = str(m);
		ExportLawsuits();
		UpdateCount();
		UpdateFinal();
	}

	UpdateShow();
}

void TExportCSV::UpdateCount()
{
	dprintf("\n"
		"Експортирани %s: %d\n"
		"%s: %d\n",
		items, count, whatWrong, wrong);
}

void export_csv(TWindow *parent)
{
	TExportCSVGroup group;

	group.minRequestDate = ADMC_DATE_MIN;
	group.minLawsuitDate = ADMC_DATE_MIN;

	if (TExportCSVDialog(parent, &group).Execute() == IDOK)
	{
		VCLP = new TVolUCLP;
		//VCLP = new TUCLP;
		TVirtPtr VP(VCLP);

		TExportCSV(parent, group).Execute();
	}
}
#endif  // ADMIN