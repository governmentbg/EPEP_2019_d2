#include "cac.h"

// 2018-04-11: LPR/IRQ
#define SHOW_ALL_RETURNED 1

// ----- TClosedBookDecisionArray --------------------------------------------
class TClosedBookDecisionArray : public TGSArray<TDecision>
{
public:
	TClosedBookDecisionArray() : TGSArray<TDecision>(LessEqlDec) { }

	static bool LessEqlDec(const long value1, const long value2);
};

bool TClosedBookDecisionArray::LessEqlDec(const long value1, const long value2)
{
	const TDecision *decision1 = (const TDecision *) value1;
	const TDecision *decision2 = (const TDecision *) value2;

	return decision1->returned != decision2->returned ? decision1->returned < decision2->returned :
		decision1->kind != decision2->kind ? decision1->kind < decision2->kind :
		decision1->eventNo <= decision2->eventNo;
}

// ----- TClosedBookSession --------------------------------------------------
class TClosedBookSession : public TSession
{
public:
	TClosedBookSession();

	char lawKind;
	long subject;
	char sourceKind;
	CDate lawFinished;
	CDate lawDate;
	CDate decFromDate;
	char eventKind;
	long eventYear;

	bool byDecision;
	TClosedBookDecisionArray decisions;
	TDatedContainer inReg;

	// 2018-04-11: closed book 2
	char age;
	CDate firstSet;
	bool canceled;
	CDate enterDate;	// 2018-04-17
	bool finalizer;	// 2018-04-17: was Finalizer()

	void SetEvent();
	void LoadDecisions();
	bool FirstInstance();
	bool Appeal();
	bool PrivAppeal();

	bool operator<=(const TClosedBookSession &that) const;
};

TClosedBookSession::TClosedBookSession()
	: eventKind('\0'), byDecision(true), inReg("T_INREG", NULL), finalizer(false)
{
	Add(
		new TChar("F_LAW_KIND", &lawKind),
		new TDate("F_LAW_DATE", &lawDate, false),
		new TLongAlias("F_SUBJECT", &subject),
		new TChar("F_SOURCE_KIND", &sourceKind),
		new TDate("F_LAW_FINISHED", &lawFinished, false),
		NEW_RCDKEY("F_INREG_", &inReg.key),
		new TChar("F_INREG_KIND", &inReg.kind),
		new TDate("F_FROM_DATE", &decFromDate, false),
		new TCharAlias("F_AGE", &age),
		new TDate("F_FIRST_SET", &firstSet, false),
		new TDate("F_ENTER_DATE", &enterDate, false),
		NULL
	);
}

void TClosedBookSession::SetEvent()
{
	if (strchr(KIND_INREG_EVENTS, kind))
	{
		TInRegEvent inRegEvent;

		inRegEvent.result = result;
		eventKind = inRegEvent.IndexKind();
		eventYear = date.year;
	}
	else if (decisions.Count())
	{
		eventKind = decisions[0]->kind;
		eventNo = decisions[0]->eventNo;
		eventYear = decisions[0]->fromDate.year;
	}
	else if (TSession::IndexType())
	{
		eventKind = TSession::IndexKind();
		eventYear = date.year;
	}
}

void TClosedBookSession::LoadDecisions()
{
#if SHOW_ALL_RETURNED
	try2decide(this, &decisions, NULL, "F_DATE, F_KIND, F_EVENT_NO, F_RESULT, F_RETURNED, F_FROM_DATE, F_TEXT, F_ENTER_DATE");
#else  // SHOW_ALL_RETURNED
	if (try2decide(this, &decisions, NULL, "F_DATE, F_KIND, F_EVENT_NO, F_RESULT, F_RETURNED, F_FROM_DATE, F_TEXT, F_ENTER_DATE"))
	{
		int index;

		for (index = 1; index < decisions.Count(); index++)
			if (decisions[index]->returned != decisions[0]->returned)
				break;

		if (index == decisions.Count())
			for (index = 1; index < decisions.Count(); index++)
				decisions[index]->returned.Clear();
	}
#endif  // SHOW_ALL_RETURNED
}

bool TClosedBookSession::FirstInstance()
{
#if COURT_TYPE == COURT_APPEAL
	return false;
#endif  // APPEAL
#if COURT_TYPE == COURT_ADMIN || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY
	return strchr(KIND_1ST_LAWSUITS, lawKind);
#endif  // ADMIN || AREAL || COURT_MILITARY
#if COURT_TYPE == COURT_REGIONAL
	return true;
#endif  // REGIONAL
}

bool TClosedBookSession::Appeal()
{
	// ----- KIND_ ---------------------------------------------------------------
	static constant
	#if COURT_TYPE == COURT_APPEAL
		KIND_APPEAL_INDOCS = "123L5XВQ",
		KIND_APPEAL_LAWS   = "ZTяН";
	#endif  // APPEAL
	#if COURT_TYPE == COURT_ADMIN
		KIND_APPEAL_INDOCS = "123L5XВQ",
		KIND_APPEAL_LAWS   = "ЯMn";
	#endif  // ADMIN
	#if COURT_TYPE == COURT_AREAL
		KIND_APPEAL_INDOCS  = "123Фцт5В-",
		KIND_APPEAL_LAWS    = "NnЯ";
	#endif  // AREAL
	#if COURT_TYPE == COURT_MILITARY
		KIND_APPEAL_INDOCS = "Ф123цт5В",
		KIND_APPEAL_LAWS   = "Z";
	#endif  // MILITARY
	#if COURT_TYPE == COURT_REGIONAL
		KIND_APPEAL_INDOCS = "",
		KIND_APPEAL_LAWS   = "";
	#endif  // REGIONAL

	return key.type == TYPE_PUNISHMENT_LAW ? strchr(KIND_APPEAL_INDOCS, sourceKind) : strchr(KIND_APPEAL_LAWS, lawKind);
}

