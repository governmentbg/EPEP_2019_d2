#include "cac.h"

#if COURT_TYPE == COURT_AREAL
static bool load_complete_decisions(const TLawsuit *lawsuit, TGroupArray &decisions)
{
	TDecision decision;
	msql m("T_DECISION D JOIN T_SESSION S ON S.F_NO = D.F_NO AND S.F_YEAR = D.F_YEAR AND S.F_TYPE = D.F_TYPE "
		"AND S.F_KIND = D.F_SESSION_KIND AND S.F_DATE = D.F_DATE",
		"D.F_DATE, D.F_FROM_DATE, D.F_SESSION_KIND, D.F_ACCOMPLYS, S.F_RETURNED AS F_RETURNED");

	m.AddLong("D.F_NO", lawsuit->key.no);
	m.AddLong("D.F_YEAR", lawsuit->key.year);
	m.AddChar("D.F_TYPE", lawsuit->key.type);

	m.AddChar("(D.F_RESULT", RESULT_CANCEL_LAWSUIT);
	m.NextOr();
	m.AddIsNull("D.F_ACCOMPLYS", false);
	m.cat(")");

	m.AddChars("((S.F_KIND", KIND_OPEN_SESSIONS);
	m.Add("S.F_DATE");
	m.printf("<= %ld)", lawsuit->completed.Value());
	m.NextOr();
	m.AddChars("(S.F_KIND", KIND_CLOSED_SESSIONS);
	m.Add("S.F_RETURNED");
	m.printf("<= %ld))", lawsuit->completed.Value());

	CDate bestDate;
	TQuery q(str(m));

	while (q.Read())
	{
		decision << q;
		const CDate &sessDate = strchr(KIND_OPEN_SESSIONS, decision.sessionKind) ? decision.date : decision.returned;

		if (sessDate > bestDate)
		{
			decisions.Flush();
			bestDate = sessDate;
		}

		if (sessDate >= bestDate)
			decisions.AddCopy(&decision);
	}

	return decisions.Count();
}

// ----- TQueryBankPart ------------------------------------------------------
struct TQueryBankPart
{
	long webQuery;
	char webFile[SIZE_OF_PATH];
	long query;
	CDate minDate;
	CDate maxDate;
};

static void make_period(mstr &m, const TQueryBankPart &part)
{
	if (part.minDate.Empty())
		m.printf("ƒŒ %0D √.", &part.maxDate);
	else if (part.maxDate.Empty())
		m.printf("Œ“ %0D √.", &part.minDate);
	else
	{
		if (part.minDate.DayOfYear() == 1 && part.maxDate.day == 31 && part.maxDate.month == 12 &&
			part.minDate.year == part.maxDate.year)
		{
			m.printf("œ–≈« %ld √Œƒ»Õ¿", part.minDate.year);
		}
		else
			m.printf("Œ“ %0D √. ƒŒ %0D √.", &part.minDate, &part.maxDate);
	}
}

// ----- TQueryBankGroup -----------------------------------------------------
class TQueryBankGroup : public TGroup
{
public:
	TQueryBankGroup();

	TQueryBankPart incomplete;
	TQueryBankPart completed;
	long header;
	long yourNo;
};

TQueryBankGroup::TQueryBankGroup()
	: TGroup("T_QUERY_BANK", NULL)
{
	Add(
		// INCOMPLETE
		new TCheck("F_INCOMPLETE_WEB_QUERY", &incomplete.webQuery, TRUE),
		new TString("F_INCOMPLETE_WEB_FILE", incomplete.webFile, sizeof incomplete.webFile),
		new TCheck("F_INCOMPLETE_QUERY", &incomplete.query, TRUE),
		new TDate("F_INCOMPLETE_MIN_DATE", &incomplete.minDate, false),
		new TDate("F_INCOMPLETE_MAX_DATE", &incomplete.maxDate, false),
		// COMPLETED
		new TCheck("F_COMPLETED_WEB_QUERY", &completed.webQuery, TRUE),
		new TString("F_COMPLETED_WEB_FILE", completed.webFile, sizeof completed.webFile),
		new TCheck("F_COMPLETED_QUERY", &completed.query, TRUE),
		new TDate("F_COMPLETED_MIN_DATE", &completed.minDate, false),
		new TDate("F_COMPLETED_MAX_DATE", &completed.maxDate, false),
		new TCheck("F_HEADER", &header, TRUE),
		new TLong("F_YOUR_NO", &yourNo),
		NULL
	);

	build_fn(OUT_PATH, "BANKRUPT.htm", incomplete.webFile);
	incomplete.query = TRUE;
	build_fn(OUT_PATH, "BANKRFIN.htm", completed.webFile);
	completed.query = TRUE;
	completed.minDate = CDate(1, 1, Today.year);
	completed.maxDate = CDate(31, 12, Today.year);
	header = TRUE;
}

