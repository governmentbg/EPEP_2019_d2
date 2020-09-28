#include "cac.h"

// ----- TFloatElectricGroup -------------------------------------------------
class TFloatElectricGroup : public TFloatGroup
{
public:
	TFloatElectricGroup();

	virtual void FormCriteria(msql &m);

	char type;
	long minNo;
	long maxNo;
	long minYear;
	long maxYear;
	CDate minDate;
	CDate maxDate;
	long variant;
};

enum
{
	VARIANT_DOCUMENT,
	VARIANT_SHEETABLE,
	VARIANT_MISSING
};

TFloatElectricGroup::TFloatElectricGroup()
	: TFloatGroup("T_ELECTRIC")
{
	Add(
		new TCharAlias("F_TYPE", &type),
		new TLong("F_MIN_NO", &minNo),
		new TLong("F_MAX_NO", &maxNo),
		new TLong("F_MIN_YEAR", &minYear),
		new TLong("F_MAX_YEAR", &maxYear),
		new TDate("F_MIN_DATE", &minDate, false),
		new TDate("F_MAX_DATE", &maxDate, false),
		new TRadio("F_DOCUMENT", &variant, VARIANT_DOCUMENT),
		new TRadio("F_SHEETABLE", &variant, VARIANT_SHEETABLE),
		new TRadio("F_MISSING", &variant, VARIANT_MISSING),
		NULL
	);

	order = ORDER_BY_TYN;
}

void TFloatElectricGroup::FormCriteria(msql &m)
{
	const char *name;
	mstr fields;

	fields.cpy("F_NO, F_YEAR, F_TYPE, F_KIND, F_DATE, F_TEXT, F_EISPP_NMR");

	switch (type)
	{
		case TYPE_REQUEST : name = "T_REQUEST"; break;
		case TYPE_INREG : name = "T_INREG"; break;
		// 2010:120 LPR: outreg support
		case TYPE_OUTREG : name = "T_OUTREG"; break;
		default :
		{
			if (type && strchr(TYPE_LAWSUITS, type))
			{
				name = "T_LAWSUIT";
				fields.cat(", F_SOURCE_NO, F_SOURCE_YEAR, F_SOURCE_TYPE, F_SOURCE_KIND, F_FINISHED");
			}
			else
				fatal("%d: type not supported by FormCriteria", type);
		}
	}

	m.Begin(name, str(fields));
	m.AddChar("F_TYPE", type);
	m.AddRange("F_NO", minNo, maxNo);
	m.AddRange("F_YEAR", minYear, maxYear);
	if (variant == VARIANT_MISSING)
		m.AddRange("F_FINISHED", minDate, maxDate);
	EndCriteria(m);
}

#if TESTVER
static void collect_sbe_index(const char *name, const void *indexes)
{
	((TIntegerArray *) indexes)->Add(eispp_get_sbe_index(name));
}

static void collect_sbe_indexes(TRCDKeyContainer *container, TIntegerArray &indexes)
{
	char name[SIZE_OF_PATH];

	electric_name(container, EISPP_XMLPATH, name, TYPE_SBE_DESC);
	html_group_action(name, collect_sbe_index, &indexes);
}
#else  // TESTVER
static void collect_sbevid(const char *name, const void *vids)
{
	((TIntegerArray *) vids)->Add(eispp_get_sbevid(name));
}

static void collect_vids(TRCDKeyContainer *container, TIntegerArray &vids)
{
	char name[SIZE_OF_PATH];

	electric_name(container, EISPP_XMLPATH, name, TYPE_SBE_DESC);
	html_group_action(name, collect_sbevid, &vids);
}
#endif  // TESTVER

#define container OFFICE_GROUP_CAST(TRCDKeyContainer)

static void floatelc_title(mstr &m, TGroup *group)
{
	m.printf("%s %ld/%ldã.", Type->Seek(container->key.type), container->key.no, container->key.year);
}

static void floatelc_extra(mstr &m, TGroup *group)
{
	m.cat(group->FindExtraText()->S());
}

static void floatelc_event_date(mstr &m, TGroup *group)
{
	container_event_date1(container).Print(m, "%D");
}

static void floatelc_attached(mstr &m, TGroup *group)
{
	container_attached(m, group);

	// quick optimization without finding college/origin
	if (any(EISPP_XMLPATH) && !strchr(TYPE_LAWSUITS, container->key.type) || container->key.type == TYPE_PUNISHMENT_LAW)
	{
	#if TESTVER
		TIntegerArray indexes;
		collect_sbe_indexes(container, indexes);

		for (int i = 0; i < indexes.Count(); i++)
		{
			long index = indexes[i];
			long vid = eispp_get_sbevid(index);

			m.sep(", ");

			if (vid)
				m.printf("%ld %s", vid, EisVal->Seek(nmk_sbevid, vid));
			else	// fallback
				m.printf("#(%ld)", index);
		}
	#else  // TESTVER
		TIntegerArray vids;
		collect_vids(container, vids);

		for (int i = 0; i < vids.Count(); i++)
		{
			long vid = vids[i];

			m.sep(", ");
			m.printf("%ld", vid);

			EisVal->nom = nmk_sbevid;
			EisVal->code = vid;

			if (EisVal->Try("F_NAME"))
				m.printf(" %s", EisVal->name);
		}
	#endif  // TESTVER
	}
}