bool TClosedBookSession::PrivAppeal()
{
	// ----- KIND_ ---------------------------------------------------------------
	constant
	#if COURT_TYPE == COURT_APPEAL
		KIND_PRIVATE_APPEAL_INDOCS = "6YF9ц4",
		KIND_PRIVATE_APPEAL_LAWS   = "NnЯЅ";
	#endif  // APPEAL
	#if COURT_TYPE == COURT_ADMIN
		KIND_PRIVATE_APPEAL_INDOCS = "6YF9ц4",
		KIND_PRIVATE_APPEAL_LAWS   = "ЅEz";
	#endif  // ADMIN
	#if COURT_TYPE == COURT_AREAL
		KIND_PRIVATE_APPEAL_INDOCS = "“Є6—йўЎ4g",
		KIND_PRIVATE_APPEAL_LAWS   = "]}Н";
	#endif  // AREAL
	#if COURT_TYPE == COURT_MILITARY
		KIND_PRIVATE_APPEAL_INDOCS = "“Є6йў4g",
		KIND_PRIVATE_APPEAL_LAWS   = "N";
	#endif  // MILITARY
	#if COURT_TYPE == COURT_REGIONAL
		KIND_PRIVATE_APPEAL_INDOCS = "",
		KIND_PRIVATE_APPEAL_LAWS   = "";
	#endif  // REGIONAL

	return key.type == TYPE_PUNISHMENT_LAW ? strchr(KIND_PRIVATE_APPEAL_INDOCS, sourceKind) :
		strchr(KIND_PRIVATE_APPEAL_LAWS, lawKind);
}

bool TClosedBookSession::operator<=(const TClosedBookSession &that) const
{
	if (returned != that.returned)
		return returned < that.returned;

	if (!byDecision)
	{
		int compare = key.year != that.key.year ? key.year - that.key.year : key.no - that.key.no;

		if (compare)
			return compare < 0;
	}

	return eventKind != that.eventKind ? eventKind < that.eventKind :
		eventYear != that.eventYear ? eventYear < that.eventYear : eventNo <= that.eventNo;
}

// ----- session_ ------------------------------------------------------------
static void session_age(mstr &m, TClosedBookSession *session, bool canceled)
{
	if (session->finalizer)
	{
		if (session->canceled == canceled)
		{
			char age = interval2LawAge(session->lawKind, session->lawDate, session->firstSet, session->lawFinished);

			m.cat(Age->Seek(age));
		}
	}
#if COURT_TYPE == COURT_AREAL
	else if (session->key.type == TYPE_FIRM_LAW && !session->enterDate.Empty())
	{
		session->canceled = strchr(RESULT_FIRM_CANCELD1S, session->result) ||
			strchr(RESULT_ALL_CANCELEDS, session->result);

		if (session->canceled == canceled)
			m.cat("да");
	}
#endif  // AREAL
}

#define session OFFICE_GROUP_CAST(TClosedBookSession)

static void session_rowspan(mstr &m, TGroup *group)
{
	if (session->decisions.Count() > 1)
		m.printf("rowspan=%d", session->decisions.Count());
}

static void session_month(mstr &m, TGroup *group)
{
	m.printf("Месец %s %d", Month->Seek(session->returned.month), session->returned.year);
	m.lower();
}

static void session_lawsuit_kind(mstr &m, TGroup *group) { m.cat(KindAlias->Seek(session->lawKind)); }
static void session_law_date(mstr &m, TGroup *group) { session->lawDate.Print(m); }

static void session_finished(mstr &m, TGroup *group)
{
	// 2010:351 LRQ: always display any finished
	session->lawFinished.Print(m, "Дата на свършване: %D");
}

static void session_composition(mstr &m, TGroup *group) { print_composition(m, session->composition, "с-в"); }
static void session_president(mstr &m, TGroup *group) { print_ucn_short(m, session->president); }
static void session_judge_x(mstr &m, TGroup *group) { print_ucn_short(m, session->judgeX); }
static void session_rejudge_x(mstr &m, TGroup *group) { print_ucn_short(m, session->reJudgeX); }
static void session_juror(mstr &m, TGroup *group) { print_ucn_short(m, session->juror); }
static void session_rejuror(mstr &m, TGroup *group) { print_ucn_short(m, session->reJuror); }
static void session_juror_x(mstr &m, TGroup *group) { print_ucn_short(m, session->jurorX); }
static void session_reserve(mstr &m, TGroup *group) { print_ucn_short(m, session->reserve); }
static void session_judge(mstr &m, TGroup *group) { print_ucn_short(m, session->judge); }

static void session_event_kind(mstr &m, TGroup *group)
{
	if (session->eventKind)
		m.cpy(Kind->Seek(session->eventKind));
}

static void session_from(mstr &m, TGroup *group) { m.printf("%s", session->eventKind ? "от" : ""); }

static void session_date(mstr &m, TGroup *group)
{
	(session->decisions.Count() ? session->decFromDate : session->date).Print(m);
}

static void session_result(mstr &m, TGroup *group)
{
	if (strchr(KIND_INREG_EVENTS, session->kind))
	{
		m.printf("%s \vвх № %d/%d \v%s", Kind->Seek(session->inReg.kind),
			session->inReg.key.no, session->inReg.key.year, Result->Seek(session->result));
	}
	else if (session->decisions.Count())
	{
		if (session->key.type == TYPE_FIRM_LAW)
		{
			TDamn damn;
			TLawsuit lawsuit;

			m.cpy(Result->Seek(session->result));	// decision.result
			lawsuit.key = session->key;

			if (find_damn(&lawsuit, &damn))
				m.printf("\v%s %s", CompanyType->Seek(damn.companyType), damn.name);
		}
		else
			m.cpy(Kind->Seek(session->kind)); // decision.kind
	}
	else
		m.cpy(Result->Seek(session->result));

	m.cat("\v");
	m.cat(session->text);
}

// 2018-04-11: closed book 2
static void session_subcount(mstr &m, TGroup *group) { subcount(m, session->decisions.Count()); }
static void session_subject(mstr &m, TGroup *group) { m.cat(TSubject::Code(session->subject, session->lawKind)); }

static void session_connect(mstr &m, TGroup *group)
{
	TConnect connect;

	if (connect.TryBy(session->key, CONNECT_TYPE_LOWER_INSTANCE))
		connect.Print(m, true, false, NULL);
}

static void session_age_decided(mstr &m, TGroup *group) { session_age(m, session, false); }
static void session_age_canceled(mstr &m, TGroup *group) { session_age(m, session, true); }
#undef session

static const offield closedSession1_fields[] =
{
	{ "F_ROWSPAN",       session_rowspan },
	{ "F_MONTH",         session_month },
	{ "F_LAWSUIT_KIND",  session_lawsuit_kind },
	{ "F_LAWSUIT_NO",    keygroup_flag_no },
	{ "F_LAWSUIT_YEAR",  keygroup_key_year },
	{ "F_LAWSUIT_DATE",  session_law_date },
	{ "F_FINISHED",      session_finished },
	{ "F_COMPOSITION",   session_composition },
	{ "F_PRESIDENT",     session_president },
	{ "F_JUDGE_X",       session_judge_x },
	{ "F_REJUDGE_X",     session_rejudge_x },
	{ "F_JUROR",         session_juror },
	{ "F_REJUROR",       session_rejuror },
	{ "F_JUROR_X",       session_juror_x },
	{ "F_RESERVE",       session_reserve },
	{ "F_JUDGE",         session_judge },
	{ "F_EVENT_KIND",    session_event_kind },
	{ "F_FROM",          session_from },
	{ "F_EVENT_NO",      container_event_no },
	{ "F_DATE",          session_date },
	{ "F_RESULT",        session_result },
	{ "F_RETURNED",      NULL },
	{ "F_ENTER_DATE",    NULL },
	{ NULL, NULL }
};

