#include "cac.h"

// ----- TFloatIndexGroup ----------------------------------------------------
class TFloatIndexGroup : public TFloatGroup
{
public:
	TFloatIndexGroup();

	virtual void FormCriteria(msql &m);

	const char *tableName;
	const char *dateFieldName;
	int index;

	char types[SIZE_OF_TYPES];
	char decKinds[SIZE_OF_KINDS];
	CDate minFromDate;
	CDate maxFromDate;
	long minEventNo;
	long maxEventNo;
	CDate minForceDate;
	CDate maxForceDate;
};

TFloatIndexGroup::TFloatIndexGroup()
	: TFloatGroup("T_INDEX_GROUP")
{
	Add(
		new TCharList("F_TYPES", types, sizeof types),
		new TCharList("F_DEC_KINDS", decKinds, sizeof decKinds),
		new TDate("F_MIN_FROM_DATE", &minFromDate, false),
		new TDate("F_MAX_FROM_DATE", &maxFromDate, false),
		new TLong("F_MIN_EVENT_NO", &minEventNo),
		new TLong("F_MAX_EVENT_NO", &maxEventNo),
		new TDate("F_MIN_FORCE_DATE", &minForceDate, false),
		new TDate("F_MAX_FORCE_DATE", &maxForceDate, false),
		NULL
	);

	order = ORDER_BY_DTYN;
}

enum
{	// Note: these must much the containers[] below
	INDEX_DECISION,
	INDEX_SESSION,
	INDEX_RESOLUTION,
	INDEX_INREGEVENT,
	INDEX_SURROUENDOC,
	INDEX_TABLECOUNT
};

void TFloatIndexGroup::FormCriteria(msql &m)
{
	mstr fields;

	fields.cpy("F_EVENT_NO, F_NO, F_YEAR, F_TYPE, F_KIND, ");
	fields.cat(index == INDEX_RESOLUTION ? "F_RESOLUTION" : "F_RESULT");

	if (index == INDEX_DECISION || index == INDEX_SESSION || index == INDEX_INREGEVENT)
		fields.cat(", F_RETURNED");

	if (index == INDEX_DECISION)
		fields.cat(", F_FORCE_DATE, F_FROM_DATE");
	else if (index == INDEX_SURROUENDOC)
		fields.cat(", F_LAWSUIT_TYPE, F_DECISION_KIND, F_DECISION_DATE");
	else
		fields.cat(", F_DATE");

	m.Begin(tableName, str(fields));

	if (minEventNo || maxEventNo)
		m.AddRange("F_EVENT_NO", minEventNo, maxEventNo);
	else
		m.AddIsNull("F_EVENT_NO", false);

	m.AddRange(dateFieldName, minFromDate, maxFromDate);

	if (index == INDEX_DECISION || index == INDEX_SESSION)
		m.AddChars("F_TYPE", types, COLLEGE_ALLS);

	if (index == INDEX_DECISION)
		m.AddChars("F_KIND", adjust_xxx_kinds(decKinds, KIND_VISIBLE_DECISIONS, KIND_DECISIONS), KIND_EVENT_INDEXES);

	EndCriteria(m);
}

// ----- query ---------------------------------------------------------------
#define floatind OFFICE_GROUP_CAST(TIndexContainer)

static void floatind_date_year(mstr &m, TGroup *group) { itom(floatind->date.year, m); }
static void floatind_returned(mstr &m, TGroup *group)
{
	TDate *returned = (TDate *) floatind->Find("F_RETURNED");
	if (returned)
		returned->Date().Print(m);
}
static void floatind_force_date(mstr &m, TGroup *group)
{
	TDate *forceDate = (TDate *) floatind->Find("F_FORCE_DATE");
	if (forceDate)
		forceDate->Date().Print(m);
}
static void floatind_count(mstr &m, TGroup *group)
{
	if (floatind->eventNo > 0)
	{
		if (floatind->eventNo == 1)
			m.cat("Един запис отговаря на зададените критерии." );
		else
			m.printf("%ld записа отговарят на зададените критерии.", floatind->eventNo);
	}
}
static void floatind_origin(mstr &m, TGroup *group)
{
	TRCDKey origin;
	TRCDKeyContainer *const tContainer = type2RCDKeyContainer(floatind->key.type);
	TVirtPtr VP(tContainer);

	*tContainer << floatind;

	if (tContainer->Try() && find_origin(tContainer, origin))
	{
		if (!strchr(TYPE_IOREGS, origin.type))
		{
			Type->Seek(origin.type);
			m.printf("%s %ld/%d", Type->alias, origin.no, origin.year);
		}
	}
}

