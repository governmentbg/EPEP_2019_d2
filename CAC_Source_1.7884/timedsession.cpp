#include "cac.h"

// ----- TTimedSessionGroup --------------------------------------------------
enum
{
	RULE_NULL	= 0,
	RULE_ACT	= 1,
	RULE_UNDO	= 2,
	RULE_NONE	= 3
};

enum
{
	ORDER_BY_TYND	= ORDER_BY_USER,
	ORDER_BY_JDTYN
};

class TTimedSessionGroup : public TFloatGroup
{
public:
	TTimedSessionGroup();

	virtual void FormCriteria(msql &m);

	char lawKinds[SIZE_OF_KINDS];
	long minNo;
	long maxNo;
	long minYear;
	long maxYear;
	TGroupArray judges;
	char compositions[SIZE_OF_COMPOSITIONS];
	char sesKinds[SIZE_OF_KINDS];
	long sessions;	// else decisions
	CDate minSesDate;
	CDate maxSesDate;
	CDate minDecDate;
	CDate maxDecDate;
	TGArray<TSubject> newSubjects;
	TGArray<TOldSubject> oldSubjects;

	long age;
	long rule;
	long pdec;
	bool undo;
};

TTimedSessionGroup::TTimedSessionGroup()
	: TFloatGroup("T_SESSION S JOIN T_LAWSUIT L ON L.F_NO = S.F_NO AND L.F_YEAR = S.F_YEAR AND L.F_TYPE = S.F_TYPE",
		"S.F_NO, S.F_YEAR, S.F_TYPE, S.F_KIND, S.F_DATE, S.F_JUDGE, S.F_RESULT, S.F_RETURNED, L.F_KIND AS F_LAW_KIND, "
		"L.F_SUBJECT, L.F_OLD_SUBJECT")
{
	Add(
		new TRadio("F_AGE_NULL", &age, AGE_NULL),
		new TRadio("F_AGE_ONE_OR_LESS", &age, AGE_ONE_OR_LESS),
		new TRadio("F_AGE_TWO_OR_LESS", &age, AGE_TWO_OR_LESS),
		new TRadio("F_AGE_MORE_THAN_TWO", &age, AGE_MORE_THAN_TWO),
		new TRadio("F_AGE_ONE_TO_THREE", &age, AGE_ONE_TO_THREE),
		new TRadio("F_AGE_MORE_THAN_THREE", &age, AGE_MORE_THAN_THREE),
		// 2008:203
		new TRadio("F_RULE_NULL", &rule, RULE_NULL),
		new TRadio("F_RULE_ACT", &rule, RULE_ACT),
		new TRadio("F_RULE_UNDO", &rule, RULE_UNDO),
		new TRadio("F_RULE_NONE", &rule, RULE_NONE),
		// 2008:205
		new TRadio("F_BY_TYND", &order, ORDER_BY_TYND),
		new TRadio("F_BY_JDTYN", &order, ORDER_BY_JDTYN),
		// 2008:212
		new TCheck("F_PUBLIC_DECISION", &pdec, TRUE),
		// 2009:077 from (removed) float session
		new TCharList("F_LAW_KINDS", lawKinds, sizeof lawKinds),
		new TLong("F_MIN_NO", &minNo),
		new TLong("F_MAX_NO", &maxNo),
		new TLong("F_MIN_YEAR", &minYear),
		new TLong("F_MAX_YEAR", &maxYear),
		// 2010:328
		new TRadio("F_SESSIONS", &sessions, TRUE),
		new TRadio("F_DECISIONS", &sessions, FALSE),
		new TDate("F_MIN_SES_DATE", &minSesDate, false),
		new TDate("F_MAX_SES_DATE", &maxSesDate, false),
		new TDate("F_MIN_DEC_DATE", &minDecDate, false),
		new TDate("F_MAX_DEC_DATE", &maxDecDate, false),
		// 2018-04-04
		new TCharList("F_COMPOSITIONS", compositions, sizeof compositions),
		new TCharList("F_SES_KINDS", sesKinds, sizeof sesKinds),
		NULL
	);

	order = ORDER_BY_TYND;	// new TRadio zeroes order, re-set it
	sessions = TRUE;
	pdec = -1;
	undo = false;
}