static const offield closedSession2_fields[] =
{
	{ "F_SUBCOUNT",      session_subcount },
	{ "F_MONTH",         session_month },
	{ "F_LAWSUIT_NO",    keygroup_flag_no },
	{ "F_LAWSUIT_YEAR",  keygroup_key_year },
	{ "F_COMPOSITION",   session_composition },
	{ "F_FINISHED",      session_finished },
	{ "F_LAWSUIT_KIND",  session_lawsuit_kind },
	{ "F_SUBJECT",       session_subject },
	{ "F_CONNECT",       session_connect },
	{ "F_AGE_DECIDED",   session_age_decided },
	{ "F_AGE_CANCELED",  session_age_canceled },
	{ "F_DATE",          session_date },
	{ "F_JUDGE",         session_judge },
	{ "F_RESULT",        session_result },
	{ "F_RETURNED",      NULL },
	{ NULL, NULL }
};

// ----- TClosedSessionBookGroup ---------------------------------------------
class TClosedSessionBookGroup : public TSessionBookGroup
{
public:
	TClosedSessionBookGroup();

	void InRegCriteria(msql &m);
	void SessionCriteria(msql &m);

	char compositions[SIZE_OF_COMPOSITIONS];
	long allResults;
	long sessions;
	long inregs;
	long createBook;
	long createReCap;
	long createResults;
	long orderByDecisions;
	long commonOutput;
	long byReturned;	// 2018-04-12: _ support
	long byInterval;	// 2018-04-23: LPR

	// --- ReCap1 ---
	enum
	{
		// CS_ -> closed session [non-punishment]
		CS_ALL 		= 0,
		// ненаказателни дела
		CS_ALL_1ST 		= 1,
		CS_FINISHED 	= 2,
		CS_CANCELED 	= 3,
		CS_OTHER_1ST 	= 4,

		CS_ALL_2ND 		= 5,
		CS_APPEALS 		= 6,
		CS_PAPPEALS 	= 7,
		CS_SLOWDOWN 	= 8,
		CS_OTHER_2ND 	= 9,

		// CSP_ -> closed session punishment
		CSP_ALL_1ST 	= 1,
		CSP_FINISHED_ND 	= 2,	// _ND -> без досъдебни
		CSP_CANCELED_ND 	= 3,
		CSP_FINISHED_D 	= 4,	// _D -> досъдебни
		CSP_CANCELED_D 	= 5,
		CSP_OTHER_1ST 	= 6,

		CSP_ALL_2ND 	= 7,
		CSP_APPEALS_ND 	= 8,
		CSP_PAPPEALS_ND 	= 9,
		CSP_APPEALS_D 	= 10,
		CSP_PAPPEALS_D 	= 11,
		CSP_OTHER_2ND 	= 12,

		CSX_COUNT		= 22	// 2018-16-04: was 0xD 
	};

	int counts[CSX_COUNT];
	bool has1inst, has2inst;
	void CountReCap1(TClosedBookSession *session);
	void CountReCap2(TClosedBookSession *session);
};

TClosedSessionBookGroup::TClosedSessionBookGroup()
	: has1inst(false), has2inst(false), byReturned(1)
{
	Add(
		new TCharList("F_COMPOSITIONS", compositions, sizeof compositions),
		new TCheck("F_ALL_RESULTS", &allResults, TRUE),
		new TCheck("F_NEW_FORMAT", &flags, BOOK_NEW_FORMAT),
		new TCheck("F_SESSIONS", &sessions, TRUE),
		new TCheck("F_INREGS", &inregs, TRUE),
		new TCheck("F_CREATE_BOOK", &createBook, TRUE),
		new TCheck("F_RECAP", &createReCap, TRUE),
		new TCheck("F_RESULTS", &createResults, TRUE),
		new TCheck("F_COMMON_OUTPUT", &commonOutput, TRUE),
		new TRadio("F_ORDER_BY_DECISIONS", &orderByDecisions, TRUE),
		new TRadio("F_ORDER_BY_LAWSUITS", &orderByDecisions, FALSE),
		new TCheck("F_BY_INTERVAL", &byInterval, TRUE),
		NULL
	);

	minDate = maxDate = Today;
	memset(counts, '\0', sizeof counts);
	// 2018-04-10: were SetupWindow()
	allResults = TRUE;
	flags |= BOOK_NEW_FORMAT;
	sessions = TRUE;
	createBook = TRUE;
	createReCap = TRUE;
	orderByDecisions = TRUE;
}

void TClosedSessionBookGroup::CountReCap1(TClosedBookSession *session)
{
	counts[CS_ALL]++;

	if (session->key.type == TYPE_PUNISHMENT_LAW)
	{
		Subject->Seek(session->key.type, session->subject);
		bool preCourt = strchr(Subject->Attrib(session->lawKind), POST_PRECOURT);

		if (session->FirstInstance())
		{
			has1inst = true;

			if (session->finalizer)
			{
				counts[CSP_ALL_1ST]++;

				if (session->canceled)
					counts[preCourt ? CSP_CANCELED_D : CSP_CANCELED_ND]++;
				else
					counts[preCourt ? CSP_FINISHED_D : CSP_FINISHED_ND]++;
			}
			else
				counts[CSP_OTHER_1ST]++;
		}
		else
		{
			has2inst = true;

			if (session->finalizer)
			{
				counts[CSP_ALL_2ND]++;

				if (session->Appeal())
					counts[preCourt ? CSP_APPEALS_D : CSP_APPEALS_ND]++;
				else if (session->PrivAppeal())
					counts[preCourt ? CSP_PAPPEALS_D : CSP_PAPPEALS_ND]++;
				else
					counts[CSP_OTHER_2ND]++;
			}
			else
				counts[CSP_OTHER_2ND]++;
		}
	}
	else
	{
		if (session->FirstInstance())
		{
			has1inst = true;

			if (session->finalizer)
			{
				counts[CS_ALL_1ST]++;
				counts[session->canceled ? CS_CANCELED : CS_FINISHED]++;
			}
			else
				counts[CS_OTHER_1ST]++;
		}
		else
		{
			has2inst = true;

			if (session->lawFinished == session->returned)
			{
				counts[CS_ALL_2ND]++;

				if (session->Appeal())
					counts[CS_APPEALS]++;
				else if (session->PrivAppeal())
				{
					if (strchr(KIND_SLOW_REQUESTS, session->sourceKind))
						counts[CS_SLOWDOWN]++;
					else
						counts[CS_PAPPEALS]++;
				}
				else
					counts[CS_OTHER_2ND]++;
			}
			else
				counts[CS_OTHER_2ND]++;
		}
	}
}

