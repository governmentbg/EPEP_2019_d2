#include "cac.h"

#if COURT_TYPE == COURT_AREAL
// ----- RESULT_ -------------------------------------------------------------
static constant RESULT_IGNORE_NEXTS = "9Q";
#endif  // AREAL

enum
{
	ROW_MAX	= 27,
	COL_MAX	= 2,
	COL_JUDGE	= 11
};

enum
{
	TB_SHORT	= 1,
	TB_LONG	= 2,
	TB_SPECIAL	= 3
};

// ------------------------- TTimeBookGroup ---------------------------
class TTimeBookGroup : public TSessionBookGroup
{
public:
	TTimeBookGroup();

	virtual void FormCriteria(msql &m);

	long instance;
	char compositions[SIZE_OF_COMPOSITIONS];
	long cumulative;
	long beginNo;
	long book;
	long bookMno;
	long summary;
	long judgeSummary;
	long resultSet;

protected:
	CDate beginDate;
	long format;
	long period;
	long order;
	CTime time;
};

TTimeBookGroup::TTimeBookGroup()
	: TSessionBookGroup()
{
	Add(
		new TCharList("F_COMPOSITIONS", compositions, sizeof compositions),
		new TCheck("F_CUMULATIVE", &cumulative, TRUE),
		new TLong("F_BEGIN_NO", &beginNo),
		new TDate("F_BEGIN_DATE", &beginDate, false),
		new TCheck("F_BOOK", &book, TRUE),
		new TCheck("F_BOOK_MNO", &bookMno, TRUE),
		new TCheck("F_SUMMARY", &summary, TRUE),
		new TCheck("F_JUDGE_SUMMARY", &judgeSummary, TRUE),
		new TCheck("F_RESULT_SET", &resultSet, TRUE),
		new TRadio("F_INSTANCE_1_LAWS", &instance, BOOK_1ST_INST),
		new TRadio("F_INSTANCE_2_LAWS", &instance, BOOK_VZ_INST),
		new TRadio("F_INSTANCE_3_LAWS", &instance, BOOK_KS_INST),
		new TRadio("F_INSTANCE_2_3_LAWS", &instance, BOOK_ALL_2ND_INST),
		new TRadio("F_PERIOD_DAY", &period, DAY_WHOLE),
		new TRadio("F_PERIOD_MORNING", &period, DAY_MORNING),
		new TRadio("F_PERIOD_AFTERNOON", &period, DAY_AFTERNOON),
		new TTime("F_TIME", &time),
		new TRadio("F_ORDER1", &order, true),
		new TRadio("F_ORDER2", &order, false),
		NULL
	);
	order = true;
	period = DAY_WHOLE;
}

void TTimeBookGroup::FormCriteria(msql &m)
{
	m.Begin("T_SESSION S JOIN T_LAWSUIT L ON S.F_TYPE = L.F_TYPE AND S.F_NO = L.F_NO AND S.F_YEAR = L.F_YEAR "
		"JOIN T_SUBJECT B ON B.F_SUBJECT = L.F_SUBJECT AND B.F_TYPE = L.F_TYPE ",
		"S.*, L.F_KIND AS F_LAW_KIND, L.F_QUICK AS F_LAW_QUICK, L.F_DATE AS F_LAWSUIT_DATE, L.F_FINISHED AS "
		"F_LAW_FINISHED, L.F_SUBJECT, L.F_GRANTED AS F_LAW_GRANTED, L.F_SOURCE_KIND AS F_LAW_SOURCE_KIND, "
		"L.F_TEXT AS F_LAWSUIT_TEXT, L.F_AGE AS F_LAW_AGE, S.F_COMPOSITION AS F_SES_COMPOSITION ");

	m.AddChar("S.F_TYPE", types[0]);
	m.AddRange("S.F_DATE", cumulative ? beginDate : minDate, maxDate);
	m.AddChars("L.F_KIND", instance_law_kinds(instance));
	m.AddChars("S.F_KIND", KIND_OPEN_SESSIONS);
	m.AddChars("S.F_RESULT NOT", RESULT_UNLINKEDS);
	m.AddCompos("S.F_COMPOSITION", compositions);
	m.AddAmPm("S.F_TIME", time, period);
#if COURT_TYPE == COURT_APPEAL
	if (types[0] == TYPE_PUNISHMENT_LAW && (book || bookMno))
		m.AddLike(bookMno ? "B.F_ATTRIB_I" : "NOT B.F_ATTRIB_I", C2S[POST_TIMEBK_MNO], true);
#endif
	m.AddOrder(order ? "S.F_DATE, S.F_TIME, L.F_YEAR, L.F_NO" : "S.F_DATE, L.F_YEAR, L.F_NO");
}

#define time_group OFFICE_GROUP_CAST(TTimeBookGroup)

static void timeb_title_instance(mstr &m, TGroup *group)
{
	switch (time_group->instance) 
	{
		case BOOK_1ST_INST : m.cat("ПЪРВОИНСТАНЦИОННИ"); break;
		case BOOK_VZ_INST : m.cat("ВЪЗЗИВНИ"); break;
		case BOOK_KS_INST : m.cat("КАСАЦИОННИ"); break;
		case BOOK_ALL_2ND_INST : m.cat("ВЪЗЗИВНИ/КАСАЦИОННИ"); break;
		default : m.cat("ВСИЧКИ");	// can't happen (tm)
	}
}

static void timeb_title_type(mstr &m, TGroup *group) 
{
	char types[2] = { time_group->types[0], '\0' };
	print_types_list(m, types, false);
}

static void timeb_title_compos(mstr &m, TGroup *group) { print_compositions_list(m, time_group->compositions, "с-в"); }

static void recap_period(mstr &m, TGroup *group)
{
	if (time_group->minDate != time_group->maxDate)
		m.printf("%0D - %0D", &time_group->minDate, &time_group->maxDate);
	else
		m.printf("%0D", &time_group->minDate);
}

#undef time_group

static const offield title_fields[]=
{
	{ "F_INSTANCE", timeb_title_instance },
	{ "F_TYPE", timeb_title_type },
	{ "F_COMPOSITIONS", timeb_title_compos },
	{ "F_PERIOD", recap_period },
	{NULL, NULL}
};

class TTimeBookDecision : public TDecision
{
public:
	bool operator<=(const TTimeBookDecision &that) const { return TRCDKeyContainer::operator<=(that); }

	static bool LoadDecisions(const TSession *session, TGSArray<TTimeBookDecision> *decisions);
};

bool TTimeBookDecision::LoadDecisions(const TSession *session, TGSArray<TTimeBookDecision> *decisions)
{
	try2decide(session, decisions);
	try2surroundecide(session, decisions);
	return decisions->Count();
}

// ------------------- TTimeBookSession -----------------------
class TTimeBookSession : public TSession
{
public:
	TTimeBookSession();
	virtual ~TTimeBookSession() { fre2surroundecide(&decisions); }

	char lawKind;
	long startNo;
	CDate lawDate;
	CDate lawFinished;
	int subcount;
	char involvement;
	char ucnType;
	char ucn[SIZE_OF_UCN];
	char lawText[SIZE_OF_ORIGIN_TEXT];
	char lawAge;
	char lawGrant;
	char lawSourceKind;
	long lawSubject;
	long lawQuick;
	long cumulative;	// -- 21.09.2012 --

	int outcomes;
	TGSArray<TTimeBookDecision> decisions;
	TGArray<TJudgement> judgements;
	bool newDecision;
	int noDecision;
	int noJudgement;
	bool emitReturned;	// 2016:355 +related

	long defenCount, minorCount;
	int month1, month3, month3m;
	int recidiv_1, recidiv_2, recidiv_3;
	int sesFinalized, sesCanceled, sesStopped, sesLeft;
	bool hasRec1, hasRec2, hasRec3;	// 2014:013 LPR: +related
};