// ----- TBankWebHeader ------------------------------------------------------
class TBankWebHeader : public TDummyGroup
{
public:
	TBankWebHeader(const TQueryBankPart &part);

	mstr period;
};

TBankWebHeader::TBankWebHeader(const TQueryBankPart &part)
{
	Add(new TText("F_PERIOD", &period, period.size));
	make_period(period, part);
}

// ----- TBankWebLawsuit -----------------------------------------------------
class TBankWebLawsuit : public TLawsuit
{
public:
	TBankWebLawsuit(const TQueryBankPart &tPart) : part(tPart) { }

	void Load();

	const TQueryBankPart &part;
	TGArray<TSideWind> sideWinds;
};

void TBankWebLawsuit::Load()
{
	sideWinds.Flush();
	load_sidebasicv(this, sideWinds);
}

// ----- TBankWebDecision ----------------------------------------------------
class TBankWebDecision : public TDecision
{
public:
	bool operator<=(const TBankWebDecision &that) const;
};

bool TBankWebDecision::operator<=(const TBankWebDecision &that) const
{
	return fromDate != that.fromDate ? fromDate > that.fromDate :
		strchr(KIND_RCDKEY_COMPARES, kind) <= strchr(KIND_RCDKEY_COMPARES, that.kind);
}

// ----- fields --------------------------------------------------------------
static const char DEBTORS[] = { INVOLVEMENT_DEBTOR, INVOLVEMENT_DEFENDANT, '\0' };
static const char CREDITORS[] = { INVOLVEMENT_REQUESTER, '\0' };
static constant CANCEL_100 = "œÂÍ‡ÚÂÌÓ Ì‡ ÓÒÌÓ‚‡ÌËÂ ˜Î. 100 √œ ";

static void full_side_list(mstr &m, TGArray<TSideWind> &sideWinds, const char *involvements)
{
	for (int i = 0; i < sideWinds.Count(); i++)
		if (strchr(involvements, sideWinds[i]->involvement))
			add_side_name(m, sideWinds[i], ", ", false);
}

#define lawsuit OFFICE_GROUP_CAST(TBankWebLawsuit)

static void bank_debtors(mstr &m, TGroup *group) { full_side_list(m, lawsuit->sideWinds, DEBTORS); }
static void bank_creditors(mstr &m, TGroup *group) { full_side_list(m, lawsuit->sideWinds, CREDITORS); }
static void bank_date(mstr &m, TGroup *group) { lawsuit->date.Print(m, "%0D"); }
static void bank_web_completed(mstr &m, TGroup *group) { lawsuit->completed.Print(m, "%0D"); }

static void bank_web_decision(mstr &m, TGroup *group)
{
	TGSArray<TBankWebDecision> decisions;

	if (load_complete_decisions(lawsuit, decisions))
	{
		const CDate &fromDate = decisions[0]->fromDate;
		mstr accomplys;
		bool canceled = false;

		for (int i = 0; i < decisions.Count(); i++)
		{
			const TBankWebDecision *decision = decisions[i];

			if (decision->fromDate != fromDate)
				break;

			if (decision->result == RESULT_CANCEL_LAWSUIT)
				canceled = true;

			for (const char *acc = decision->accomplys; *acc; acc++)
				accomplys.cach_nx(*acc);
		}

		if (any(accomplys))
		{
			for (const char *acc = str(accomplys); *acc; acc++)
			{
				m.sep("\v");
				m.cat(Accomply->Seek(*acc));
			}
		}
		else if (canceled)
			m.cat(CANCEL_100);
	}
	else
	{
		TSession session;

		if (find_finalizer(lawsuit, session, lawsuit->part.minDate, lawsuit->part.maxDate))
			if (strchr(RESULT_ALL_CANCELEDS, session.result))
				m.cat(CANCEL_100);
	}
}

