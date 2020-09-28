#include "cac.h"

// ----- TEPortTable -------------------------------------------------------
class TEPortTable : public TGroup
{
public:
	TEPortTable(const char *name, TGroup *(*tClone)());

	virtual void FloatCriteria(msql &m, const char *fields = NULL);
	virtual bool Insert();

	TRCDKey key;
	long what;
	char kind;
	CDate date;
	char ucn[SIZE_OF_UCN];
	char ucnType;
	char involvement;
	char eventStatus;
	long subpoenaNo;
	char electric;
};

TEPortTable::TEPortTable(const char *name, TGroup *(*tClone)())
	: TGroup(name, tClone)
{
	Add(
		NEW_RCDKEY("F_", &key),
		new TLong("F_WHAT", &what),
		new TCharAlias("F_KIND", &kind),
		new TDate("F_DATE", &date, false),
		new TString("F_UCN", ucn, sizeof ucn),
		new TCharAlias("F_UCN_TYPE", &ucnType),
		new TCharAlias("F_INVOLVEMENT", &involvement),
		new TCharAlias("F_EVENT_STATUS", &eventStatus),
		new TLong("F_SUBPOENA_NO", &subpoenaNo),
		new TChar("F_ELECTRIC", &electric),
		NULL
	);
}

void TEPortTable::FloatCriteria(msql &m, const char *fields)
{
	m.Begin(Name, fields);
	m.Add(this, "F_WHAT, F_NO, F_YEAR, F_TYPE, F_KIND, F_DATE, F_UCN, F_UCN_TYPE, F_INVOLVEMENT, F_EVENT_STATUS, "
		"F_SUBPOENA_NO, F_ELECTRIC");
}

bool TEPortTable::Insert()
{
	Delete(false);
	return TGroup::Insert();
}

// ----- TEPortGuid --------------------------------------------------------
class TEPortGuid : public TEPortTable  // see also TEPortGuidAsUCNGroup
{
public:
	TEPortGuid();
	static TGroup *Clone();

	virtual void ExactCriteria(msql &m, const char *fields = NULL);

	char guid[SIZE_OF_UUID];
	CDateTime stamp;
};

TEPortGuid::TEPortGuid()
	: TEPortTable("T_EPORT_GUID", TEPortGuid::Clone)
{
	Add(
		new TString("F_GUID", guid, sizeof guid),
		new TTimeStamp("F_STAMP", &stamp),
		NULL
	);

	descs = Descs + TABLE_EPORT_GUID;
}

TGroup *TEPortGuid::Clone()
{
	return new TEPortGuid();
}

void TEPortGuid::ExactCriteria(msql &m, const char *fields)
{
	m.Begin(Name, fields);
	m.Add(this, "F_GUID");
}

// ----- TEPortSend --------------------------------------------------------
enum
{
	EPORT_SEND  = 'S',
	EPORT_WAIT  = 'W',
	EPORT_ERROR = 'E'
};

class TEPortSend : public TEPortTable
{
public:
	TEPortSend();
	static TGroup *Clone();

	virtual void ExactCriteria(msql &m, const char *fields = NULL);
	virtual bool Insert();

	bool Send();

	long recordNo;
	char oper;
	char status;
	mstr text;

protected:
	enum
	{
		TEXT_LEN = 1024,
		TEXT_SIZE = TEXT_LEN + 1
	};
};

TEPortSend::TEPortSend()
	: TEPortTable("T_EPORT_SEND", TEPortSend::Clone)
{
	Add(
		new TLong("F_RECORD_NO", &recordNo),
		new TChar("F_OPER", &oper),
		new TChar("F_STATUS", &status),
		new TText("F_TEXT", &text, TEXT_SIZE),
		NULL
	);

	descs = Descs + TABLE_EPORT_SEND;
	status = EPORT_SEND;
}

TGroup *TEPortSend::Clone()
{
	return new TEPortSend();
}

void TEPortSend::ExactCriteria(msql &m, const char *fields)
{
	m.Begin(Name, fields);
	m.Add(this, "F_NO, F_YEAR, F_TYPE, F_RECORD_NO");
}

bool TEPortSend::Insert()
{
	msql m(Name, "MAX(F_RECORD_NO) AS F_RECORD_NO");

	m.Add(this, "F_NO, F_YEAR, F_TYPE");
	recordNo = dataBase->GetLong(str(m), "F_RECORD_NO");

	if (++recordNo <= 0)
		generator_limit(Name);

	return TEPortTable::Insert();
}

static const TRCDKey EMPTY_KEY = { 1, YEAR_MIN, TYPE_EMPTY };
static constant EPORTALOG_PREFIX = "EP: ";

bool TEPortSend::Send()
{
	mstr m;

	m.printf("%s%04d %c ", EPORTALOG_PREFIX, what, oper);
	text.cut(TEXT_LEN);

	if (ucnType)
		m.printf(" %s %s - ", UCNType->Seek(ucnType), ucn);

	bool result = Insert();
	m.cat(result ? "изпращане" : "грешка");

	TRCDKey tKey = key;

	if (tKey == EMPTY_KEY)
		tKey.no = tKey.year = 0;

	log(LOG_TRANSFER, 0, &tKey, kind, &date, str(m));
	return result;
}

// ----- internal functions --------------------------------------------------
bool lawsuit_blocked_limited(const TLawsuit *lawsuit)
{
	return XFER_EPORTAL == 2 && (lawsuit->flags & LAWSUIT_LIMITED);
}

static bool lawsuit_sendable(const TLawsuit *lawsuit)
{
	if (lawsuit_blocked_limited(lawsuit))
		return false;

	return XFER_EPORTAL && !(lawsuit->flagsY & LAWSUIT_Y_NOEPORT);
}

static void send_sideWind(const TSideWind *sideWind, const TLawsuit *lawsuit, char oper)
{
	TEPortSend send;

	send.what = WHAT_SIDEWIND;
	send.key = lawsuit->key;
	strcpy(send.ucn, sideWind->ucn);
	send.ucnType = sideWind->ucnType;
	send.involvement = sideWind->involvement;
	send.oper = oper;
	send.Send();
}