TTimeBookSession::TTimeBookSession()
{
	 Add(
		new TCharAlias("F_LAW_KIND", &lawKind),
		new TDate("F_LAWSUIT_DATE", &lawDate, false),
		new TDate("F_LAW_FINISHED", &lawFinished, false),
		new TLong("F_SUBJECT",&lawSubject),
		new TString("F_LAWSUIT_TEXT", lawText, sizeof lawText),
		new TCharAlias("F_LAW_AGE", &lawAge),
		new TCharAlias("F_LAW_GRANTED", &lawGrant),
		new TCharAlias("F_LAW_SOURCE_KIND", &lawSourceKind),
		new TQuick("F_LAW_QUICK", &lawQuick),
		NULL
	);

	Rename("F_COMPOSITION", "F_SES_COMPOSITION");
}

static bool check_finalize_law(TTimeBookSession *timebook)
{
	return !timebook->final.Empty() && timebook->final == timebook->lawFinished;
}

#define timebook OFFICE_GROUP_CAST(TTimeBookSession)

static void tb_subcount(mstr &m, TGroup *group) { subcount(m, timebook->decisions.Count() + timebook->judgements.Count()); }

static void tb_style(mstr &m, TGroup *group)
{
	int count = timebook->decisions.Count() + timebook->judgements.Count();
	int br = 0;

	count = (count ? count : 1);

	if (timebook->judgements.Count())
		br = timebook->noJudgement + 1;

	if (timebook->decisions.Count())
		br += timebook->noDecision + 1;

	if (count == 1)//20.01.2012
		m.cat("border-top:none;border-left:none;border-bottom:solid windowtext .5pt;border-right"
			"solid windowtext .5pt;mso-border-top-alt");
	else if ((timebook->judgements.Count() <= timebook->noJudgement + 1) && !timebook->newDecision)
		m.cat("border-top:none;border-left:none;border-bottom:solid windowtext .5pt;border-right");
	else if (timebook->noJudgement + timebook->noDecision == 0 && timebook->newDecision)
		m.cat("border:none;mso-border-top-alt");
	else if (count == br)
		m.cat("border-top:none;border-left:none;border-bottom:solid windowtext .5pt;border-right");
	else
		m.cat("border:none;border-right");
}

static void tb_print_user(mstr &m, const char *ucn, int type, char ucnType)
{
	TCitizen citizen;
	char s[SIZE_OF_USER_NAME];
	strcpy(citizen.ucn, ucn);
	citizen.ucnType = ucnType;

	if (citizen.Try())
	{
		if (type == TB_SHORT)
		{
			sprintf(s, "%c. ", *citizen.name);
			sprintf(s  + strlen(s), "%c. ", *citizen.reName);
			sprintf(s  + strlen(s), "%s ", citizen.family);
			if (any(citizen.reFamily)) sprintf(s  + strlen(s), "-%s", citizen.reFamily);
			m.cat(s);
		}
		else if (type == TB_LONG)
		{
			m.printf(" %s", citizen.name);
			if (any(citizen.reName)) m.printf(" %s", citizen.reName);
			m.printf(" %s", citizen.family);
			if (any(citizen.reFamily)) m.printf("-%s", citizen.reFamily);
		}
		else
		{
			m.printf(" %s", citizen.name);
			if (any(citizen.reName)) m.printf(" %c.", *citizen.reName);
			m.printf(" %s", citizen.family);
			if (any(citizen.reFamily)) m.printf("-%s", citizen.reFamily);
		}
	}
	else
		print_name(m, ucn, ucnType);
}

static void tb_date(mstr &m, TGroup *group)
{
	timebook->date.Print(m, "%0D");
#if 20180326
	if (timebook->end > timebook->date)
#else  // 20180326
#if COURT_TYPE == COURT_APPEAL
	if (strchr(KIND_CALL_PUNISHMENT_LAWS, timebook->lawKind) && timebook->end > timebook->date)
#endif  // APPEAL
#endif  // 20180326
		m.printf("- %0D", &timebook->end);
}

static void tb_law_kind(mstr &m, TGroup *group) { m.cat(KindAlias->Seek(timebook->lawKind)); }
static void tb_begin_no(mstr &m, TGroup *group) { m.printf("%d",timebook->startNo); }
static void tb_law_year(mstr &m, TGroup *group) { m.printf("%d", timebook->key.year); }
static void tb_law_data(mstr &m, TGroup *group) { m.printf("%0D", &timebook->lawDate); }
static void tb_finished(mstr &m, TGroup *group) { timebook->lawFinished.Print(m, " Дата на свършване: %0D"); }

static void tb_print_sui_def(mstr &m, TGroup *group, char category, bool print_involvement = false)
{
	TGArray<TSideWind> sideWinds;
	TSession session;
	session.key = timebook->key;
	session.kind = timebook->kind;
	session.date = timebook->date;
	load_sidewinds(&session, &sideWinds);

	for (int i = 0; i < sideWinds.Count(); i++)
	{
		if (timebook->Category(sideWinds[i]->involvement) == category)
		{
			m.sep(",\v");
			if (print_involvement)
			{
				m.cat(Involvement->Seek(sideWinds[i]->involvement));
				m.cat("\v");
			}
			tb_print_user(m, sideWinds[i]->ucn, TB_LONG, sideWinds[i]->ucnType);
		}
	}
}

static void tb_suitors(mstr &m, TGroup *group)  { tb_print_sui_def(m, group, SIDEWIND_LEFT); }
static void tb_defendants(mstr &m, TGroup *group) { tb_print_sui_def(m, group, SIDEWIND_RIGHT); }
static void tb_composition(mstr &m, TGroup *group) { print_composition(m, timebook->composition, "с-в\v"); }

static void tb_judge_name(mstr &m, TGroup *group, const char *ucn)
{
	if (atob(ucn))
	{
		TUser user;
		strcpy(user.ucn, ucn);
		if (user.Try() && user.post == POST_APPRENTICE) m.cat("мс ");
		tb_print_user(m, ucn, TB_SHORT, UCN_CITIZEN_UCN);
		if (!strcmp(timebook->judge, ucn)) m.cat(" -докл.\v");
	}
}

static void tb_president(mstr &m, TGroup *group) { tb_judge_name(m, group, timebook->president); }
static void tb_judge_x(mstr &m, TGroup *group)  { tb_judge_name(m, group, timebook->judgeX); }
static void tb_rejudge_x(mstr &m, TGroup *group) { tb_judge_name(m, group, timebook->reJudgeX); }

static void tb_print_name(mstr &m, const char *ucn, bool juror)
{
	if (atob(ucn))
	{
		if (juror) m.cat("з ");
		tb_print_user(m, ucn, TB_SHORT, UCN_CITIZEN_UCN);
		m.cat("\v");
	}
}

static void tb_juror(mstr &m, TGroup *group) { tb_print_name(m, timebook->juror, true); }
static void tb_rejuror(mstr &m, TGroup *group) { tb_print_name(m, timebook->reJuror, true); }
static void tb_juror_x(mstr &m, TGroup *group) { tb_print_name(m, timebook->jurorX, true); }
static void tb_reserve(mstr &m, TGroup *group) { tb_print_name(m, timebook->reserve, true); }
static void tb_judge(mstr &m, TGroup *group) { tb_print_name(m, timebook->judge, false);}
static void tb_prosecutor(mstr &m, TGroup *group) { tb_print_name(m, timebook->prosecutor, false); }
static void tb_secretar(mstr &m, TGroup *group) {  tb_print_name(m, timebook->secretar, false); }

