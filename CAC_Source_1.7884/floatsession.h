// ----- TPeriodUCNGroup -----------------------------------------------------
class TPeriodUCNGroup : public TFloatGroup
{
public:
	TPeriodUCNGroup(const char *name);

	void Title(mstr &m, const char *prefix, const char *infix);

	CDate minDate;
	CDate maxDate;
	char ucn[SIZE_OF_UCN];
	char ucnType;

protected:
	void InnerCriteria(msql &m);
};

// ----- float ---------------------------------------------------------------
void timed_session(TWindow *parent);
void period_session(TWindow *parent);
void unwritten_ses(TWindow *parent);
