#include "cac.h"

// ----- TFloatSurroundGroup ----------------------------------------------------------
class TFloatSurroundGroup : public TFloatGroup
{
public:
	TFloatSurroundGroup();

	virtual void FormCriteria(msql &m);

	char lawTypes[SIZE_OF_TYPES];
	long minLawNo;
	long maxLawNo;
	long minLawYear;
	long maxLawYear;
	TGArray<TSubject> newSubjects;
	TGArray<TOldSubject> oldSubjects;
	TGroupArray judges;
	char kinds[SIZE_OF_KINDS];
	CDate minDate;
	CDate maxDate;
	CDate minSessionDate;
	CDate maxSessionDate;
	long firmData;
	char text[SIZE_OF_ORIGIN_TEXT];
};

TFloatSurroundGroup::TFloatSurroundGroup()
	// 2014:210 FB2: nested selects -> join, see below
	// 2016:060 LPR: from TFloatOutRegGroup
	: TFloatGroup("T_SURROUND S",
		"S.F_NO, S.F_YEAR, S.F_TYPE, S.F_KIND, S.F_DATE, S.F_TEXT, S.F_LAWSUIT_NO, S.F_LAWSUIT_YEAR, S.F_LAWSUIT_TYPE")
{
	Add(
		new TCharList("F_LAWSUIT_TYPES", lawTypes, sizeof lawTypes),
		new TLong("F_MIN_LAWSUIT_NO", &minLawNo),
		new TLong("F_MAX_LAWSUIT_NO", &maxLawNo),
		new TLong("F_MIN_LAWSUIT_YEAR", &minLawYear),
		new TLong("F_MAX_LAWSUIT_YEAR", &maxLawYear),
		new TCharList("F_KINDS", kinds, sizeof kinds),
		new TDate("F_MIN_DATE", &minDate, false),
		new TDate("F_MAX_DATE", &maxDate, false),
		new TDate("F_MIN_SESSION_DATE", &minSessionDate, false),
		new TDate("F_MAX_SESSION_DATE", &maxSessionDate, false),
		new TCheck("F_FIRM_DATA", &firmData, TRUE),
		new TString("F_TEXT", text, sizeof text),
		NULL
	);

	order = ORDER_BY_TYN;
}

void TFloatSurroundGroup::FormCriteria(msql &m)
{
	mstr tables(Name);

	// 2016:060 LPR: copied from TFloatOutRegGroup
	bool requests = !any(lawTypes) || strchr(lawTypes, TYPE_REQUEST);
	bool lawsuits = strcmp(lawTypes, TYPE_REQUESTS);

	if (lawsuits)
	{
		tables.cat(" LEFT JOIN T_LAWSUIT L ON L.F_NO = S.F_LAWSUIT_NO AND L.F_YEAR = S.F_LAWSUIT_YEAR "
			"AND L.F_TYPE = S.F_LAWSUIT_TYPE");
	}

	if (requests)
	{
		tables.cat(" LEFT JOIN T_REQUEST R ON R.F_NO = S.F_LAWSUIT_NO "
		"AND R.F_YEAR = S.F_LAWSUIT_YEAR AND R.F_TYPE = S.F_LAWSUIT_TYPE");
	}

	m.Begin(str(tables), fields);
	m.AddChars("S.F_LAWSUIT_TYPE", lawTypes, TYPE_SURROUNDABLES);
	m.AddRange("S.F_LAWSUIT_NO", minLawNo, maxLawNo);
	m.AddRange("S.F_LAWSUIT_YEAR", minLawYear, maxLawYear);

	// 2016:060 LPR: copied from TFloatOutRegGroup
	if (newSubjects.Count())
	{
		if (lawsuits && requests)
		{
			m.AddArray("(L.F_SUBJECT", newSubjects);
			m.NextOr();
			m.AddArray("R.F_SUBJECT", newSubjects);
			m.cat(")");
		}
		else
			m.AddArray(lawsuits ? "L.F_SUBJECT" : "R.F_SUBJECT", newSubjects);
	}

	if (oldSubjects.Count())
	{
		if (lawsuits && requests)
		{
			m.AddArray("F_SUBJECT", oldSubjects, "(L.F_OLD_SUBJECT");
			m.NextOr();
			m.AddArray("F_SUBJECT", oldSubjects, "R.F_OLD_SUBJECT");
			m.cat(")");
		}
		else
			m.AddArray("F_SUBJECT", oldSubjects, lawsuits ? "L.F_OLD_SUBJECT" : "R.F_OLD_SUBJECT");
	}

	m.AddArray("F_UCN", judges, "S.F_JUDGE");
	m.AddChars("S.F_KIND", kinds, KIND_SURROUNDS);
	m.AddRange("S.F_DATE", minDate, maxDate);
	// 2014:210 FB2: select count, not joined
	if (!minSessionDate.Empty() || !maxSessionDate.Empty())
	{
		m.AddRange("(SELECT COUNT(M.F_NO) FROM T_SURROUNDMENT M WHERE M.F_NO = S.F_NO AND M.F_YEAR = S.F_YEAR "
			"AND M.F_TYPE = S.F_TYPE AND M.F_DATE", minSessionDate, maxSessionDate);
		m.cat(") > 0");
	}

	m.AddILike1("S.F_TEXT", text, true);
}

