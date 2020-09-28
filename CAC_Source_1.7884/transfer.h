// ----- TField --------------------------------------------------------------
class TField : public TObject
{
public:
	TField(const char *name);

	virtual void Print(mstr &m) const = 0;
	virtual void Quote(mstr &m) const;		// 2012:054 LPR: NULL | [quoted] value
	virtual bool IsNULL() const = 0;
	virtual void Nullify() = 0;

	virtual void ExportToControl(TControl *control) const = 0;
	virtual void ImportFromQuery(const pFLDDesc desc, const TQuery *query) = 0;
	virtual void ImportFromField(const TField *field) = 0;
	virtual void ImportFromControl(TControl *control) = 0;

	static void WindowImport(TWindow *window, void *group);

protected:
	bool Quoted;

private:
	TField(const TField &that);
};

// ----- TFixedField ---------------------------------------------------------
class TFixedField : public TField
{
public:
	TFixedField(const char *name, void *data, size_t size);

	virtual bool IsNULL() const;
	virtual void Nullify();

protected:
	void SetData(const void *data);

	virtual void ExportToControl(TControl *control) const;
	virtual void ImportFromQuery(const pFLDDesc desc, const TQuery *query);
	virtual void ImportFromField(const TField *field);
	virtual void ImportFromControl(TControl *control);

	pBYTE const Data;
	size_t const Size;
};

// ----- TBaseString ---------------------------------------------------------
class TBaseString : public TField
{
public:
	TBaseString(const char *name, size_t size);

	virtual const char *S() const = 0;
	virtual void SetS(const char *s) = 0;
	virtual void Print(mstr &m) const;
	size_t Limit() { return Size; }

	virtual bool IsNULL() const;
	virtual void Nullify();

	size_t const Size;	// ::= maxLen + 1

protected:
	virtual void ExportToControl(TControl *control) const;
	virtual void ImportFromField(const TField *field);
	virtual void ImportFromControl(TControl *control);
};

// ----- TString -------------------------------------------------------------
#define TString TNTString
class TString : public TBaseString
{
public:
	TString(const char *name, char *data, size_t size);

	virtual const char *S() const;
	virtual void SetS(const char *s);

protected:
	virtual void ImportFromQuery(const pFLDDesc desc, const TQuery *query);

	char *const Data;
};

// ----- TLongString ---------------------------------------------------------
class TLongString : public TString
{
public:
	TLongString(const char *name, char *data, size_t size);

protected:
	virtual void ImportFromQuery(const pFLDDesc desc, const TQuery *query);
};

// ----- TName -------------------------------------------------------------
class TName : public TString
{
public:
	TName(const char *name, char *data, size_t size);

protected:
	virtual void ImportFromControl(TControl *control);
};

// ----- TJudicialName -----------------------------------------------------
class TJudicialName : public TName
{
public:
	TJudicialName(const char *name, char *data, size_t size);

protected:
	virtual void ImportFromControl(TControl *control);
};

// ----- TText ---------------------------------------------------------------
class TText : public TBaseString
{
public:
	TText(const char *name, mstr *data, size_t size);

	virtual const char *S() const;
	virtual void SetS(const char *s);

protected:
	virtual void ImportFromQuery(const pFLDDesc desc, const TQuery *query);
	virtual void ImportFromControl(TControl *control);

	mstr *const Data;
};

// ----- TShort --------------------------------------------------------------
class TShort : public TFixedField
{
public:
	TShort(const char *name, short *data);

	short H() const { return (*(short *) Data); }
	void SetH(short h) { SetData(&h); }
	virtual void Print(mstr &m) const;

protected:
	virtual void ExportToControl(TControl *control) const;
	virtual void ImportFromControl(TControl *control);
};

// ----- TLong ---------------------------------------------------------------
class TLong : public TFixedField
{
public:
	TLong(const char *name, long *data);

	long L() const { return (*(long *) Data); }
	void SetL(long l) { SetData(&l); }
	virtual void Print(mstr &m) const;

protected:
	virtual void ExportToControl(TControl *control) const;
	virtual void ImportFromControl(TControl *control);
};

// ----- TZLong --------------------------------------------------------------
class TZLong : public TLong
{
public:
	TZLong(const char *name, long *data) : TLong(name, data) { }

	virtual void Print(mstr &m) const { m.printf("%d", L()); }
	virtual bool IsNULL() const { return false; }
};