static void tb_returned(mstr &m, TGroup *group)
{
#if 20160341
	// 2016:341 IRQ/TRQ: always session returned date
	// 2016:355 LRQ/URQ: try to avoid multiply dates
	if (timebook->emitReturned && !timebook->returned.Empty())
		m.printf("%0D", &timebook->returned);
#else  // 20160341
	if (timebook->decisions.Count())
	{
		if (timebook->newDecision)
		{
			if (!timebook->decisions[timebook->noDecision]->returned.Empty())
				m.printf("%0D", &timebook->decisions[timebook->noDecision]->returned);
		}
	}
	else if (!timebook->returned.Empty())
		timebook->returned.Print(m);
#endif  // 20160341
}

static void tb_judged(mstr &m, TGroup *group)
{
	if (timebook->outcomes !=  SESSION_STOPPED)
	{
		if (!timebook->newDecision && timebook->judgements.Count())
		{
		#if COURT_TYPE == COURT_REGIONAL
			if (timebook->lawSourceKind == KIND_COMPL_ELEC_CARD)
			{
				if (timebook->Category(timebook->judgements[timebook->noJudgement]->involvement) == SIDEWIND_LEFT)
				{
					tb_print_user(m, timebook->judgements[timebook->noJudgement]->ucn, TB_LONG,
					timebook->judgements[timebook->noJudgement]->ucnType);
				}
			}
			else
		#endif
			tb_print_user(m, timebook->judgements[timebook->noJudgement]->ucn, TB_LONG,
				timebook->judgements[timebook->noJudgement]->ucnType);
		}
		else if (!timebook->judgements.Count())
		{
		#if COURT_TYPE == COURT_REGIONAL
			if (timebook->lawSourceKind == KIND_COMPL_ELEC_CARD)
				tb_print_sui_def(m, group, SIDEWIND_LEFT);
			else
			{
		#endif
				tb_print_sui_def(m, group, SIDEWIND_RIGHT);
				if (!any(m)) tb_print_sui_def(m, group, SIDEWIND_LEFT);
		#if COURT_TYPE == COURT_REGIONAL
			}
		#endif
		}
	}
}

static bool read_next_session(TSession *source, TSession *target, const char *fields, const char *kinds, const char *results)
{
	msql m(source->Name, fields);

	m.Add(source, "F_NO, F_YEAR, F_TYPE");
	m.AddChars("F_KIND", kinds);
	m.Add("F_DATE");
	m.printf("> %ld", source->date.Value());

	if (results)
		m.AddChars("F_RESULT", results);

	m.AddOrder("F_DATE");

	TQuery q(str(m));
	if (q.Read())
	{
		*target << q;
		return true;
	}

	return false;
}

static void tb_show_next(mstr &m, TSession *session)
{
	m.sep("\v");
	m.printf("%s от %0D.", KindAlias->Seek(session->kind), &session->date);

	if (strchr(RESULT_UNDOS, session->result))
	{
		m.sep("\v");
		m.cat(session->text);
	}
	else
	{
		TGSArray<TTimeBookDecision> decisions;

		if (TTimeBookDecision::LoadDecisions(session, &decisions))
		{
			for (int i = 0; i < decisions.Count(); i++)
			{
				TDecision *decision = decisions[i];

				m.sep("\v");
				m.cat(Kind->Seek(decision->kind));
				if (PRINT_JUDGEMENTS && decision->eventNo)
					m.printf("\v№ %d", decision->eventNo);
				m.printf(" от %0D", &decision->fromDate);
				if (decision->surrouendoc)
					m.cat(" по възражение");
				m.cat("\v");
				m.cat(decision->text);
			}

			fre2surroundecide(&decisions);
		}
	}
}

static void tb_assessment(mstr &m, TGroup *group, int mark)
{
	if ((timebook->outcomes != SESSION_LEFT && timebook->outcomes != SESSION_STOPPED &&
		(timebook->outcomes == mark || (mark == SESSION_DECIDE && timebook->outcomes == SESSION_OTHER)))
		  || mark == SESSION_DECIDE && timebook->result == RESULT_LEFT_BULWARK)
	{
		if (timebook->decisions.Count() && timebook->newDecision)
		{
			TTimeBookDecision *decision = timebook->decisions[timebook->noDecision];

			if (timebook->noDecision == 0)
				m.cat(timebook->text);

			m.sep("\v");
			m.cat(Kind->Seek(decision->kind));

			if (PRINT_JUDGEMENTS && decision->eventNo)
				m.printf("\v№ %d", decision->eventNo);
			m.printf(" от %0D", &decision->fromDate);
// ---- 09.07.2012
			static const TFlagName DecisionFlags[] =
			{
				{ DECISION_CONFESS,	"При признание на иска" },
				{ DECISION_ABSENCE,	"Неприсъствено" },
				{ 0,				NULL }
			};
			print_flags(m, DecisionFlags, decision->flags ^ DECISION_PUBLIC, ", ", "\v");
// ---- end ----
			if (!strchr(RESULT_WITH_READY_ANYS, decision->result))
			{
				m.sep("\v");
				m.cat(Result->Seek(decision->result));
			}
			m.sep("\v");
			m.cat(decision->text);
		}
		else if (timebook->judgements.Count() && !timebook->newDecision)
		{
			m.cat(timebook->judgements[timebook->noJudgement]->text);
		}
		else if (!timebook->decisions.Count()) // ako nqma vspisan sydeben akt
		{
			if (timebook->result != RESULT_FIXED)
				m.cpy(Result->Seek(timebook->result));

			m.sep("\v");
			m.cat(timebook->text);

		#if COURT_TYPE == COURT_AREAL
			if (strchr(RESULT_IGNORE_NEXTS, timebook->result))
				return;
		#endif  // AREAL
			// ----- 22.02.2012

			TSession session;
			char results[SIZE_OF_RESULTS];

			strcpy(results, RESULT_FINALIZEDS);
			strcat(results, RESULT_UNDOS);
			// 2014:029 FIX: RESULT_FINALIZEDS -> results
			if (read_next_session(timebook, &session, NULL, KIND_CLOSED_SESSIONS, results))
				tb_show_next(m, &session);
		}
	}
}

static void tb_decided(mstr &m, TGroup *group) { tb_assessment(m, group, SESSION_DECIDE); }
static void tb_canceled(mstr &m, TGroup *group) { tb_assessment(m, group, SESSION_CANCEL); }

static void tb_left(mstr &m, TGroup *group)
{
	if (timebook->outcomes == SESSION_LEFT || timebook->outcomes == SESSION_STOPPED)
	{
		m.cat(Result->Seek(timebook->result));
		m.cat("\v");
		m.cat(timebook->text);

		TSession session;

		// 2013:091 LRQ: was RESULT_FIXEDS
		if (read_next_session(timebook, &session, "F_DATE, F_TIME", KIND_OPEN_SESSIONS, NULL))
		{
			m.printf("\vНаср. за %0D", &session.date);
			if (!session.time.Empty())
				m.printf(" от %T", &session.time);
		}
	}
}

static void tb_other_session(mstr &m, TGroup *group) { if (timebook->kind == KIND_OTHER_OPEN_SESSION) m.cpy("#"); }
static void tb_quick_short(mstr &m, TGroup *group) { print_quick_short(m, timebook->lawQuick); }
static void tb_subject_code(mstr &m, TGroup *group) { m.cat(TSubject::Code(timebook->lawSubject, timebook->lawKind)); }
static void tb_subject_name(mstr &m, TGroup *group) { m.cpy(Subject->Seek(timebook->key.type, timebook->lawSubject)); }

static void tb_sides(mstr &m, TGroup *group)
{
	mstr l, r;
	tb_print_sui_def(l, group, SIDEWIND_LEFT);
	tb_print_sui_def(r, group, SIDEWIND_RIGHT);

	if (any(l) && any(r))
	{
		m.cat(l);
		m.cat("\vсрещу\v");
		m.cat(r);
	}
	else if (any(l) && !any(r)) { tb_print_sui_def(m, group, SIDEWIND_LEFT, true); }
	else { tb_print_sui_def(m, group, SIDEWIND_RIGHT, true); }
}

