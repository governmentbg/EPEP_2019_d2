#include "cac.h"

// ----- FloatImportantGroup -------------------------------------------------
TFloatImportantGroup::TFloatImportantGroup()
	: TFloatGroup("T_LAWSUIT")
{
	Add(
		new TCharList("F_TYPES", types, sizeof types),
		new TDate("F_START", &start, false),
		new TDate("F_FINAL", &final, false),
		NULL
	);

	order = ORDER_BY_TYN;

	// 2002:161
	start = Today;

	while (start.DayOfWeek() != 1)
		start += -1;

	final = start;

	while (!start.Work())
		start += -1;

	final += 11;

	while (!final.Work())
		final += 1;
}

void TFloatImportantGroup::FormCriteria(msql &m)
{
	BeginCriteria(m);
	m.AddChars("F_TYPE", types, TYPE_LAWSUITS);
	m.AddFlags("F_FLAGS", LAWSUIT_IMPORTANT, LAWSUIT_IMPORTANT, LAWSUIT_ALL);
	EndCriteria(m);
}

// ----- query ---------------------------------------------------------------
class TFloatImportantQuery : public TFloatQuery
{
public:
	TFloatImportantQuery(TWindow *parent, TQuery &tq, TFloatImportantGroup *tGroup, const char *title);

protected:
	TQuery &q;
	TFloatImportantGroup *group;

	virtual void Count();
};

TFloatImportantQuery::TFloatImportantQuery(TWindow *parent, TQuery &tq, TFloatImportantGroup *tGroup, const char *title)
	: TFloatQuery(parent, NULL, "FloatImp.htm", title), q(tq), group(tGroup)
{
}

#define session OFFICE_GROUP_CAST(TSession)

static void floatimp_lawsuit_kind(mstr &m, TGroup *group) { m.cpy(Kind->Seek(session->key.type)); }
static void floatimp_judge(mstr &m, TGroup *group) { print_user(m, session->judge); }

#undef session

static const offield floatimp_fields[] =
{
	{ "F_LAWSUIT_KIND", floatimp_lawsuit_kind },
	{ "F_NO", keygroup_key_no },
	{ "F_YEAR", keygroup_key_year },
	{ "F_SESSION_KIND", container_kind },
	{ "F_DATE", NULL },
	{ "F_JUDGE", floatimp_judge },
	{ NULL, NULL }
};

void TFloatImportantQuery::Count()
{
	TLawsuit lawsuit;
	TSession session;
	msql m;

	while (q.Read())
	{
		lawsuit << q;

		m.Begin(session.Name, "F_NO, F_YEAR, F_TYPE, F_KIND, F_DATE, F_JUDGE");
		m.Add(&lawsuit, "F_NO, F_YEAR, F_TYPE");
		m.AddRange("F_DATE", Today, group->final);
		m.AddOrder("F_DATE");

		TQuery r(str(m));
		while (r.Read() && Increment())
		{
			session << r;
			session.key.type = lawsuit.kind;
			office.Emit(tr1start, tr1final, floatimp_fields, &session);
		}
	}
}

void query_important(TWindow *parent, TFloatImportantGroup *group)
{
	THTMLFile h;
	h.OpenTemp();

	TWaitWindow wait(parent, WAIT);
	TWindowHideParent hide(parent);

	msql m;
	group->FormCriteria(m);
	TQuery q(str(m));

	TLawsuit lawsuit;

	bool dirty = false;
	// PASS 1
	while (q.Read())
	{
		lawsuit << q;

		TGSArray<TRCDKeyContainer> array;

		query_events(&lawsuit, &array);

		bool first = true;
		for (int i = 0; i < array.Count(); i++)
		{
			if
			(
				array[i]->date <= Today &&
				(group->start.Empty() || array[i]->date >= group->start) &&
				(group->final.Empty() || array[i]->date <= group->final)
			)
			{
				if (first)
				{
					if (dirty)
						h.puts("\n----------\n\n");
					else
						dirty = true;
					query_law(h, &lawsuit, false);
					h.putc('\n');
					first = false;
				}

				query_event(h, array[i], true);
			}
		}
	}

	h.Show();

	if (group->final.Empty() || group->final > Today)
	{
		// PASS 2
		q.Restart();

		m.clear();
		m.printf("Дела от обществен интерес насрочени от %Dг. до %Dг.", &Today, &group->final);
		TFloatImportantQuery(parent, q, group, str(m)).Execute();
	}
}

// ----- float ---------------------------------------------------------------
#include "FloatImportantDialog.h"	// 2002:158
#include "FloatImportantDialog.cpp"

void float_important(TWindow *parent)
{
	TFloatImportantGroup group;

	if (TFloatImportantDialog(parent, &group).Execute() == IDOK)
		query_important(parent, &group);
}