static void floatind_document(mstr &m, TGroup *group)
{
	if (strchr(TYPE_IOREGS, floatind->key.type))
	{
		Type->Seek(floatind->key.type);
		m.printf("%s %ld/%d", Type->alias, floatind->key.no, floatind->key.year);
	}
}

#undef floatind

static const offield floatind_fields[]=
{
	{ "F_EVENT_NO", NULL },
	{ "F_DATE_YEAR", floatind_date_year },
	{ "F_KIND", container_kind },
	{ "F_DATE", NULL },
	{ "F_RETURNED", floatind_returned },
	{ "F_FORCE_DATE", floatind_force_date },
	{ "F_TYPE", keygroup_key_type },
	{ "F_NO", NULL },
	{ "F_YEAR", NULL },
	{ "F_COUNT", floatind_count },
	{ "F_ORIGIN", floatind_origin },
	{ "F_DOCUMENT", floatind_document },
	{ NULL, NULL }
};

static void query_index(TWindow *parent, TFloatIndexGroup *group)
{
	TSession session;
	TDecision decision;
	TInRegEvent inRegEvent;
	TRequestResolution resolution;
	TSurroundment surrouendoc;

	long count = 0;
	char indexKind;
	char indexType;

	TWaitWindow wait(parent, WAIT);
	TISArray<TIndexContainer> array;
	TIndexContainer *containers[INDEX_TABLECOUNT] = { &decision, &session, &resolution, &inRegEvent, &surrouendoc };
	msql m;

	for (group->index = 0; group->index < INDEX_TABLECOUNT; group->index++)
	{
		TIndexContainer *container = containers[group->index];
		TDate *date = (TDate *) container->Find("F_FROM_DATE");

		if (!date)
			date = (TDate *) container->Find("F_DECISION_DATE");

		if (!date)
			date = (TDate *) container->Find("F_DATE");

		group->tableName = container->Name;
		group->dateFieldName = date->Name;
		group->FormCriteria(m);
		TQuery q(str(m));

		while (q.Read())
		{
			*container << q;
			indexKind = container->IndexKind();
			indexType = container->IndexType();

			if (indexType && indexKind && (strmatch(indexKind, group->decKinds) || container == &decision) &&
				strmatch(indexType, group->types))
			{
				TIndexContainer *tContainer = (TIndexContainer *) container->clone();

				*tContainer << container;
				tContainer->kind = indexKind;
				tContainer->date = date->Date();

				array.Add(tContainer);
				wait.TickCount(++count, "обработени акта");
			}
		}
	}

	TOffice office;

	if (office.Open("FloatInd.htm"))
	{
		constant tr1start = office.Find("tr1 start");
		constant tr1final = office.Find("tr1 final");

		office.Emit(NULL, tr1start, null_fields, NULL);

		for (int i = 0; i < array.Count(); i++)
			office.Emit(tr1start, tr1final, floatind_fields, array[i]);

		if (count)
			array[0]->eventNo = count;

		office.Emit(tr1final, NULL, count ? floatind_fields : null_fields, count ? array[0] : NULL);
		office.Show(&wait);
	}
}

// ----- float ---------------------------------------------------------------
#include "FloatIndexDialog.h"
#include "FloatIndexDialog.cpp"

void float_index(TWindow *parent)
{
	TFloatIndexGroup group;
	TFloatIndexDialog(parent, &group).Execute();
}