void TClosedSessionBookGroup::CountReCap2(TClosedBookSession *session)
{
	counts[CS_ALL]++;

#if COURT_TYPE == COURT_REGIONAL
	enum
	{
		CS_RC_CANCELED               = 1,
		CS_RC_CANCELED_REINVEST      = 2,
		CS_RC_RESULT_WITH_SETTLEMENT = 3,
		CS_RC_RESULT_WITH_DECISION   = 4,
		CS_RC_RESULT_SETLEMENT       = 5,
		CS_RC_RESULT_ORDER           = 6
	};

	if (session->finalizer && session->canceled)
	{
		counts[CS_RC_CANCELED]++;

		if (strchr(RESULT_REINVESTS, session->result))
			counts[CS_RC_CANCELED_REINVEST];
	}

	if (session->result == RESULT_WITH_SETTLEMENT)
		counts[CS_RC_RESULT_WITH_SETTLEMENT]++;
	else if (session->result == RESULT_WITH_DECISION)
		counts[CS_RC_RESULT_WITH_DECISION]++;
	else if (strchr(RESULT_CSB_SETTLEMENT, session->result))
		counts[CS_RC_RESULT_SETLEMENT]++;
	else if (strchr(RESULT_CSB_ORDER, session->result))
		counts[CS_RC_RESULT_ORDER]++;
#endif  // REGIONAL

#if COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY
	if (session->key.type == TYPE_PUNISHMENT_LAW)
	{
		enum
		{
			CS_XCP_FINISHED_1            = 1,
			CS_XCP_F1_DECIDED_WO_PRC243  = 2,
			CS_XCP_F1_CANCELED_WO_PRC243 = 3,
			CS_XCP_F1_PRECOURT           = 4,
			CS_XCP_F1_243                = 5,
			CS_XCP_SET_ORD_1             = 6,
			CS_XCP_FINISHED_2            = 7,
			CS_XCP_F2_APPL_COMM_WO_PRC   = 8,
			CS_XCP_F2_APPL_PRIV_WO_PRC   = 9,
			CS_XCP_F2_PRECOURT           = 10,
			CS_XCP_SET_ORD_2             = 11
		};

		Subject->Seek(session->key.type, session->subject);
		bool preCourt = strchr(Subject->Attrib(session->lawKind), POST_PRECOURT);

		if (session->FirstInstance())
		{
			if (session->finalizer)
			{
				counts[CS_XCP_FINISHED_1]++;

				if (session->sourceKind == KIND_CANCEL_ENACTMENT)
					counts[CS_XCP_F1_243]++;
				else if (preCourt)
					counts[CS_XCP_F1_PRECOURT]++;
				else
					counts[session->canceled ? CS_XCP_F1_CANCELED_WO_PRC243 : CS_XCP_F1_DECIDED_WO_PRC243]++;
			}
			else
				counts[CS_XCP_SET_ORD_1]++;
		}
		else
		{
			if (session->finalizer)
			{
				counts[CS_XCP_FINISHED_2]++;

				if (preCourt)
					counts[CS_XCP_F2_PRECOURT]++;
				else if (session->Appeal())
					counts[CS_XCP_F2_APPL_COMM_WO_PRC]++;
				else if (session->PrivAppeal())
					counts[CS_XCP_F2_APPL_PRIV_WO_PRC]++;
			}
			else
				counts[CS_XCP_SET_ORD_2]++;
		}
	}
#endif  // AREAL || MILITARY

#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_AREAL
	if (strchr(TYPE_TRADEALIKE_LAWS, session->key.type))
	{
		enum
		{
			CS_XCX_DECIDED_1             = 1,
			CS_XCX_CANCELED_1            = 2,
			CS_XCX_SET_ORD_1             = 3,
			CS_XCX_FINISHED_APPL_COMM    = 4,
			CS_XCX_FINISHED_APPL_PRIV    = 5,
			CS_XCX_FINISHED_APPL_SLOW    = 6,
			CS_XCX_SET_ORD_2             = 7
		};

		if (session->FirstInstance())
		{
			if (session->finalizer)
				counts[session->canceled ? CS_XCX_CANCELED_1 : CS_XCX_DECIDED_1]++;
			else
				counts[CS_XCX_SET_ORD_1]++;
		}
		else
		{
			if (session->finalizer)
			{
				if (session->Appeal())
					counts[CS_XCX_FINISHED_APPL_COMM]++;
				else if (session->PrivAppeal())
				{
					counts[CS_XCX_FINISHED_APPL_PRIV]++;

					if (strchr(KIND_SLOW_REQUESTS, session->sourceKind))
						counts[CS_XCX_FINISHED_APPL_SLOW]++;
				}
			}
			else
				counts[CS_XCX_SET_ORD_2]++;
		}
	}
#endif  // APPEAL || AREAL

#if COURT_TYPE == COURT_ADMIN || COURT_TYPE == COURT_AREAL
	if (CourtType == COURT_ADMIN || session->key.type == TYPE_ADMIN_LAW)
	{
		enum
		{
			CS_ADM_DECIDED_1             = 1,
			CS_ADM_CANCELED_1            = 2,
			CS_ADM_SET_ORD_1             = 3,
			CS_ADM_FINISHED_APPL_COMM    = 4,
			CS_ADM_FINISHED_APPL_PRIV    = 5,
			CS_ADM_SET_ORD_KS            = 6
		};

		if (session->FirstInstance())
		{
			if (session->finalizer)
				counts[session->canceled ? CS_ADM_CANCELED_1 : CS_ADM_DECIDED_1]++;
			else
				counts[CS_ADM_SET_ORD_1]++;
		}
		else
		{
			if (session->finalizer)
			{
				if (session->Appeal())
					counts[CS_ADM_FINISHED_APPL_COMM]++;
				else if (session->PrivAppeal())
					counts[CS_ADM_FINISHED_APPL_PRIV]++;
			}
			else
				counts[CS_ADM_SET_ORD_KS]++;
		}
	}
#endif  // AREAL || ADMIN

#if COURT_TYPE == COURT_APPEAL
	if (session->key.type == TYPE_PUNISHMENT_LAW)
	{
		enum
		{
			CS_ACP_FINISHED_APPL_COMM = 1,
			CS_ACP_FINISHED_APPL_PRIV = 2,
			CS_ACP_CANCEL_ENACTMENT   = 3,
			CS_ACP_SETTLEMENT_326     = 4,
			CS_ACP_ORDER              = 5,
			CS_ACT_SET_ORD_AFTER_FIN  = 6
		};

		if (session->finalizer)
		{
			if (session->Appeal())
				counts[CS_ACP_FINISHED_APPL_COMM]++;
			else if (session->PrivAppeal())
				counts[CS_ACP_FINISHED_APPL_PRIV]++;
		}

		if (session->sourceKind == KIND_CANCEL_ENACTMENT)
			counts[CS_ACP_CANCEL_ENACTMENT]++;

		if (session->result == RESULT_SETTLEMENT_326)
			counts[CS_ACP_SETTLEMENT_326]++;

		if (session->eventKind == KIND_ORDER)
			counts[CS_ACP_ORDER]++;

		if ((session->eventKind == KIND_SETTLEMENT || session->eventKind == KIND_ORDER) &&
			!session->lawFinished.Empty() && session->date > session->lawFinished)
		{
			counts[CS_ACT_SET_ORD_AFTER_FIN]++;
		}
	}
	else
	{
		if (session->finalizer)
		{
			bool inconsist = strchr(KIND_INCONSISTENCE_INDOCS, session->lawKind);

			if (session->Appeal())
			{
				enum
				{
					CS_ACX_FINISHED_VZ            = 1,
					CS_ACX_FVZ_DECIDED            = 2,
					CS_ACX_FVZ_DECIDED_INCONSIST  = 3,
					CS_ACX_FVZ_CANCELED           = 4,
					CS_ACX_FVZ_CANCELED_INCONSIST = 5
				};

				counts[CS_ACX_FINISHED_VZ]++;

				if (session->canceled)
				{
					counts[CS_ACX_FVZ_CANCELED]++;

					if (inconsist)
						counts[CS_ACX_FVZ_CANCELED_INCONSIST]++;
				}
				else
				{
					counts[CS_ACX_FVZ_DECIDED]++;

					if (inconsist)
						counts[CS_ACX_FVZ_DECIDED_INCONSIST]++;
				}
			}
			else if (session->PrivAppeal())
			{

				enum
				{
					CS_ACX_FINISHED_PRIV          = 6,
					CS_ACX_FPR_PRIV_APPEAL        = 7,
					CS_ACX_FPR_SLOWDOWN           = 8,
					CS_ACX_FPR_REQUEST            = 9,
					CS_ACX_FPR_SETTLED_INCONSIST  = 10,
					CS_ACX_FPR_CANCELED_INCONSIST = 11
				};

				counts[CS_ACX_FINISHED_PRIV]++;

				if (strchr(KIND_NONP_PRIV_APPL_INDOCS, session->sourceKind))
					counts[CS_ACX_FPR_PRIV_APPEAL]++;
				else if (strchr (KIND_SLOW_REQUESTS, session->sourceKind))
					counts[CS_ACX_FPR_SLOWDOWN]++;
				else if (strchr(KIND_NONP_REQUEST_INDOCS, session->sourceKind))
					counts[CS_ACX_FPR_REQUEST]++;

				if (inconsist)
				{
					if (session->eventKind == KIND_SETTLEMENT)
						counts[CS_ACX_FPR_SETTLED_INCONSIST]++;

					if (session->canceled)
						counts[CS_ACX_FPR_CANCELED_INCONSIST]++;
				}
			}
		}
		else
		{
			enum
			{
				CS_ACX_NONFIN_ALL             = 12,
				CS_ACX_SETTLEMENT_205         = 13,
				CS_ACX_RESULT_UNDO            = 14,
				CS_ACX_RESULT_WITH_ORDER      = 15,
				CS_ACX_RESULT_WITH_SETTLEMENT = 16
			};

			counts[CS_ACX_NONFIN_ALL]++;

			switch (session->result)
			{
				case RESULT_SETTLEMENT_205 : counts[CS_ACX_SETTLEMENT_205]++; break;
				case RESULT_WITH_ORDER : counts[CS_ACX_RESULT_WITH_ORDER]++; break;
				case RESULT_WITH_SETTLEMENT : counts[CS_ACX_RESULT_WITH_SETTLEMENT]++; break;
				default :
				{
					if (strchr(RESULT_UNDOS, session->result))
						counts[CS_ACX_RESULT_UNDO]++;
				}
			}

			if (!session->lawFinished.Empty() && session->date > session->lawFinished)
			{
				enum
				{
					CS_ACX_AFTER_FIN_ALL              = 17,
					CS_ACX_AFN_SETTLEMENT_192_4       = 18,
					CS_ACX_AFN_SETTLEMENT_192_5       = 19,
					CS_ACX_AFN_RESULT_WITH_ORDER      = 20,
					CS_ACX_AFN_RESULT_WITH_SETTLEMENT = 21
				};

				counts[CS_ACX_AFTER_FIN_ALL]++;

				switch (session->result)
				{
					case RESULT_SETTLEMENT_192_4 : counts[CS_ACX_AFN_SETTLEMENT_192_4]++; break;
					case RESULT_SETTLEMENT_192_5 : counts[CS_ACX_AFN_SETTLEMENT_192_5]++; break;
					case RESULT_WITH_ORDER : counts[CS_ACX_AFN_RESULT_WITH_ORDER]++; break;
					case RESULT_WITH_SETTLEMENT : counts[CS_ACX_AFN_RESULT_WITH_SETTLEMENT]++; break;
				}
			}
		}
	}
#endif  // APPEAL
}

