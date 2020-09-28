#include "cac.h"

// ----- TRandomActGroup -----------------------------------------------------
enum
{
	RANDOM_ACT_ALLS,
	RANDOM_ACT_DECISIONS,
	RANDOM_ACT_FINALIZERS
};

class TRandomActGroup : public TDummyGroup
{
public:
	TRandomActGroup();

	char judge[SIZE_OF_UCN];
	long acts;
	CDate minDate;
	CDate maxDate;
	long forceDate;
	long count;
};

TRandomActGroup::TRandomActGroup()
{
	Add(
		new TPersonAlias("F_JUDGE", judge, sizeof judge),
		new TRadio("F_ALLS", &acts, RANDOM_ACT_ALLS),
		new TRadio("F_DECISIONS", &acts, RANDOM_ACT_DECISIONS),
		new TRadio("F_FINALIZERS", &acts, RANDOM_ACT_FINALIZERS),
		new TDate("F_MIN_DATE", &minDate, false),
		new TDate("F_MAX_DATE", &maxDate, true),
		new TCheck("F_FORCE_DATE", &forceDate, TRUE),
		new TLong("F_COUNT", &count),
		NULL
	);

	minDate = Today;
	minDate.YearsBack(3);
	acts = RANDOM_ACT_ALLS;
}

#define random OFFICE_GROUP_CAST(TRandomActGroup)

static void random_judge(mstr &m, TGroup *group) { print_user(m, random->judge, UCN_CITIZEN_UCN, false, true); }
static void random_min_date(mstr &m, TGroup *group) { random->minDate.Print(m, "%0D"); }
static void random_max_date(mstr &m, TGroup *group) { random->maxDate.Print(m, "%0D"); }
static void random_user(mstr &m, TGroup *) { print_user(m, Default->ucn, UCN_CITIZEN_UCN, false, true); }
static void random_now(mstr &m, TGroup *) { Now().Print(m, "%0lT"); }

static const offield title_fields[] =
{
	{ "F_JUDGE",	random_judge },
	{ "F_MIN_DATE",	random_min_date },
	{ "F_MAX_DATE",	random_max_date },
	{ "F_USER_NAME",	random_user },
	{ "F_COUNT",	NULL },
	{ "F_NOW",		random_now },
	{ NULL, NULL }
};

#undef random

// ----- TRandomActQuery -----------------------------------------------------
class TRandomActQuery : TProcess
{
public:
	TRandomActQuery(TWindow *parent, TRandomActGroup *tGroup);

	void Execute();

protected:
	TRandomActGroup *group;

	TGArray<TIndexContainer> acts;
};

TRandomActQuery::TRandomActQuery(TWindow *parent, TRandomActGroup *tGroup)
	: TProcess(parent, "справката", WAIT), group(tGroup)
{
}

static void print_connected(mstr &m, const TOutReg *outReg, char connectType, const char *instance, char result)
{
	TConnect connect;

	m.printf("Резултат от %s инстанция: %s", instance, Resolution->Seek(result));
	connect.key = outReg->key;
	connect.connectType = connectType;

	if (connect.Try())
	{
		m.printf(" от ");
		connect.decisionDate.Print(m, "%D ");
		m.cat(connect.connectKind ? ConnectKind->Seek(connect.connectKind) : UCNType->Seek(connect.ucnType));
		m.printf(" № %s/%ld", connect.connectNo, connect.connectYear);
	}

	if (any(outReg->vssIndex))
		m.printf(", индекс %s", outReg->vssIndex);
}

#define random OFFICE_GROUP_CAST(TIndexContainer)

static void random_kind(mstr &m, TGroup *group)
{
	// 2015:142 TRQ: session index if possible instead of settlement
	m.cat(Kind->Seek(random->IndexType() ? random->IndexKind() : KIND_SETTLEMENT));
}

static void random_event(mstr &m, TGroup *group)
{
	if (PRINT_JUDGEMENTS)
	{
		m.cat("\v");
		if (random->eventNo)
			m.printf("%ld/", random->eventNo);
		m.printf("%ld", random->IndexYear());
	}
}

static void random_date(mstr &m, TGroup *group)
{
	random->IndexDate()->Date().Print(m, "%0D");
}

static void random_type(mstr &m, TGroup *group) { Type->Seek(random->key.type); m.cat(Type->alias); }

static void random_appeal_result(mstr &m, TGroup *group)
{
	TGSArray<TRCDKeyContainer> array;
	random->LoadOutRegs(array);

	for (int i = 0; i < array.Count(); i++)
	{
		const TOutReg *outReg = (const TOutReg *) array[i];

		if (outReg->resultI != RESOLUTION_FAKE)
		{
			print_connected(m, outReg, CONNECT_TYPE_VZ_LAWSUIT, "въззивна",
				outReg->resultII != RESOLUTION_FAKE ? outReg->resultII : outReg->resultI);
		}

		if (outReg->resultKS != RESOLUTION_FAKE)
		{
			m.sep("\v\v");
			print_connected(m, outReg, CONNECT_TYPE_KS_LAWSUIT, "касационна", outReg->resultKS);
		}
	}
}