static void print_cumulative(mstr &m, TGroup *group, int value)
{
	if (timebook->cumulative)
		itom(value, m);
	else
		m.cpy("*");
}

static void tb_age_1m(mstr &m, TGroup *group)
{
	if (check_finalize_law(timebook) && timebook->lawAge == AGE_ONE_OR_LESS)
		print_cumulative(m, group, timebook->month1);
}

static void tb_age_3m(mstr &m, TGroup *group)
{
	if ((timebook->lawAge == AGE_ONE_TO_THREE || timebook->lawAge == AGE_THREE_OR_LESS)
		&& check_finalize_law(timebook))
	{
		print_cumulative(m, group, timebook->month3);
	}
}

static void tb_age_up3m(mstr &m, TGroup *group)
{
	if (strchr(AGES_MORE_THAN_THREE, timebook->lawAge))
		print_cumulative(m, group, timebook->month3m);
}

#if COURT_TYPE ==  COURT_REGIONAL
static void check_law(mstr &m, TGroup *group, char kind)
{
	if (timebook->lawKind == kind)
	{
		m.printf("%d/%d\v%0D",timebook->key.no, timebook->key.year, &timebook->lawDate);
		timebook->lawFinished.Print(m, " Дата на свършване: %0D");
	}
}

static void tb_law1(mstr &m, TGroup *group) { check_law(m, group, KIND_COMMON_PUNISHMENT_LAW); }
static void tb_law2(mstr &m, TGroup *group) { check_law(m, group, KIND_PUNISHMENT_PRIVATE_CHAR_LAW); }
static void tb_law3(mstr &m, TGroup *group) { check_law(m, group, KIND_ADMIN_PUNISHMENT_LAW); }
static void tb_law4(mstr &m, TGroup *group) { check_law(m, group, KIND_PRIVATE_PUNISHMENT_LAW); }
#endif

static void tb_defen_count(mstr &m, TGroup *group)
{
	if (check_finalize_law(timebook) && timebook->judgements.Count())
		itom(timebook->defenCount, m);
}

static void tb_minor_count(mstr &m, TGroup *group)
{
	if (check_finalize_law(timebook) && timebook->judgements.Count())
		itom(timebook->minorCount, m);
}

static void tb_recidiv1(mstr &m, TGroup *group)
{
	if (check_finalize_law(timebook) && timebook->hasRec1)
		print_cumulative(m, group, timebook->recidiv_1);
}

static void tb_recidiv2(mstr &m, TGroup *group)
{
	if (check_finalize_law(timebook) && timebook->hasRec2)
		print_cumulative(m, group, timebook->recidiv_2);
}

static void tb_recidiv3(mstr &m, TGroup *group)
{
	if (check_finalize_law(timebook) && timebook->hasRec3)
		print_cumulative(m, group, timebook->recidiv_3);
}

static void tb_finalized_count(mstr &m, TGroup *group)
{
	if (timebook->outcomes == SESSION_DECIDE && timebook->decisions.Count())
		print_cumulative(m, group, timebook->sesFinalized);
}

static void tb_canceled_count(mstr &m, TGroup *group)
{
	if (timebook->outcomes == SESSION_CANCEL && timebook->decisions.Count())
		print_cumulative(m, group, timebook->sesCanceled);
}

static void tb_stoped_count(mstr &m, TGroup *group)
{
	if (timebook->outcomes == SESSION_STOPPED)
		print_cumulative(m, group, timebook->sesStopped);
}

static void tb_left_count(mstr &m, TGroup *group)
{
	if (timebook->outcomes == SESSION_LEFT)
		print_cumulative(m, group, timebook->sesLeft);
}

static void tb_decision_kind(mstr &m, TGroup *group)
{
	if (timebook->decisions.Count() && timebook->newDecision)
		m.cat(Kind->Seek(timebook->decisions[timebook->noDecision]->kind));
}

static void tb_decision_no(mstr &m, TGroup *group)
{
	if (PRINT_JUDGEMENTS && timebook->decisions.Count() && timebook->newDecision)
		itom(timebook->decisions[timebook->noDecision]->eventNo, m);
}

static void tb_decision_date(mstr &m, TGroup *group)
{
	if (timebook->decisions.Count() && timebook->newDecision)
		m.printf("от %0D", &timebook->decisions[timebook->noDecision]->fromDate);
}

static void tb_session_result(mstr &m, TGroup *group)
{
	m.cat(Result->Seek(timebook->result));
}

#undef timebook

static const offield timebook_fields[]=
{
	{ "F_SUBCOUNT", tb_subcount },
	{ "F_STYLE", tb_style },
	{ "F_DATE", tb_date },
	{ "F_SESSION_DATE", tb_date },
	{ "F_NO", tb_begin_no },
	{ "F_LAWSUIT_KIND", tb_law_kind },
	{ "F_LAWSUIT_NO", keygroup_flag_no },
	{ "F_LAWSUIT_YEAR", tb_law_year},
	{ "F_LAWSUIT_DATE", tb_law_data },
	{ "F_FINISHED", tb_finished },
	{ "F_SUITORS", tb_suitors },
	{ "F_DEFENDANTS", tb_defendants },
	{ "F_COMPOSITION", tb_composition },
	{ "F_PRESIDENT", tb_president },
	{ "F_JUDGE_X", tb_judge_x },
	{ "F_REJUDGE_X", tb_rejudge_x },
	{ "F_JUROR", tb_juror },
	{ "F_REJUROR", tb_rejuror },
	{ "F_JUROR_X", tb_juror_x },
	{ "F_RESERVE", tb_reserve },
	{ "F_JUDGE", tb_judge },
	{ "F_PROSECUTOR", tb_prosecutor },
	{ "F_SECRETAR", tb_secretar },
	{ "F_RETURNED_DATE", tb_returned },
	{ "F_JUDGED", tb_judged }, 
	{ "F_DECIDED", tb_decided },
	{ "F_CANCELED", tb_canceled },
	{ "F_LEFT", tb_left },
	{ "F_OTHER_SESSION", tb_other_session },
	{ "F_QUICK_SHORT", tb_quick_short },
	{ "F_SUBJECT", tb_subject_code },
	{ "F_SUBJECT_NAME", tb_subject_name },
	{ "F_LAWSUIT_TEXT", NULL },
	{ "F_SIDES", tb_sides },
	{ "F_AGE_1M", tb_age_1m },		// -- 21.09.2012 --
	{ "F_AGE_3M", tb_age_3m },		// -- 21.09.2012 --
	{ "F_AGE_UP3M", tb_age_up3m },	// -- 21.09.2012 --
#if COURT_TYPE ==  COURT_REGIONAL
	{ "F_LAWSUIT_1", tb_law1 },
	{ "F_LAWSUIT_2", tb_law2 },
	{ "F_LAWSUIT_3", tb_law3 },
	{ "F_LAWSUIT_4", tb_law4 },
#endif
	{ "F_DEFENDANT_COUNT", tb_defen_count },
	{ "F_MINOR_COUNT", tb_minor_count },
	{ "F_RECIDIV_1", tb_recidiv1 },			// -- 21.09.2012 --
	{ "F_RECIDIV_2", tb_recidiv2 },			// -- 21.09.2012 --
	{ "F_RECIDIV_3", tb_recidiv3 },			// -- 21.09.2012 --
	{ "F_FINALIZED_COUNT", tb_finalized_count },	// -- 21.09.2012 --
	{ "F_CANCELED_COUNT", tb_canceled_count },	// -- 21.09.2012 --
	{ "F_STOPED_COUNT", tb_stoped_count },		// -- 21.09.2012 --
	{ "F_LEFT_COUNT", tb_left_count },			// -- 21.09.2012 --
	{ "F_DECISION_KIND", tb_decision_kind },
	{ "F_DECISION_NO", tb_decision_no },
	{ "F_DECISION_DATE", tb_decision_date },
	{ "F_SESSION_RESULT", tb_session_result },
	{NULL, NULL}
};