void TClosedSessionBookGroup::InRegCriteria(msql &m)
{
	m.Begin("T_INREG_EVENT IE JOIN T_INREG I ON I.F_NO = IE.F_NO AND I.F_YEAR = IE.F_YEAR "
		"JOIN T_LAWSUIT L ON L.F_NO = I.F_LAWSUIT_NO AND L.F_YEAR = I.F_LAWSUIT_YEAR AND L.F_TYPE = I.F_LAWSUIT_TYPE",
		// 2015:187 JRQ/LPR: qualified fields only
		"L.F_NO, L.F_YEAR, L.F_TYPE, L.F_SUBJECT, IE.F_RESULT, IE.F_EVENT_NO, IE.F_RETURNED, L.F_SOURCE_KIND, "
		"IE.F_JUDGE AS F_JUDGE, IE.F_DATE AS F_DATE, IE.F_KIND AS F_KIND, I.F_NO AS F_INREG_NO, L.F_COMPOSITION, "
		"I.F_YEAR AS F_INREG_YEAR, I.F_TYPE AS F_INREG_TYPE, I.F_KIND AS F_INREG_KIND, L.F_DATE AS F_LAW_DATE, "
		"IE.F_TEXT AS F_TEXT, L.F_KIND AS F_LAW_KIND, L.F_FINISHED AS F_LAW_FINISHED, L.F_AGE, L.F_FIRST_SET");

	m.AddChar("I.F_LAWSUIT_TYPE", *types);
	m.AddRange(byReturned ? "IE.F_RETURNED" : "IE.DATE", minDate, maxDate);
	m.AddCompos("L.F_COMPOSITION", compositions);
	m.AddChars("IE.F_RESULT", RESULT_INREG_RESOLUTIONS, RESULT_EVENTABLES);
}