#undef lawsuit

static offield bank_web_fields[] =
{
	{ "F_PERIOD",	NULL },
	{ "F_NO",		NULL },
	{ "F_YEAR",		NULL },
	{ "F_DEBTORS",	bank_debtors },
	{ "F_CREDITORS",	bank_creditors },
	{ "F_DATE",		bank_date },
	{ "F_COMPLETED",	bank_web_completed },
	{ "F_DECISION",	bank_web_decision },
	{ NULL,	NULL },
};

// ----- query_bank_web ------------------------------------------------------
static void active_wait(int cycles)
{
	for (int i = 0; i < cycles; i++)
	{
		Sleep(50);
		GetApplicationObject()->PumpWaitingMessages();
	}
}

static void make_criteria(msql &m, const TQueryBankPart &part, bool complete)
{
	m.AddChars("F_TYPE", TYPE_TRADEALIKE_LAWS);
	m.AddRange(complete ? "F_COMPLETED" : "F_DATE", part.minDate, part.maxDate);
	m.AddChars("F_SOURCE_KIND", KIND_INCONSISTENCE_REQUESTS);
	if (!complete)
		m.AddIsNull("F_COMPLETED", true);
	m.AddOrder("F_YEAR, F_NO, F_TYPE");
}

static void query_bank_web(const TQueryBankPart &part, const char *name, bool complete)
{
	TOffice office;
	office.SetStub("\x06");

	if (!part.webQuery || !office.Open(name, OFFICE_COPY_ALL, part.webFile))
		return;

	constant tr1start = office.Find("tr1 start");
	constant tr1final = office.Find("tr1 final");
	TBankWebLawsuit lawsuit(part);
	msql m(lawsuit.Name, "F_NO, F_YEAR, F_TYPE, F_KIND, F_DATE, F_COMPLETED");
	TBankWebHeader header(part);

	make_criteria(m, part, complete);
	TQuery q(str(m));
	office.Emit(NULL, tr1start, bank_web_fields, &header);

	while (q.Read())
	{
		lawsuit << q;
		lawsuit.Load();
		office.Emit(tr1start, tr1final, bank_web_fields, &lawsuit);
	}

	office.Emit(tr1final, NULL, null_fields, NULL);
	office.Close();
	show_html(NULL, part.webFile, false);
	active_wait(15);
}

// ----- TBankQueryHeader ----------------------------------------------------
class TBankQueryHeader : public TBankWebHeader
{
public:
	TBankQueryHeader(const TQueryBankPart &part, const char *tWhat);

	mstr what;
};

TBankQueryHeader::TBankQueryHeader(const TQueryBankPart &part, const char *tWhat)
	: TBankWebHeader(part)
{
	Add(new TText("F_WHAT", &what, what.size));
	what.cat(tWhat);
}

// ----- TBankQueryDecision --------------------------------------------------
class TBankQueryDecision : public TDecision
{
public:
	bool operator<=(const TBankQueryDecision &that) const;
};

bool TBankQueryDecision::operator<=(const TBankQueryDecision &that) const
{
	return fromDate != that.fromDate ? fromDate < that.fromDate : date != that.date ? date < that.date :
		strchr(KIND_RCDKEY_COMPARES, kind) <= strchr(KIND_RCDKEY_COMPARES, that.kind);
}

// ----- TBankQueryOutReg ----------------------------------------------------
class TBankQueryOutReg : public TOutReg
{
public:
	bool Load(TGSArray<TBankQueryDecision> &decisions);

	void AppealBefore(mstr &m);
	void AppealActs(mstr &m);
	void AppealResults(mstr &m);

	TInReg inReg;
	const TBankQueryDecision *decision;
};

