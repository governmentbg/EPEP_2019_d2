// ----- TReportGroup --------------------------------------------------------
class TReportGroup : public TGroup	// 2017:025 +related
{
public:
	TReportGroup();
	void Import(begMYEndMYTypeCIU *pFMJWACSW);

	CDate start;
	CDate final;
	long instances;
	char types[SIZE_OF_TYPES];
	long composition;
	char connectType;
#if COURT_TYPE == COURT_REGIONAL
	TIntegerArray subjects;
#endif  // REGIONAL
};

// ----- TReportLawsuit ------------------------------------------------------
class TReportLawsuit : public TLawsuit
{
public:
	TReportLawsuit();

	bool SessionLeft();		// 2017:040 == outcomes LEFT and STOPED
	bool SessionOtherAct();		// final but not equal to any finished

	long intervalFlags;
	char sessionKind;
	char sessionResult;
	CDate sessionFinal;
	CDate started;
	char ucnType;
	char ucn[SIZE_OF_UCN];
};

// ----- TReport -------------------------------------------------------------
enum
{
	COUNT_UNFIN_START	= 1,	// unfinished at period start
	COUNT_NEW_PERIOD	= 2,	// constituted in the period
	COUNT_FINISHED	= 3,	// finished in the period
	COUNT_UNFIN_FINAL	= 4,	// unfinished at period final
	COUNT_RL_SESSION	= 5,
	COUNT_REPORT	= 5	// +1 base for derived classes
};

class TReport : public TOutput
{
public:
	TReport(TWindow *parent, const TReportGroup &tReportGroup);

	const char *const types;
	const CDate &start;
	const CDate &final;
	const long composition;
#if COURT_TYPE == COURT_REGIONAL
	const TIntegerArray &subjects;
#endif  // REGIONAL

	void dlprintf(const char *format, ...);

protected:
	const TReportGroup &reportGroup;

	char sOutKey[0x40];
	char sInKey[0x40];

	TReportLawsuit lawsuit;
	TOutReg outReg;
	TInReg inReg;
	TSession session;

	void SetLawKey() { TOutput::SetLawKey(lawsuit); }
	void SetOutKey();
	void SetInKey();
	bool FindFinalizer();
	char PublicActAge();		// 2017:031; AGE_NULL if not public act
	TSubject *SeekSubject();
	const char *SubjectAttrib() { return SeekSubject()->SD(lawsuit.kind).attrib; }
	TIntegerArray debugLaws;

	msql lawCrit;
	void CritBasic(msql &m, const char *typeName);	// 2017:030 +related
	void LawCritBasic();
	virtual void SesCritExtra();
	virtual void LawCritExtra();

	virtual void CountLawsuits(int what) = 0;
	void QueryLawsuits(const char *tWait, int count, const char *waitCount = NULL);
	// these create criteria lawCrit and call QueryLawsuits(COUNT_*)
	void CountStart();
	void CountPeriod();
	void CountFinished();
	void CountFinal();
	void CountSessions();
	// 2017:037 for stoppeds
	void CheckStopped();
	void ShowStopped();
	THTMLFile htmlog;
	long stoppedLaws;
};

extern const offield report_fields[];
