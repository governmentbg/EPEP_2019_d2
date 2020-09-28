// ----- TRCDKeyContainer ----------------------------------------------------
class TRCDKeyContainer : public TRCDKeyGroup
{
public:
	TRCDKeyContainer(const char *name, TGroup *(*const tClone)());

	virtual void Involvements(mstr &m) const;
	virtual bool Suitor(char involvement);
	bool Defendant(char involvement) { return !Other(kind, involvement) && !Suitor(involvement); }
	static bool Other(char kind, char involvement);
	bool Other(char involvement) { return Other(kind, involvement); }
	char Category(char involvement);

	virtual bool Insert();
	virtual bool Combine(TRCDKeyContainer *source);
	virtual bool Update(const char *fields = NULL);
	virtual bool Delete(bool exact);

	int Kompare(const TRCDKeyContainer &that) const;	// kind, year, no
	bool operator<=(const TRCDKeyContainer &that) const;	// doesn't match Kompare()
#if TESTVER
	void SetStamp(const TRCDKeyContainer *parent) const;	// 2018-10-18
#endif  // TESTVER

	char kind;
	// 2003:287 BRW: The Big Change - date is now in TRCDKeyContainer
	// 2003:287 BRW: This breaks some stuff and obsoletes a lot of code
	CDate date;

protected:
	virtual bool Latest(TRCDKeyContainer *container);
	virtual void Signal(int sigtype, const void *data = NULL);

private:
	// 2013:319 LRQ/LPR: for sorting, +related
	mutable CDateTime stamp;
	mutable bool triedStamp;

	bool TimeStamp() const;
	void TryStamp() const;
	friend class TRCDKeyContainer;
};

#if TESTVER
// 2018-10-22: LPR: for logical re-sorting
class TGSArray<TRCDKeyContainer> : public TGroupArray
{
public:
	TGSArray() { lesseql = LessEqlT; }
	TGSArray(bool (*tLessEqlT)(const long t1, const long t2)) { lesseql = tLessEqlT; }

	TRCDKeyContainer *operator[](int index) const { return (TRCDKeyContainer *) ItemAt(index); }
	int Add(const TRCDKeyContainer *container);
	bool Contains(const TRCDKeyContainer *t) const { return TGroupArray::Contains(t); }
	TRCDKeyContainer *Detach(int index) { return (TRCDKeyContainer *) TGroupArray::Detach(index); }
	int Find(const TRCDKeyContainer *t) const { return TGroupArray::Find(t); }

protected:
	static bool LessEqlT(const long t1, const long t2) { return *(TRCDKeyContainer *) t1 <= *(TRCDKeyContainer *) t2; }
};
#endif  // TESTVER

// ----- TExternContainer ----------------------------------------------------
class TSubject;

class TExternContainer :  public TRCDKeyContainer
{
public:
	TExternContainer(const char *name, TGroup *(*const tClone)());

	virtual long Excess() const = 0;
	virtual char LawKind() const = 0;

	const char *Code() const { return TSubject::Code(subject, LawKind()); }
	const CDate &ForceDate() const;

	long subject;
	long quick;
};

// ----- TOriginContainer ----------------------------------------------------
class TOriginContainer : public TExternContainer
{
public:
	TOriginContainer(const char *name, TGroup *(*const tClone)());

	virtual long Excess() const;
	char MainConnectType() const;

	char eispp_nmr[SIZE_OF_EISPP_NMR];
	char oldKind;
	char college;
	long oldSubject;
};

// ----- TDummyContainer -----------------------------------------------------
class TDummyContainer : public TRCDKeyContainer
{
public:
	// 2003:287 BRW: TDummyContainer is now based on TRCDKeyContainer
	TDummyContainer() : TRCDKeyContainer("T_CONTAINER", NULL) { }
};

// ----- TDatedContainer -----------------------------------------------------
class TDatedContainer : public TRCDKeyContainer
{
public:
	TDatedContainer(const char *name, TGroup *(*const tClone)());
	TDatedContainer(TRCDKeyContainer *container, TGroup *(*const tClone)());	// 2002:094 for sched

	virtual void ExactCriteria(msql &m, const char *fields = NULL);
	virtual void FloatCriteria(msql &m, const char *fields = NULL);
	void FloatCriteria(char *s, const char *fields = NULL) { TGroup::FloatCriteria(s, fields); }
	virtual void Involvements(mstr &m) const;
	virtual bool Suitor(char involvement);

	virtual bool Insert();

	// 2003:283 BRW: for internal use mostly: former Insert(source)
	virtual bool Combine(TRCDKeyContainer *source);	// TGroup/involvement
};

// ----- TIndexContainer -----------------------------------------------------
class TIndexContainer : public TDatedContainer
{
public:
	TIndexContainer(const char *name, TGroup *(*const tClone)());

	virtual bool Insert();			// always insert event index
	virtual bool Delete(bool exact);	// also delete index if any

	virtual char IndexType() const = 0;	// index type or TYPE_NULL
	virtual char IndexKind() const = 0;
	virtual bool IndexEvent();		// insert into T_EVENT_INDEX

	long IndexYear() const;
	bool DeleteIndex();

	sign IndexRead(bool mandatory);
	bool IndexRead();
	bool operator<=(const TIndexContainer &that) const;
	// 2017:110
	void LoadOutRegs(TGroupArray &array) const;
	TDate *IndexDate() const;
	void OutRegCriteria(msql &w, const char *fields) const;     // 2017:313

	long eventNo;
};

// ----- TPrimeContainer -----------------------------------------------------
class TPrimeContainer : public TDatedContainer	// 2011:287
{
public:
	TRCDKey lawsuit;

	virtual void FloatCriteria(msql &m, const char *fields = NULL);	// by lawsuit
	void FloatCriteria(char *s, const char *fields = NULL) { TGroup::FloatCriteria(s, fields); }

protected:
	TPrimeContainer(const char *name, TGroup *(*const tClone)());

	virtual bool Latest(TRCDKeyContainer *container);
};
