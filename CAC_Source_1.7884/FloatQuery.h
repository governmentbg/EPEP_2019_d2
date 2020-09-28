class TFloatQuery : public TProcess
{
public:
	TFloatQuery(TWindow *tParent, const char *tCriteria, const char *tName, const char *tTitle = NULL);

	void Execute();
	// public so that offield functions can access it
	const char *title;

protected:
	virtual void GetMarkers();
	bool Increment();
	bool ExecuteLL();
	void Show();

	const char *criteria;
	const char *name;
	TOfficeCopy copy;
	TOffice office;
	const char *tr1start;
	const char *tr1final;

	virtual void Count() = 0;
};

void floatquery_title(mstr &m, TGroup *group);
void floatquery_count(mstr &m, TGroup *group);

extern const offield float_fields[];