// ----- TCheck --------------------------------------------------------------
class TCheck : public TLong
{
public:
	TCheck(const char *name, long *data, long tMask);

protected:
	virtual void ExportToControl(TControl *control) const;
	virtual void ImportFromControl(TControl *control);

	unsigned mask;
};

// ----- TQuick --------------------------------------------------------------
class TQuick : public TLong
{
public:
	TQuick(const char *name, long *data);

protected:
	virtual void ExportToControl(TControl *control) const;
	virtual void ImportFromControl(TControl *control);
};

// ----- TRadio --------------------------------------------------------------
class TRadio : public TLong
{
public:
	TRadio(const char *name, long *data, long tValue);

protected:
	virtual void ExportToControl(TControl *control) const;
	virtual void ImportFromControl(TControl *control);

	long value;
};

// ----- TDouble ------------------------------------------------------------
class TDouble : public TFixedField
{
public:
	TDouble(const char *name, double *data);

	double D() const { return (*(double *) Data); }
	void SetD(double d) { SetData(&d); }
	virtual void Print(mstr &m) const;	// fatal
};

// ----- TMoney -------------------------------------------------------------
class TMoney : public TDouble
{
public:
	TMoney(const char *name, double *data);

	double M() const { return (*(double *) Data); }
	void SetM(double m) { SetData(&m); }
	virtual void Print(mstr &m) const;

protected:
	virtual void ExportToControl(TControl *control) const;
	virtual void ImportFromControl(TControl *control);

private:
	double D() const { return TDouble::D(); }
	void SetD(double d) { TDouble::SetD(d); }
};

// ----- TCoord -------------------------------------------------------------
class TCoord : public TDouble		// 2016:322 FIX: Print() for Update()
{
public:
	TCoord(const char *name, double *data);

	virtual void Print(mstr &m) const;
};

// ----- TChar ---------------------------------------------------------------
class TChar : public TFixedField
{
public:
	TChar(const char *name, char *data);

	char C() const { return (*(char *) Data); }
	void SetC(char c) { SetData(&c); }
	virtual void Print(mstr &m) const;

protected:
	virtual void ExportToControl(TControl *control) const;
	virtual void ImportFromQuery(const pFLDDesc desc, const TQuery *query);
	virtual void ImportFromControl(TControl *control);
};

// ----- TStampBase ----------------------------------------------------------
class TStampBase : public TFixedField
{
public:
	TStampBase(const char *name, CDateTime *data);

	const CDate &Date() const { return *(const CDate *) Data; }
	const CTime &Time() const { return *(const CTime *) (Data + sizeof(CDate)); }
	void SetDate(const CDate &date) { memcpy(Data, &date, sizeof(CDate)); }
	void SetTime(const CTime &time) { memcpy(Data + sizeof(CDate), &time, sizeof(CTime)); }
	virtual void Print(mstr &m) const;

protected:
	CDateTime &Stamp() const { return *(CDateTime *) Data; }
	virtual void ImportFromQuery(const pFLDDesc desc, const TQuery *query) = 0;
};

// ----- TTimeStamp ----------------------------------------------------------
class TTimeStamp : public TStampBase
{
public:
	TTimeStamp(const char *name, CDateTime *data);

protected:
	virtual void ImportFromQuery(const pFLDDesc desc, const TQuery *query);
};

// ----- TDate ---------------------------------------------------------------
class TDate : public TFixedField
{
public:
	TDate(const char *name, CDate *data, bool today);

	const CDate &Date() const { return *(const CDate *) Data; }
	void SetDate(const CDate &date) { SetData(&date); }
	virtual void Print(mstr &m) const;
	virtual void Quote(mstr &m) const;

protected:
	virtual void ExportToControl(TControl *control) const;
	virtual void ImportFromQuery(const pFLDDesc desc, const TQuery *query);
	virtual void ImportFromControl(TControl *control);
};

// ----- TTime ---------------------------------------------------------------
class TTime : public TFixedField
{
public:
	TTime(const char *name, CTime *time);

	const CTime &Time() const { return (*(const CTime *) Data); }
	void SetTime(const CTime &time) { SetData(&time); }
	virtual void Print(mstr &m) const;
	virtual void Quote(mstr &m) const;

protected:
	virtual void ExportToControl(TControl *control) const;
	virtual void ImportFromQuery(const pFLDDesc desc, const TQuery *query);
	virtual void ImportFromControl(TControl *control);
};