// ----- TFloatSurround ------------------------------------------------------
class TFloatSurround : public TSurround
{
public:
	TFloatSurround() { }

	long lawSubject;
	long oldSubject;
	char lawKind;
	char damnName[SIZE_OF_DAMN_NAME];
	char companyType;
	TRCDKey origin;

	static bool LessEqlTyn(const long t1, const long t2);
	static bool LessEqlKyn(const long t1, const long t2);
	bool operator<=(const TFloatSurround &that) { return LessEqlTyn((const long) this, (const long) &that); }
};

bool TFloatSurround::LessEqlTyn(long t1, const long t2) // po nomer godina
{
	return ((const TFloatSurround *) t1)->key <= ((const TFloatSurround *) t2)->key;
}

bool TFloatSurround::LessEqlKyn(long t1, const long t2) // po data na vnasqne (LPR: ami?)
{
	const TFloatSurround *s1 = (const TFloatSurround *) t1;
	const TFloatSurround *s2 = (const TFloatSurround *) t2;

	return s1->kind != s2->kind ? s1->kind < s2->kind :
		s1->key.year != s2->key.year ? s1->key.year < s2->key.year :
		s1->key.no <= s2->key.no;
}

// ----- TFloatSurroundQuery -------------------------------------------------
class TFloatSurroundQuery : public TFloatQuery
{
public:
	TFloatSurroundQuery(TWindow *parent, const char *criteria, const char *name, bool tFirmData, long tOrder);

protected:
	void Count();
	bool firmData;
	long order;
};

TFloatSurroundQuery::TFloatSurroundQuery(TWindow *parent, const char *criteria, const char *name, bool tFirmData, long tOrder)
	: TFloatQuery(parent, criteria, name), firmData(tFirmData), order(tOrder)
{
}

#define surround OFFICE_GROUP_CAST(TFloatSurround)

static void floatsur_kind(mstr &m, TGroup *group)
{
	if (strchr(KIND_TEXTUAL_SURROUNDS, surround->kind) && *surround->text)
		m.cpy(surround->text);
	else
		m.cpy(Kind->Seek(surround->kind));
}
static void floatsur_type(mstr &m, TGroup *group) { m.printf("%c", *Type->Seek(surround->lawsuit.type)); }
static void floatsur_law_no(mstr &m, TGroup *group) { itom(surround->lawsuit.no, m); }
static void floatsur_law_year(mstr &m, TGroup *group) { itom(surround->lawsuit.year, m); }

static void floatsur_subject(mstr &m, TGroup *group)
{
	if (surround->lawSubject && surround->lawKind)
		m.cat(TSubject::Code(surround->lawSubject, surround->lawKind));

	print_old_subject(m, surround->oldSubject);
}

static void floatsur_from_date(mstr &m, TGroup *group)
{
	TSurroundment surroundment;
	msql w;

	surroundment.key = surround->key;
	surroundment.kind = surround->kind;
	surroundment.FloatCriteria(w, "F_DATE");

	TQuery q(str(w));
	if (q.Read())
	{
		surroundment << q;
		surroundment.sessionDate.Print(m);
	}
}

static void floatsur_firm(mstr &m, TGroup *group) { m.cpy(surround->damnName); }
static void floatsur_firm_type(mstr &m, TGroup *group)
{
	if (surround->companyType)
		m.cpy(CompanyType->Seek(surround->companyType));
}
static void floasur_origin(mstr &m, TGroup *group)
{
	Type->Seek(surround->origin.type);
	m.printf("%s %ld/%d", Type->alias, surround->origin.no, surround->origin.year);
}
static void floasur_document(mstr &m, TGroup *group)
{
	if (surround->origin.type != surround->lawsuit.type)
	{
		Type->Seek(surround->lawsuit.type);
		m.printf("%s %ld/%d", Type->alias, surround->lawsuit.no, surround->lawsuit.year);
	}
}

