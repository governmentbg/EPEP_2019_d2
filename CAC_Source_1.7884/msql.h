extern constant
	MQ_INSERT,
	MQ_UPDATE,
	MQ_DELETE,
	MQ_SUBSEQ;

class msql : public mstr
{
public:
	msql();
	msql(const char *name, const char *fields = NULL);	// Begin()

	void Add(const char *name);	// WHERE | AND [name]
	void Add(TField *field, const char *dbName = NULL);	// WHERE | AND dbName = value | IS NULL
	void Add(const TGroup *group, const char *fields);
	void AddAmPm(const char *name, const CTime &time, int dayType);	// nothing if whole day
	void AddArray(const char *name, const TGroupArray &array, const char *dbName = NULL);
	void AddChar(const char *name, char c);
	void AddChars(const char *name, const char *chars);
	void AddChars(const char *name, const char *chars, const char *alls);
	void AddCompos(const char *name, const char *compos);
	void AddDate(const char *name, const CDate &date);
	void AddExtra(const char *name, const char *text, const char *extra);
	void AddFlags(const char *name, long mask, long value, long all);
	void AddIsNull(const char *name, bool isNull);
	void AddJuSet(constant dbNames[], const TGroupArray &array, const char *grName, bool nonNull = false);
	void AddLike(const char *name, const char *text, bool full);	// false == text%
	void AddILike1(const char *name, const char *text, bool full);
	void AddLong(const char *name, long l);
	void AddLongs(const char *name, const TIntegerArray &array);
	void AddNnSet(constant dbNames[]);
	void AddNonElectron(const char *prefix);	// 2017:122
	void AddOrder(const char *order, const char *prefix = NULL);	// may contain DESC-s
	void AddPairs(const char *grName1, const char *grName2, const TGroupArray &array, const char *dbName1 = NULL,
		const char *dbName2 = NULL, const char *prefix = NULL);
	void AddQuick(const char *name, long quick);
	void AddRange(const char *name, long min, long max);
	void AddRange(const char *name, const CDate &min, const CDate &max);
	void AddRange(const char *name, const CTime &min, const CTime &max);
	void AddRange(const char *name, const CDateTime &min, const CDateTime &max);
	void AddString(const char *name, const char *s);
	void AddTriad(const char *dbName1, const char *dbName2, const char *dbName3, const TGroupArray &array,
		const char *grName, bool nonNull = false);
	void AddValue(TField *field);
	void AddWords(const char *name, const char *keyWords);
	void AddXferKinds(const char *name, const char *kinds);
	void AddZLong(const char *name, long l);
	// "SELECT fields | * FROM name"
	void Begin(const char *name, const char *fields = NULL);
	void CutOrder();	// for compatibility
	bool IsFirst() { return isFirst; }
	// next Add() does OR, not AND
	void NextOr() { nextAnd = false; }

protected:
	bool isFirst;
	bool nextAnd;
};