// ----- TDateTime -----------------------------------------------------------
class TDateTime : public TStampBase
{
public:
	TDateTime(const char *name, CDateTime *data);

	virtual void Quote(mstr &m) const;

protected:
	virtual void ImportFromQuery(const pFLDDesc desc, const TQuery *query);
};

// ----- TCharAlias ----------------------------------------------------------
class TCharAlias : public TChar
{
public:
	TCharAlias(const char *name, char *data);

protected:
	virtual void ExportToControl(TControl *control) const;
	virtual void ImportFromControl(TControl *control);
};

// ----- TListAlias ----------------------------------------------------------
class TListAlias : public TChar
{
public:
	TListAlias(const char *name, char *data);

protected:
	virtual void ExportToControl(TControl *control) const;
	virtual void ImportFromControl(TControl *control);
};

// ----- TStringAlias --------------------------------------------------------
class TStringAlias : public TString
{
public:
	TStringAlias(const char *name, char *data, size_t size);

protected:
	virtual void ExportToControl(TControl *control) const;
	virtual void ImportFromControl(TControl *control);
};

// ----- TPersonAlias --------------------------------------------------------
class TPersonAlias : public TStringAlias
{
public:
	TPersonAlias(const char *name, char *data, size_t size);

	virtual const char *S() const;	// etz
	virtual void SetS(const char *s);	// etz

protected:
	virtual void ExportToControl(TControl *control) const;	// etz
	virtual void ImportFromControl(TControl *control);	// etz
};

// ----- TLongAlias ----------------------------------------------------------
class TLongAlias : public TLong
{
public:
	TLongAlias(const char *name, long *data);

protected:
	virtual void ExportToControl(TControl *control) const;
	virtual void ImportFromControl(TControl *control);
};

// ----- TCharList -----------------------------------------------------------
class TCharList : public TString
{
public:
	TCharList(const char *name, char *data, size_t size);

protected:
	virtual void ExportToControl(TControl *control) const;
	virtual void ImportFromControl(TControl *control);
};

// ----- TCharArray ----------------------------------------------------------
class TCharArray : public TCharList
{
public:
	TCharArray(const char *name, char *data, size_t size);

	virtual void Print(mstr &m) const;

protected:
	virtual void ImportFromQuery(const pFLDDesc desc, const TQuery *query);
};

// ----- TTextList -----------------------------------------------------------
class TTextList : public TText
{
public:
	TTextList(const char *name, mstr *data, size_t size);

protected:
	virtual void ExportToControl(TControl *control) const;
	virtual void ImportFromControl(TControl *control);
};

// ----- TBinary -------------------------------------------------------------
class TMemBlock : public memblock_t		// mstr should be derived from this
{
public:
	TMemBlock() { mbk_init(this); }
	~TMemBlock() { mbk_clear(this); }
};

class TBLOB : public TField
{
public:
	TBLOB(const char *name, TMemBlock *data);

	virtual void Print(mstr &m) const;
	virtual bool IsNULL() const;
	virtual void Nullify();
	virtual TCACParameter *CreateParam() const;

protected:
	virtual void ExportToControl(TControl *) const;
	virtual void ImportFromQuery(const pFLDDesc desc, const TQuery *query);
	virtual void ImportFromField(const TField *field);
	virtual void ImportFromControl(TControl *);

	TMemBlock *Data;
};

// ----- TKeyWords -----------------------------------------------------------
class TKeyWords : public TString
{
public:
	TKeyWords(const char *name, char *data);

protected:	
	virtual void ExportToControl(TControl *control) const;
	virtual void ImportFromControl(TControl *control);
};

// ----- TGroup --------------------------------------------------------------
class msql;

class TGroup : public TTable, protected TIArray<TField>
{
public:
	TGroup(const char *name, TGroup *(*const tClone)());
	virtual ~TGroup();

	TField *Find(const char *name) const;
	TIArray<FLDDesc> *Parse(const char *fields) const;
	void Clear(TIArray<FLDDesc> *fields) const;
	void Quote(mstr &m, const char *fields);