bool TBankQueryOutReg::Load(TGSArray<TBankQueryDecision> &decisions)
{
	inReg.key.no = inNo;
	inReg.key.year = inYear;

	if (!inReg.Get("F_KIND, F_DATE, F_RECEIVER, F_RECEIVER_TYPE, F_DECISION_KIND, F_DECISION_DATE"))
	{
		return error("√Â¯Í‡ ÔË ˜ÂÚÂÌÂ Ì‡ ‚ıÓ‰ˇ˘ ‰ÓÍÛÏÂÌÚ %ld/%ld„. ÔÓ %s %ld/%ld„.", inNo, inYear,
			Type->Seek(lawsuit.type), lawsuit.no, lawsuit.year);
	}

	for (int i = 0; i < decisions.Count(); i++)
	{
		decision = decisions[i];

		if (decision->kind == inReg.decisionKind && decision->date == inReg.decisionDate)
			return true;
	}

	return false;
}

void TBankQueryOutReg::AppealBefore(mstr &m)
{
	m.printf("%0D\v", &inReg.date);
	print_sender(m, inReg.receiverType, inReg.receiver);
}

void TBankQueryOutReg::AppealActs(mstr &m)
{
	m.printf("%s π %ld/%ld „. ", Kind->Seek(inReg.kind), inReg.key.no, inReg.key.year);
	m.printf("ÔÓÚË‚ %s ÓÚ %0D", Kind->Seek(decision->kind), &decision->fromDate);

	TGArray<TSideWind> sideWinds;
	load_sidebasicv(&inReg, sideWinds);

	for (int i = 0; i < sideWinds.Count(); i++)
		if (sideWinds[i]->Appealer())
			add_side_name(m, sideWinds[i], i ? ", " : " ÓÚ ", false);
}

static void print_resolution(mstr &m, const char *prefix, char resolution)
{
	if (resolution != RESOLUTION_FAKE)
	{
		m.sep("\v");
		m.printf("%s: %s", prefix, Resolution->Seek(resolution));
	}
}

void TBankQueryOutReg::AppealResults(mstr &m)
{
	print_resolution(m, HIGHER_INST_1, resultI);
	print_resolution(m, "¬ —", resultKS);
	print_resolution(m, HIGHER_INST_2, resultII);
	print_resolution(m, AppNameMinistryShort, resultMJ);
}

#define outReg OFFICE_GROUP_CAST(TBankQueryOutReg)

static void bank_appeal_before(mstr &m, TGroup *group) { outReg->AppealBefore(m); }
static void bank_appeal_acts(mstr &m, TGroup *group) { outReg->AppealActs(m); }
static void bank_appeal_results(mstr &m, TGroup *group) { outReg->AppealResults(m); }

#undef outReg

static offield bank_query_outreg_fields[] =
{
	{ "F_APPEAL_COURT",	bank_appeal_before },
	{ "F_APPEAL_ACTS",	bank_appeal_acts },
	{ "F_APPEAL_RESULTS",	bank_appeal_results },
	{ NULL, NULL }
};

// ----- TBankQueryLawsuit ---------------------------------------------------
class TBankQueryLawsuit : public TBankWebLawsuit
{
public:
	TBankQueryLawsuit(const TQueryBankPart &part) : TBankWebLawsuit(part) { }
	void Load();

	TGSArray<TSession> sessions;
	TGSArray<TBankQueryDecision> decisions;
	TGSArray<TInReg> inRegs;
	TGSArray<TBankQueryOutReg> outRegs;

	void AccomplyDates(mstr &m, const char *accomplys) const;
	void AccomplyDates(mstr &m, char accomply) const { AccomplyDates(m, C2S[accomply]); }
	void FirstCreditorsMeeting(mstr &m) const;
	void Judge(mstr &m) const;
	void Remark(mstr &m) const;
	void StoppedLaw(mstr &m) const;
	void Syndicates(mstr &m) const;
};

