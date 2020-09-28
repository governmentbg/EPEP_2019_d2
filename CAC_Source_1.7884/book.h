// ----- TBookGroup ----------------------------------------------------------
class TBookGroup : public TFloatGroup
{
public:
	TBookGroup(const char *name, const char *tTypeName, const char *tDateName, bool tCollege, bool tAliasFace = false,
		const char *tNoName = NULL, const char *tYearName = NULL);

	virtual void FormCriteria(msql &m);
	virtual void Title(mstr &m);

	bool aliasFace;
	char types[SIZE_OF_TYPES];
	long by;
	long minNo;
	long maxNo;
	long year;
	CDate minDate;
	CDate maxDate;
	long autoSerialNo;	// 2011:259

	long flags;
	bool college;
	long count;			// 2014:063

	const char *xTypes;
	constant typeName;
	constant dateName;
	constant noName, yearName;	// 2017:157 +related
};

void book_types(mstr &m, TGroup *group);
void book_range(mstr &m, TGroup *group);
void book_dates(mstr &m, TGroup *group);
void book_year(mstr &m, TGroup *group);
void book_title(mstr &m, TGroup *group);		// standard

extern const offield book_fields[];

// ----- TSessionBookGroup ---------------------------------------------------
class TSessionBookGroup : public TBookGroup
{
public:
	TSessionBookGroup();

	class TMResult : public TDummyGroup
	{
	public:
		TMResult();

		char sesResult;
		char decResult;
		bool dateAfterFinish;
		bool returnedAfterFinish;
		long count;

		bool operator<=(const TMResult &that) const;
	};

	// results
	int total;
	TGSArray<TMResult> results;
	void CountResults(TSession *session, CDate &lawFinished);
};

// ----- TBaseBook -----------------------------------------------------------
const
	BOOK_EXEC_NULL	= 0,	// Collect() only
	BOOK_EXEC_NORMAL	= 1,
	BOOK_EXEC_HEADER	= 2;

class TBaseBook : public TOutput
{
public:
	TBaseBook(TWindow *parent, TBookGroup *tGroup, const char *tFileName, const offield *bodyFields,
		const offield *bookFields = ::book_fields, long tFlags = BOOK_OLD_FORMAT);

	virtual bool Execute(int bookExec = BOOK_EXEC_NORMAL);

protected:
	TBookGroup *group;
	long flags;
	TOffice office;
	const offield *body_fields;
	const offield *book_fields;
	const char *fileName;

	virtual void EmitBody();
	void Increment();
	void MinMax(long no);
	void MinMax(long no, const CDate &date);
	virtual void Markers();

	// 2016:155 LPR: speed tests only
#if SPEEDTESTING
	DWORD speedTicks;
#endif  // SPEEDTESTING
};

// ----- TBook ---------------------------------------------------------------
class TBook : public TBaseBook
{
public:
	TBook(TWindow *parent, TBookGroup *group, const char *fileName, const offield *bodyFields,
		const offield *bookFields = ::book_fields, long flags = BOOK_OLD_FORMAT, TGroupArray *tArray = NULL);
	virtual ~TBook();

	virtual bool Execute(int bookExec = BOOK_EXEC_NORMAL);

protected:
	TGroupArray *array;
	const char *tr1start;
	const char *tr1final;

	virtual void EmitBody();
	virtual void Markers();
	virtual TGroup *Process(TQuery &q) = 0;
	void Collect(const char *criteria);
	virtual void Collect();
	virtual void EmitRow(TGroup *group);
};

// ----- TSessionBookResults -------------------------------------------------
class TSessionBookResults : public TBaseBook
{
public:
	TSessionBookResults(TWindow *parent, TSessionBookGroup *group, const char *fileName, const offield *bookFields);

	void EmitPartial(TOffice &tOffice);

protected:
	const char *tb1start;
	const char *tr1start;
	const char *tr1break;
	const char *tr1final;
	const char *tb1final;

	void EmitContent(TOffice &office);

	virtual void Markers();
	virtual void EmitBody();
};