void send_sideWinds(const TGArray<TSideWind> *sideWinds, const TLawsuit *lawsuit, char oper)
{
	for (int i = 0; i < sideWinds->Count(); i++)
		send_sideWind((*sideWinds)[i], lawsuit, oper);
}

static bool send_container(const TRCDKeyContainer *container, char kind, const CDate &date, long what, char oper,
	long extraNo = 0)
{
	TEPortSend send;

	send.what = what;
	send.key = container->key;
	send.kind = kind;
	send.date = date;
	send.oper = oper;
	itom(extraNo, send.text);	// 2018-10-03: computing moved to upper levels
	return send.Send();
}

static void send_lawsuit(const TLawsuit *lawsuit, char oper)
{
	send_container(lawsuit, '\0', CDate(), WHAT_LAWSUIT, oper, PRINT_COMPOSITIONS != 0);
}

static void send_session(const TSession *session, long what, char oper)
{
	send_container(session, session->kind, session->date, what, oper, PRINT_JUDGEMENTS != 0);
}

static void send_decision(const TDecision *decision, char oper)
{
	// 2016:257 PRINT_JUDGEMENTS; 2018-10-03: LPR: PRINT_JUDGEMENTS != 0
	send_container(decision, decision->kind, decision->date, WHAT_ENDOC, oper, PRINT_JUDGEMENTS != 0);
}

static void send_ioreg(const TIOReg *ioReg, char oper, sign eportalSendable)
{
	if (eportalSendable)
	{
		long what = ioReg->key.type == TYPE_OUTREG ? WHAT_OUTREG : WHAT_INREG;

		send_container(ioReg, '\0', CDate(), what, oper, eportalSendable == -1);
	}
}

static void send_surround(const TSurround *surround, char oper)
{
	send_container(surround, '\0', CDate(), WHAT_SURROUND, oper);
}

static void lawsuit_send_full(const TLawsuit *lawsuit)
{
	TGArray<TSideWind> sideWinds;
	TGSArray<TRCDKeyContainer> containers;

	send_lawsuit(lawsuit, LOG_WRITE);
	load_sidebasicv(lawsuit, sideWinds);
	send_sideWinds(&sideWinds, lawsuit, LOG_WRITE);
	query_events(lawsuit, &containers);

	for (int i = 0; i < containers.Count(); i++)
	{
		const TRCDKeyContainer *container = containers[i];

		if (strchr(KIND_SESSIONS, container->kind))
		{
			const TSession *session = (const TSession *) container;

			if (session->IsEPortSession())
				send_session(session, WHAT_SESSION, LOG_WRITE);

			if (session->IsEPortSessEndoc())
				send_session(session, WHAT_SESS_ENDOC, LOG_WRITE);
		}
		else if (strchr(KIND_ENDOCS, container->kind))
			send_decision((const TDecision *) container, LOG_WRITE);
		// place surround support here
		else if (strchr(KIND_APPEAL_IN_DOCS, container->kind))
		{
			const TInReg *inReg = (const TInReg *) container;
			send_ioreg(inReg, LOG_WRITE, inReg->EPortalSendable());
		}
		else if (strchr(KIND_OUT_REG_DOCS, container->kind))
		{
			const TOutReg *outReg = (const TOutReg *) container;
			send_ioreg(outReg, LOG_WRITE, outReg->EPortalSendable());
		}
		else if (strchr(KIND_SURROUNDS, container->kind))
			send_surround((const TSurround *) container, LOG_WRITE);	// 2018-10-04
	}
}

static bool lawsuit_has_guid(const TLawsuit *lawsuit)
{
	TEPortGuid guid;
	msql m;

	guid.what = WHAT_LAWSUIT;
	guid.key = lawsuit->key;
	guid.FloatCriteria(m, "F_GUID");

	TQuery q(str(m));
	return q.Read();
}

static bool lawsuit_in_progress(const TLawsuit *lawsuit)
{
	TEPortSend send;
	msql m(send.Name, "F_NO");

	m.Add(lawsuit, "F_NO, F_YEAR, F_TYPE");
	m.AddChar("NOT F_STATUS", EPORT_ERROR);
	TQuery q(str(m));
	return q.Read();
}

static bool lawsuit_check_send(TWindow *parent, const TLawsuit *lawsuit, const char *message)
{
	if (lawsuit_has_guid(lawsuit))
		return true;

	if (lawsuit_in_progress(lawsuit))
		return true;

	TWaitWindow wait(parent, message);
	lawsuit_send_full(lawsuit);
	return false;
}

static bool lawsuit_check_send(TWindow *parent, const TLawsuit *lawsuit, char oper)
{
	if (!lawsuit_sendable(lawsuit))
		return false;

	if (oper == LOG_DELETE || !parent)
		return true;

	return lawsuit_check_send(parent, lawsuit, "Начално изпращане на делото към " EPORTAL_NAME "...");
}

// ----- public functions ----------------------------------------------------
bool eportal_check_send(TWindow *parent, const TLawsuit *lawsuit, long eportalMode)
{
	if (!lawsuit_sendable(lawsuit))
		return error("Делото е изключено от обмена с %s.", EPORTAL_NAME);

	if (eportalMode == EPORTAL_REVOKE)
		return true;

	lawsuit_check_send(parent, lawsuit, WAIT);
	return true;
}

void eportal_send(TWindow *parent, const TSideWind *sideWind, const TLawsuit *lawsuit, char oper)
{
	if (lawsuit_check_send(parent, lawsuit, oper))
		send_sideWind(sideWind, lawsuit, oper);
}

void eportal_send(TWindow *parent, const TGArray<TSideWind> *sideWinds, const TLawsuit *lawsuit, char oper)
{
	if (lawsuit_check_send(parent, lawsuit, oper))
		send_sideWinds(sideWinds, lawsuit, oper);
}