static void floatelc_subject(mstr &m, TGroup *group)
{
	TRCDKey origin;

	if (container->key.no && find_origin(container, origin) && strchr(TYPE_ORIGINS, origin.type))
	{
		TOriginContainer *const tContainer = (TOriginContainer *) type2RCDKeyContainer(origin.type);
		TVirtPtr VP(tContainer);

		tContainer->key = origin;

		if (tContainer->Get(tContainer->key.type == TYPE_REQUEST ? "F_SUBJECT, F_LAW_KIND" : "F_SUBJECT, F_KIND"))
			m.cat(tContainer->Code());
	}
}

#undef container

static const offield floatelc_fields[] =
{
	{ "F_TITLE",     floatelc_title },
	{ "F_EXTRA",     floatelc_extra },
	{ "F_DATE",      floatelc_event_date },
	{ "F_EVENT",     container_event_text },
	{ "F_ATTACHED",  floatelc_attached },
	{ "F_SUBJECT",   floatelc_subject },
	{ "F_NO",        NULL },
	{ "F_YEAR",      NULL },
	{ "F_KIND",      container_kind },
	{ "F_EISPP_NMR", NULL },
	{ "F_FINISHED",  NULL },	
	{ NULL, NULL }
};

// ----- TElectricQuery ------------------------------------------------------
class TElectricQuery : public TElectricCollect
{
public:
	TElectricQuery(TRCDKeyContainer *container, TGArray<TRCDKeyContainer> *tArray, TFloatElectricGroup *felGroup);

	virtual void AddItem(TRCDKeyContainer *container);

	CDate &minDate, &maxDate;
	TGArray<TRCDKeyContainer> *array;
};

TElectricQuery::TElectricQuery(TRCDKeyContainer *container, TGArray<TRCDKeyContainer> *tArray, TFloatElectricGroup *felGroup)
:
	TElectricCollect(container), array(tArray), minDate(felGroup->minDate), maxDate(felGroup->maxDate)
{
}

void TElectricQuery::AddItem(TRCDKeyContainer *container)
{
	array->Add(container);
}

// ----- query ---------------------------------------------------------------
void query_electric(TWindow *parent, const char *criteria, TFloatElectricGroup *group)
{
	static constant FILE_NAMES[] = { "FloatElc.htm", "FloatElc1.htm", "FloatElc2.htm" };
	TWaitWindow wait(parent, WAIT_MORE);
	TWindowHideParent hide(parent);
	TRCDKeyContainer *const container = type2RCDKeyContainer(group->type);
	TVirtPtr VP(container);
	TOffice office;
	mstr title;

	if (group->minDate.Empty())
		group->minDate = DATE_MIN;
	if (group->maxDate.Empty())
		group->maxDate = DATE_MAX;

	if (office.Open(FILE_NAMES[group->variant]))
	{
		constant hd1start = office.Find("hd1 start");
		constant tr1start = office.Find("tr1 start");
		constant tr1break = office.Find("tr1 break");
		constant tr1final = office.Find("tr1 final");
		constant hd1final = office.Find("hd1 final");

		office.Emit(NULL, hd1start, null_fields, NULL);
		TQuery q(criteria);
		mstr m;

		while (q.Read())
		{
			bool match = false;
			mstr title;
		#if TESTVER
			TIntegerArray sbe_indexes;
		#else  // TESTVER
			TIntegerArray vids;
		#endif  // TESTVER

			*container << q;
			m.clear();
			m.printf("%s %ld/%ldã...", Type->Seek(container->key.type), container->key.no, container->key.year);
			wait.TickText(str(m));

			TGArray<TRCDKeyContainer> array;
			TElectricQuery query(container, &array, group);
			query.Collect();

			for (int i = 0; i < array.Count(); i++)
			{
				TRCDKeyContainer *tContainer = array[i];

				if (group->variant == VARIANT_MISSING)
				{
				#if TESTVER
					collect_sbe_indexes(tContainer, sbe_indexes);

					if (sbe_indexes.Count())
						break;
				#else  // TESTVER
					collect_vids(tContainer, vids);

					if (vids.Count())
						break;
				#endif  // TESTVER
				}
				else if (tContainer->date >= group->minDate && tContainer->date <= group->maxDate)
				{
					if (!match)
					{
						if (group->variant == VARIANT_SHEETABLE)
						{
							floatelc_title(title, container);
							container->AddExtraText(str(title));
						}

						office.Emit(hd1start, tr1start, floatelc_fields, container);
						match = true;
					}

					if (group->variant == VARIANT_SHEETABLE)
					{
						office.Emit(tr1start, tr1break, floatelc_fields, container);
						office.Emit(tr1break, tr1final, floatelc_fields, tContainer);
					}
					else
						office.Emit(tr1start, tr1final, floatelc_fields, tContainer);
				}
			}

			if (group->variant == VARIANT_MISSING)
			{
			#if TESTVER
				if (!sbe_indexes.Count())
			#else  // TESTVER
				if (!vids.Count())
			#endif  // TESTVER
					office.Emit(tr1start, tr1final, floatelc_fields, container);
			}
			else if (match)
				office.Emit(tr1final, hd1final, null_fields, NULL);

			container->DeleteExtraText();
			array.DeleteExtraText();
		}

		office.Emit(hd1final, NULL, null_fields, NULL);
		office.Show(&wait);
	}
}

#include "FloatElectricDialog.h"	// 2003:149
#include "FloatElectricDialog.cpp"

void float_electric(TWindow *parent)
{
	TFloatElectricGroup group;
	TFloatElectricDialog(parent, &group).Execute();
}
