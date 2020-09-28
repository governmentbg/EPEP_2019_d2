#include "cac.h"

// ----- TSideWindByUCN ------------------------------------------------------
class TSideWindByUCN : public TSideWind
{
public:
	TSideWindByUCN() { }

	bool operator<=(const TSideWindByUCN &that) const;
};

bool TSideWindByUCN::operator<=(const TSideWindByUCN &that) const
{
	return key.no != that.key.no ? key.no < that.key.no : key.year != that.key.year ? key.year < that.key.year :
		key.type != that.key.type ? key.type < that.key.type : ucnType != that.ucnType ? ucnType < that.ucnType :
		strcmp(ucn, that.ucn) <= 0;
}

// ----- TSideWindName -------------------------------------------------------
class TSideWindName : public TSideWind
{
public:
	TSideWindName(TSideWind *sideWind);

	bool operator<=(const TSideWindName &that) const;

	mstr name;
	long subject;
	char lawKind;
};

TSideWindName::TSideWindName(TSideWind *sideWind)
{
	Add(
		new TLong("F_SUBJECT", &subject),
		new TCharAlias("F_LAW_KIND", &lawKind),
		NULL
	);

	*this << sideWind;
	PrintNames(name);

	if (strchr(TYPE_ORIGINS, key.type))
	{
		TOriginContainer *const container = (TOriginContainer *) type2RCDKeyContainer(key.type);
		TVirtPtr VP(container);

		*container << this;
		container->Get(key.type == TYPE_REQUEST ? "F_SUBJECT, F_LAW_KIND" : "F_SUBJECT, F_KIND");
		subject = container->subject;
		lawKind = container->LawKind();
	}
}

bool TSideWindName::operator<=(const TSideWindName &that) const
{
	int result = strcmp(str(name), str(that.name));

	return result ? result < 0 : key.type != that.key.type ? key.type < that.key.type :
		key.year != that.key.year ? key.year < that.key.year : key.no <= that.key.no;
}

static const offield title_fields[] =
{
	{ "F_MIN_DATE",	NULL },
	{ "F_MAX_DATE",	NULL },
	{ NULL, NULL }
};

#define sideWind OFFICE_GROUP_CAST(TSideWindName)

static void sidename_name(mstr &m, TGroup *group) { m.cat(sideWind->name); }

static void sidename_ucn(mstr &m, TGroup *group)
{
	if (sideWind->ucnType == UCN_CITIZEN_UCN)
		m.cat(sideWind->ucn);
}

static void sidewind_subject(mstr &m, TGroup *group)
{
	if (sideWind->subject)
		m.cat(TSubject::Code(sideWind->subject, sideWind->lawKind));
}

#undef sideWind

static const offield sidename_fields[] =
{
	{ "F_NAMES",	sidename_name },
	{ "F_UCN",		sidename_ucn },
	{ "F_TYPE",		basewind_key_type },
	{ "F_NO",		NULL },
	{ "F_YEAR",		NULL },
	{ "F_SUBJECT",	sidewind_subject },
	{ NULL, NULL }
};

// ----- functions -----------------------------------------------------------
void period_side(TWindow *parent)
{
	TPeriodGroup period("T_PERIOD");
	period.minDate.Clear();

	if (TPeriodDialog(parent, &period).Execute() == IDOK)
	{
		TGSArray<TSideWindByUCN> sideWinds;
		TWaitWindow wait(parent, WAIT);
		TSideWindByUCN sideWind;
		msql m(sideWind.Name, "F_NO, F_YEAR, F_TYPE, F_KIND, F_UCN, F_UCN_TYPE, F_FLAGS, F_DATE");
		int count = 0;

		m.AddRange("F_DATE", period.minDate, period.maxDate);
		m.AddChar("(F_UCN_TYPE", UCN_CITIZEN_UCN);
		m.NextOr();
		m.AddFlags("F_FLAGS", SIDE_WIND_YOUNG, SIDE_WIND_YOUNG, SIDE_WIND_ALL);
		m.cat(")");

		TQuery q(str(m));
		while (q.Read())
		{
			bool process = false;

			sideWind << q;

			if (sideWind.flags & SIDE_WIND_CHILD)
				process = true;
			else if (sideWind.ucnType == UCN_CITIZEN_UCN)
			{
				CDate birth(sideWind.ucn);
				birth += -1;
				process = interval2Months(birth, sideWind.date) <= 216;
			}

			if (process && !strchr(TYPE_ORIGINS, sideWind.key.type))
			{
				TRCDKeyContainer *const container = kind2RCDKeyContainer(sideWind.kind);
				TVirtPtr VP(container);

				*container << sideWind;
				process = container->Get() && find_origin(container, sideWind.key);
			}

			if (process && !sideWinds.Contains(&sideWind))
			{
				TSideWindByUCN *tSideWind = new TSideWindByUCN;
				*tSideWind << sideWind;
				sideWinds.Add(tSideWind);
			}

			wait.TickCount(++count, "обработени страни");
		}

		TOffice office;
		TGSArray<TSideWindName> sideNames;

		for (int i = 0; i < sideWinds.Count(); i++)
			sideNames.Add(new TSideWindName(sideWinds[i]));

		if (office.Open("Minors.htm"))
		{
			constant tr1start = office.Find("tr1 start");
			constant tr1final = office.Find("tr1 final");

			office.Emit(NULL, tr1start, title_fields, &period);

			for (int i = 0; i < sideNames.Count(); i++)
				office.Emit(tr1start, tr1final, sidename_fields, sideNames[i]);

			TCounterGroup counter("Няма лица", "Едно лице", "лица");
			counter.count = sideNames.Count();

			office.Emit(tr1final, NULL, counter_fields, &counter);
			office.Show(&wait);
		}
	}
}