void TBankQueryLawsuit::Load()
{
	TBankWebLawsuit::Load();

	TSession session;
	TDecision decision;

	sessions.Flush();
	session.key = key;
	session.LoadFloat(sessions, "F_KIND, F_DATE, F_RETURNED, F_JUDGE, F_RESULT, F_FLAGS, F_TEXT");

	decisions.Flush();
	decision.key = key;
	decision.LoadFloat(decisions, "F_KIND, F_DATE, F_FROM_DATE, F_SESSION_KIND, F_ACCOMPLYS, F_SYNDICATE, F_TEXT");

	TOutReg outReg;
	msql m;

	outRegs.Flush();
	outReg.lawsuit = key;
	outReg.FloatCriteria(m, "F_IN_NO, F_IN_YEAR, F_RESULT_I, F_RESULT_II, F_RESULT_KS, F_RESULT_MJ");
	m.AddIsNull("F_IN_NO", false);

	TQuery q(str(m));
	while (q.Read())
	{
		TBankQueryOutReg *tOutReg = new TBankQueryOutReg();

		*tOutReg << q;
		tOutReg->lawsuit = key;

		if (tOutReg->Load(decisions))
			outRegs.Add(tOutReg);
		else
			delete tOutReg;
	}
}

void TBankQueryLawsuit::AccomplyDates(mstr &m, const char *accomplys) const
{
	for (int i = 0; i < decisions.Count(); i++)
	{
		const TDecision *decision = decisions[i];

		if (intersect(decision->accomplys, accomplys))
		{
			m.sep("\v");
			decision->fromDate.Print(m, "%0D");
		}
	}
}

void TBankQueryLawsuit::FirstCreditorsMeeting(mstr &m) const
{
	for (int i = 0; i < sessions.Count(); i++)
	{
		const TSession *session = sessions[i];

		if (session->kind == KIND_CREDITORS_MEETING && !strchr(RESULT_UNCARRIEDS, session->result) &&
			LEFT_MEETING || !strchr(RESULT_LEFTS, session->result))
		{
			m.printf("%0D", &session->date);
			break;
		}
	}
}

void TBankQueryLawsuit::Judge(mstr &m) const
{
	const char *lastJudge = judge;
	CDate lastDate = date;

	for (int i = 0; i < sessions.Count(); i++)
	{
		const TSession *session = sessions[i];

		if ((session->flags & SESSION_CHANJED))
		{
			lastJudge = session->judge;
			lastDate = session->date;
		}
	}

	print_user(m, lastJudge);
	m.printf("\v%0D", &lastDate);
}

void TBankQueryLawsuit::StoppedLaw(mstr &m) const
{
	const TSession *stopped = NULL;

	for (int i = 0; i < sessions.Count(); i++)
	{
		const TSession *session = sessions[i];
		bool open = strchr(KIND_OPEN_SESSIONS, session->kind);

		if (strchr(RESULT_STOPPEDS, session->result))
			stopped = session;
		else if (open)  // even if fixed or uncarried?
			stopped = NULL;
	}

	if (stopped)
		m.printf("%0D\v%s", &(open ? stopped->date : stopped->returned), str(stopped->text));
}

void TBankQueryLawsuit::Syndicates(mstr &m) const
{
	for (int i = 0; i < decisions.Count(); i++)
	{
		const TDecision *decision = decisions[i];

		if (any(decision->syndicate))
		{
			m.sep("\v\v");
			m.cat(decision->syndicate);
		}
	}
}

void TBankQueryLawsuit::Remark(mstr &m) const
{
	if (completed.Empty())
	{
		if (sessions.Count() > 0)
		{
			const TSession *session = sessions[sessions.Count() - 1];
			const char *format = NULL;

			if (session->result == RESULT_FIXED)
				format = "%s Á‡ %0D";
			else if (session->result == RESULT_PUBLIC_DECISION)
				format = "%s ÓÚ %0D";

			if (format)
				m.printf(format, Result->Seek(session->result), &session->date);
		}
	}
	else if (decisions.Count() > 0)
	{
		const TDecision *decision = decisions[decisions.Count() - 1];

		m.printf("%0D\n%s", &decision->fromDate, str(decision->text));
	}
}

// ----- fields --------------------------------------------------------------
#define lawsuit OFFICE_GROUP_CAST(TBankQueryLawsuit)

static void bank_subcount(mstr &m, TGroup *group) { subcount(m, lawsuit->outRegs.Count()); }

static void bank_canceled_100(mstr &m, TGroup *group)
{
	TSession session;

	if (find_finalizer(lawsuit, session) && strchr(RESULT_ALL_CANCELEDS, session.result))
		m.printf("%0D", &session.date);
}