// ----- TRJudge -------------------------------------------------------------
struct TRJudge : public TDummyGroup
{
	TRJudge() { memset(conJudge, '\0', sizeof conJudge); }

	char judge[SIZE_OF_UCN];
	int conJudge[COL_JUDGE];
};

// ----- TTimeBookRecap ------------------------------------------------------
class TTimeBookRecap : public TDummyGroup
{
public:
	TTimeBookRecap() { memset(container, '\0', sizeof container); }

	void Process(TGroup *group);
	void Increment(TTimeBookSession *timebook, int index);

	long instance;
	char type;
	int container[ROW_MAX][COL_MAX];
	TGArray<TRJudge> judges;
	TIntegerArray sortJudges;
	int conAllJudge[COL_JUDGE];
	int no;
};

void TTimeBookRecap::Increment(TTimeBookSession *timebook, int index)
{
	container[index][0]++;

	if (timebook->lawQuick & QUICK_QUICK)
		container[index][1]++;

}

void TTimeBookRecap::Process(TGroup *group)
{
	TTimeBookSession *timebook = (TTimeBookSession *) group;
	bool finalize = check_finalize_law(timebook);
	int i = 0;
	int tbOutComs = session_outcomes(timebook);

	while (i < judges.Count() && strcmp(judges[i]->judge, timebook->judge))
		i++;

	if (i == judges.Count())
	{
		int index = 0;

		while (index < sortJudges.Count())
		{
			if (strcmp(judges[sortJudges[index]]->judge, timebook->judge))
			{
				TCitizen judge1, judge2;
				strcpy(judge1.ucn, judges[sortJudges[index]]->judge);
				strcpy(judge2.ucn, timebook->judge);
				judge1.ucnType = judge2.ucnType = UCN_CITIZEN_UCN;

				if (judge1.Try() && judge2.Try())
				{
					if (!strcmpi(judge1.name, judge2.name))
					{
						if (!strcmpi(judge1.reName, judge2.reName))
						{
							if (!strcmpi(judge1.family, judge2.family))
							{
								if (strcmpi(judge1.reFamily, judge2.reFamily) > 0)
									break;
							}
							else if (strcmpi(judge1.family, judge2.family) > 0)
								break;
						}
						else if (strcmpi(judge1.reName, judge2.reName) > 0)
							break;
					}
					else if (strcmpi(judge1.name, judge2.name) > 0)
						break;
				}
			}

			index++;
		}

		if (index == judges.Count())
			sortJudges.Add(i);
		else
			sortJudges.Insert(i, index);

		TRJudge *tbJuge = new TRJudge();
		strcpy(tbJuge->judge, timebook->judge);
		judges.Add(tbJuge);
	}

	judges[i]->conJudge[0]++;
	Increment(timebook, 0);

	if (instance == BOOK_1ST_INST && type != TYPE_PUNISHMENT_LAW)
	{
	#if COURT_TYPE == COURT_REGIONAL
		if (strchr(KIND_TEMP_MEASURES_SESSIONS, timebook->kind))
			Increment(timebook, 1);
	#endif

	#if COURT_TYPE == COURT_AREAL
		if (timebook->kind == KIND_CREDITORS_MEETING)
			Increment(timebook, 2);
	#endif

		bool any_left = true;

		if (timebook->kind == KIND_OTHER_OPEN_SESSION)
		{
			judges[i]->conJudge[1]++;
			Increment(timebook, 3);
		}
		switch (tbOutComs)
		{
			case SESSION_LEFT :
			{
				judges[i]->conJudge[2]++;
				Increment(timebook, 4);

			#if COURT_TYPE == COURT_REGIONAL || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_ADMIN || COURT_TYPE == COURT_MILITARY
				if (timebook->result == RESULT_LEFT_1ST && timebook->kind == KIND_1ST_SESSION)	// L - 1во заседание
				{
					judges[i]->conJudge[3]++;
					Increment(timebook, 6);
					any_left = false;
				}
				if (timebook->kind == KIND_2ND_SESSION && strchr(RESULT_HOLDS, timebook->result))	// I - 2во заседание
				{
					Increment(timebook, 7);
					any_left = false;
				}
			#endif

			#if COURT_TYPE == COURT_REGIONAL
				if (timebook->kind == KIND_PEACE_SESSION && strchr(RESULT_HOLDS, timebook->result))	// 4 - Помирително заседание
				{
					Increment(timebook, 5);
					any_left = false;
				}
				else if (strchr(KIND_TEMP_MEASURES_SESSIONS, timebook->kind) && strchr(RESULT_HOLDS, timebook->result))	// привремени мерки
				{
					Increment(timebook, 8);
					any_left = false;
				}
			#endif
				if (any_left && strchr(RESULT_HOLDS, timebook->result))
					Increment(timebook, 9);

				if (strchr(RESULT_HOLDS, timebook->result))
					Increment(timebook, 10);

				break;
			}
			case SESSION_STOPPED :
			{
				judges[i]->conJudge[4]++;
				Increment(timebook, 11);
				break;
			}
			case SESSION_DECIDE:
			{
			//	TTimeBookDecision::LoadDecisions(timebook, &timebook->decisions);
				if (!finalize && !timebook->decisions.Count())		// Oбявено за решение без вписан съдебен акт
				{
					Increment(timebook, 12);
				}
				else if (finalize)								// Решено, приключващо
				{
					judges[i]->conJudge[5]++;

					if (timebook->decisions.Count())					// с вписан съдебен акт
					{
						judges[i]->conJudge[6]++;
						Increment(timebook, 13);
					}

				#if COURT_TYPE == COURT_REGIONAL || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_ADMIN || COURT_TYPE == COURT_MILITARY
					if (timebook->lawGrant == GRANT_ENTIRE)
						Increment(timebook, 14);
					else if (timebook->lawGrant == GRANT_PARTIAL)
						Increment(timebook, 15);
					else if (timebook->lawGrant == GRANT_REJECTED)
						Increment(timebook, 16);
				#endif

					if (!strchr(AGES_MORE_THAN_THREE, timebook->lawAge))		// Решени, до 3 месеца
					{
						judges[i]->conJudge[9]++;
						Increment(timebook, 17);
					}
					else										// над 3 месеца
					{
						judges[i]->conJudge[10]++;
						Increment(timebook, 18);
					}
				}

				break;
			}
			case SESSION_CANCEL:
			{
				if (finalize)
				{
					judges[i]->conJudge[5]++;

					if (timebook->decisions.Count())	// с вписан съдебен акт
					{
						judges[i]->conJudge[7]++;
						Increment(timebook, 19);
					}

				#if COURT_TYPE == COURT_REGIONAL || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_ADMIN || COURT_TYPE == COURT_MILITARY
					if (timebook->result == RESULT_CANCEL_BY_AGREEMENT)		// по спогодба
					{
						judges[i]->conJudge[8]++;
						Increment(timebook, 20);
					}
					else if (timebook->result == RESULT_SEND)				//  изпратени по подсъдност
						Increment(timebook, 21);
				#endif

					// прекратено или прекратено по други причини
					if (timebook->result == RESULT_CANCELED || timebook->result == RESULT_CANCEL_EXACT_OTH_CAUSES)
						Increment(timebook, 22);

					if (!strchr(AGES_MORE_THAN_THREE, timebook->lawAge))		// свършено, до 3 месеца
					{
						judges[i]->conJudge[9]++;
						Increment(timebook, 23);
					}
					else
					{
						judges[i]->conJudge[10]++;
						Increment(timebook, 24);
					}
				}

				break;
			}
		}

	#if COURT_TYPE == COURT_REGIONAL
		if (timebook->result == RESULT_DECISION_ALLOW_SPLIT)				// С решение допускане на делба
			Increment(timebook, 25);

		if (timebook->result == RESULT_SESS_DEC_TEMP_MEASURES)			// С определение по привремени мерки
			Increment(timebook, 26);
	#endif
	}
	else if (instance == BOOK_1ST_INST && type == TYPE_PUNISHMENT_LAW)
	{
	#if COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
		if (timebook->kind == KIND_PRE_HEARING)
			Increment(timebook, 1);
	#endif  // AREAL || MILITARY || REGIONAL
		if (timebook->kind == KIND_OTHER_OPEN_SESSION)
		{
			judges[i]->conJudge[1]++;
			Increment(timebook, 2);
		}

		switch (tbOutComs)
		{
			case SESSION_LEFT:
			{
				judges[i]->conJudge[2]++;
				Increment(timebook, 3);
				break;
			}
			case SESSION_STOPPED:
			{
				judges[i]->conJudge[3]++;
				Increment(timebook, 4);
				break;
			}
			case SESSION_DECIDE:
			{
			//	TTimeBookDecision::LoadDecisions(timebook, &timebook->decisions);
				if (!finalize && timebook->decisions.Count() == 0)	// Oбявено за решение без вписан съдебен акт
				{
					Increment(timebook, 5);
				}
				else if (finalize) 						//  приключващо
				{
					judges[i]->conJudge[4]++;

					if (timebook->decisions.Count())			// с вписан съдебен акт
					{
						judges[i]->conJudge[5]++;
						Increment(timebook, 6);

						switch (timebook->result)
						{
							case RESULT_PUBLIC_DECISION :		// обявено за решаване
							{
								Increment(timebook, 7);
								break;
							}
							case RESULT_WITH_DECISION :		// С решение
							{
								Increment(timebook, 8);
								break;
							}
							case 	RESULT_WITH_JUDGEMENT :		// С присъда
							{
								Increment(timebook, 9);
								break;
							}
							case RESULT_SUB_SETTLEMENT :		// С определение
							case RESULT_NON_JOIN_SETTL :
							{
								Increment(timebook, 10);
								break;
							}
							case RESULT_WITH_ORDER :		// С разпореждане
							case RESULT_NON_JOIN_ORDER :
							{
								Increment(timebook, 11);
								break;
							}
						}
					}

					if (!strchr(AGES_MORE_THAN_THREE, timebook->lawAge))		// Решени, до 3 месеца
					{
						judges[i]->conJudge[7]++;
						Increment(timebook, 12);
					}
					else
					{
						judges[i]->conJudge[8]++;
						Increment(timebook, 13);
					}
				}

				break;
			}
			case SESSION_CANCEL :
			{
				if (finalize)
				{
					judges[i]->conJudge[4]++;

					if (timebook->decisions.Count())
					{
						judges[i]->conJudge[6]++;
						Increment(timebook, 14);

					#if COURT_TYPE == COURT_REGIONAL || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_ADMIN || COURT_TYPE == COURT_MILITARY
						switch (timebook->result)
						{
							// Върнато за доразглеждане В РС, прекратено за доразследване по чл.249 в ОС/ВС/СНС
/* така ли да остане*/				case RESULT_CANCEL_FOR_MORE_LOOK_INTO :
							{
								Increment(timebook, 15);
								break;
							}
							case RESULT_WITH_BULWARK :	// Със споразумение
							{
								Increment(timebook, 16);
								break;
							}
							case RESULT_SEND :		// Изпратено по подсъдност
							{
								Increment(timebook, 17);
								break;
							}
						}
					#endif

						// прекратено или прекратено по други причини
						if (timebook->result == RESULT_CANCELED || timebook->result == RESULT_CANCEL_EXACT_OTH_CAUSES)
							Increment(timebook, 18);
					}

					if (!strchr(AGES_MORE_THAN_THREE, timebook->lawAge))		// прекратени, до 3 месеца
					{
						judges[i]->conJudge[7]++;
						Increment(timebook, 19);
					}
					else
					{
						judges[i]->conJudge[8]++;
						Increment(timebook, 20);
					}
				}

				break;
			}
		}
	}
	else if (instance != BOOK_1ST_INST)
	{
		if (timebook->kind == KIND_OTHER_OPEN_SESSION)
		{
			judges[i]->conJudge[1]++;
			Increment(timebook, 1);
		}
		switch (tbOutComs)
		{
			case SESSION_LEFT :
			{
				judges[i]->conJudge[2]++;
				Increment(timebook, 2);			// Отложени общо
				break;
			}
			case SESSION_STOPPED :
			{
				judges[i]->conJudge[3]++;
				Increment(timebook, 3);
				break;
			}
			case SESSION_DECIDE :
			{
			//	TTimeBookDecision::LoadDecisions(timebook, &timebook->decisions);
				// Oбявено за решение без вписън съдебен акт
				if (!finalize && !timebook->decisions.Count())
				{
					Increment(timebook, 4);
				}
				else if (finalize) 								//  приключващо
				{
					judges[i]->conJudge[4]++;

					if (timebook->decisions.Count())					// с вписан съдебен акт
					{
						judges[i]->conJudge[5]++;
						Increment(timebook, 5);
					}

					if (!strchr(AGES_MORE_THAN_THREE, timebook->lawAge))		// Решени, до 3 месеца
					{
						judges[i]->conJudge[7]++;
						Increment(timebook, 6);
					}
					else
					{
						judges[i]->conJudge[8]++;
						Increment(timebook, 7);
					}
				}

				break;
			}
			case SESSION_CANCEL :
			{
				if (finalize)	//  приключващо
				{
					judges[i]->conJudge[4]++;

					if (timebook->decisions.Count())
					{
						judges[i]->conJudge[6]++;
						Increment(timebook, 8);
					}

					if (!strchr(AGES_MORE_THAN_THREE, timebook->lawAge))		// прекратени, до 3 месеца
					{
						judges[i]->conJudge[7]++;
						Increment(timebook, 9);
					}
					else
					{
						judges[i]->conJudge[8]++;
						Increment(timebook, 10);
					}
				}

				break;
			}
		}
	}
}

