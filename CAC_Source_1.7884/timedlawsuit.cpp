#include "cac.h"

// ----- TTimedLawsuitGroup --------------------------------------------------
enum
{
	AGE_ALL_SET		= -1,
	AGE_TO_3_DAYS	= -3,
	AGE_TO_7_DAYS	= -7,
	AGE_NOT_SET		= AGE_NONE_YET
};

enum
{
	ORDER_BY_TYND	= ORDER_BY_USER,
	ORDER_BY_JDTYN
};

class TTimedLawsuitGroup : public TFloatGroup
{
public:
	TTimedLawsuitGroup();

	virtual void FormCriteria(msql &m);

	char lawKinds[SIZE_OF_KINDS];
	long minNo;
	long maxNo;
	long minYear;
	long maxYear;
	TGroupArray judges;
	char compositions[SIZE_OF_COMPOSITIONS];
	TGArray<TSubject> newSubjects;
	TGArray<TOldSubject> oldSubjects;
	long requestDate;	// else lawsuit date
	CDate minRequestDate;
	CDate maxRequestDate;
	CDate minLawsuitDate;
	CDate maxLawsuitDate;
	long age;
};

TTimedLawsuitGroup::TTimedLawsuitGroup()
	: TFloatGroup("T_LAWSUIT L LEFT JOIN T_REQUEST R ON R.F_NO = L.F_SOURCE_NO AND R.F_YEAR = L.F_SOURCE_YEAR "
		"AND R.F_TYPE = L.F_SOURCE_TYPE")
{
	Add(
		new TCharList("F_LAW_KINDS", lawKinds, sizeof lawKinds),
		new TLong("F_MIN_NO", &minNo),
		new TLong("F_MAX_NO", &maxNo),
		new TLong("F_MIN_YEAR", &minYear),
		new TLong("F_MAX_YEAR", &maxYear),
		new TCharList("F_COMPOSITIONS", compositions, sizeof compositions),
		new TRadio("F_REQUEST_DATE", &requestDate, TRUE),
		new TRadio("F_LAWSUIT_DATE", &requestDate, FALSE),
		new TDate("F_MIN_REQUEST_DATE", &minRequestDate, false),
		new TDate("F_MAX_REQUEST_DATE", &maxRequestDate, false),
		new TDate("F_MIN_LAWSUIT_DATE", &minLawsuitDate, false),
		new TDate("F_MAX_LAWSUIT_DATE", &maxLawsuitDate, false),
		new TRadio("F_AGE_ALL_SET", &age, AGE_ALL_SET),
		new TRadio("F_AGE_TO_3_DAYS", &age, AGE_TO_3_DAYS),
		new TRadio("F_AGE_TO_7_DAYS", &age, AGE_TO_7_DAYS),
		new TRadio("F_AGE_ONE_OR_LESS", &age, AGE_ONE_OR_LESS),
		new TRadio("F_AGE_ONE_TO_TWO", &age, AGE_ONE_TO_TWO),
		new TRadio("F_AGE_ONE_TO_THREE", &age, AGE_ONE_TO_THREE),
		new TRadio("F_AGE_MORE_THAN_THREE", &age, AGE_MORE_THAN_THREE),
		new TRadio("F_AGE_NOT_SET", &age, AGE_NOT_SET),
		new TRadio("F_BY_TYND", &order, ORDER_BY_TYND),
		new TRadio("F_BY_JDTYN", &order, ORDER_BY_JDTYN),
		NULL
	);

	age = AGE_ALL_SET;
	order = ORDER_BY_TYND;
}

void TTimedLawsuitGroup::FormCriteria(msql &m)
{
	m.Begin(Name, "L.F_NO, L.F_YEAR, L.F_TYPE, L.F_KIND, L.F_DATE AS F_LAWSUIT_DATE, L.F_RECEIVED_DATE, L.F_JUDGE, "
		"L.F_SUBJECT, R.F_DATE AS F_REQUEST_DATE, L.F_OLD_SUBJECT");

	m.AddChars("L.F_KIND", lawKinds, KIND_LAWSUITS);
	m.AddRange("L.F_NO", minNo, maxNo);
	m.AddRange("L.F_YEAR", minYear, maxYear);
	m.AddArray("F_UCN", judges, "L.F_JUDGE");
	m.AddCompos("L.F_COMPOSITION", compositions);
	m.AddArray("L.F_SUBJECT", newSubjects);
	m.AddArray("F_SUBJECT", oldSubjects, "L.F_OLD_SUBJECT");

	if (requestDate)
	{
		if (!minRequestDate.Empty() || !maxRequestDate.Empty())
		{
			m.AddRange("(R.F_DATE", minRequestDate, maxRequestDate);
			m.NextOr();
			m.AddRange("F_RECEIVED_DATE", minRequestDate, maxRequestDate);
			m.cat(")");
		}
	}
	else
		m.AddRange("L.F_DATE", minLawsuitDate, maxLawsuitDate);

	m.AddOrder(order == ORDER_BY_TYND ? "F_TYPE, F_YEAR, F_NO, F_DATE" : "F_JUDGE, F_DATE, F_TYPE, F_YEAR, F_NO", "L.");
}

// ----- TTimedLawsuit -------------------------------------------------------
class TTimedLawsuit : public TLawsuit
{
public:
	TTimedLawsuit();

	CDate requestDate;
	TSession session;
};

TTimedLawsuit::TTimedLawsuit()
{
	Add(new TDate("F_REQUEST_DATE", &requestDate, false));
	Rename("F_DATE", "F_LAWSUIT_DATE");
}

#define lawsuit OFFICE_GROUP_CAST(TTimedLawsuit)