void eportal_send(const TRequest *request, char oper)
{
	if (XFER_EPORTAL)
		send_container(request, '\0', CDate(), WHAT_REQUEST, oper);
}

void eportal_send(TWindow *parent, const TLawsuit *lawsuit, char oper)
{
	if (lawsuit_check_send(parent, lawsuit, oper))
		send_lawsuit(lawsuit, oper);
}

void eportal_send(TWindow *parent, const TSession *session, const TLawsuit *lawsuit, long what, char oper)
{
	if (lawsuit_check_send(parent, lawsuit, oper))
		send_session(session, what, oper);
}

void eportal_send(TWindow *parent, const TDecision *decision, const TLawsuit *lawsuit, char oper)
{
	if (lawsuit_check_send(parent, lawsuit, oper))
		send_decision(decision, oper);
}

static void send_ioreg(TWindow *parent, const TIOReg *ioReg, TRCDKeyGroup *group, char oper, sign eportalSendable)
{
	if (XFER_EPORTAL && eportalSendable)
	{
		const TLawsuit *tLawsuit = dynamic_cast<const TLawsuit *> (group);

		if (tLawsuit)
		{
			if (lawsuit_check_send(parent, tLawsuit, oper))
				send_ioreg(ioReg, oper, eportalSendable);
		}
		else if (ioReg->lawsuit.type && strchr(TYPE_LAWSUITS, ioReg->lawsuit.type))
		{
			TLawsuit lawsuit;
			lawsuit.key = ioReg->lawsuit;

			if (lawsuit.Get() && lawsuit_check_send(parent, &lawsuit, oper))
				send_ioreg(ioReg, oper, eportalSendable);
		}
	}
}

void eportal_send(TWindow *parent, const TInReg *inReg, TRCDKeyGroup *group, char oper, long eportalSendable)
{
	send_ioreg(parent, inReg, group, oper, eportalSendable);
}

void eportal_send(TWindow *parent, const TOutReg *outReg, TRCDKeyGroup *group, char oper)
{
	send_ioreg(parent, outReg, group, oper, outReg->EPortalSendable());
}

bool eportal_side_assigned(const TRCDKey &lawsuit, char ucnType, const char *ucn, char involvement)
{
	if (ucnType == UCN_CITIZEN_UCN || (strchr(INVOLVEMENT_LAWERS, involvement) && ucnType == UCN_CITIZEN_CODE))
	{
		TEPortGuid guid;
		msql m(guid.Name, "F_GUID");

		m.AddLong("F_WHAT", involvement2assignment(involvement));
		m.AddLong("F_NO", lawsuit.no);
		m.AddLong("F_YEAR", lawsuit.year);
		m.AddChar("F_TYPE", lawsuit.type);
		m.AddString("F_UCN", ucn);
		m.AddChar("F_UCN_TYPE", ucnType);
		m.AddChar("F_INVOLVEMENT", involvement);

		TQuery q(str(m));
		return q.Read();
	}

	return false;
}

bool eportal_side_assigned(const TBaseWind *sideWind)
{
	return eportal_side_assigned(sideWind->key, sideWind->ucnType, sideWind->ucn, sideWind->involvement);
}

bool eportal_invoke_granted(const TRCDKey &lawsuit, char ucnType, const char *ucn, char involvement)
{
	TEPortAssign assign;
	msql m("T_EPORT_ASSIGN A "
		"JOIN T_INREG_EVENT IE ON IE.F_NO = A.F_NO AND IE.F_YEAR = A.F_YEAR AND IE.F_TYPE = A.F_TYPE "
		"JOIN T_INREG I ON I.F_NO = A.F_NO AND I.F_YEAR = A.F_YEAR AND I.F_TYPE = A.F_TYPE "
		"JOIN T_SIDE_WIND W ON W.F_NO = I.F_NO AND W.F_YEAR = I.F_YEAR AND W.F_TYPE = I.F_TYPE "
			"AND W.F_KIND = I.F_KIND AND W.F_DATE = I.F_DATE",
		"A.F_FLAGS");

	m.AddLong("A.F_LAWSUIT_NO", lawsuit.no);
	m.AddLong("A.F_LAWSUIT_YEAR", lawsuit.year);
	m.AddChar("A.F_LAWSUIT_TYPE", lawsuit.type);
	m.AddChar("A.F_INVOLVEMENT", involvement);
	m.AddChar("IE.F_RESULT", RESULT_EPORTAL_ALLOWED);
	m.AddLong("I.F_EPORTAL_MODE", EPORTAL_GRANT);
	m.AddChar("W.F_UCN_TYPE", ucnType);
	m.AddString("W.F_UCN", ucn);
	m.AddOrder("IE.F_DATE DESC");
	return AppName->GetLong(str(m), "F_FLAGS") & EPORTAL_SUBPOENA;
}

static bool einvoke_granted(const TSubpoena *subpoena)
{
	return eportal_invoke_granted(subpoena->key, subpoena->ucnType, subpoena->ucn, subpoena->involvement);
}

struct TSubkindToWhat
{
	const char *kinds;
	const char *genericKinds;
	long what;
};

static const TSubkindToWhat SubkToWhats[] =
{
	{ KIND_ENDOCS,			SUBPOENA_KIND_GENERIC_MESSAGES,	WHAT_ENDOC_MESSAGE },
	{ KIND_INREG_USER_EVENTS,	SUBPOENA_KIND_GENERIC_MESSAGES,	WHAT_APPEAL_MESSAGE },
	// 2016:319 FIX: disabled, these are for the main lawsuit/request
//	{ KIND_SESSIONS,			SUBPOENA_KIND_GENERIC_MESSAGES,	WHAT_LAWSUIT_MESSAGE },
	{ KIND_SESSIONS,			SUBPOENA_KIND_GENERIC_WRITS,		WHAT_SESSION_SUBPOENA },
	// 2018-10-02: LPR: sess-endoc-s
	{ KIND_SESSIONS,			SUBPOENA_KIND_GENERIC_MESSAGES,	WHAT_ENDOC_MESSAGE },
	{ NULL, NULL, 0 }
};