void TTimedSessionGroup::FormCriteria(msql &m)
{
	if (sessions)
		BeginCriteria(m);
	else
	{
		m.Begin("T_DECISION D JOIN T_SESSION S ON S.F_NO = D.F_NO AND S.F_YEAR = D.F_YEAR AND S.F_TYPE = D.F_TYPE "
			"AND S.F_KIND = D.F_SESSION_KIND AND S.F_DATE = D.F_DATE JOIN T_LAWSUIT L ON L.F_NO = D.F_NO AND "
			"L.F_YEAR = D.F_YEAR AND L.F_TYPE = D.F_TYPE",
			"S.F_NO, S.F_YEAR, S.F_TYPE, S.F_KIND, S.F_DATE, S.F_JUDGE, S.F_RESULT, D.F_FROM_DATE, L.F_SUBJECT, "
			"D.F_RETURNED AS F_JOIN_RET, D.F_KIND AS F_DEC_KIND, D.F_FLAGS, L.F_KIND AS F_LAW_KIND, L.F_OLD_SUBJECT");
	}

	m.AddChars("L.F_KIND", lawKinds, KIND_LAWSUITS);
	m.AddRange(sessions ? "S.F_NO" : "D.F_NO", minNo, maxNo);
	m.AddRange(sessions ? "S.F_YEAR" : "D.F_YEAR", minYear, maxYear);
	m.AddArray("F_UCN", judges, "S.F_JUDGE");
	m.AddCompos("L.F_COMPOSITION", compositions);
	m.AddChars("S.F_KIND", adjust_xxx_kinds(sesKinds, KIND_VISIBLE_CLOSEDS, KIND_CLOSED_SESSIONS), KIND_SESSIONS);
	m.AddArray("L.F_SUBJECT", newSubjects);
	m.AddArray("F_SUBJECT", oldSubjects, "L.F_OLD_SUBJECT");

	if (undo)
	{
		m.AddRange("S.F_DATE", minDecDate, maxDecDate);
		m.AddChars("S.F_RESULT", RESULT_UNDOS);
	}
	else
	{
		if (sessions)
			m.AddRange("S.F_DATE", minSesDate, maxSesDate);
		else
			m.AddRange("D.F_FROM_DATE", minDecDate, maxDecDate);

		if (pdec != -1)
			m.AddChar(pdec ? "S.F_RESULT" : "NOT S.F_RESULT", RESULT_PUBLIC_DECISION);
	}

	// 2014:212
	const char *sOrder;

	switch (order)
	{
		case ORDER_BY_DTYN :
		{
			sOrder = sessions ? "S.F_DATE, S.F_TYPE, S.F_YEAR, S.F_NO" : "D.F_FROM_DATE, D.F_TYPE, D.F_YEAR, D.F_NO";
			break;
		}
		case ORDER_BY_JDTYN :
		{
			sOrder = sessions ? "S.F_JUDGE, S.F_DATE, S.F_TYPE, S.F_YEAR, S.F_NO" :
				"S.F_JUDGE, D.F_FROM_DATE, D.F_TYPE, D.F_YEAR, D.F_NO";	// 2014:324 FIX: D.F_JUDGE -> S.
			break;
		}
		default :	// TYND
			sOrder = sessions ? "S.F_TYPE, S.F_YEAR, S.F_NO, S.F_DATE" : "D.F_TYPE, D.F_YEAR, D.F_NO, D.F_FROM_DATE";
	}

	m.AddOrder(sOrder);
}

// ----- TTimedSession -------------------------------------------------------
class TTimedSession : public TSession
{
public:
	TTimedSession();

	char decision[SIZE_OF_NAME];
	CDate decDate;
	CDate surDate;
	CDate joinRet;
	char decKind;
	char lawKind;
	long subject;
	long oldSubject;
};

TTimedSession::TTimedSession()
{
	Add(
		new TString("F_DECISION", decision, sizeof decision),
		new TDate("F_DEC_DATE", &decDate, false),
		new TDate("F_FROM_DATE", &decDate, false),
		new TDate("F_SUR_DATE", &surDate, false),
		new TDate("F_JOIN_RET", &joinRet, false),
		new TChar("F_DEC_KIND", &decKind),
		new TChar("F_LAW_KIND", &lawKind),
		new TLong("F_SUBJECT", &subject),
		new TLong("F_OLD_SUBJECT", &oldSubject),
		NULL
	);
}

#define session OFFICE_GROUP_CAST(TTimedSession)

static void session_lawsuit_kind(mstr &m, TGroup *group) { m.cat(Kind->Seek(session->lawKind)); }

static void session_lawsuit_subject(mstr &m, TGroup *group)
{
	m.printf("%s\v%s", TSubject::Code(session->subject, session->lawKind),
		Subject->Seek(session->key.type, session->subject));
	print_old_subject(m, session->oldSubject);
}

static void session_judge(mstr &m, TGroup *group) { if (atob(session->judge)) print_user(m, session->judge); }
static void session_result(mstr &m, TGroup *group) { m.cpy(Result->Seek(session->result)); }

#undef session

static const offield session_fields[] =
{
	{ "F_NO",		NULL },
	{ "F_YEAR",		NULL },
	{ "F_KIND",		session_lawsuit_kind },
	{ "F_SUBJECT",	session_lawsuit_subject },
	{ "F_JUDGE",	session_judge },
	{ "F_DATE",		NULL },
	{ "F_RESULT", 	session_result },
	{ "F_DECISION", 	NULL },
	{ "F_DEC_DATE",	NULL },
	{ "F_RETURNED",	NULL },
	{ "F_SUR_DATE",	NULL },
	{ NULL,		NULL }
};