#undef random

static const offield random_fields[] =
{
	{ "F_KIND",		random_kind },
	{ "F_EVENT",	random_event },
	{ "F_DATE",		random_date },
	{ "F_TYPE",		random_type },
	{ "F_NO",		NULL },
	{ "F_YEAR",		NULL },
	{ "F_FORCE_DATE",	NULL },
	{ "F_APPEAL_RESULT", random_appeal_result },
	{ NULL, NULL }
};

void TRandomActQuery::Execute()
{
	TSession *session = new TSession;
	msql m(session->Name);

	{
		CDate minDate = group->minDate;
		CDate maxDate = group->maxDate;

		minDate.YearsBack(1);
		maxDate.YearsForward(1);
		m.AddRange("F_DATE", minDate, maxDate);
	}

	m.AddString("F_JUDGE", group->judge);
	m.AddChars("NOT F_RESULT", RESULT_UNCARRIEDS);

	if (group->acts != RANDOM_ACT_ALLS)
		m.AddIsNull("F_FINAL", false);

	m.AddOrder("F_DATE");
	TQuery q(str(m));

	while (q.Read())
	{
		*session << q;

		if (group->acts == RANDOM_ACT_FINALIZERS)
		{
			adjust_cancelaw(session);

			if (strchr(RESULT_ALL_CANCELEDS, session->result))
				continue;
		}

		TGArray<TDecision> decisions;

		if (try2decide(session, &decisions))
		{
			for (int i = 0; i < decisions.Count(); i++)
			{
				TDecision *decision = decisions[i];
				CDate &checkDate = group->forceDate ? decision->forceDate : decision->fromDate;

				if (checkDate >= group->minDate && checkDate <= group->maxDate)
					acts.AddCopy(decision);
			}
		}
		else if (!strchr(RESULT_ENTERABLES, session->result))
		{
			CDate &checkDate = group->forceDate ? session->forceDate : session->date;

			if (checkDate >= group->minDate && checkDate <= group->maxDate)
			{
				acts.Add(session);
				session = new TSession;
			}
		}

		wait->TickCount(acts.Count(), "обработени акта");
	}

	TVirtPtr VP(session);

	if (acts.Count() < 3)
		error("Няма достатъчно актове, постановени от съдията в посочения период.");
	else
	{
		TGArray<TIndexContainer> selected;
		TOffice office;

		srand(time(NULL) + GetTickCount());

		for (int i = 0; i < 3; )
		{
			TIndexContainer *container = acts[rand() % acts.Count()];
			int n;

			for (n = 0; n < selected.Count(); n++)
				if (selected[n] == container)
					break;

			if (n == selected.Count())
			{
				selected.Add(container);
				i++;
			}
		}

		if (office.Open("RandSel.htm"))
		{
			constant tr1start = office.Find("tr1 start");
			constant tr1final = office.Find("tr1 final");
			constant tr2start = office.Find("tr2 start");
			constant tr2final = office.Find("tr2 final");

			office.Emit(NULL, tr1start, title_fields, group);

			for (int i = 0; i < 3; i++)
				office.Emit(tr1start, tr1final, random_fields, selected[i]);

			office.Emit(tr1final, tr2start, title_fields, group);

			for (int i = 0; i < acts.Count(); i++)
				office.Emit(tr2start, tr2final, random_fields, acts[i]);

			group->count = acts.Count();
			office.Emit(tr2final, NULL, title_fields, group);
			office.Show(wait);

			mstr m;
			static constant aliases[] = { "Всички", "Без тек", "Без пре" };
			mstr randsel;

			m.printf("%s, %s, %0D-%0D", group->judge, aliases[group->acts], &group->minDate, &group->maxDate);

			for (int i = 0; i < 3; i++)
				log(LOG_QUERY, 0, selected[i], str(m));

			m.clear();
			m.printf("%s\\RANDSEL\\%s_%010d.htm", Electrics[TYPE_TEXT].docketPath, group->judge, time(NULL));
			copy_overead_file(office.Name, str(m), false, true);
		}

		selected.Clear();
	}
}

#include "RandomActDialog.h"
#include "RandomActDialog.cpp"

void random_act(TWindow *parent)
{
	TRandomActGroup group;

	if (TRandomActDialog(parent, &group).Execute() == IDOK)
		TRandomActQuery(parent, &group).Execute();
}