static void bank_decision_refusal(mstr &m, TGroup *group) { lawsuit->AccomplyDates(m, ACCOMPLY_TL_631); }
static void bank_decision_cancel_632(mstr &m, TGroup *group) { lawsuit->AccomplyDates(m, ACCOMPLY_TL_632S); }
static void bank_agreement_740(mstr &m, TGroup *group) { lawsuit->AccomplyDates(m, ACCOMPLY_TL_740); }
static void bank_decision_630(mstr &m, TGroup *group) { lawsuit->AccomplyDates(m, ACCOMPLY_TL_630S); }
static void bank_first_creditors_meeting(mstr &m, TGroup *group) { lawsuit->FirstCreditorsMeeting(m); }
static void bank_claims_list(mstr &m, TGroup *group) { lawsuit->AccomplyDates(m, ACCOMPLY_TL_692); }
static void bank_decision_health_plan_707(mstr &m, TGroup *group) { lawsuit->AccomplyDates(m, ACCOMPLY_TL_707); }
static void bank_decision_bankruptcy_711(mstr &m, TGroup *group) { lawsuit->AccomplyDates(m, ACCOMPLY_TL_711); }
static void bank_distribution_account_729(mstr &m, TGroup *group) { lawsuit->AccomplyDates(m, ACCOMPLY_TL_729); }
static void bank_decis_bankruptcy_finalized(mstr &m, TGroup *group) { lawsuit->AccomplyDates(m, ACCOMPLY_TL_735); }
static void bank_decis_restore_debtor_rights(mstr &m, TGroup *group) { lawsuit->AccomplyDates(m, ACCOMPLY_TL_755); }
static void bank_stopped_law(mstr &m, TGroup *group) { lawsuit->StoppedLaw(m); }
static void bank_renew_744(mstr &m, TGroup *group) { lawsuit->AccomplyDates(m, ACCOMPLY_TL_744); }
static void bank_assignee_in_bankruptcy(mstr &m, TGroup *group) { lawsuit->Syndicates(m); }
static void bank_judge(mstr &m, TGroup *group) { lawsuit->Judge(m); }
static void bank_remark(mstr &m, TGroup *group) { lawsuit->Remark(m); }

#undef lawsuit

static offield bank_query_fields[] =
{
	{ "F_YOUR_NO",	NULL },
	{ "F_WHAT",		NULL },
	{ "F_PERIOD",	NULL },
	{ "F_NO",		NULL },
	{ "F_YEAR",		NULL },
	{ "F_DEBTORS",	bank_debtors },
	{ "F_CREDITORS",	bank_creditors },
	{ "F_DATE",		bank_date },
	{ "F_SUBCOUNT",		bank_subcount },
	{ "F_CANCELED_100",	bank_canceled_100 },
	{ "F_ACCOMPLY_TL_631",	bank_decision_refusal },
	{ "F_ACCOMPLY_TL_632S",	bank_decision_cancel_632 },
	{ "F_ACCOMPLY_TL_740",	bank_agreement_740 },
	{ "F_ACCOMPLY_TL_630S",	bank_decision_630 },
	{ "F_FIRST_MEETING",	bank_first_creditors_meeting },
	{ "F_ACCOMPLY_TL_692",	bank_claims_list },
	{ "F_ACCOMPLY_TL_707",	bank_decision_health_plan_707 },
	{ "F_ACCOMPLY_TL_711",	bank_decision_bankruptcy_711 },
	{ "F_ACCOMPLY_TL_729",	bank_distribution_account_729 },
	{ "F_ACCOMPLY_TL_735",	bank_decis_bankruptcy_finalized },
	{ "F_ACCOMPLY_TL_755",	bank_decis_restore_debtor_rights },
	{ "F_STOPPED",		bank_stopped_law },
	{ "F_ACCOMPLY_TL_744",	bank_renew_744 },
	{ "F_JUDGE",		bank_judge },
	{ "F_SYNDICATES",		bank_assignee_in_bankruptcy },
	{ "F_REMARK",		bank_remark },
	{ NULL, NULL },
};