#define recap OFFICE_GROUP_CAST(TTimeBookRecap)

static void recap_count(mstr &m, TGroup *group)
{
	int row;
	static bool colN = true;
	bool *col1 = &colN;

	if (sscanf(recap->Name, "F_COUNT_%d", &row) == 1 && row < ROW_MAX && colN)	// F_COUNT
	{
		*col1 = false;
		itom(recap->container[row-1][0], m);
	}
	else if (!colN)	// F_COUNT _QUICK
	{
		*col1 = true;
		itom(recap->container[row-1][1], m);
	}
}

static void recap_judge(mstr &m, TGroup *group)
{
	if (recap->no == recap->sortJudges.Count())
		m.cat("Общо");
	else
		tb_print_user(m, recap->judges[recap->no]->judge, TB_SPECIAL, UCN_CITIZEN_UCN);
}

static void recap_judge_col(mstr &m, TGroup *group)
{
	int colN;

	if (sscanf(recap->Name, "F_COL_%d", &colN) == 1)	// F_COUNT
	{
		if (recap->no == recap->sortJudges.Count())
			itom(recap->conAllJudge[colN - 1], m);
		else
			itom(recap->judges[recap->no]->conJudge[colN-1], m);
	}
}
#undef recap

static const offield recap_filds[]=
{
	{ "*F_COUNT_", recap_count },
	{ "F_JUDGE", recap_judge },
	{ "*F_COL_", recap_judge_col },
	{ NULL, NULL }
};

// ----- TBookRecap ----------------------------------------------------------
class TBookRecap : public TBook
{
public:
	TBookRecap(TWindow *parent, TBookGroup *group, TTimeBookRecap *tRecaps, bool tRows, const char *fileName);

protected:
	virtual TGroup *Process(TQuery &q) { return NULL; }
	virtual void Collect() { }
	virtual void EmitBody();

