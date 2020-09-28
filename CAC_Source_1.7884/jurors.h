#if COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
// ----- TJurorHistory -------------------------------------------------------
class TJurorHistory : public TJurorLog
{
public:
	TJurorHistory() { }
	TJurorHistory(const TJurorDistrib *distrib);

	const char *title;
	const TLawsuit *lawsuit;
	static const offield fields[];

	void Execute();
	const char *Jurors(int index) { return jurors[index]; }

protected:
	char jurors[JUROR_COUNT][SIZE_OF_UCN];

	virtual void Event() = 0;
};

void new_distrib_jurors(TWindow *parent, TBaseSession *session, const TLawsuit *lawsuit);
void edit_distrib_jurors(TWindow *parent, TBaseSession *session, const TLawsuit *lawsuit);
void query_juror_distrib(TWindow *parent, const TLawsuit *lawsuit, TJurorDistrib *distrib);
void query_juror_distrib(TWindow *parent, const TExternSession *suggest, bool forceExcess);
void query_juror_load(TWindow *parent);
#endif  // AREAL || MILITARY || REGIONAL
