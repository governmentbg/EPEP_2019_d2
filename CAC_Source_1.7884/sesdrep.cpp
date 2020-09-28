#include "cac.h"

// ----- TSessDistUser -------------------------------------------------------
class TSessDistUser
{
public:
	TSessDistUser(const char *tUCN);
	~TSessDistUser() { delete[] ucn; }

	bool operator<=(const TSessDistUser &that) const;

	char *ucn;
	mstr name;
};

TSessDistUser::TSessDistUser(const char *tUCN)
	: ucn(strnew(tUCN))
{
	TCitizen citizen;

	citizen.ucnType = UCN_CITIZEN_UCN;
	strcpy(citizen.ucn, ucn);

	if (citizen.Try())
		name.printf("%c. %s", *citizen.name, citizen.family);
	else
		name.cat("?. ??");
}

bool TSessDistUser::operator<=(const TSessDistUser &that) const
{
	return name.cmp(that.name) <= 0;
}

// ----- TSessDistUsers ------------------------------------------------------
class TSessDistUsers : public TISArray<TSessDistUser>
{
public:
	TSessDistUser *Find(const char *ucn);
};

TSessDistUser *TSessDistUsers::Find(const char *ucn)
{
	for (int i = 0; i < Count(); i++)
	{
		TSessDistUser *user = (*this)[i];

		if (!strcmp(user->ucn, ucn))
			return user;
	}

	TSessDistUser *user = new TSessDistUser(ucn);

	Add(user);
	return user;
}

// ----- TSessDistSecrs ------------------------------------------------------
class TSessDistSecrs : public TISArray<TSessDistUser>
{
public:
	void Collect(const char *ucn);

	~TSessDistSecrs() { Clear(); }
};

// ----- TSessDistUnit -------------------------------------------------------
class TSessDistUnit : public TDummyGroup
{
public:
	TSessDistUnit(const TSession &session);
	void Collect(const TSession &session);

	bool operator<=(const TSessDistUnit &that) const;
	bool operator==(const TSessDistUnit &that) const;

	CDate date;
	long instance;
	bool bef;
	TSessDistUser *judge;
	long count;
	TSessDistSecrs secrs;

	static TSessDistUsers Cache;
};

// ----- TSessDistSecrs ------------------------------------------------------
void TSessDistSecrs::Collect(const char *ucn)
{
	if (atob(ucn))
	{
		TSessDistUser *user = TSessDistUnit::Cache.Find(ucn);

		if (Find(user) == INT_MAX)
			Add(user);
	}
}

// ----- TSessDistUnit -------------------------------------------------------
TSessDistUnit::TSessDistUnit(const TSession &session)
{
	date = session.date;
	instance = secondary(session.kind) ? 2 : 1;
	bef = session.time.Bef();
	judge = Cache.Find(session.judge);
	count = 1;
	secrs.Collect(session.secretar);
}

void TSessDistUnit::Collect(const TSession &session)
{
	count++;
	secrs.Collect(session.secretar);
}

TSessDistUsers TSessDistUnit::Cache;

bool TSessDistUnit::operator<=(const TSessDistUnit &that) const
{
	return date != that.date ? date < that.date :
		instance != that.instance ? instance < that.instance :
		bef != that.bef ? bef > that.bef :
		judge->name.cmp(that.judge->name) <= 0;
}

bool TSessDistUnit::operator==(const TSessDistUnit &that) const
{
	return operator<=(that) && that.operator<=(*this);
}

static constant SESS_DIST_INSTANCES[] = { "", "I", "II" };

#define distunit OFFICE_GROUP_CAST(TSessDistUnit)

static void distunit_date(mstr &m, TGroup *group) { m.printf("%0D", &distunit->date); }
static void distunit_inst(mstr &m, TGroup *group) { m.cat(SESS_DIST_INSTANCES[distunit->instance]); }
static void distunit_am_pm(mstr &m, TGroup *group) { m.cat(distunit->bef ? "Преди обяд" : "След обяд"); }
static void distunit_judge(mstr &m, TGroup *group) { m.cat(distunit->judge->name); }
static void distunit_count(mstr &m, TGroup *group) { itom(distunit->count, m); }

static void distunit_secrs(mstr &m, TGroup *group)
{
	for (int i = 0; i < distunit->secrs.Count(); i++)
	{
		m.sep("\ahr\b");
		m.cat(distunit->secrs[i]->name);
	}
}

#undef distunit

static offield distunit_fields[] =
{
	{ "F_DATE",  distunit_date },
	{ "F_INST",  distunit_inst },
	{ "F_AM_PM", distunit_am_pm },
	{ "F_JUDGE", distunit_judge },
	{ "F_COUNT", distunit_count },
	{ "F_SECRS", distunit_secrs },		
	{ NULL, NULL }
};

// ----- TSessDistReport -----------------------------------------------------
class TSessDistReport : public TReport
{
public:
	TSessDistReport(TWindow *parent, const TReportGroup &reportGroup);

	void Execute();

protected:
	TGSArray<TSessDistUnit> units;

	virtual void CountLawsuits(int);
};

TSessDistReport::TSessDistReport(TWindow *parent, const TReportGroup &reportGroup)
	: TReport(parent, reportGroup)
{
}

void TSessDistReport::CountLawsuits(int)
{
	lawCrit.Begin("T_SESSION S JOIN T_LAWSUIT L ON L.F_NO = S.F_NO AND L.F_YEAR = S.F_YEAR AND L.F_TYPE = S.F_TYPE",
		"S.F_DATE, L.F_KIND, S.F_TIME, S.F_JUDGE, S.F_SECRETAR");
	CritBasic(lawCrit, "S.F_TYPE");
	lawCrit.AddRange("S.F_DATE", start, final);
	lawCrit.AddChars("S.F_KIND", KIND_OPEN_SESSIONS);
	//lawCrit.AddChars("NOT S.F_RESULT", RESULT_UNLINKEDS);
	lawCrit.AddChar("S.F_RESULT", RESULT_FIXED);

	TQuery q(str(lawCrit));
	mstr m;
	TSession session;

	wait->SetText("Брой заседания...");
	TSessDistUnit::Cache.Flush();

	while (q.Read())
	{
		session << q;

		TSessDistUnit unit(session);
		int index = units.Find(&unit);

		m.printf("заседания...", ++count);
		wait->TickText(str(m));

		if (index == INT_MAX)
			units.Add(new TSessDistUnit(session));
		else
			units[index]->Collect(session);
	}
}

void TSessDistReport::Execute()
{
	TOffice office;

	if (office.Open("SesDRep.htm"))
	{
		constant tr1start = office.Find("tr1 start");
		constant tr1final = office.Find("tr1 final");

		CountLawsuits(-1);
		office.Emit(NULL, tr1start, report_fields, this);

		for (int i = 0; i < units.Count(); i++)
			office.Emit(tr1start, tr1final, distunit_fields, units[i]);

		office.Emit(tr1final, NULL, report_fields, this);
		office.Show(wait);
		TSessDistUnit::Cache.Flush();
	}
}

void session_dist_report(TWindow *parent, const TReportGroup &group)
{
	TSessDistReport(parent, group).Execute();
}