#undef surround

static const offield floatsur_fields[]=
{
	{ "F_NO", NULL },
	{ "F_YEAR", NULL },
	{ "F_SUR_DATE", container_date },
	{ "F_SUR_KIND", floatsur_kind },
	{ "F_SUR_TYPE", floatsur_type },
	{ "F_SUR_LAW_NO", floatsur_law_no },
	{ "F_SUR_LAW_YEAR", floatsur_law_year },
	{ "F_SUBJECT", floatsur_subject },
	{ "F_FROM_DATE", floatsur_from_date },
	{ "F_FIRM", floatsur_firm },
	{ "F_FIRM_TYPE", floatsur_firm_type },
	{ "F_ORIGIN", floasur_origin },
	{ "F_DOCUMENT", floasur_document },
	{ NULL, NULL }
};

void TFloatSurroundQuery::Count()
{
	TDamn damn;
	TLawsuit lawsuit;
	TRequest request;
	TFloatSurround surround;
	long nom = 0;
	TGSArray<TFloatSurround> floatSurroundArray(order == ORDER_BY_TYN ? TFloatSurround::LessEqlTyn :
		TFloatSurround::LessEqlKyn);
	TQuery q(criteria);

	while (q.Read())
	{
		bool lex = false;

		wait->TickCount(++nom, "обработени записа");
		surround << q;
		surround.lawSubject = 0;
		surround.oldSubject = 0;
		surround.lawKind = '\0';

	#if 20160103
		if (strchr(TYPE_ORIGINS, surround.lawsuit.type))
		{
			TOriginContainer *const container = (TOriginContainer *) type2RCDKeyContainer(surround.lawsuit.type);
			TVirtPtr VP(container);

			container->key = surround.lawsuit;

			if (container->Get("F_SUBJECT, F_KIND, F_OLD_SUBJECT"))
			{
				surround.lawSubject = container->subject;
				surround.oldSubject = container->oldSubject;
				surround.lawKind = container->LawKind();
			}
		}
	#else
		if (surround.lawsuit.type == TYPE_REQUEST)
		{
			request.key = surround.lawsuit;

			if (request.Get("F_SUBJECT, F_KIND, F_OLD_SUBJECT"))
			{
				surround.lawSubject = request.subject;
				surround.oldSubject = request.oldSubject;
				surround.lawKind = request.LawKind();
			}
		}
		else if (surround.lawsuit.type != TYPE_INREG)
		{
			lawsuit.key = surround.lawsuit;

			if (lawsuit.Try("F_SUBJECT, F_KIND, F_OLD_SUBJECT"))
			{
				surround.lawSubject = lawsuit.subject;
				surround.oldSubject = lawsuit.oldSubject;
				surround.lawKind = lawsuit.kind;
				lex = true;
			}
		}
	#endif

		if (firmData && lex && lawsuit.key.type == TYPE_FIRM_LAW && find_damn(&lawsuit, &damn, false))
		{
			strcpy(surround.damnName, damn.name);
			surround.companyType = damn.companyType;
		}
		else
		{
			strcpy(surround.damnName, "");
			surround.companyType = '\0';
		}

		TRCDKeyContainer *const tContainer = type2RCDKeyContainer(surround.key.type);
		TVirtPtr VP(tContainer);

		*tContainer << surround;

		if (tContainer->Try() && find_origin(tContainer, surround.origin))
		{
			TFloatSurround *sur = new TFloatSurround();

			*sur << surround;
			sur->origin = surround.origin;
			sur->lawSubject = surround.lawSubject;
			sur->oldSubject = surround.oldSubject;
			sur->lawKind = surround.lawKind;
			strcpy(sur->damnName, surround.damnName);
			sur->companyType = surround.companyType;
			floatSurroundArray.Add(sur);

			if (++count > maximum)
				break;
		}
	}

	for (int i = 0; i < floatSurroundArray.Count(); i++)
	{
		wait->TickCount(i, "отпечатани записа");
		office.Emit(tr1start, tr1final, floatsur_fields, floatSurroundArray[i]);
	}
}

static void query_surround(TWindow *parent, const char *criteria, bool firmData, long order)
{
	TFloatSurroundQuery(parent, criteria, firmData ? "QuerySuF.htm" : "QuerySur.htm", firmData, order).Execute();
}

#include "FloatSurroundDialog.h"	// 2002:142
#include "FloatSurroundDialog.cpp"

void float_surround(TWindow *parent)
{
	TFloatSurroundGroup group;
	TFloatSurroundDialog(parent, &group).Execute();
}