void TClosedSessionBookGroup::SessionCriteria(msql &m)
{
	m.Begin("T_SESSION S JOIN T_LAWSUIT L ON L.F_NO = S.F_NO AND L.F_YEAR = S.F_YEAR AND L.F_TYPE = S.F_TYPE",
		"S.F_NO, S.F_YEAR, S.F_TYPE, L.F_SUBJECT, S.F_RESULT, S.F_EVENT_NO, S.F_RETURNED, L.F_SOURCE_KIND, "
		"S.F_PRESIDENT, S.F_JUDGE_X, S.F_REJUDGE_X, S.F_JUROR, S.F_REJUROR, S.F_JUROR_X, S.F_JUDGE AS F_JUDGE, "
		"S.F_DATE AS F_DATE, S.F_KIND AS F_KIND, S.F_TEXT AS F_TEXT, L.F_DATE AS F_LAW_DATE, L.F_KIND AS F_LAW_KIND, "
		"L.F_FINISHED AS F_LAW_FINISHED, S.F_COMPOSITION AS F_COMPOSITION, L.F_AGE, L.F_FIRST_SET");

	m.AddChar("S.F_TYPE", *types);
	m.AddRange(byReturned ? "S.F_RETURNED" : "S.F_DATE", minDate, maxDate);
	m.AddCompos("S.F_COMPOSITION", compositions);
	m.AddChars("S.F_RESULT", allResults ? RESULT_SESSIONS : RESULT_FINALIZEDS, RESULT_SESSIONS);
	m.AddChars("S.F_KIND", KIND_CLOSED_SESSIONS, KIND_SESSIONS);
}

#define bookgroup OFFICE_GROUP_CAST(TClosedSessionBookGroup)

static void closed_session_title_type(mstr &m, TGroup *group)
{
	print_types_list(m, C2S[*bookgroup->types], false);
	m.upper();
}

static void closed_session_title_period(mstr &m, TGroup *group)
{
	if (bookgroup->minDate == bookgroup->maxDate)
		bookgroup->minDate.Print(m, C_DATE_GOD);
	else
	{
		bookgroup->minDate.Print(m, C_DATE_GOD);
		m.cat(" - ");
		bookgroup->maxDate.Print(m, C_DATE_GOD);
	}
}

static void closed_session_count_value(mstr &m, TGroup *group)
{
	unsigned col;

	if (sscanf(bookgroup->Name, "F_COUNT_%d", &col) == 1 && --col < TClosedSessionBookGroup::CSX_COUNT)
		itom(bookgroup->counts[col], m);
}

#undef bookgroup

static const offield closedSessionTitle_fields[] =
{
	{ "F_TYPE",		closed_session_title_type },
	{ "F_PERIOD",	closed_session_title_period },
	{ "*F_COUNT_",	closed_session_count_value },
	{ NULL, NULL }
};

// ----- TClosedSessionBookReCap ---------------------------------------------
class TClosedSessionBookReCap : public TBaseBook
{
public:
	TClosedSessionBookReCap(TWindow *parent, TClosedSessionBookGroup *group, const char *fileName,
		const offield *bodyFields, const offield *bookFields);

	void EmitPartial(TOffice &tOffice);

protected:
	const char *tb1start;
	const char *tb1final;

	virtual void EmitContent(TOffice &office) = 0;

	virtual void Markers() = 0;
	virtual void EmitBody();
};

TClosedSessionBookReCap::TClosedSessionBookReCap(TWindow *parent, TClosedSessionBookGroup *group, const char *fileName,
	const offield *bodyFields, const offield *bookFields)
	: TBaseBook(parent, group, fileName, bodyFields, bookFields)
{
}

void TClosedSessionBookReCap::EmitPartial(TOffice &tOffice)
{
	if (office.Open(fileName, OFFICE_COPY_NULL))
	{
		Markers();
		EmitContent(tOffice);
	}
}

void TClosedSessionBookReCap::EmitBody()
{
	office.Emit(NULL, tb1start, null_fields, NULL);
	EmitContent(office);
	office.Emit(tb1final, NULL, null_fields, NULL);
}

// ----- TClosedSessionBookReCap1 --------------------------------------------
class TClosedSessionBookReCap1 : public TClosedSessionBookReCap
{
public:
	TClosedSessionBookReCap1(TWindow *parent, TClosedSessionBookGroup *group);

protected:
	const char *tr1start;
	const char *tr1break;
	const char *tr2start;
	const char *tr2final;
	const char *tr1final;

	virtual void Markers();
	virtual void EmitContent(TOffice &office);
};

TClosedSessionBookReCap1::TClosedSessionBookReCap1(TWindow *parent, TClosedSessionBookGroup *group)
	: TClosedSessionBookReCap(parent, group, *group->types == TYPE_PUNISHMENT_LAW ? "ClosR1fip_.htm" : "ClosR1fi__.htm",
		closedSessionTitle_fields, closedSessionTitle_fields)
{
}

void TClosedSessionBookReCap1::Markers()
{
	tb1start = office.Find("tb1 start");
	tr1start = office.Find("tr1 start");
	tr1break = office.Find("tr1 break");
	tr2start = office.Find("tr2 start");
	tr2final = office.Find("tr2 final");
	tr1final = office.Find("tr1 final");
	tb1final = office.Find("tb1 final");
}