static void eportal_send(const TSubpoena *subpoena, const TLawsuit *lawsuit, char oper, const char *receiver, char electric)
{
	if (lawsuit && lawsuit_sendable(lawsuit) && (any(receiver) || oper == LOG_DELETE))
	{
		const TSubkindToWhat *sw;

		SubpoenaKind->Seek(subpoena->subpoenaKind);

		for (sw = SubkToWhats; sw->what; sw++)
			if (strchr(sw->kinds, subpoena->kind) && strchr(sw->genericKinds, SubpoenaKind->genericKind))
				break;

		if (sw->what && eportal_side_assigned(subpoena) && (einvoke_granted(subpoena) || oper == LOG_DELETE))
		{
			TEPortSend send;

			send.what = sw->what;
			send << subpoena;
			send.oper = oper;
			send.text.cat(receiver);
			send.text.rep('\v', ' ');

			if (electric)
			{
				send.electric = electric;
				send.what += (WHAT_SUBPOENA_FILE_BASE - WHAT_SUBPOENA_BASE);
			}

			send.Send();
		}
	}
}

void eportal_send(const TSubpoena *subpoena, const TLawsuit *lawsuit, char oper, const char *receiver)
{
	eportal_send(subpoena, lawsuit, oper, receiver, '\0');
}

struct TElectricToWhat
{
	const char *kinds;
	int electricType;
	long what;
	bool fragment;
};

static const TElectricToWhat ElectricToWhats[] =
{
	{ KIND_INDOCS, TYPE_IMAGE, WHAT_REQUEST_FILE, true },
	{ KIND_SESSIONS, TYPE_TEXT, WHAT_PRIVATE_PROTOCOL_FILE, false },
	{ KIND_SESSIONS, TYPE_HTML, WHAT_PUBLIC_PROTOCOL_FILE, false },
	{ KIND_ENDOCS, TYPE_TEXT, WHAT_PRIVATE_ACT_FILE, false },
	{ KIND_ENDOCS, TYPE_HTML, WHAT_PUBLIC_ACT_FILE, false },
	{ C2S[KIND_MOTIVES], TYPE_TEXT, WHAT_PRIVATE_MOTIVE_FILE, false },
	{ C2S[KIND_MOTIVES], TYPE_HTML, WHAT_PUBLIC_MOTIVE_FILE, false },
	{ KIND_OUT_REG_DOCS, TYPE_TEXT, WHAT_OUTREG_FILE, true },
	{ NULL, 0, 0 }
};

void eportal_delete(const TElectric *electric, const TLawsuit *lawsuit, const TRCDKeyContainer *container)
{
	if (XFER_EPORTAL)
	{
		if (!electric->extraNo)
		{
			// 2016:316 LPR: fixed NULL lawsuit for IOReg and enabled indocs
			TLawsuit tLawsuit;

			if (!lawsuit && strchr(TYPE_IOREGS, container->key.type))
			{
				const TIOReg *ioReg = (const TIOReg *) container;

				if (ioReg->lawsuit.type && strchr(TYPE_LAWSUITS, ioReg->lawsuit.type))
				{
					tLawsuit.key = ioReg->lawsuit;

					if (tLawsuit.Get("F_FLAGS"))
						lawsuit = &tLawsuit;
				}
			}

			if ((lawsuit && lawsuit_sendable(lawsuit)) || strchr(KIND_INDOCS, container->kind))
			{
				const TElectricToWhat *ew;

				for (ew = ElectricToWhats; ew->what; ew++)
					if (strchr(ew->kinds, electric->kind) && ew->electricType == electric->extraType)
						break;

				if (ew->what)
				{
					TEPortSend send;

					send.what = ew->what;
					send << electric;
					send.oper = LOG_DELETE;
					send.electric = (char) (electric->extraType + '0');

					if (electric->kind == KIND_MOTIVES)
						send.kind = TYPECHECK_PTR_CAST(const TDecisionRider, container)->decision->kind;

					// 2016:313 LPR
					if (ew->fragment)
					{
						send.kind = '\0';
						send.date.Clear();
					}

					send.Send();
				}
			}
		}
		else if (electric->ExtraSubpoena() && electric->subpoenaKind)
		{
			TSubpoena subpoena;

			subpoena << electric;
			subpoena << container;
			subpoena.subpoenaNo = electric->extraNo;
			eportal_send(&subpoena, lawsuit, LOG_DELETE, "", (char) (electric->extraType + '0'));
		}
	}
}

static int involvement2registration(char involvement)
{
	return strchr(INVOLVEMENT_LAWERS, involvement) ? WHAT_LAWYER_REGISTRATION : WHAT_PERSON_REGISTRATION;
}

bool eportal_guid(const TUCNGroup *ucnGroup, char involvement)
{
	TEPortGuid guid;
	msql m(guid.Name, "F_GUID");

	m.AddLong("F_WHAT", involvement2registration(involvement));
	m.AddLong("F_NO", EMPTY_KEY.no);
	m.AddLong("F_YEAR", EMPTY_KEY.year);
	m.AddChar("F_TYPE", EMPTY_KEY.type);
	m.AddString("F_UCN", ucnGroup->ucn);
	m.AddChar("F_UCN_TYPE", ucnGroup->ucnType);
	TQuery q(str(m));
	return q.Read();
}

void eportal_send(const TUCNGroup *ucnGroup, char involvement)
{
	TEPortSend send;

	send.what = involvement2registration(involvement);
	send.key = EMPTY_KEY;
	send.ucnType = ucnGroup->ucnType;
	strcpy(send.ucn, ucnGroup->ucn);
	send.oper = LOG_WRITE;
	send.Send();
}

void eportal_send(const TEPortAssign *assign, const TSideWind *sideWind, char oper, long what)
{
	TEPortSend send;

	send.what = what;
	send.oper = oper;
	send.key = assign->lawsuit;
	send.ucnType = sideWind->ucnType;
	strcpy(send.ucn, sideWind->ucn);
	send.involvement = assign->involvement;
	send.Send();
}

