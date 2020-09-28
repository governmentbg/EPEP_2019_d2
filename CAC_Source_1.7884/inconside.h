#if COURT_TYPE == COURT_AREAL
// ----- TInconBaseGroup -----------------------------------------------------
class TInconBaseGroup : public TFloatGroup
{
public:
	TInconBaseGroup(const char *name, const char *fields, long tCreditor);

	virtual void FormCriteria(msql &m);

	long creditor;	// FALSE = debtor + requester, -1 = any debtor
	
protected:
	void AddCount(msql &m, const char *involvements, const char *condition);
};

// ----- TInconSideGroup -----------------------------------------------------
class TInconSideGroup : public TInconBaseGroup
{
public:
	TInconSideGroup(TUCNGroup *ucnGroup, const char *name, const char *fields);

	virtual void FormCriteria(msql &m);

	char ucn[SIZE_OF_UCN];
	char ucnType;
	CDate minDate;
	CDate maxDate;

protected:
	void AddCount(msql &m, const char *involvements, const char *condition);
};

void query_inconside(TWindow *parent);
#endif  // AREAL
