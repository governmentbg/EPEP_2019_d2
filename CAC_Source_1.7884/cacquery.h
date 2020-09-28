// ----- TCACQuery -----------------------------------------------------------
class TCACQuery : public TVObject
{
public:
	TCACQuery(const char *expr);
	virtual ~TCACQuery() = 0;

	virtual bool Read() const = 0;
	virtual bool GetField(const pFLDDesc desc, pBYTE data) const = 0;
	virtual bool GetText(const pFLDDesc desc, mstr *m, bool zero) const = 0;
	virtual bool GetStamp(const pFLDDesc desc, ISetDateTime &stamp) const = 0;
	virtual bool GetBLOB(const pFLDDesc desc, memblock_t *m) const = 0;
	virtual void Restart() = 0;
	virtual unsigned _Count() = 0;	// for _

	pFLDDesc Find(const char *name) const;

protected:
	pFLDDesc descs;	// ODBC: iFldNum szName iFldType iOffset iLen
	int count;
	pBYTE buffer;

	// 2014:216 LPR: for restart
	virtual void Open() = 0;
	virtual void Close() = 0;
};

// ----- TODBCQuery ----------------------------------------------------------
class TODBCQuery : public TCACQuery
{
public:
	TODBCQuery(const char *expr, TODBCDataBase *tDataBase);
	virtual ~TODBCQuery();

	virtual bool Read() const;
	virtual bool GetField(const pFLDDesc desc, pBYTE data) const;
	virtual bool GetText(const pFLDDesc desc, mstr *m, bool zero) const;
	virtual bool GetStamp(const pFLDDesc desc, ISetDateTime &stamp) const;
	virtual bool GetBLOB(const pFLDDesc desc, memblock_t *m) const;
	virtual void Restart();

protected:
	TODBCDataBase *const dataBase;
	SQLHSTMT hstmt;
	SQLLEN *states;

	bool GetNull(const pFLDDesc desc, pBYTE data) const;

	virtual void Open();
	virtual void Close();
	virtual unsigned _Count();
};

#ifdef __BORLANDC__
// ----- TBDEQuery -----------------------------------------------------------
class TBDEQuery : public TCACQuery
{
public:
	TBDEQuery(const char *expr, TBDEDataBase *tDataBase);
	virtual ~TBDEQuery();

	virtual bool Read() const;
	virtual bool GetField(const pFLDDesc desc, pBYTE data) const;
	virtual bool GetText(const pFLDDesc desc, mstr *m, bool zero) const;
	virtual bool GetStamp(const pFLDDesc desc, ISetDateTime &stamp) const;
	virtual bool GetBLOB(const pFLDDesc desc, memblock_t *m) const;
	virtual void Restart();

protected:
	TBDEDataBase *const dataBase;
	hDBIStmt statement;
	hDBICur cursor;

	virtual void Open();
	virtual void Close();
	virtual unsigned _Count();
};
#endif  // __BORLANDC__

// ----- TQuery --------------------------------------------------------------
class TQuery : public TObject
{
public:
	TQuery(const char *expr, TCACDataBase *dataBase = NULL);
	~TQuery();

	bool Read() const;
	bool GetField(const pFLDDesc desc, pBYTE data) const;
	bool GetText(const pFLDDesc desc, mstr *m, bool zero) const;
	virtual bool GetStamp(const pFLDDesc desc, ISetDateTime &stamp) const;
	bool GetBLOB(const pFLDDesc desc, memblock_t *m) const;
	void Restart();

	pFLDDesc Find(const char *name) const { return query->Find(name); }

protected:
	TCACQuery *query;

	unsigned _Count();
};