void eportal_send(TWindow *parent, const TSurround *surround, const TLawsuit *lawsuit, char oper)
{
	if (lawsuit_check_send(parent, lawsuit, oper))
		send_surround(surround, oper);
}

const char *not_in_progress(TRCDKeyGroup *lawsuit, char)
{
	return lawsuit_in_progress((TLawsuit *) lawsuit) ? "В момента се изпращат или изтриват данни за делото" : NULL;
}

static bool lawsuit_check_unlock(TLawsuit *lawsuit)
{
	if (lawsuit_blocked_limited(lawsuit))
		return error("Делото е с ограничен достъп.");

	if (lawsuit_has_guid(lawsuit) && !ask("Делото има идентификатор в %s. %s", EPORTAL_NAME, Q_CONTINUE))
		return false;

	if (lawsuit->flagsY & LAWSUIT_Y_NOEPORT)
	{
		if (!ask("Внимание! Делото е било изтрито от %s. %s", EPORTAL_NAME, Q_CONTINUE))
			return false;

		lawsuit->flagsY &= ~LAWSUIT_Y_NOEPORT;

		if (!lawsuit->Update("F_FLAGS_Y"))
			return false;
	}

	return true;
}

void eportal_send_unlock(TWindow *parent, TLawsuit *lawsuit)
{
	if (lawsuit_check_unlock(lawsuit))
	{
		log(LOG_SEND, 0, lawsuit, "на дело към " EPORTAL_NAME);
		TWaitWindow wait(parent, "Изпращане на движението на делото към " EPORTAL_NAME "...");
		lawsuit_send_full(lawsuit);
	}
}

static void load_movement_guids(const TLawsuit *lawsuit, TGArray<TEPortGuid> &guids, int minWhat)
{
	TEPortGuid guid;
	msql m(guid.Name);

	m.Add(lawsuit, "F_NO, F_YEAR, F_TYPE");
	m.Add("F_WHAT");
	m.printf(">= %ld", minWhat);
	m.AddOrder("F_WHAT DESC");
	guid.LoadArray(guids, str(m));

	// 2018-10-23: LPR: FIX: different key-s
	static constant SUBTABLES[] = { "T_SURROUND T", "T_OUTREG T", "T_INREG T", NULL };

	for (int i = 0; SUBTABLES[i]; i++)
	{
		mstr tables(SUBTABLES[i]);

		tables.cat(" JOIN T_EPORT_GUID G ON G.F_NO = T.F_NO AND G.F_YEAR = T.F_YEAR AND G.F_TYPE = T.F_TYPE");
		m.Begin(str(tables), "G.*");
		m.AddLong("T.F_LAWSUIT_NO", lawsuit->key.no);
		m.AddLong("T.F_LAWSUIT_YEAR", lawsuit->key.year);
		m.AddChar("T.F_LAWSUIT_TYPE", lawsuit->key.type);
		m.AddOrder("F_WHAT DESC");

		TQuery q(str(m));
		while (q.Read())
		{
			guid << q;
			guids.InsertCopy(&guid, 0);
		}
	}
}

static bool delete_movement_guids(TGArray<TEPortGuid> &guids)
{
	bool result = true;

	for (int i = 0; i < guids.Count(); i++)
	{
		TEPortSend send;

		send << guids[i];
		send.oper = LOG_DELETE;

		if (!send.Send())
			result = false;
	}

	return result;
}

void eportal_delete_lock(TWindow *parent, TLawsuit *lawsuit)
{
	TGArray<TEPortGuid> guids;
	mstr m;

	load_movement_guids(lawsuit, guids, WHAT_LAWSUIT);
	m.printf("%s %ld/%ld", Type->Seek(lawsuit->key.type), lawsuit->key.no, lawsuit->key.year);

	if (!(lawsuit->flagsY & LAWSUIT_Y_NOEPORT))
	{
		if (!ask("Внимание! След изтриване, последващите действия по %s няма да бъдат изпращани към %s. %s",
			str(m), EPORTAL_NAME, Q_CONTINUE))
		{
			return;
		}

		lawsuit->flagsY |= LAWSUIT_Y_NOEPORT;

		if (!lawsuit->Update("F_FLAGS_Y"))
			return;
	}
	else if (!guids.Count())
	{
		info("Няма идентификатори на успешно регистрирани в %s обекти по %s.", EPORTAL_NAME, str(m));
		return;
	}

	log(LOG_DELETE, 0, lawsuit, "на дело от " EPORTAL_NAME);
	TWaitWindow wait(parent, "Изтриване на делото от " EPORTAL_NAME "...");
	delete_movement_guids(guids);
}

bool eportal_delete_lawsuit(const TLawsuit *lawsuit, int minWhat)
{
	TGArray<TEPortGuid> guids;
	load_movement_guids(lawsuit, guids, minWhat);
	return delete_movement_guids(guids);
}

void eportal_delete_movement(const TLawsuit *lawsuit)
{
	if (XFER_EPORTAL)
		eportal_delete_lawsuit(lawsuit, WHAT_SESSION);
}

bool eportal_delete_limited(const TLawsuit *lawsuit)
{
	return eportal_delete_lawsuit(lawsuit, WHAT_LAWSUIT);
}

// ----- TFloatEPortalGroup --------------------------------------------------
enum
{
	INPUT_LOG,
	INPUT_GUID,
	OUTPUT_ALL_RECORDS = 0,
	OUTPUT_SUM_ORIGINS,
	OUTPUT_TOTAL_WHATS
};

class TFloatEPortalGroup : public TFloatGroup
{
public:
	TFloatEPortalGroup();

	bool LogDate();

	long input;
	CDate minLogDate;
	CDate maxLogDate;
	CDate minGuidDate;
	CDate maxGuidDate;
	TIntegerArray whats;
	char types[SIZE_OF_TYPES];
	long minNo;
	long maxNo;
	long minYear;
	long maxYear;
	long output;
	TIntegerArray realWhats;