	TTimeBookRecap *recaps;
	bool rows;
};

TBookRecap::TBookRecap(TWindow *parent, TBookGroup *group, TTimeBookRecap *tRecaps, bool tRows, const char *fileName)
	: TBook(parent, group, fileName, recap_filds, title_fields), recaps(tRecaps), rows(tRows)
{
}

void TBookRecap::EmitBody()
{
	office.Emit(NULL, tr1start, book_fields, recaps);
	memset(recaps->conAllJudge, '\0', sizeof recaps->conAllJudge);

	for (int index = 0; index <= recaps->sortJudges.Count(); index++)
	{
		if (index < recaps->sortJudges.Count())
		{
			for (int i = 0; i < COL_JUDGE; i++) // All Records
				recaps->conAllJudge[i] += recaps->judges[index]->conJudge[i];

			recaps->no = recaps->sortJudges[index];
		}
		else
			recaps->no = index;

		office.Emit(tr1start, tr1final, body_fields, recaps);
		wait->TickCount(index, "отпечатани записа");

		if (!rows)
			break;
	}

	office.Emit(tr1final, NULL, book_fields, recaps);
}

// ----- TTimeBook -----------------------------------------------------------
class TTimeBook : public TBook
{
public:
	TTimeBook(TWindow *parent, TTimeBookGroup *tGroup, const char *fileName);
	virtual TGroup *Process(TQuery &q);
	virtual void Markers();
	virtual void EmitRow(TGroup *group);
	TTimeBookRecap recap;

private:
	TTimeBookGroup *timeBookGroup;
	int day;
	int tbOutComs;
	CDate min_date;
	long count;
	long beginNo;
	long cumulative;
	bool flag_nom;
	const char *tr2start;
	const char *tr2final;
	const char *sub1_start;
	const char *sub1_final;
	const char *sub2_start;
	const char *sub2_final;
	const char *sub_head_st;
	const char *sub_head_fin;
	int dCount, mCount, month1, month3, month3m, rec_1, rec_2, rec_3;
	int sessFinalized, sessCanceled, sessStopped, sessLeft;
	long book, bookMno, summary, judgeSummary, resultSet;
};

TTimeBook::TTimeBook(TWindow *parent, TTimeBookGroup *tGroup, const char *fileName)
	: TBook(parent, tGroup, fileName, timebook_fields, title_fields, BOOK_FIXED_DATES), timeBookGroup(tGroup)
{
	count = 0;
	day = 0;
	flag_nom = tGroup->beginNo != 0;
	beginNo = tGroup->beginNo ? tGroup->beginNo : 1;
	min_date = tGroup->minDate;
	dCount = mCount = month1 = month3 = month3m = rec_1 = rec_2 = rec_3 = 0;
	sessFinalized = sessCanceled = sessStopped = sessLeft = 0;

	cumulative = tGroup->cumulative;
	recap.type = tGroup->types[0];
	recap.instance = tGroup->instance;
}

void TTimeBook::Markers()
{
	tr1start = office.Find("tr1 start");
	tr1final = office.Find("tr1 final");
	tr2start = office.Find("tr2 start");
	sub_head_st = office.Find("tr1 head_st");
	sub1_start = office.Find("tr1 sub_start");
	sub1_final = office.Find("tr1 sub_final");
	sub2_start = office.Find("tr2 sub_start");
	sub2_final = office.Find("tr2 sub_final");
	sub_head_fin = office.Find("tr1 head_fin");
	tr2final = office.Find("tr2 final");

	if (!array->Count())
		tr1final = tr2final;
}

TGroup *TTimeBook::Process(TQuery &q)
{
	TTimeBookSession *session = new TTimeBookSession;
	*session << q;

	if (session->date >= min_date)
	{
		TTimeBookDecision::LoadDecisions(session, &session->decisions);

		if (timeBookGroup->summary || timeBookGroup->judgeSummary)
			recap.Process(session);

		if (timeBookGroup->resultSet)
			timeBookGroup->CountResults(session, session->lawFinished);
	}

	return session;
}

void TTimeBook::EmitRow(TGroup *group)
{
	TTimeBookSession *timebook = ((TTimeBookSession *) group);

	tbOutComs = session_outcomes(timebook);	// result from timebook
	timebook->hasRec1 = timebook->hasRec2 = timebook->hasRec3 = false;

	if (tbOutComs == SESSION_CANCEL && timebook->result == RESULT_WITH_BULWARK)
		tbOutComs = SESSION_DECIDE;

	switch (tbOutComs)
	{
		case SESSION_LEFT : sessLeft++; break;
		case SESSION_STOPPED : sessStopped++; break;
		case SESSION_DECIDE :
		{
			if (timebook->decisions.Count())
				sessFinalized++;
			break;
		}
		case SESSION_CANCEL :
		{
			if (timebook->decisions.Count())
				sessCanceled++;
			break;
		}
	}

	int br, i = 0;
	TIntegerArray arrayDec; // of each Decision how many Judgements

	do {
		br = 0;

		if (timebook->decisions.Count())
		{
			TJudgement judge;
			msql m;

			judge << timebook->decisions[i];
			judge.FloatCriteria(m);

			TQuery q(str(m));
			while (q.Read())
			{
				TJudgement *judgement = new TJudgement;
				*judgement << q;

				switch (judgement->recidive)
				{
					case RECIDIVE_GENERAL : rec_1++; timebook->hasRec1 = true; break;
					case RECIDIVE_SPECIAL : rec_2++; timebook->hasRec2 = true; break;
					case RECIDIVE_DANGEROUS : rec_3++; timebook->hasRec3 = true; break;
				}

				if (timebook->Category(judgement->involvement) == SIDEWIND_RIGHT)
				{
					if (check_finalize_law(timebook) && strchr(UCN_CITIZENS, judgement->ucnType) && i == 0)
					{
						dCount++;

						if (judgement->flags & SIDE_WIND_CHILD)
							mCount++;
					}

					timebook->judgements.Add(judgement);
					br++;
				}
				else
					delete judgement;
			}
		}

		arrayDec.Add(br);
		i++;

	} while (i < timebook->decisions.Count());

	if (check_finalize_law(timebook))
	{
		if (timebook->lawAge == AGE_ONE_OR_LESS)
			month1++;
		else if (timebook->lawAge == AGE_ONE_TO_THREE || timebook->lawAge == AGE_THREE_OR_LESS)
			month3++;
	}
	else if (strchr(AGES_MORE_THAN_THREE, timebook->lawAge)) month3m++;

	if (timebook->date >= min_date)
	{
		if (timebook->date.day != day)
		{
			office.Emit(tr1start, tr1final, timebook_fields, group);
			day = timebook->date.day;
		}

		if (!flag_nom)
			timebook->startNo = count + 1;
		else
		{
			timebook->startNo = beginNo;
			beginNo++;
		}

		timebook->cumulative = cumulative;
		timebook->outcomes = tbOutComs;
		timebook->defenCount = dCount;
		timebook->minorCount = mCount;
		timebook->recidiv_1 = rec_1;
		timebook->recidiv_2 = rec_2;
		timebook->recidiv_3 = rec_3;
		timebook->sesFinalized = sessFinalized;
		timebook->sesCanceled = sessCanceled;
		timebook->sesStopped = sessStopped;
		timebook->sesLeft = sessLeft;
		timebook->month1 = month1;
		timebook->month3 = month3;
		timebook->month3m = month3m;

		int j;

		i = 0;
		br = 0;

		do {
			timebook->noDecision = i;

			if (i)	// other head on Decision
			{
				timebook->newDecision = true;
				office.Emit(tr2start, sub_head_st, null_fields, NULL);
				office.Emit(sub1_start, sub1_final, timebook_fields, group);
				timebook->emitReturned = false;
				office.Emit(sub2_start, sub2_final, timebook_fields, group);
				office.Emit(sub_head_fin, tr2final, null_fields, NULL);
			}

			j = 0;

			do {
				timebook->noJudgement = br;

				if (!i && !j)	// first head on Decision
				{
					timebook->newDecision = true;
					office.Emit(tr2start, sub_head_st, null_fields, NULL);
					office.Emit(sub_head_st, sub1_start, timebook_fields, group);
					office.Emit(sub1_start, sub1_final, timebook_fields, group);
					office.Emit(sub1_final, sub2_start, timebook_fields, group);
					timebook->emitReturned = true;
					office.Emit(sub2_start, sub2_final, timebook_fields, group);
					office.Emit(sub2_final, sub_head_fin, timebook_fields, group);
					office.Emit(sub_head_fin, tr2final, null_fields, NULL);
				}

				if (arrayDec[i])	// for Judged
				{
					timebook->newDecision = false;
					office.Emit(tr2start, sub_head_st, null_fields, NULL);
					office.Emit(sub1_start, sub1_final, timebook_fields, group);
					timebook->emitReturned = false;
					office.Emit(sub2_start, sub2_final, timebook_fields, group);
					office.Emit(sub_head_fin, tr2final, null_fields, NULL);
				}

				br++;
				j++;

			} while (j < arrayDec[i]);

			i++;

		} while (i < timebook->decisions.Count());
	}

	count++;

	if (count >= array->Count())
		tr1final = tr2final;
}

