// ----- TFloatOutRegGroup ---------------------------------------------------
class TFloatOutRegGroup : public TFloatGroup
{
public:
	TFloatOutRegGroup();

	virtual void FormCriteria(msql &m);

	char lawTypes[SIZE_OF_TYPES];
	TGArray<TSubject> newSubjects;
	TGArray<TOldSubject> oldSubjects;
	char inRegKinds[SIZE_OF_KINDS];
	CDate minDate;
	CDate maxDate;
	CDate minReturned;
	CDate maxReturned;
	TGroupArray receivers;
	TGroupArray judges;
	char resultsIII[SIZE_OF_RESOLUTIONS];
	char resultsKS[SIZE_OF_RESOLUTIONS];
	TGroupArray vssIndexes;
	char decKinds[SIZE_OF_KINDS];
	// 2008:213 for derived classes
	char compositions[SIZE_OF_COMPOSITIONS];	// 2013:199 LRQ: long -> char[]
	char text[SIZE_OF_ORIGIN_TEXT];
	// 2017:234 +related
	char lawKindsRT[SIZE_OF_KINDS];
	long finalDecs;
};

// ----- query ---------------------------------------------------------------
void float_outreg(TWindow *parent);
void period_outreg(TWindow *parent, TFoundGroup *ucnGroup, TPeriodGroup *period);