	virtual void FormCriteria(msql &m);

protected:
	void BeginCriteria(msql &m, const char *Name);
};

TFloatEPortalGroup::TFloatEPortalGroup()
	: TFloatGroup("T_LOG", NULL)
{
	Add(
		new TRadio("F_LOG", &input, INPUT_LOG),
		new TRadio("F_GUID", &input, INPUT_GUID),
		new TDate("F_MIN_LOG_DATE", &minLogDate, false),
		new TDate("F_MAX_LOG_DATE", &maxLogDate, false),
		new TDate("F_MIN_GUID_DATE", &minGuidDate, false),
		new TDate("F_MAX_GUID_DATE", &maxGuidDate, false),
		new TCharList("F_TYPES", types, sizeof types),
		new TLong("F_MIN_NO", &minNo),
		new TLong("F_MAX_NO", &maxNo),
		new TLong("F_MIN_YEAR", &minYear),
		new TLong("F_MAX_YEAR", &maxYear),
		new TRadio("F_ALL_RECORDS", &output, OUTPUT_ALL_RECORDS),
		new TRadio("F_SUM_ORIGINS", &output, OUTPUT_SUM_ORIGINS),
		new TRadio("F_TOTAL_WHATS", &output, OUTPUT_TOTAL_WHATS),
		NULL
	);
}

void TFloatEPortalGroup::BeginCriteria(msql &m, const char *Name)
{
	m.Begin(Name);

	CDateTime minLogStamp(minLogDate.day, minLogDate.month, minLogDate.year, 0, 0, 0);
	CDateTime maxLogStamp(maxLogDate.day, maxLogDate.month, maxLogDate.year, 23, 59, 59);

	if (maxLogDate.Empty())
		maxLogStamp.hour = maxLogStamp.minute = maxLogStamp.second = 0;

	m.AddRange("F_STAMP", minLogStamp, maxLogStamp);
}

void TFloatEPortalGroup::FormCriteria(msql &m)
{
	if (input == INPUT_LOG)
	{
		BeginCriteria(m, "T_LOG");
		m.AddChar("F_GOP", LOG_TRANSFER);
		m.AddLike("F_TEXT", EPORTALOG_PREFIX, false);
	}
	else
	{
		BeginCriteria(m, "T_EPORT_GUID");
		m.AddRange("F_DATE", minGuidDate, maxGuidDate);
	}

	m.AddChars("F_TYPE", types);
	m.AddRange("F_NO", minNo, maxNo);
	m.AddRange("F_YEAR", minYear, maxYear);

	// 2018-09-28: LPR: add whats criteria
	realWhats.Clear();

	for (int i = 0; i < whats.Count(); i++)
	{
		long first = whats[i];
		long last;

		switch (first)
		{
			case WHAT_SESSION_JUDGE_FIRST : last = WHAT_SESSION_JUDGE_LAST; break;
			case WHAT_DECISION_JUDGE_FIRST : last = WHAT_DECISION_JUDGE_LAST; break;
			default : last = first;
		}

		for (long what = first; what <= last; what++)
			realWhats.Add(what);
	}

	if (input == INPUT_GUID)
		m.AddLongs("F_WHAT", realWhats);

	if (output == OUTPUT_ALL_RECORDS)
	{
		if (LogDate())
			m.AddOrder("F_STAMP");
		else
			order = ORDER_BY_DTYN;
	}
}

bool TFloatEPortalGroup::LogDate()
{
	return input == INPUT_LOG || !minLogDate.Empty() || !maxLogDate.Empty();
}

#define eporgroup OFFICE_GROUP_CAST(TFloatEPortalGroup)

static void eporgroup_date_name(mstr &m, TGroup *group)
{
	m.cat(eporgroup->LogDate() ? "изпращане" : "документ/събитие");
}

#undef eporgroup

static offield eporgroup_fields[] =
{
	{ "F_DATE_NAME", eporgroup_date_name },
	{ NULL, NULL }
};

static long what_visual(long what)
{
	if (what > WHAT_SESSION_JUDGE_FIRST && what <= WHAT_SESSION_JUDGE_LAST)
		what = WHAT_SESSION_JUDGE_FIRST;
	else if (what > WHAT_DECISION_JUDGE_FIRST && what <= WHAT_DECISION_JUDGE_LAST)
		what = WHAT_DECISION_JUDGE_FIRST;

	return what;
}

#define eportsend OFFICE_GROUP_CAST(TEPortSend)

static void eportsend_origin(mstr &m, TGroup *group)
{
	if (eportsend->key.type != EMPTY_KEY.type)
	{
		Type->Seek(eportsend->key.type);
		m.printf("%s %ld/%ld", Type->alias, eportsend->key.no, eportsend->key.year);

		if (strchr(TYPE_LAWSUITS, eportsend->key.type))
		{
			TLawsuit lawsuit;
			lawsuit.key = eportsend->key;

			if (lawsuit.Try("F_FLAGS"))
				m.cat(lawsuit.flags & LAWSUIT_LIMITED ? "\vОграничен достъп" : "");
			else
				m.cat("\vГрешка при четене");
		}
	}
}

static void eportsend_what(mstr &m, TGroup *group)
{
	long what = what_visual(eportsend->what);

	m.cat(What->Seek(what));

	// 2018-10-09: IRQ: extra data for registrations
	if (eportsend->ucnType)
	{
		mstr w;

		if (what == WHAT_PERSON_REGISTRATION)
		{
			if (eportsend->ucnType == UCN_CITIZEN_UCN)
				w.cat(eportsend->ucn);
		}
		else if (what == WHAT_LAWYER_REGISTRATION)
		{
			TKnownPerson known;

			strcpy(known.ucn, eportsend->ucn);
			known.ucnType = eportsend->ucnType;
			known.involvement = INVOLVEMENT_LEGALESE;

			if (known.Try("F_PERSONAL_NO"))
				w.cat(known.personalNo);
		}

		if (eportsend->what == WHAT_PERSON_REGISTRATION || what == WHAT_LAWYER_REGISTRATION)
		{
			TCitizen citizen;

			strcpy(citizen.ucn, eportsend->ucn);
			citizen.ucnType = eportsend->ucnType;

			if (citizen.Try())
			{
				w.sep(" ");
				w.printf("%s %s %s %s", citizen.name, citizen.reName, citizen.family, citizen.reFamily);
				w.compress();
			}

			if (any(w))
			{
				m.sep("\v");
				m.cat(w);
			}
		}
	}
}