void TClosedSessionBookReCap1::EmitContent(TOffice &office)
{
	TClosedSessionBookGroup *tGroup = (TClosedSessionBookGroup *) group;

	office.Emit(tb1start, tr1start, book_fields, group);

	if (tGroup->has1inst)
	{
		if (tGroup->has2inst)
			office.Emit(tr1start, tr1final, body_fields, group);
		else
			office.Emit(tr1start, tr2start, body_fields, group);
	}
	else if (tGroup->has2inst)
	{
		office.Emit(tr1start, tr1break, body_fields, group);
		office.Emit(tr2final, tr1final, body_fields, group);
	}
	else	// required?
		office.Emit(tr2start, tr2final, null_fields, NULL);

	office.Emit(tr1final, tb1final, null_fields, NULL);
}

// ----- TClosedSessionBookReCap2 --------------------------------------------
class TClosedSessionBookReCap2 : public TClosedSessionBookReCap
{
public:
	TClosedSessionBookReCap2(TWindow *parent, TClosedSessionBookGroup *group, const char *fileName);

protected:
	const char *tb1break;

	virtual void Markers();
	virtual void EmitContent(TOffice &office);
};

TClosedSessionBookReCap2::TClosedSessionBookReCap2(TWindow *parent, TClosedSessionBookGroup *group, const char *fileName)
	: TClosedSessionBookReCap(parent, group, fileName, closedSessionTitle_fields, closedSessionTitle_fields)
{
}

void TClosedSessionBookReCap2::Markers()
{
	tb1start = office.Find("tb1 start");
	tb1break = office.Find("tb1 break");
	tb1final = office.Find("tb1 final");
}

void TClosedSessionBookReCap2::EmitContent(TOffice &office)
{
	office.Emit(tb1start, tb1break, book_fields, group);
	office.Emit(tb1break, tb1final, body_fields, group);
}

// ----- TClosedSessionBookResults -------------------------------------------
class TClosedSessionBookResults : public TSessionBookResults
{
public:
	TClosedSessionBookResults(TWindow *parent, TClosedSessionBookGroup *group);
};

TClosedSessionBookResults::TClosedSessionBookResults(TWindow *parent, TClosedSessionBookGroup *group)
	: TSessionBookResults(parent, group, "ClosS1fi__.htm", closedSessionTitle_fields)
{
}

// ----- TClosedSessionBook --------------------------------------------------
class TClosedSessionBook : public TBook
{
public:
	TClosedSessionBook(TWindow *parent, TClosedSessionBookGroup *tGroup, const char *fileName, const offield *bodyFields);

	virtual TClosedSessionBookReCap *NewReCap(TWindow *parent, TClosedSessionBookGroup *group) = 0;

protected:
	TClosedSessionBookGroup *group;	// alias
	CDate returned;

	const char *tb1start;
	const char *tb1final;

	virtual void EmitBody();
	virtual TGroup *Process(TQuery &q);
	virtual void Collect();
};

TClosedSessionBook::TClosedSessionBook(TWindow *parent, TClosedSessionBookGroup *tGroup, const char *fileName,
	const offield *bodyFields)
	: TBook(parent, tGroup, fileName, bodyFields, closedSessionTitle_fields, BOOK_OLD_FORMAT,
		new TGSArray<TClosedBookSession>), group(tGroup)
{
}

void TClosedSessionBook::EmitBody()
{
	if (group->commonOutput)
	{
		const char *mainFinal = office.mainFinal;

		office.mainFinal = tb1final;

		if (group->createBook)
			TBook::EmitBody();
		else
			office.Emit(NULL, tb1start, book_fields, group);

		if (group->createReCap)
		{
			if (group->createBook)
				office.EmitBreak();

			TClosedSessionBookReCap *const reCap = NewReCap(parent, group);
			TVirtPtr VP(reCap);

			reCap->EmitPartial(office);
		}

		if (group->createResults)
		{
			if (group->createBook || group->createReCap)
				office.EmitBreak();

			TClosedSessionBookResults(parent, group).EmitPartial(office);
		}

		office.Emit(tb1final, mainFinal, null_fields, NULL);
	}
	else	// checked by caller
		TBook::EmitBody();
}

TGroup *TClosedSessionBook::Process(TQuery &q)
{
	TClosedBookSession *session = new TClosedBookSession;
	*session << q;

	if (group->byInterval)
	{
		TLawInterval interval(session->key, session->date);

		if (interval.Read("F_DATE, F_FINISHED, F_FIRST_SET", false))
		{
			session->lawDate = interval.date;
			session->lawFinished = interval.finished;
			session->firstSet = interval.firstSet;
		}
		else
		{
			dprintf("%s %ld/%ld %s %D: събитието е преди началото на делото", Type->Seek(session->key.type),
				session->key.no, session->key.year, Kind->Seek(session->kind), &session->date);
		}
	}

	if (strchr(RESULT_FINALIZEDS, session->result) && session->lawFinished == session->returned)
	{
		char tResult = session->result;

		session->finalizer = true;
		adjust_cancelaw(session);
		session->canceled = strchr(RESULT_ALL_CANCELEDS, session->result);
		session->result = tResult;
	}

	if (group->createBook)
	{
		session->byDecision = group->orderByDecisions;

		if (strchr(KIND_CLOSED_SESSIONS, session->kind))
			session->LoadDecisions();

		session->SetEvent();
	}

	if (group->createResults)
		group->CountResults(session, session->lawFinished);

	return session;
}

void TClosedSessionBook::Collect()
{
	if (group->sessions)
	{
		msql m;
		group->SessionCriteria(m);
		TBook::Collect(str(m));
	}

	if (group->inregs)
	{
		msql m;
		group->InRegCriteria(m);
		TBook::Collect(str(m));
	}
}

// ----- TClosedSessionBook1 -------------------------------------------------
class TClosedSessionBook1 : public TClosedSessionBook
{
public:
	TClosedSessionBook1(TWindow *parent, TClosedSessionBookGroup *tGroup);

	virtual TClosedSessionBookReCap *NewReCap(TWindow *parent, TClosedSessionBookGroup *group);

protected:
	const char *tr1break;
	const char *tr2start;
	const char *tr2break;
	const char *tr2final;

	virtual void Markers();
	virtual void EmitRow(TGroup *group);
	virtual TGroup *Process(TQuery &q);
};

TClosedSessionBook1::TClosedSessionBook1(TWindow *parent, TClosedSessionBookGroup *tGroup)
	: TClosedSessionBook(parent, tGroup, "ClosB1fi__.htm", closedSession1_fields)
{
}

TClosedSessionBookReCap *TClosedSessionBook1::NewReCap(TWindow *parent, TClosedSessionBookGroup *group)
{
	return new TClosedSessionBookReCap1(parent, group);
}

