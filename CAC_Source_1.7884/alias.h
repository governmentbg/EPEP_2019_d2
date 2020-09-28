// ----- CACHE_ --------------------------------------------------------------
enum
{
	CACHE_USERS,
	CACHE_HALLS,
	CACHE_COUNTRIES,
	CACHE_COURTS,
	CACHE_JAIL_MODE,
	CACHE_CRIME_FORM,
	CACHE_CRIME_STAGE,
	CACHE_RECIDIVE,
	CACHE_GOPS,
	CACHE_SUM_TYPE,
	CACHE_SUM_STATE,
	CACHE_JUDGED,
	CACHE_PUNISHMENT,
	CACHE_GRANT,
	CACHE_AGE,
	CACHE_MONTH,
	CACHE_INTERVAL,
	CACHE_ACCOUNT_TYPE,
	CACHE_WEEKDAY,
	CACHE_SECTION,
	CACHE_APPEAL_RESOLUTION,
	CACHE_COMPANY_TYPE,
	CACHE_RECEIVED_STATUS,
	CACHE_VSS_INDEX,
	CACHE_RESULT,
	CACHE_CAL_INTERVAL,
	CACHE_POST,
	CACHE_RANK,
	CACHE_RIGHT,
	CACHE_ACCOMPLY,
	CACHE_UXN_TYPE,
	CACHE_UCN_TYPE,
	CACHE_INVOLVEMENT,
	CACHE_CONNECT_TYPE,
	CACHE_CONNECT_KIND,
	CACHE_SUBPOENA_KIND,
	CACHE_RESOLUTION,
	CACHE_KIND,
	CACHE_PENALITY_TYPES,
	CACHE_REASON_TYPES,
	CACHE_TYPE,
	CACHE_COLLEGE,
	CACHE_COLLEGE_RIGHT,
	CACHE_COMPOSITION,
	CACHE_CHAR_COMPOSITION,
	CACHE_ARTICLES,
	CACHE_DANGERS,
	CACHE_AREA,
	CACHE_REGION,
	CACHE_DANGER_SUBJECT,
	CACHE_ARTICLE_SUBJECT,
	CACHE_DEBTOR_STATUS,
	CACHE_BANCONSIST,
	CACHE_EISPP_DEPART,	// on-demand
	CACHE_EISPP_CRIME,	// on-demand
	CACHE_XFER_KIND,
	CACHE_EISPP_TRANSL,
	CACHE_EISPP_VAL,		// on-demand
	CACHE_LEGAL_AID_TYPE,
	CACHE_ARCHIVE_INDEX,
	CACHE_ARCHIVE_SUBJECT,
	CACHE_KIND_ALIAS,
	CACHE_CURRENCY,
	CACHE_SLC_TYPE,
	CACHE_VSS_INDEXES,
	CACHE_WHATS,
	CACHE_COUNT
};

// ----- TAliasGroup ---------------------------------------------------------
class TGroupList;

class TAliasGroup : public TGroup
{
public:
	TAliasGroup(const char *name, TGroup *(*tClone)(), TField *tValueField, const char *nameFieldName,
		char *nameFieldData, int nameFieldSize, TCharAlias *tXField, TLong *tFlagsField, bool ordered);
	TAliasGroup(const char *name, TGroup *(*tClone)(), TField *tValueField, TString *tNameField, TCharAlias *tXField,
		TLong *tFlagsField, bool ordered);

	virtual bool Try(const char *fields = NULL);	// Fixup(true)

	virtual void AliasCriteria(msql &m, const char *xCrit, long flags);
	void LoadData(TGroupArray *array, const char *xCrit, long flags);
	bool LoadCache(TGroupList *glist, const char *xCrit, long flags);
	void ReloadCache();

	virtual void Fixup(bool force);
	virtual void Print(mstr &m);		// Fixup(false), nameField->S()

	TString *const &NameField;
	TLong *OrderField() { return orderField; }

	static void InitializeCache();
	static void ShutdownCache();
	static void InitializeBase();
	static void ShutdownBase();
	static void InitializeConst();
	static void ShutdownConst();
	static void InitializeNoms();
	static void ShutdownNoms();

	virtual void ExactCriteria(msql &m, const char *fields = NULL);

protected:
	TField *valueField;
	TString *nameField;
	TCharAlias *xField;
	TLong *flagsField;
	TLong *orderField;

	long order;

	virtual void FloatCriteria(msql &m, const char *fields = NULL);	// unsorted
	void FloatCriteria(char *s, const char *fields = NULL) { TGroup::FloatCriteria(s, fields); }

	virtual bool Match(const char *xCrit, long flags);
	const char *Seek();

	TGArray<TAliasGroup> *cache;
	static TGArray<TAliasGroup> *Cache;
	//static DGA(Cache);
	const char *Found(TAliasGroup *alias);

	virtual void InitCache();
	virtual void CacheCriteria(msql &m);
	void CacheInsert(TGroupList *glist, TGroup *group);
	void CacheCollect(TGroupList *glist, TGroupArray *array, TAliasGroup *alias);
	virtual bool CacheFill(TGroupList *glist, TGroupArray *array, const char *xCrit, long flags);

private:
	void Init(bool ordered);
};

// ----- TStringAliasGroup ---------------------------------------------------
class TStringAliasGroup : public TAliasGroup
{
public:
	TStringAliasGroup(const char *name, TGroup *(*tClone)(), const char *valueFieldName, char *valueFieldData,
		int valueFieldSize, const char *nameFieldName, char *nameFieldData, int nameFieldSize, TCharAlias *xField,
		TLong *flagsField, bool ordered = false);