static void eportsend_oper(mstr &m, TGroup *group)
{
	if (eportsend->oper)
		m.printf(GOP->Seek(eportsend->oper));
}

static void eportsend_count(mstr &m, TGroup *group) { itom(eportsend->recordNo, m); }

#undef eportsend

static offield eportsend_fields[] =
{
	{ "F_ORIGIN", eportsend_origin },
	{ "F_DATE",   NULL },	
	{ "F_WHAT",   eportsend_what },
	{ "F_OPER",   eportsend_oper },
	{ "F_COUNT",  eportsend_count },
	{ NULL, NULL }
};

// ----- TFloatEPortalArray --------------------------------------------------
class TFloatEPortalArray : public TGArray<TEPortSend>
{
public:
	TFloatEPortalArray(long output);

	int Find(const TEPortSend *send) { return TGroupArray::Find(send); }

protected:
	static bool lesseql_sum_origins(const long t1, const long t2);
	static bool lesseql_total_whats(const long t1, const long t2);
};

TFloatEPortalArray::TFloatEPortalArray(long output)
{
	lesseql = (output == OUTPUT_SUM_ORIGINS) ? lesseql_sum_origins : lesseql_total_whats;
}

bool TFloatEPortalArray::lesseql_sum_origins(const long t1, const long t2)
{
	const TRCDKey &key1 = ((const TEPortSend *) t1)->key;
	const TRCDKey &key2 = ((const TEPortSend *) t2)->key;

	return key1.year != key2.year ? key1.year < key2.year : key1.no != key2.no ? key1.no < key2.no :
		key1.type <= key2.type;
}

bool TFloatEPortalArray::lesseql_total_whats(const long t1, const long t2)
{
	const what1 = ((const TEPortSend *) t1)->what;
	const what2 = ((const TEPortSend *) t2)->what;
	TWhat tWhat;

	What->Seek(what_visual(what1));
	tWhat.Seek(what_visual(what2));
	return strcmp(What->name, tWhat.name) <= 0;
}

// ----- TFloatEPortalQuery --------------------------------------------------
class TFloatEPortalQuery : public TFloatQuery
{
public:
	TFloatEPortalQuery(TWindow *parent, const char *criteria, TFloatEPortalGroup *tGroup);

	void Execute();

protected:
	TFloatEPortalGroup *group;

	virtual void Count();
};

TFloatEPortalQuery::TFloatEPortalQuery(TWindow *parent, const char *criteria, TFloatEPortalGroup *tGroup)
	: TFloatQuery(parent, criteria, NULL, "Справка за обмен на данни с " EPORTAL_NAME), group(tGroup)
{
	switch (group->output)
	{
		case OUTPUT_ALL_RECORDS : name = (group->input == INPUT_LOG) ? "eportq_al.htm" : "eportq_ag.htm"; break;
		case OUTPUT_SUM_ORIGINS : name = "eportq_so.htm"; break;
		case OUTPUT_TOTAL_WHATS : name = "eportq_tw.htm"; break;
	}
}

void TFloatEPortalQuery::Execute()
{
	if (office.Open(name))
	{
		TWindowHideParent hide(parent);

		GetMarkers();
		office.Emit(NULL, tr1start, eporgroup_fields, group);
		Count();
		office.Emit(tr1final, NULL, float_fields, this);
		Show();
	}
}

void TFloatEPortalQuery::Count()
{
	TQuery q(criteria);
	TLog log;
	TEPortSend send;
	TEPortGuid guid;
	TFloatEPortalArray array(group->output);

	while (q.Read() && Increment())
	{
		if (group->input == INPUT_GUID)
		{
			send << q;

			if (group->LogDate())
			{
				guid << q;
				send.date = (CDate &) guid.stamp;
			}
		}
		else
		{
			long what;
			char oper;

			log << q;
			// 2018-10-09: IRQ: +related: scan ucnType, ucn
			char ucnType[SIZE_OF_UCN_TYPE];
			int fields = sscanf(log.text + strlen(EPORTALOG_PREFIX), "%ld %c %3c %10c", &what, &oper, ucnType, send.ucn);

			if (fields < 2)
			{
				count--;
				continue;
			}

			if ((group->realWhats.Count() ? group->realWhats : WHAT_LOGS).Find(what) == INT_MAX ||
				(oper != LOG_WRITE && oper != LOG_DELETE))
			{
				count--;
				continue;
			}

			if (fields == 4)
			{
				send.ucn[LENGTH_OF_UCN] = '\0';
				ucnType[LENGTH_OF_UCN_TYPE] = '\0';

				if (atoi(send.ucn))
				{
					if (!strcmp(ucnType, UCNType->Seek(UCN_CITIZEN_UCN)))
						send.ucnType = UCN_CITIZEN_UCN;
					else if (!strcmp(ucnType, UCNType->Seek(UCN_CITIZEN_CODE)))
						send.ucnType = UCN_CITIZEN_CODE;
				}
			}

			send.key = log.key;
			send.what = what;
			send.date = (CDate &) log.stamp;
			send.oper = oper;
		}

		if (group->output == OUTPUT_ALL_RECORDS)
			office.Emit(tr1start, tr1final, eportsend_fields, &send);
		else
		{
			if (!strchr(TYPE_ORIGINS, send.key.type) && send.key.type != TYPE_EMPTY)
			{
				TRCDKeyContainer *container = type2RCDKeyContainer(send.key.type);
				TVirtPtr VP(container);

				container->key = send.key;
				find_origin(container, send.key);
			}

			int index = array.Find(&send);

			if (index == INT_MAX)
				index = array.AddCopy(&send);

			array[index]->recordNo++;
		}
	}

	if (group->output != OUTPUT_ALL_RECORDS)
		for (int i = 0; i < array.Count(); i++)
			office.Emit(tr1start, tr1final, eportsend_fields, array[i]);
}