	virtual bool Delete(bool exact);
	virtual void ExactCriteria(msql &m, const char *fields = NULL);
	virtual void FloatCriteria(msql &m, const char *fields = NULL);
	void FloatCriteria(char *s, const char *fields = NULL) { GroupCriteria(s, fields, false); }	// for compatibility
	bool Get(const char *fields = NULL);
	bool Get(const msql &m);
	virtual bool Insert();
	virtual bool Try(const char *fields = NULL);
	virtual void Print(mstr &m);
	virtual bool Update(const char *fields = NULL);
	void ImportFields(const TGroup *group, const char *fields);  // 2017:048

#if RANGECHECK
	void Add(TField *field);	// check for duplicates
#else  // RANGECHECK
	void Add(TField *field) { TIArray<TField>::Add(field); }
#endif  // RANGECHECK
	void Add(TField *field1, TField *field2, ...);
	void Remove(const char *name);
	void Rename(const char *oldName, const char *newName);	// 2011:102

	// 2009:153 LPR: new electric and ucn selector
	void AddExtraText(const char *s);
	TString *FindExtraText() const { return (TString *) Find("F_EXTRA_TEXT"); }
	void DeleteExtraText();
	// 2011:081 TObject -> TGroup; should be avoided
	void SetName(const char *name) { ((const char *) Name) = name; }
	bool Save(bool edit) { return edit ? Update() : Insert(); }

	void operator>>(TWindow *window) const { ExportToWindow(window); }
	void operator>>(TWindow &window) const { ExportToWindow(&window); }

	void operator<<(const TQuery *query) { ImportFromQuery(query); }
	void operator<<(const TQuery &query) { ImportFromQuery(&query); }
	void operator<<(const TGroup *group) { ImportFromGroup(group); }
	void operator<<(const TGroup &group) { ImportFromGroup(&group); }
	void operator<<(const TWindow *window) { ImportFromWindow(window); }
	void operator<<(const TWindow &window) { ImportFromWindow(&window); }

	TGroup *(*const clone)();
	void LoadArray(TIArray<TGroup> *array, const char *criteria);
	void LoadArray(TIArray<TGroup> &array, const char *criteria) { LoadArray(&array, criteria); }
	void LoadFloat(TIArray<TGroup> *array, const char *fields = NULL);
	void LoadFloat(TIArray<TGroup> &array, const char *fields = NULL) { LoadFloat(&array, fields); }

protected:
	TIArray<TCACParameter> params;
	void QuoteField(mstr &m, const TField *field);

	void ExportToQuery(TQuery *query) const;
	void ExportToGroup(TGroup *group) const;
	void ExportToWindow(TWindow *window) const;

	void ImportFromQuery(const TQuery *query);
	void ImportFromGroup(const TGroup *group);
	void ImportFromWindow(const TWindow *window);

	// _
	void ClearGroupData();
	friend void clearGroupData(TGroup &group);

private:
	TGroup(const TGroup &that);

	static TGroup *Clone();
	void GroupCriteria(char *s, const char *fields, bool exact);
};

// ----- TDummyGroup ---------------------------------------------------------
class TDummyGroup : public TGroup
{
public:
	TDummyGroup() : TGroup("T_GROUP", NULL) { }
};

// ----- TGroupArray ---------------------------------------------------------
class TGroupArray : public TIArray<TGroup>
{
public:
	TGroupArray() { }

	int AddCopy(const TGroup *group);
	int AddCopy(const TGroup &group) { return AddCopy(&group); }
	void InsertCopy(const TGroup *group, int index);
	void DeleteExtraText();
};

// ----- TGArray -------------------------------------------------------------
template <class T> class TGArray : public TGroupArray
{
public:
	TGArray() { }

	T *operator[](int index) const { return (T *) ItemAt(index); }
	int Add(const T *t) { return TGroupArray::Add(t); }
	T *Detach(int index) { return (T *) TGroupArray::Detach(index); }
	void Insert(const T *t, int index) { TGroupArray::Insert(t, index); }
};

// ----- TGSArray ------------------------------------------------------------
template <class T> class TGSArray : public TGroupArray
{
public:
	TGSArray() { lesseql = LessEqlT; }
	TGSArray(bool (*tLessEqlT)(const long t1, const long t2)) { lesseql = tLessEqlT; }

	T *operator[](int index) const { return (T *) ItemAt(index); }
	int Add(const T *t) { return TGroupArray::Add(t); }
	bool Contains(const T *t) const { return TGroupArray::Contains(t); }
	T *Detach(int index) { return (T *) TGroupArray::Detach(index); }
	int Find(const T *t) const { return TGroupArray::Find(t); }

protected:
	static bool LessEqlT(const long t1, const long t2) { return *(T *) t1 <= *(T *) t2; }
};
