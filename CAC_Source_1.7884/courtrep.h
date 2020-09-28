// ----- TReportCourt --------------------------------------------------------
class TReportCourt : public TSender
{
public:
	TReportCourt(const char *tUCN, char tUCNType);
	TReportCourt(const char *tName);

	bool operator<=(const TReportCourt &that) const;
	size_t Positive() const;

	long values[COURT_COLUMNS_MAX];
	long index;

protected:	
	enum
	{
		PRIOR_UNKNOWN,
		PRIOR_THIS_AREA,
		PRIOR_OTHER_AREA,
		PRIOR_PRIV_EXEC,
		PRIOR_SUMMARY,
	} prior;
};

// ----- TReportCourtArray ---------------------------------------------------
class TReportCourtArray : public TGSArray<TReportCourt>
{
public:
	TReportCourt *CourtAt(int i) { return (*this)[i]; }
	void Increment(const char *ucn, char ucnType, int column);
	TReportCourt *Summary(const char *name);
};

// ----- TCourtReport --------------------------------------------------------
class TCourtReport : public TReport
{
public:
	TCourtReport(TWindow *parent, const TReportGroup &group);

protected:
	TReportCourtArray courts;

	TGArray<TDecision> decisions;
	TDecision *decision;

	int FindFinishedColumn(constant *resultSplitFinisheds, int columnCount);

	void Increment(int column);

	virtual void Check();

	TOffice office;		// must be open by Show
	const char *tr1start;	// loaded by ShowUpper
	const char *tr1final;	// loaded by ShowUpper

	void ShowUpper();
};