static void query_eportal(TWindow *parent, const char *criteria, TFloatEPortalGroup *group)
{
	TFloatEPortalQuery(parent, criteria, group).Execute();
}

#include "EPorTypeListFace.h"
#include "EPorTypeListFace.cpp"

#include "WhatListBox.h"
#include "WhatListBox.cpp"

#include "FloatEPortalDialog.h"
#include "FloatEPortalDialog.cpp"

void eportal_query(TWindow *parent)
{
	TFloatEPortalGroup group;
	TFloatEPortalDialog(parent, &group).Execute();
}

bool eportal_delete_update(const TIndexContainer *container, long what)
{
	return send_container(container, container->kind, container->date, what, LOG_DELETE);
}

bool eportal_delete_public_file(const TSession *session, long what)
{
	TEPortSend send;

	send.what = what;
	send.key = session->key;
	send.kind = session->kind;
	send.date = session->date;
	send.oper = LOG_DELETE;
	send.electric = TYPE_HTML + '0';
	return send.Send();
}

#define eportguid OFFICE_GROUP_CAST(TEPortGuid)

static void eportguid_lawsuit(mstr &m, TGroup *group)
{
	if (eportguid->key.type)
	{
		Type->Seek(eportguid->key.type);
		m.printf("%s %ld/%ld", Type->alias, eportguid->key.no, eportguid->key.year);
	}
}

static void eportguid_person(mstr &m, TGroup *group)
{
	if (atob(eportguid->ucn))
	{
		TCitizen citizen;

		strcpy(citizen.ucn, eportguid->ucn);
		citizen.ucnType = eportguid->ucnType;

		if (citizen.Get())
		{
			m.sep(" ");
			m.printf("%s %s %s %s", citizen.name, citizen.reName, citizen.family, citizen.reFamily);
			m.compress();
		}
		else
			m.printf("%s %s", UCNType->Seek(eportguid->ucnType), eportguid->ucn);

		if (eportguid->involvement)
			m.printf(" - %s", Involvement->Seek(eportguid->involvement));
	}
}

#undef eportguid

static offield eportguid_fields[] =
{
	{ "F_LAWSUIT",  eportguid_lawsuit },
	{ "F_PERSON",   eportguid_person },
	{ "F_EINVOKE",  NULL },
	{ NULL, NULL }
};

static void eportal_query_access(TWindow *parent, const char *criteria, TEPortGuid *group)
{
	TOffice office;

	if (office.Open("eportq_acc.htm"))
	{
		constant tr1start = office.Find("tr1 start");
		constant tr1final = office.Find("tr1 final");

		TGArray<TEPortGuid> guids;
		TWaitWindow wait(parent, WAIT);
		char einvoke[3];

		office.Emit(NULL, tr1start, null_fields, NULL);
		TEPortGuid().LoadArray(guids, criteria);

		if (guids.Count())
		{
			for (int i = 0; i < guids.Count(); i++)
			{
				TEPortGuid *guid = guids[i];

				guid->Add(new TString("F_EINVOKE", einvoke, sizeof einvoke));
				strcpy(einvoke, eportal_invoke_granted(guid->key, guid->ucnType,
					guid->ucn, guid->involvement) ? "Да" : "Не");
				office.Emit(tr1start, tr1final, eportguid_fields, guid);
			}
		}
		else
		{
			group->Add(new TString("F_EINVOKE", einvoke, sizeof einvoke));
			office.Emit(tr1start, tr1final, eportguid_fields, group);
			group->Remove("F_EINVOKE");
		}

		office.Emit(tr1final, NULL, null_fields, NULL);
		office.Show(&wait);
	}
}

void eportal_query_access(TWindow *parent, const TTransGroup *ucnGroup)
{
	TEPortGuid guid;
	msql m(guid.Name);

	guid.what = involvement2assignment(ucnGroup->involvement);
	guid.ucnType = ucnGroup->ucnType;
	strcpy(guid.ucn, ucnGroup->ucn);
	m.Add(&guid, "F_WHAT, F_UCN, F_UCN_TYPE");
	m.AddOrder("F_TYPE, F_YEAR, F_NO");
	eportal_query_access(parent, str(m), &guid);
}

void eportal_query_access(TWindow *parent, const TLawsuit *lawsuit)
{
	TEPortGuid guid;
	msql m(guid.Name);

	guid.key = lawsuit->key;
	m.AddRange("F_WHAT", WHAT_PERSON_ASSIGNMENT, WHAT_LAWYER_ASSIGNMENT);
	m.Add(&guid, "F_NO, F_YEAR, F_TYPE");
	m.AddOrder("F_UCN_TYPE, F_UCN");
	eportal_query_access(parent, str(m), &guid);
}

bool citizen_in_progress(TUCNGroup *ucnGroup)
{
	TEPortSend send;
	msql m(send.Name, "F_UCN");

	m.Add(ucnGroup, "F_UCN, F_UCN_TYPE");
	m.AddChar("NOT F_STATUS", EPORT_ERROR);
	TQuery q(str(m));
	return q.Read();
}

void eportal_renumber_guid(TUCNGroup *source, TUCNGroup *target)
{
	TEPortGuid eportGuid;
	msql m(eportGuid.Name, MQ_UPDATE);

	target->Quote(m, "F_UCN, F_UCN_TYPE");
	m.Add(source, "F_UCN, F_UCN_TYPE");
	eportGuid.DataBase->Execute(str(m), ET_ERROR);
}