// ----- entry ---------------------------------------------------------------
static void time_book(TWindow *parent, TTimeBookGroup *group)
{
	const char *file_name;
	const char *file_recap;
	const char *file_rjudge;

#if COURT_TYPE == COURT_REGIONAL
	switch (group->types[0])
	{
		case TYPE_PUNISHMENT_LAW :
		{
			file_name = "TimeB_f1pr.htm";
			file_recap = "TimeR_f1p_.htm";
			file_rjudge = "TimeRJf2__.htm";
			break;
		}
		case TYPE_MARRIAGE_LAW :
		case TYPE_CITIZEN_LAW :
		case TYPE_ADMIN_LAW :
		{
			file_name = "TimeB_f1_r.htm";
			file_recap = "TimeR_f1__.htm";
			file_rjudge = "TimeRJf1__.htm";
			break;
		}
	}
#endif

#if COURT_TYPE == COURT_AREAL
	switch (group->types[0])
	{
		case TYPE_PUNISHMENT_LAW :
		{
			if (group->instance == BOOK_1ST_INST)
			{
				file_name = "TimeB_f1p_.htm";
				file_recap = "TimeR_f1p_.htm";
			}
			else
			{
				file_name = "TimeB_f2__.htm";
				file_recap = "TimeR_f2__.htm";
			}

			file_rjudge = "TimeRJf2__.htm";
			break;
		}
		case TYPE_CITIZEN_LAW :
		case TYPE_TRADE_LAW :
		{
			if (group->instance == BOOK_1ST_INST)
			{
				file_name = "TimeB_f1__.htm";
				file_recap = "TimeR_f1__.htm";
				file_rjudge = "TimeRJf1__.htm";
			}
			else
			{
				file_name = "TimeB_f2__.htm";
				file_recap = "TimeR_f2__.htm";
				file_rjudge = "TimeRJf2__.htm";
			}

			break;
		}
		case TYPE_ADMIN_LAW :
		{
			if (group->instance == BOOK_1ST_INST)
			{
				file_name = "TimeB_f1a_.htm";
				file_recap = "TimeR_f1__.htm";
				file_rjudge = "TimeRJf1__.htm";
			}
			else
			{
				file_name = "TimeB_f2__.htm";
				file_recap = "TimeR_f2__.htm";
				file_rjudge = "TimeRJf2__.htm";
			}

			break;
		}
		case TYPE_FIRM_LAW :
		{
			file_name = "TimeB_f1d_.htm";
			file_recap = "\0";
			file_rjudge = "\0";
			break;
		}
	}
#endif

#if COURT_TYPE == COURT_APPEAL
	file_name = "TimeB_f2__.htm";
	file_recap = "TimeR_f2__.htm";
	file_rjudge = "TimeRJf2__.htm";
#endif

#if COURT_TYPE == COURT_ADMIN
	switch (group->types[0])
	{
		case TYPE_ADMIN_LAW :
		{
			if (group->instance == BOOK_1ST_INST)
			{
				file_name = "TimeB_f1a_.htm";
				file_recap = "TimeR_f1__.htm";
				file_rjudge = "TimeRJf1__.htm";
			}
			else
			{
				file_name = "TimeB_f2__.htm";
				file_recap = "TimeR_f2__.htm";
				file_rjudge = "TimeRJf2__.htm";
			}

			break;
		}
		case TYPE_CITIZEN_LAW :
		{
			if (group->instance == BOOK_1ST_INST)
			{
				file_name = "TimeB_f1__.htm";
				file_recap = "TimeR_f1__.htm";
				file_rjudge = "TimeRJf1__.htm";
			}
			else
			{
				file_name = "TimeB_f2__.htm";
				file_recap = "TimeR_f2__.htm";
				file_rjudge = "TimeRJf2__.htm";
			}

			break;
		}
		case TYPE_TRICKY_LAW :
		{
			if (group->instance == BOOK_1ST_INST)
			{
				file_name = "TimeB_f1p_.htm";
				file_recap = "TimeR_f1__.htm";
				file_rjudge = "TimeRJf1__.htm";
			}
			else
			{
				file_name = "TimeB_f2__.htm";
				file_recap = "TimeR_f2__.htm";
				file_rjudge = "TimeRJf2__.htm";
			}

			break;
		}
	}
#endif

#if COURT_TYPE == COURT_MILITARY
	switch (group->types[0])
	{
		case TYPE_CITIZEN_LAW:
		{
			if (group->instance == BOOK_1ST_INST)
			{
				file_name = "TimeB_f1__.htm";
				file_recap = "TimeR_f1__.htm";
			}
			else
			{
				file_name = "TimeB_f2__.htm";
				file_recap = "\0";
			}

			file_rjudge = "TimeRJf1__.htm";
			break;
		}
		case TYPE_PUNISHMENT_LAW :
		{
			if (group->instance == BOOK_1ST_INST)
			{
				file_name = "TimeB_f1p_.htm";
				file_recap = "TimeR_f1p_.htm";
			}
			else
			{
				file_name = "TimeB_f2__.htm";
				file_recap = "\0";
			}

			file_rjudge = "TimeRJf2__.htm";
			break;
		}
	}
#endif

	TTimeBook timeBook(parent, group, file_name);
	// 2014:324 FIX: BOOK_EXEC_NORMAL for no book -> BOOK_EXEC_NULL
	timeBook.Execute(group->book || group->bookMno ? BOOK_EXEC_HEADER : BOOK_EXEC_NULL);

	if (group->summary || group->judgeSummary)
	{
		if (strcmp(file_recap,"\0") && group->summary)
			TBookRecap(parent, group, &timeBook.recap, false, file_recap).Execute(BOOK_EXEC_HEADER);

		if (strcmp(file_recap,"\0") && group->judgeSummary)
			TBookRecap(parent, group, &timeBook.recap, true, file_rjudge).Execute(BOOK_EXEC_HEADER);
	}

	if (group->resultSet)
		TSessionBookResults(parent, group, "TimeR_res_.htm", title_fields).Execute(BOOK_EXEC_HEADER);
}

#include "TimeBookDialog.h"
#include "TimeBookDialog.cpp"

void time_book(TWindow *parent)
{
	TTimeBookGroup group;

	if (TTimeBookDialog(parent, &group).Execute() == IDOK)
		time_book(parent, &group);
}