// ----- query_bank ----------------------------------------------------------
static void query_bank(TQueryBankGroup &group, bool complete, const char *what)
{
	const TQueryBankPart &part = complete ? group.completed : group.incomplete;
	TOffice office;

	if (!part.query || !office.Open("BankQuery.htm"))
		return;

	constant hd1start = office.Find("hd1 start");
	constant hd1final = office.Find("hd1 final");
	constant tr1start = office.Find("tr1 start");
	constant tr1noYear = office.Find("tr1 no year");
	constant tr1appealBefore = office.Find("tr1 appeal before");
	constant tr1firstMeeting = office.Find("tr1 first meeting");
	constant tr1appealActs = office.Find("tr1 appeal acts");
	constant tr1beforeJudge = office.Find("tr1 before judge");
	constant tr1afterRemark = office.Find("tr1 after remark");
	constant tr1final = office.Find("tr1 final");

	TBankQueryLawsuit lawsuit(part);
	msql m(lawsuit.Name, "F_NO, F_YEAR, F_TYPE, F_KIND, F_DATE, F_JUDGE, F_COMPLETED");
	TBankQueryHeader header(part, what);

	make_criteria(m, part, complete);
	TQuery q(str(m));

	office.Emit(NULL, hd1start, null_fields, NULL);
	if (group.header)
		office.Emit(hd1start, hd1final, bank_query_fields, &group);
	office.Emit(hd1final, tr1start, bank_query_fields, &header);

	while (q.Read())
	{
		lawsuit << q;
		lawsuit.Load();

		office.Emit(tr1start, tr1appealBefore, bank_query_fields, &lawsuit);
		if (lawsuit.outRegs.Count())
			office.Emit(tr1appealBefore, tr1firstMeeting, bank_query_outreg_fields, lawsuit.outRegs[0]);
		else
			office.Emit(tr1appealBefore, tr1firstMeeting, null_fields, NULL);
		office.Emit(tr1firstMeeting, tr1appealActs, bank_query_fields, &lawsuit);
		if (lawsuit.outRegs.Count())
			office.Emit(tr1appealActs, tr1beforeJudge, bank_query_outreg_fields, lawsuit.outRegs[0]);
		else
			office.Emit(tr1appealActs, tr1beforeJudge, null_fields, NULL);
		office.Emit(tr1beforeJudge, tr1final, bank_query_fields, &lawsuit);

		for (int i = 1; i < lawsuit.outRegs.Count(); i++)
		{
			office.Emit(tr1start, tr1noYear, null_fields, NULL);
			office.Emit(tr1appealBefore, tr1firstMeeting, bank_query_outreg_fields, lawsuit.outRegs[i]);
			office.Emit(tr1appealActs, tr1beforeJudge, bank_query_outreg_fields, lawsuit.outRegs[i]);
			office.Emit(tr1afterRemark, tr1final, null_fields, NULL);
		}
	}

	office.Emit(tr1final, NULL, null_fields, NULL);
	office.Show(NULL);
	active_wait(15);
}

// ----- entry point ---------------------------------------------------------
#include "QueryBankDialog.h"
#include "QueryBankDialog.cpp"

#include "QueryBankWebDialog.h"
#include "QueryBankWebDialog.cpp"

void query_bank_html(TWindow *parent, bool allowWeb)
{
	TQueryBankGroup group;
	int result;

	group.incomplete.webQuery = allowWeb;
	group.completed.webQuery = allowWeb;

	if (allowWeb)
		result = TQueryBankWebDialog(parent, &group).Execute();
	else
		result = TQueryBankDialog(parent, &group).Execute();

	if (result == IDOK)
	{
		TWaitWindow wait(parent, WAIT);

		query_bank_web(group.incomplete, "Bankrupt.htm", false);
		query_bank_web(group.completed, "BankrFin.htm", true);
		query_bank(group, false, "—œ–¿¬ ¿ «¿ ƒ¬»∆≈Õ»≈“Œ Õ¿ ƒ≈À¿“¿ œŒ Õ≈—⁄—“Œﬂ“≈ÀÕŒ—“\vŒ¡–¿«”¬¿Õ»");
		query_bank(group, true, "ƒ≈À¿ œŒ Õ≈—⁄—“Œﬂ“≈ÀÕŒ—“, œ–» Àﬁ◊»À» œ–≈«");
	}
}
#endif  // AREAL
