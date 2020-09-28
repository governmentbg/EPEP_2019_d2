// ----- TRCDKey -------------------------------------------------------------
#define NEW_RCDKEY(prefix_, key) \
	new TLong(prefix_"NO", &(key)->no), \
	new TLong(prefix_"YEAR", &(key)->year), \
	new TCharAlias(prefix_"TYPE", &(key)->type)

struct TRCDKey
{
	long no;
	long year;
	char type;

	int Compare(const TRCDKey &that) const;	// type, year, no

	bool operator==(const TRCDKey &that) const { return !Compare(that); }
	bool operator!=(const TRCDKey &that) const { return Compare(that); }
	bool operator>(const TRCDKey &that) const { return Compare(that) > 0; }
	bool operator>=(const TRCDKey &that) const { return Compare(that) >= 0; }
	bool operator<(const TRCDKey &that) const { return Compare(that) < 0; }
	bool operator<=(const TRCDKey &that) const { return Compare(that) <= 0; }
};

// ----- TRCDKeyGroup --------------------------------------------------------
class TMovement;
class TDatedContainer;

class TRCDKeyGroup : public TGroup
{
public:
	TRCDKeyGroup(const char *name, TGroup *(*const tClone)());

	virtual void ExactCriteria(msql &m, const char *fields = NULL);
	virtual void FloatCriteria(msql &m, const char *fields = NULL);
	void FloatCriteria(char *s, const char *fields = NULL) { TGroup::FloatCriteria(s, fields); }

	void LastKey(msql &m, TDatedContainer *target, const char *fields, const char *kinds = NULL) const;
	TMovement *LastMove(const CDate *date = NULL, const CTime *time = NULL) const;
	TMovement *LastMove(const CDate &date, const CTime &time) const { return (LastMove(&date, &time)); }
	virtual void Print(mstr &m);
	virtual char Status() const;	// 2009:146 LPR: full generic implementation
	virtual bool Generate();

	TRCDKey key;

protected:
	char antiNoRight;		// 2007:208 LRQ: +related: anti# hack
};

// ----- TUCNGroup -----------------------------------------------------------
class TUCNGroup : public TGroup
{
public:
	TUCNGroup(const char *name, TGroup *(*const tClone)());

	virtual void ExactCriteria(msql &m, const char *fields = NULL);
	virtual void FloatCriteria(msql &m, const char *fields = NULL);
	void FloatCriteria(char *s, const char *fields = NULL) { TGroup::FloatCriteria(s, fields); }
	virtual void Print(mstr &m);
	bool FindKey(const char *name);

	virtual void Generate();
	virtual bool Insert();	// <ucnType> [ucn] create a new group

	char ucnType;
	char ucn[SIZE_OF_UCN];
};

// ----- TFoundGroup ---------------------------------------------------------
class TFoundGroup : public TUCNGroup
{
public:
	// 2003:302 BRW: ucn group, not generic one
	TFoundGroup();
	TFoundGroup(const char *name, TGroup *(*const tClone)());
};

// ----- TNamedGroup ---------------------------------------------------------
class TNamedGroup : public TFoundGroup	// 2008:017 TUCNGroup -> TFoundGroup
{
public:
	TNamedGroup(TName *tNameField);
	TNamedGroup(const char *name, TGroup *(*const tClone)(), TName *tNameField);	// xCrit = ucnType

	virtual void FloatCriteria(msql &m, const char *fields = NULL);
	void FloatCriteria(char *s, const char *fields = NULL) { TGroup::FloatCriteria(s, fields); }
	virtual void NamedCriteria(msql &m, const char *fields, const char *extra, char involvement, bool active) = 0;
	virtual bool ValidCriteria() = 0;

	static TNamedGroup *NewXGroup(char uxnType);	// former UXNType2UCNGroup
	static TNamedGroup *NewGroup(char ucnType);	// former UCNType2UCNGroup

	TName *nameField;

protected:
	// 2010:279 LPR: cat known person check to s
	void KnownCriteria(msql &m, char involvement, bool active);
};

// ----- TTransGroup ---------------------------------------------------------
class TTransGroup : public TNamedGroup
{
public:
	// 2004:163 LPR: dummy holded for ucn searches
	TTransGroup();

	virtual void NamedCriteria(msql &, const char *, const char *, char, bool);
	virtual bool ValidCriteria();

	char name[SIZE_OF_DAMN_NAME];
	char family[SIZE_OF_CITIZEN_FAMILY];
	char reName[SIZE_OF_CITIZEN_RENAME];
	char reFamily[SIZE_OF_CITIZEN_REFAMILY];
	char involvement;
	char fromSide;
	char bulstat[SIZE_OF_BULSTAT];
	char country[SIZE_OF_COUNTRY];
};

// ----- TPeriodGroup --------------------------------------------------------
class TPeriodGroup : public TGroup
{
public:
	TPeriodGroup(const char *name);

	CDate minDate;
	CDate maxDate;

	void AddRange(msql &m) { m.AddRange("F_DATE", minDate, maxDate); }
};

// ----- TGetGroup -----------------------------------------------------------
class TGetGroup : public TDummyGroup
{
public:
	TGetGroup(const char *expr, TField *field);
};