	TStringAlias *ValueField() { return (TStringAlias *) valueField; }
	const char *Seek(const char *s);
};

// ----- TPersonAliasGroup ---------------------------------------------------
class TPersonAliasGroup : public TStringAliasGroup
{
public:
	TPersonAliasGroup(const char *name, TGroup *(*tClone)(), TCharAlias *xField);

	virtual void Fixup(bool force);

	char ucn[SIZE_OF_UCN];
	long flags;

	bool operator<=(const TPersonAliasGroup &that) { return strcmp(uname, that.uname) <= 0; }

protected:
	char uname[SIZE_OF_USER_NAME];	// 2012:081 _NAME -> _USER_NAME
};

// ----- TUserAliasGroup -----------------------------------------------------
class TUserAliasGroup : public TPersonAliasGroup
{
public:
	TUserAliasGroup();

	static TGroup *Clone();

	bool Serves(long uclp, long street, const char *adrNo, const char *blockNo);	// 2009:356

	char post;
	char lawKinds[SIZE_OF_LAW_KINDS];
	char colleges[SIZE_OF_COLLEGES];
	char compositions[SIZE_OF_COMPOSITIONS];	// 2015:300 from TUser, for intersect
	char functions[SIZE_OF_FUNCTIONS];		// 2018-11-16: from TUser, for EPorUserAliasFace

protected:
	virtual bool CacheFill(TGroupList *glist, TGroupArray *array, const char *xCrit, long flags);
	virtual bool Match(const char *xCrit, long flags);	// USER_LIMIT

	virtual void CacheCriteria(msql &m);
	virtual void ExactCriteria(msql &m, const char *fields = NULL);	// F_UCN, F_FLAGS, F_POST, F_LAW_KINDS, F_COLLEGES
	virtual void FloatCriteria(msql &m, const char *fields = NULL);	// F_UCN, F_FLAGS, F_POST, F_LAW_KINDS, F_COLLEGES
	void FloatCriteria(char *s, const char *fields = NULL) { TGroup::FloatCriteria(s, fields); }
};

extern TUserAliasGroup *UserAliasGroup;

// ----- TKnownAliasGroup ----------------------------------------------------
class TKnownAliasGroup : public TPersonAliasGroup
{
public:
	TKnownAliasGroup(TGroup *(*const tClone)() = TKnownAliasGroup::Clone);
	static TGroup *Clone();

	virtual void ExactCriteria(msql &m, const char *fields = NULL);	// ucnType ::= citizen
	void PrintPrefixed(mstr &m);

	char involvement;
	char ucnType;

protected:
	virtual bool Match(const char *xCrit, long flags);	// ucnType ::= citizen
};

extern TKnownAliasGroup *KnownAliasGroup;

// ----- TLongAliasGroup -----------------------------------------------------
class TLongAliasGroup : public TAliasGroup
{
public:
	TLongAliasGroup(const char *name, TGroup *(*tClone)(), const char *valueFieldName, long *valueFieldData,
		const char *nameFieldName, char *nameFieldData, int nameFieldSize, TCharAlias *xField, TLong *flagsField,
		bool ordered = false);
	TLongAliasGroup(const char *name, TGroup *(*tClone)(), const char *valueFieldName, long *valueFieldData,
		TString *nameField, TCharAlias *xField, TLong *flagsField, bool ordered = false);

	long genLimit;	// auto generate limit

	virtual bool Insert();	// auto generate

	TLongAlias *ValueField() { return (TLongAlias *) valueField; }
	const char *Seek(long l);
};

// ----- TCharAliasGroup -----------------------------------------------------
class TCharAliasGroup : public TAliasGroup
{
public:
	TCharAliasGroup(const char *name, TGroup *(*tClone)(), const char *valueFieldName, char *valueFieldData,
		const char *nameFieldName, char *nameFieldData, int nameFieldSize, bool ordered = true);

	TCharAlias *ValueField() { return xField; }
	const char *Seek(char c);
};

// ----- TXferAliasGroup -----------------------------------------------------
class TXferAliasGroup : public TCharAliasGroup
{
public:
	TXferAliasGroup(const char *name, TGroup *(*tClone)(), const char *valueFieldName, char *valueFieldData,
		const char *nameFieldName, char *nameFieldData, int nameFieldSize, bool ordered = true);

	long Xfer(char c);	// seek and return xfer

	long xfer;
};

// ----- TLongAliasArray -----------------------------------------------------
class TLongAliasArray : public TGroupArray
{
public:
	TLongAliasArray() { lesseql = LessEqlT; }

	TLongAliasGroup *operator[](int index) const { return (TLongAliasGroup *) ItemAt(index); }
	int Add(const TLongAliasGroup *t) { return TGroupArray::Add(t); }
	bool Contains(const TLongAliasGroup *t) const { return TGroupArray::Contains(t); }
	int Find(const TLongAliasGroup *t) const { return TGroupArray::Find(t); }

protected:
	static bool LessEqlT(const long t1, const long t2);	// ValueField()->L()
};

// ----- TCharAliasArray -----------------------------------------------------
class TCharAliasArray : public TGroupArray
{
public:
	TCharAliasArray() { lesseql = LessEqlT; }

	TCharAliasGroup *operator[](int index) const { return (TCharAliasGroup *) ItemAt(index); }
	int Add(const TCharAliasGroup *t) { return TGroupArray::Add(t); }
	bool Contains(const TCharAliasGroup *t) const { return TGroupArray::Contains(t); }
	int Find(const TCharAliasGroup *t) const { return TGroupArray::Find(t); }

protected:
	static bool LessEqlT(const long t1, const long t2);	// OrderField()->L()
};