static void lawsuit_kind(mstr &m, TGroup *group) { m.cat(Kind->Seek(lawsuit->kind)); }

static void lawsuit_subject(mstr &m, TGroup *group)
{
	m.printf("%s\v%s", lawsuit->Code(), Subject->Seek(lawsuit->key.type, lawsuit->subject));
	print_old_subject(m, lawsuit->oldSubject);
}

static void lawsuit_received_date(mstr &m, TGroup *group) { lawsuit->receivedDate.Print(m); }
static void lawsuit_judge(mstr &m, TGroup *group) { print_user(m, lawsuit->judge); }

static void lawsuit_session_kind(mstr &m, TGroup *group)
{
	if (lawsuit->session.kind)
		m.cat(Kind->Seek(lawsuit->session.kind));
}

static void lawsuit_session_date(mstr &m, TGroup *group) { lawsuit->session.date.Print(m); }

static void print_index_container(mstr &m, TIndexContainer *container, CDate *date)
{
	m.sep("\v");
	m.cat(Kind->Seek(container->IndexType() ? container->IndexKind() : container->kind));
	if (PRINT_JUDGEMENTS && container->eventNo)
		m.printf(" № %ld", container->eventNo);
	m.printf(" от %0D", date);
}

static void lawsuit_result(mstr &m, TGroup *group)
{
	TSession session;

	if (find_finalizer(lawsuit, session))
	{
		TGArray<TDecision> decisions;

		if (try2decide(&session, &decisions))
		{
			for (int i = 0; i < decisions.Count(); i++)
				print_index_container(m, decisions[i], &decisions[i]->fromDate);
		}
		else
			print_index_container(m, &session, &session.date);
	}
	// 2014:268 IRQ/SGS
	else if (!lawsuit->session.kind)
	{
		msql w;
		CDate date;

		session.key = lawsuit->key;
		session.FloatCriteria(w, "F_DATE, F_RESULT");
		w.AddChars("F_KIND", KIND_CLOSED_SESSIONS);
		w.AddOrder("F_DATE DESC");

		TQuery q(str(w));
		while (q.Read())
		{
			session << q;

			if (date.Empty())
				date = session.date;
			else if (date != session.date)
				break;

			if (strchr(RESULT_HOLDS, session.result) || session.result == RESULT_STOPPED)
			{
				m.printf("%s от %0D", Result->Seek(session.result), &session.date);
				break;
			}
		}
	}
}

#undef lawsuit

static const offield lawsuit_fields[] =
{
	{ "F_KIND",			lawsuit_kind },
	{ "F_NO",			NULL },
	{ "F_YEAR",			NULL },
	{ "F_SUBJECT",		lawsuit_subject },
	{ "F_RECEIVED_DATE",	lawsuit_received_date },
	{ "F_LAWSUIT_DATE",	NULL },
	{ "F_JUDGE",		lawsuit_judge },
	{ "F_SESSION_KIND",	lawsuit_session_kind },
	{ "F_SESSION_DATE",	lawsuit_session_date },
	{ "F_RESULT",		lawsuit_result },
	{ NULL,			NULL }
};

// ----- TTimedLawsuitQuery --------------------------------------------------
class TTimedLawsuitQuery : public TFloatQuery
{
public:
	TTimedLawsuitQuery(TWindow *parent, const char *criteria, long tRequestDate, long tAge);

protected:
	long requestDate;
	long age;
	int total;

	TTimedLawsuit lawsuit;

	virtual void Count();
};

TTimedLawsuitQuery::TTimedLawsuitQuery(TWindow *parent, const char *criteria, long tRequestDate, long tAge)
	: TFloatQuery(parent, criteria, "TimedLaw.htm", NULL), requestDate(tRequestDate), age(tAge), total(0)
{
	SetNames("Няма дела", "Едно дело", "дела");
}

void TTimedLawsuitQuery::Count()
{
	TWindowHideParent hide(parent);
	TQuery q(criteria);
	char kinds[SIZE_OF_KINDS];

	strcpy(kinds, KIND_OPEN_SESSIONS);
	stremove(kinds, KIND_OTHER_SESSION);

	while (q.Read())
	{
		wait->TickCount(++total, "обработени записа");
		msql m("T_SESSION", "F_KIND, F_DATE");

		lawsuit << q;
		if (lawsuit.receivedDate.Empty())
			lawsuit.receivedDate = lawsuit.requestDate;
		m.Add(&lawsuit, "F_NO, F_YEAR, F_TYPE");
		m.AddChars("F_KIND", kinds);
		m.AddOrder("F_DATE");

		TQuery r(str(m));
		bool found = r.Read();

		if (found != (age != AGE_NOT_SET))
			continue;

		if (found)
		{
			// 2014:339 FIX: moved outside if (age !=
			lawsuit.session << r;

			if (age != AGE_ALL_SET)
			{
				CDate start = requestDate ? lawsuit.receivedDate : lawsuit.date;

				if (age < AGE_ALL_SET)
				{
					for (int i = 0; i < -age; i++)
						do start += 1; while (!start.Work());

					if (start < lawsuit.session.date)
						continue;
				}
				else if (interval2ActAge(start, lawsuit.session.date, true) != age)
					continue;
			}
		}
		else
		{
			lawsuit.session.kind = '\0';
			lawsuit.session.date.Clear();
		}

		office.Emit(tr1start, tr1final, lawsuit_fields, &lawsuit);
		count++;
	}
}

// ----- query ---------------------------------------------------------------
#include "TimedLawsuitDialog.h"
#include "TimedLawsuitDialog.cpp"

void timed_lawsuit(TWindow *parent)
{
	TTimedLawsuitGroup group;
	TTimedLawsuitDialog(parent, &group).Execute();
}
