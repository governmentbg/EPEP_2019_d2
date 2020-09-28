// ----- TUnwrittenActGroup --------------------------------------------------
class TUnwrittenGroup : public TFloatGroup
{
public:
	TUnwrittenGroup(const char *name, const char *fields = NULL);

	void Title(mstr &m);

	TGroupArray judges;
	char kinds[SIZE_OF_KINDS];
	TGArray<TSubject> newSubjects;
	TGArray<TOldSubject> oldSubjects;
	CDate date;
};

// ----- TUnfinishedLawsuit --------------------------------------------------
class TUnfinishedLawsuit : public TLawsuit
{
public:
	TUnfinishedLawsuit(const CDate &tFinished);

	long Age();
	void FindPrevious() { FindPrevious(this); }

	CDate started;
	const CDate &finished;
	TGArray<TLawsuit> previous;

protected:
	void FindPrevious(const TLawsuit *lawsuit);
};

// ----- query ---------------------------------------------------------------
bool unwritten_act(const TSession *session, const CDate &maxActDate = DATE_MAX);
void unfinished_law(TWindow *parent);
void unwritten_act(TWindow *parent);