// ----- TTimedSessionQuery --------------------------------------------------
class TTimedSessionQuery : public TProcess
{
public:
	TTimedSessionQuery(TWindow *parent, const char *tCriteria, const char *tUndoCrit, long tAge, long tRule, long tPDec);

	void Execute();

protected:
	const char *criteria;
	const char *undocrit;
	long age;
	long rule;
	long pdec;		// 2010:341 FIX: for BackFill()

	const char *tr1start;
	const char *tr2start;
	const char *tr2final;
	const char *tr1final;

	TOffice office;
	TTimedSession session;
	TSurround surround;

	mstr sSesKey;
	void SetSesKey();
	bool MatchAge(const CDate &start, const CDate &final);
	sign TwoMonths() { return age == AGE_TWO_OR_LESS || age == AGE_MORE_THAN_TWO ? -1 : false; }

	bool Match();
	bool MatchFirm();
	bool Fill(TDecision *decision);
	bool BackFill();
#if COURT_TYPE == COURT_AREAL
	bool SurrFill(TSurroundment *surroundment);
#endif  // AREAL
	void Emit(const char *start, const char *final);
};

TTimedSessionQuery::TTimedSessionQuery(TWindow *parent, const char *tCriteria, const char *tUndoCrit, long tAge, long tRule, long tPDec)
	: TProcess(parent, "справката", WAIT), criteria(tCriteria), undocrit(tUndoCrit), age(tAge), rule(tRule), pdec(tPDec)
{
}

void TTimedSessionQuery::SetSesKey()
{
	sSesKey.printf("%s от %Dг. по %s %ld/%ldг.", Kind->Seek(session.kind), &session.date, Type->Seek(session.key.type),
		session.key.no, session.key.year);
}

bool TTimedSessionQuery::MatchAge(const CDate &start, const CDate &final)
{
	return !age || age == interval2ActAge(start, final.Empty() ? Today : final, TwoMonths());
}

bool TTimedSessionQuery::Match()
{
	// 2008:198 LRQ (PUBLIC won't be finalizing any more)
	return strchr(RESULT_FINALIZEDS, session.result) || MatchFirm() || session.result == RESULT_PUBLIC_DECISION;
}

bool TTimedSessionQuery::MatchFirm()
{
#if COURT_TYPE == COURT_AREAL
	if (session.key.type == TYPE_FIRM_LAW && strchr(RESULT_FIRM_PSEUDOLIZEDS, session.result) && !try2surround(&session, NULL))
		return true;
#endif  // AREAL
	return false;
}

bool TTimedSessionQuery::Fill(TDecision *decision)
{
	long object = RULE_NULL;
	bool motives = false;	// 2012:048 LRQ: +related

	// 2012:097 LPR: moved here from object == RULE_NULL
	*session.decision = '\0';
	session.decDate.Clear();

	if (decision)
	{
		strcpy(session.decision, Kind->Seek(decision->kind));
		session.decDate = decision->fromDate;
		session.returned = decision->returned;
		motives = decision->flags & DECISION_MOTIVES;
		// motives without returned date is rule none
		if (!motives || !decision->returned.Empty())
			object = RULE_ACT;
	}
	// 2003:175 BTW: why for PUBLIC_DECISION only?
	else if (session.result == RESULT_PUBLIC_DECISION)
	{
		msql m;

		m.Begin(session.Name, "F_DATE, F_RESULT");
		m.Add(&session, "F_NO, F_YEAR, F_TYPE");
		m.Add("F_DATE");
		m.printf(">= %ld", session.date.Value());
		m.AddChars("F_RESULT", RESULT_UNDOS);
		m.AddOrder("F_DATE");

		TQuery q(str(m));
		if (q.Read())
		{
			TSession tSession;

			tSession << q;
			strcpy(session.decision, Result->Seek(tSession.result));
			session.decDate = tSession.date;
			// session.returned set by <<
			object = RULE_UNDO;
		}
	}

	if (object == RULE_NULL)
	{
		session.returned.Clear();
		object = RULE_NONE;
	}

	if (rule && rule != object)
		return false;

	return motives ? MatchAge(decision->fromDate, decision->returned) : MatchAge(session.date, session.decDate);
}

