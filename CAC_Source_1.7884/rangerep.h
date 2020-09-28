// ----- TRangeReportGroup ---------------------------------------------------
class TRangeReportGroup : public TReportGroup
{
public:
	TRangeReportGroup();

	char attrib;
	long withSubjects;
#if COURT_TYPE == COURT_REGIONAL
	long excludeLow;
	long excludeHigh;
#endif  // REGIONAL
};

// ----- TRangeReport --------------------------------------------------------
enum
{
	COUNT_APPEALED	= COUNT_REPORT + 1,
	COUNT_RANGEREP	= COUNT_REPORT + 1
};

class TRangeReport : public TReport
{
public:
	TRangeReport(TWindow *parent, const TRangeReportGroup &group);

protected:
	TGSArray<TReportRange> reportRanges;

	bool AddCount(int column, int count);
	bool Increment(int column) { return AddCount(column, 1); }

	void CountAppealed();

	virtual void Check();

	TOffice office;		// must be open by Show
	const char *tr1start;	// loaded by ShowUpper
	const char *tr1final;	// loaded by ShowUpper

	void ShowUpper();
	static const offield reprange_fields[];
};