void TClosedSessionBook1::Markers()
{
	tb1start = office.Find("tb1 start");
	tr1start = office.Find("tr1 start");
	tr1break = office.Find("tr1 break");
	tr2start = office.Find("tr2 start");
	tr2break = office.Find("tr2 break");
	tr2final = office.Find("tr2 final");
	tr1final = office.Find("tr1 final");
	tb1final = office.Find("tb1 final");
}

void TClosedSessionBook1::EmitRow(TGroup *group)
{
	TClosedBookSession *session = (TClosedBookSession *) group;

	if (returned.month != session->returned.month || returned.year != session->returned.year)
	{
		office.Emit(tr1start, tr1break, body_fields, session);
		returned = session->returned;
	}

	if (session->decisions.Count())
		*session << session->decisions[0];

	office.Emit(tr1break, tr1final, body_fields, session);

	for (int i = 1; i < session->decisions.Count(); i++)
	{
		*session << session->decisions[i];
		session->eventKind = session->kind;
		office.Emit(tr1break, tr2start, null_fields, NULL);
		office.Emit(tr2break, tr1final, body_fields, session);
	}
}

TGroup *TClosedSessionBook1::Process(TQuery &q)
{
	TClosedBookSession *session = (TClosedBookSession *) TClosedSessionBook::Process(q);

	if (group->createReCap)
		group->CountReCap1(session);

	return session;
}

// ----- TClosedSessionBook2 -------------------------------------------------
class TClosedSessionBook2 : public TClosedSessionBook
{
public:
	TClosedSessionBook2(TWindow *parent, TClosedSessionBookGroup *tGroup);

	TClosedSessionBookReCap *NewReCap(TWindow *parent, TClosedSessionBookGroup *group);

protected:
	const char *hd1start;
	const char *tr1break;
	const char *tr1judge;
	const char *tr1result;

	const char *Book2FileName(TClosedSessionBookGroup *group);
	const char *ReCap2FileName();

	virtual void Markers();
	virtual void EmitRow(TGroup *group);
	virtual TGroup *Process(TQuery &q);
};

TClosedSessionBook2::TClosedSessionBook2(TWindow *parent, TClosedSessionBookGroup *tGroup)
	: TClosedSessionBook(parent, tGroup, Book2FileName(tGroup), closedSession2_fields)
{
}

const char *TClosedSessionBook2::Book2FileName(TClosedSessionBookGroup *tGroup)
{
	(void) tGroup;

	if (CourtType == COURT_APPEAL)
		return "ClosB2fi_p.htm";

#if COURT_TYPE == COURT_AREAL
	if (*tGroup->types == TYPE_FIRM_LAW)
		return "ClosB2fida.htm";
#endif  // AREAL

	return "ClosB2fi__.htm";
}

const char *TClosedSessionBook2::ReCap2FileName()
{
	bool punishment = *group->types == TYPE_PUNISHMENT_LAW;

	switch (CourtType)
	{
		case COURT_REGIONAL : return punishment ? "ClosR2fipr.htm" : "ClosR2fi_r.htm";
		case COURT_AREAL :
		case COURT_MILITARY :
		{
			if (punishment)
				return "ClosR2fip_.htm";
		#if COURT_TYPE == COURT_AREAL
			else if (strchr(TYPE_TRADEALIKE_LAWS, *group->types))
				return "ClosR2fi__.htm";
		#endif  // AREAL
			// falldown
		}
		case COURT_ADMIN : return "ClosR2fid_.htm";
		case COURT_APPEAL : return punishment ? "ClosR2fipp.htm" : "ClosR2fi_p.htm";
	}

	return NULL;
}

TClosedSessionBookReCap *TClosedSessionBook2::NewReCap(TWindow *parent, TClosedSessionBookGroup *group)
{
	const char *fileName = ReCap2FileName();

	if (fileName)
		return new TClosedSessionBookReCap2(parent, group, fileName);

	return new TClosedSessionBookReCap1(parent, group);
}

void TClosedSessionBook2::Markers()
{
	tb1start = office.Find("tb1 start");
	hd1start = office.Find("hd1 start");
	tr1start = office.Find("tr1 start");
	tr1break = office.Find("tr1 break");
	tr1judge = office.Find("tr1 judge");
	tr1result = office.Find("tr1 result");
	tr1final = office.Find("tr1 final");
	tb1final = office.Find("tb1 final");
}

void TClosedSessionBook2::EmitRow(TGroup *group)
{
	TClosedBookSession *session = (TClosedBookSession *) group;

	if (returned.month != session->returned.month || returned.year != session->returned.year)
	{
		office.Emit(hd1start, tr1start, body_fields, session);
		returned = session->returned;
	}

	if (session->decisions.Count())
		*session << session->decisions[0];

	office.Emit(tr1start, tr1final, body_fields, session);

	for (int i = 1; i < session->decisions.Count(); i++)
	{
		*session << session->decisions[i];
		session->eventKind = session->kind;  // decision kind
		office.Emit(tr1break, tr1judge, body_fields, session);
		office.Emit(tr1result, tr1final, body_fields, session);
	}
}

TGroup *TClosedSessionBook2::Process(TQuery &q)
{
	TClosedBookSession *session = (TClosedBookSession *) TClosedSessionBook::Process(q);

	if (group->createReCap)
	{
		if (ReCap2FileName())
			group->CountReCap2(session);
		else
			group->CountReCap1(session);
	}

	return session;
}

// ----- functions -----------------------------------------------------------
static void closed_session_book(TWindow *parent, TClosedSessionBookGroup *group)
{
	TClosedSessionBook *const book = group->flags & BOOK_NEW_FORMAT ?
		(TClosedSessionBook *) new TClosedSessionBook1(parent, group) :
		(TClosedSessionBook *) new TClosedSessionBook2(parent, group);
	TVirtPtr VP(book);

	if (group->commonOutput)
		book->Execute(BOOK_EXEC_NORMAL);
	else
	{
		book->Execute(group->createBook ? BOOK_EXEC_NORMAL : BOOK_EXEC_NULL);

		if (group->createReCap)
		{
			TClosedSessionBookReCap *const reCap = book->NewReCap(parent, group);
			TVirtPtr VP(reCap);
			reCap->Execute();
		}

		if (group->createResults)
			TClosedSessionBookResults(parent, group).Execute();
	}
}

#include "ClosedSessionBookDialog.h"
#include "ClosedSessionBookDialog.cpp"

void closed_session_book(TWindow *parent)
{
	TClosedSessionBookGroup group;

	if (TClosedSessionBookDialog(parent, &group).Execute() == IDOK)
		closed_session_book(parent, &group);
}
