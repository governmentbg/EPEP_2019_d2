#include "cac.h"

// ----- TFloatMovementGroup -------------------------------------------------
enum { ORDER_BY_DTTYN = ORDER_BY_USER };

class TFloatMovementGroup : public TFloatGroup
{
public:
	TFloatMovementGroup();

	virtual void FormCriteria(msql &m);

	char types[SIZE_OF_TYPES];
	char kinds[SIZE_OF_KINDS];
	CDate minDate;
	CDate maxDate;
	TGroupArray users;
	char text[SIZE_OF_ORIGIN_TEXT];
};

TFloatMovementGroup::TFloatMovementGroup()
	: TFloatGroup("T_MOVEMENT")
{
	Add(
		new TCharList("F_TYPES", types, sizeof types),
		new TCharList("F_KINDS", kinds, sizeof kinds),
		new TDate("F_MIN_DATE", &minDate, false),
		new TDate("F_MAX_DATE", &maxDate, false),
		new TString("F_TEXT", text, sizeof text),
		new TRadio("F_BY_DTTYN", &order, ORDER_BY_DTTYN),
		NULL
	);

	order = ORDER_BY_TKYN;
}

void TFloatMovementGroup::FormCriteria(msql &m)
{
	BeginCriteria(m);

	m.AddChars("F_TYPE", types, TYPE_MOVABLES);
	m.AddChars("F_KIND", kinds, KIND_MOVEMENTS);
	m.AddRange("F_DATE", minDate, maxDate);
	m.AddArray("F_UCN", users);
	m.AddLike("F_TEXT", text, true);
}

// ----- TFloatMovement ------------------------------------------------------
class TFloatMovement : public TMovement
{
public:
	TRCDKey origin;

	// 2014:212 LPR: were manual sort functions
	static bool LessEqlTkyn(const long t1, const long t2);
	static bool LessEqlDttyn(const long t1, const long t2);
	bool operator<=(const TFloatMovement &that) { return LessEqlTkyn((const long) this, (const long) &that); }
};

bool TFloatMovement::LessEqlTkyn(const long t1, const long t2) // F_TYPE, F_NO, F_YEAR  -- na ORIGIN
{
	const TFloatMovement *m1 = (const TFloatMovement *) t1;
	const TFloatMovement *m2 = (const TFloatMovement *) t2;

	return m1->origin != m2->origin ? m1->origin < m2->origin :
		m1->date != m2->date ? m1->date < m2->date :
		m1->time != m2->time ? m1->time < m2->time :
		m1->kind <= m2->kind;
}

bool TFloatMovement::LessEqlDttyn(const long t1, const long t2) // F_DATE, F_TIME, F_TYPE, F_NO, F_YEAR  -- na ORIGIN
{
	const TFloatMovement *m1 = (const TFloatMovement *) t1;
	const TFloatMovement *m2 = (const TFloatMovement *) t2;

	return m1->date != m2->date ? m1->date < m2->date :
		m1->time != m2->time ? m1->time < m2->time :
		m1->origin != m2->origin ? m1->origin < m2->origin :
		m1->kind <= m2->kind;
}

// ----- TFloatMovementQuery -------------------------------------------------
class TFloatMovementQuery : public TFloatQuery
{
public:
	TFloatMovementQuery(TWindow *parent, const char *criteria, const char *name, TRCDKeyGroup *tGroup, TGSArray<TFloatMovement> *tArray, long tOrderBy);

protected:
	void Count();
	long orderBy;
	TRCDKeyGroup *group;
	TGSArray<TFloatMovement> *array;
};

TFloatMovementQuery::TFloatMovementQuery(TWindow * parent, const char *criteria, const char *name, TRCDKeyGroup *tGroup, TGSArray<TFloatMovement> *tArray, long tOrderBy)
#if 20181127
	: TFloatQuery(parent, criteria, name, array ? "" : NULL), array(tArray), group(tGroup), orderBy(tOrderBy)
#else  // 20181127
	: TFloatQuery(parent, criteria, name), array(tArray), group(tGroup), orderBy(tOrderBy)
#endif  // 20181127
{
}

#define movement OFFICE_GROUP_CAST(TFloatMovement)

static void movement_user(mstr &m, TGroup *group)
{
	if (any(movement->ucn))
		print_user(m, movement->ucn, UCN_CITIZEN_UCN);
}
static void movement_origin(mstr &m, TGroup *group)
{
	TRCDKey origin;
	TRCDKeyContainer *tContainer = type2RCDKeyContainer(movement->key.type);
	*tContainer << movement;
	if (tContainer->Try() && find_origin(tContainer, origin))
	{
		Type->Seek(origin.type);
		m.printf("%s %ld/%d", Type->alias, origin.no, origin.year);
	}
	delete tContainer;
}
static void movementr_document(mstr &m, TGroup *group)
{
	if (movement->origin.type != movement->key.type)
	{
		Type->Seek(movement->key.type);
		m.printf("%s %ld/%d", Type->alias, movement->key.no, movement->key.year);
	}
}
#undef movement