bool TTimedSessionQuery::BackFill()
{
	msql m(session.Name, "F_DATE, F_RESULT");

	m.Add(&session, "F_NO, F_YEAR, F_TYPE");
	m.Add("F_DATE");
	m.printf("<= %ld", session.date.Value());

	if (pdec != -1)
		m.AddChar(pdec ? "F_RESULT" : "NOT F_RESULT", RESULT_PUBLIC_DECISION);

	m.AddOrder("F_DATE DESC");
	strcpy(session.decision, Result->Seek(session.result));
	session.decDate = session.date;

	TQuery q(str(m));
	if (q.Read())
	{
		TSession tSession;

		tSession << q;
		session.date = tSession.date;
		session.result = tSession.result;

		return MatchAge(session.date, session.decDate);
	}

	dprintf("Липсва заседанието с резултат %s преди %s с резултат %s",
		Result->Seek(RESULT_PUBLIC_DECISION), str(sSesKey), session.decision);

	return false;
}

#if COURT_TYPE == COURT_AREAL
bool TTimedSessionQuery::SurrFill(TSurroundment *surroundment)
{
	if (surroundment->kind == KIND_CHANGE_SURROUND)
	{
		surround.key = surroundment->key;

		if (surround.Try("F_DATE"))
		{
			if (MatchAge(surround.date, session.date))
			{
				session.surDate = surround.date;
				return true;
			}
		}
		else
		{
			dprintf("Липсва %s %ld/%ldг. разгледан в %s",
				Type->Seek(surround.key.type), surround.key.no, surround.key.year, str(sSesKey));
		}
	}

	return false;		
}
#endif  // AREAL

void TTimedSessionQuery::Emit(const char *start, const char *final)
{
	office.Emit(start, final, session_fields, &session);
	count++;
}

void TTimedSessionQuery::Execute()
{
	if (office.Open("TimedSes.htm"))
	{
		int total = 0;

		tr1start = office.Find("tr1 start");
		tr1final = office.Find("tr1 final");
		tr2start = office.Find("tr2 start");
		tr2final = office.Find("tr2 final");

		office.Emit(NULL, tr1start, null_fields, NULL);
		count = 0;

		if (criteria)
		{
			TQuery q(criteria);
			while (q.Read())
			{
				session << q;
				SetSesKey();

				if (undocrit)
				{
					// 2012:086 LPR: motives date logic
					if ((session.flags & DECISION_MOTIVES) ?
						MatchAge(session.decDate, session.joinRet) &&
						(session.joinRet.Empty() ? rule == RULE_NULL || rule == RULE_NONE : rule != RULE_NONE) :
						// 2012:093 LPR/LRQ: decDate is guaranteed, can't be none
						rule != RULE_NONE && MatchAge(session.date, session.decDate))
					{
						strcpy(session.decision, Kind->Seek(session.decKind));
						session.returned = session.joinRet;
						Emit(tr1start, tr1final);
					}
				}
				else if (Match())
				{
					TGArray<TDecision> decisions;

					if (try2decide(&session, &decisions, NULL, "F_KIND, F_FROM_DATE, F_RETURNED, F_FLAGS"))
					{
						for (int i = 0; i < decisions.Count(); i++)
							if (Fill(decisions[i]))
								Emit(tr1start, tr1final);
					}
					else if (Fill(NULL))
						Emit(tr1start, tr1final);
				}

				wait->TickCount(++total, "обработени записа");
			}
		}

		if (undocrit)
		{
			if (rule != RULE_ACT)
			{
				TQuery q(undocrit);
				while (q.Read())
				{
					session << q;
					SetSesKey();

					if (BackFill())
						Emit(tr1start, tr1final);

					wait->TickCount(++total, "обработени записа");
				}
			}

			SetNames("Няма съдебни актове", "Един съдебен акт", "съдебни акта");
		}
		else
			SetNames("Няма заседания", "Едно заседание", "заседания");

	#if COURT_TYPE == COURT_AREAL
		if (criteria)
		{
			TQuery q(criteria);
			bool first = true;

			while (q.Read())
			{
				session << q;
				SetSesKey();

				TGArray<TSurroundment> surroundments;

				if (session.key.type == TYPE_FIRM_LAW && try2surround(&session, &surroundments))
				{
					for (int i = 0; i < surroundments.Count(); i++)
					{
						if (!SurrFill(surroundments[i]))
							continue;

						if (first)
						{
							// flush
							office.Emit(tr1final, tr2start, counter_fields, this);
							// reset
							SetNames(NULL, "Едно заявление", "заявления");
							count = 1;
							first = false;
						}
						else
							count++;

						Emit(tr2start, tr2final);
					}
				}

				wait->TickCount(++total, "обработени заявления");
			}
		}
	#endif  // AREAL

		maximum = LONG_MAX;
		office.Emit(tr2final, NULL, counter_fields, this);
		office.Show(NULL);
	}
}

// ----- query ---------------------------------------------------------------
#include "TimedSessionDialog.h"
#include "TimedSessionDialog.cpp"

void timed_session(TWindow *parent)
{
	TTimedSessionGroup group;
	TTimedSessionDialog(parent, &group).Execute();
}
