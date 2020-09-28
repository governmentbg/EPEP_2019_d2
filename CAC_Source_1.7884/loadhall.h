// ----- TLoadUnit -----------------------------------------------------------
class TLoadUnit : public TDummyGroup, public TLoadData
{
public:
	TLoadUnit();
	virtual ~TLoadUnit();

	void PrintTime(mstr &m);
	void PrintSession(mstr &m);
	void PrintNonSession(mstr &m);

	void PrintLoad(mstr &m);
	void PrintHtml(mstr &m);

	bool Empty();
	bool SameSlot(const TLoadUnit &that);

	CDate date;
	char dow;
	long hall;
	bool bef;
	TSession *session;
	bool start;		// first session unit
	bool ender;		// last session unit + has entime
	bool subseq;	// same date & hall

	bool operator<=(const TLoadUnit &that);
};

// ----- TLoadGroup ----------------------------------------------------------
class TLoadGroup : public TPeriodGroup
{
public:
	TLoadGroup();
	virtual ~TLoadGroup();

	TIntegerArray halls;
	TGSArray<TLoadUnit> units;	// 2014:156 unified sort
	int mon_days[3][37];
	int mon_count;
	TOffice office;
	bool Generate();
	char *names[2];

	int mon_idx;
	int day_idx;
	int hall_idx;
	CDate date;

	void PrepareUnits();
	void AddCriteria(msql &m);
	void FillMonDays();
	void AddSession(TSession &session);
	void FixupUnits();

	void CheckPeriod(mstr &m, TSession *session);
	void CheckUsage(mstr &m, TSession *session, const TLawsuit *lawsuit);
	bool CheckAsk(mstr &m, TSession *session, const TLawsuit *lawsuit, bool outside);

protected:
	void FillUnits(const CDate &date, long hall, bool bef);
	void AddUnit(TSession &session, const CDate &date, bool bef);
};