static const offield movement_fields[]=
{
	{ "F_TYPE", keygroup_key_type },
	{ "F_NO", NULL },
	{ "F_YEAR", NULL },
	{ "F_KIND", container_kind },
	{ "F_DATE", NULL },
	{ "F_TIME", NULL },
	{ "F_TEXT", NULL },
	{ "F_USER", movement_user },
	{ "F_ORIGIN", movement_origin },
	{ "F_DOCUMENT", movementr_document },
	{NULL, NULL}
};

void TFloatMovementQuery::Count()
{
	TGSArray<TFloatMovement> floatMoveArray(orderBy == ORDER_BY_TKYN ? TFloatMovement::LessEqlTkyn :
		TFloatMovement::LessEqlDttyn);
	long nom = 0;

	if (array)
	{
		for (int i = 0; i< array->Count(); i++)
		{
			office.Emit(tr1start, tr1final, movement_fields, (*array)[i]);
		}
	}
	else  // FloatMov.htm
	{
		TFloatMovement movement;
		TQuery q(criteria);

		while (q.Read())
		{
			wait->TickCount(++nom, "обработени записа");
			movement << q;
			TRCDKeyContainer *tContainer = type2RCDKeyContainer(movement.key.type);
			*tContainer << movement;
			if (tContainer->Try() && find_origin(tContainer, movement.origin))
			{
				TFloatMovement *mov = new TFloatMovement();
				*mov << movement;
				mov->origin = movement.origin;
				floatMoveArray.Add(mov);

				if (++count > maximum)
					break;
			}
			delete tContainer;
		}

		for (int i = 0; i < floatMoveArray.Count(); i++)
		{
			wait->TickCount(i, "обработени записа в документа");
			office.Emit(tr1start, tr1final, movement_fields, floatMoveArray[i]);
		}
	}
}

// ----- query ---------------------------------------------------------------
static void query_movement(TWindow *parent, const char *criteria, long orderBy)
{
	TFloatMovementQuery(parent, criteria, "FloatMov.htm", NULL, NULL, orderBy).Execute();
}

#include "FloatMovementDialog.h"
#include "FloatMovementDialog.cpp"

void float_movement(TWindow *parent)
{
	TFloatMovementGroup group;
	TFloatMovementDialog(parent, &group).Execute();
}

#if 20181127
// 2018-10-31: LPR/URQ: separate query, sql sorting
class TQueryMovementQuery : public TFloatQuery
{
public:
	TQueryMovementQuery(TWindow *parent, const char *criteria, const char *title);

protected:
	virtual void Count();
};

TQueryMovementQuery::TQueryMovementQuery(TWindow *parent, const char *criteria, const char *title)
	: TFloatQuery(parent, criteria, "QueryMov.htm", title)
{
}

void TQueryMovementQuery::Count()
{
	TQuery q(criteria);
	TMovement movement;

	while (q.Read() && Increment())
	{
		movement << q;
		office.Emit(tr1start, tr1final, movement_fields, &movement);
	}
}

void query_movement(TWindow *parent, TRCDKeyGroup *group)
{
	msql m("T_MOVEMENT");
	mstr title(" на ");

	title.printf("%s %ld/%ldг.", Type->Seek(group->key.type), group->key.no, group->key.year);
	m.Add(group, "F_NO, F_YEAR, F_TYPE");
	m.AddOrder("F_DATE, F_TIME, F_RESULT");
	TQueryMovementQuery(parent, str(m), str(title)).Execute();
}
#else  // 20181127
// N.B. required because of TMovement sort operators
static void query_moves(TRCDKeyGroup *group, TGSArray<TFloatMovement> *array)
{
	TFloatMovement movement;
	msql m;

	movement.key = group->key;
	movement.FloatCriteria(m);

	TQuery q(str(m));
	while (q.Read())
	{
		TFloatMovement *tMovement = new TFloatMovement;
		*tMovement << q;
		array->Add(tMovement);
	}
}

void query_movement(TWindow *parent, TRCDKeyGroup *group)
{
	TGSArray<TFloatMovement> array;
	query_moves(group, &array);
	TFloatMovementQuery(parent, NULL, "QueryMov.htm", group, &array, ORDER_BY_TKYN).Execute();
}
#endif  // 20181127
