// ----- ET_ -----------------------------------------------------------------
enum
{
	ET_SILENT,
	ET_ERROR,
	ET_FATAL,
	ET_READ,
	ET_DELETE,
	ET_WRITE,
	ET_DRIVER,
	ET_COUNT
};

// ----- TCACParameter -------------------------------------------------------
struct TCACParameter
{
	TCACParameter(void *tData, size_t tSize) : data(tData), size(tSize) { }

	void *data;
	SQLLEN size;	// real max = 2GB-1
};

// ----- ISetDateTime --------------------------------------------------------
class ISetDateTime
{
public:
	virtual void set(int year, int month, int day, int hour, int minute, int second) = 0;
};

// ----- FLAG_ ---------------------------------------------------------------
enum { FLAG_ACTIVE = 0x01 };

// ----- TDataBase -----------------------------------------------------------
class TCACQuery;

class TCACDataBase : public TVObject
{
public:
	TCACDataBase(const char *name);
	virtual ~TCACDataBase() = 0;

	virtual const char *Protocol() const = 0;
	virtual const char *BaseType() const = 0;

	virtual bool Connect(const char *userName, const char *password, int errorType) = 0;
	virtual bool DefaultConnect(int errorType) = 0;
	virtual void Disconnect() = 0;
	virtual bool GetDescs(const char *name, TIArray<FLDDesc> *descs) = 0;  // true == primary key found

	virtual bool Execute(const char *stmt, int errorType, const char *name = NULL, const char *oper = NULL,
		const TIArray<TCACParameter> *params = NULL) = 0;
	virtual int ExecCount(const char *stmt, int errorType, const char *name = NULL, const char *oper = NULL,
		const TIArray<TCACParameter> *params = NULL) = 0;
	bool ExecProc(mstr &m, int errorType, const char *name = NULL, const char *oper = NULL,
		const TIArray<TCACParameter> *params = NULL);

	virtual TCACQuery *NewQuery(const char *expr) = 0;
	virtual void CondActive(mstr &m, const char *field) const = 0;

	long GetLong(const char *expr, const char *field);
	bool GetString(const char *expr, const char *field, char *value);
	long GetNext(const char *generator, const char *field);
	void GetNow(ISetDateTime &stamp);

	static TCACDataBase *DefaultDB;

	struct TRegularError
	{
		int code;
		const char *format;
	};

protected:
	virtual const char *GetNextFormat() const = 0;
	virtual const char *GetNowFormat() const = 0;
	virtual const char *GetExecPrefix() const = 0;

	static char *MaxDisplay(const char *name);
	static const char *WrongField(const char *name, const char *marker, char *message);
	static void RegularError(const TRegularError *regerr, const char *name, const char *oper, const char *marker,
		char *errorText);
};

// ----- TODBCDataBase -------------------------------------------------------
#define FIREBIRD_ODBC_CURSOR_CACHE 16

class TODBCDataBase : public TCACDataBase
{
public:
	struct TODBCError
	{
		SQLCHAR sqlState[SQL_SQLSTATE_SIZE + 1];
		SQLINTEGER Code;
		SQLCHAR Msg[SQL_MAX_MESSAGE_LENGTH + 1];
	};

	TODBCDataBase(const char *name, bool (*tRegularError)(const char *, const char *, TODBCError &, int));
	virtual ~TODBCDataBase();

	virtual bool Connect(const char *userName, const char *password, int errorType);
	virtual void Disconnect();
	virtual bool GetDescs(const char *table, TIArray<FLDDesc> *descs);

	virtual bool Execute(const char *stmt, int errorType, const char *name = NULL, const char *oper = NULL,
		const TIArray<TCACParameter> *params = NULL);
	virtual int ExecCount(const char *stmt, int errorType, const char *name = NULL, const char *oper = NULL,
		const TIArray<TCACParameter> *params = NULL);

	virtual TCACQuery *NewQuery(const char *expr);

	bool CheckRC(const char *name, const char *oper, SQLHSTMT hstmt, SQLRETURN result, int errorType)
		{ return CheckRC(name, oper, SQL_HANDLE_STMT, hstmt, result, errorType); }
#if FIREBIRD_ODBC_CURSOR_CACHE
	virtual SQLHSTMT AllocStmt();
	virtual void FreeStmt(SQLHSTMT hstmt);
#else  // FIREBIRD_ODBC_CURSOR_CACHE
	SQLHSTMT AllocStmt();
	void FreeStmt(SQLHSTMT hstmt);
#endif  // FIREBIRD_ODBC_CURSOR_CACHE
	void BindColumn(SQLHSTMT hstmt, SQLUSMALLINT column, SQLSMALLINT type, SQLPOINTER data, SQLINTEGER size,
		SQLLEN *state = NULL);
	static void AssignDesc(pFLDDesc desc, SQLSMALLINT type, SQLUINTEGER size);

	static void Initialize(bool tAutoDown);
	static void Shutdown();
	static void SetSystemDSN();
	static bool CheckInst(const char *name, const char *oper, BOOL result);  // name may be NULL

protected:
	void BindParams(SQLHSTMT hstmt, const TIArray<TCACParameter> *params);

	SQLHDBC hdbc;
	bool connected;

	static int FindDriverIndex(constant *driverNames);

	static SQLHENV henv;

	bool (*const RegularError)(const char *name, const char *oper, TODBCError &odbcError, int errorType);
	bool CheckRC(const char *name, const char *oper, SQLSMALLINT type, SQLHANDLE handle, SQLRETURN result, int errorType)
		{ return CheckRC(name, oper, type, handle, result, errorType, RegularError); }

	static bool NotARegularError(const char *, const char *, TODBCError &, int);
	static bool CheckRC(const char *name, const char *oper, SQLSMALLINT type, SQLHANDLE handle, SQLRETURN result,
		int errorType, bool (*regularError)(const char *, const char *, TODBCError &, int));

private:
	static void (*Initializers[])();
	static int use_count;
	static bool autoDown;
};

// ----- TPostGresDataBase ------------------------------------------------------
class TPostGresDataBase : public TODBCDataBase
{
public:
	TPostGresDataBase(const char *name);

	virtual const char *Protocol() const;
	virtual const char *BaseType() const;
	virtual bool DefaultConnect(int errorType);
	virtual void CondActive(mstr &m, const char *field) const;

	static void Initialize();
	static bool EDBDriver() { return driverIndex == 0; }
	static const char *DriverName() { return DriverNames[driverIndex]; }

protected:
	virtual const char *GetNextFormat() const;
	virtual const char *GetNowFormat() const;
	virtual const char *GetExecPrefix() const;

	static bool PostGresRegularError(const char *name, const char *oper, TODBCError &odbcError, int errorType);

private:
	static int driverIndex;
	static constant DriverNames[];
};

// ----- TPostGresExtraDataBase -------------------------------------------------
class TPostGresExtraDataBase : public TPostGresDataBase
{
public:
	TPostGresExtraDataBase(const char *name);

	virtual bool Connect(const char *userName, const char *password, int errorType);
	virtual bool DefaultConnect(int errorType);

protected:
	static bool MissingDriverRegularError(const char *, const char *, TODBCError &odbcError, int errorType);
};

// ----- TFireBirdDataBase ---------------------------------------------------
class TFireBirdDataBase : public TODBCDataBase
{
public:
	TFireBirdDataBase(const char *name);

	virtual const char *Protocol() const;
	virtual const char *BaseType() const;
	virtual bool DefaultConnect(int errorType);
	virtual void CondActive(mstr &m, const char *field) const;

#if FIREBIRD_ODBC_CURSOR_CACHE
	virtual SQLHSTMT AllocStmt();
	virtual void FreeStmt(SQLHSTMT hstmt);
#endif  // FIREBIRD_ODBC_CURSOR_CACHE

	static void Initialize();
	static bool BCCDriver() { return driverIndex == 0; }
	static const char *DriverName() { return DriverNames[driverIndex]; }

protected:
	virtual const char *GetNextFormat() const;
	virtual const char *GetNowFormat() const;
	virtual const char *GetExecPrefix() const;

	static bool FireBirdRegularError(const char *name, const char *oper, TODBCError &odbcError, int errorType);

private:
	static int driverIndex;
	static constant DriverNames[];

#if FIREBIRD_ODBC_CURSOR_CACHE
	HSTMT handles[FIREBIRD_ODBC_CURSOR_CACHE];
	size_t used_handles;
#endif  // FIREBIRD_ODBC_CURSOR_CACHE
};

#ifdef __BORLANDC__
// ----- TBDEDataBase --------------------------------------------------------
class TBDEDataBase : public TCACDataBase
{
public:
	TBDEDataBase(const char *name);
	virtual ~TBDEDataBase();

	virtual const char *Protocol() const;
	virtual const char *BaseType() const;

	virtual bool Connect(const char *userName, const char *password, int errorType);
	virtual bool DefaultConnect(int errorType);
	virtual void Disconnect();
	virtual bool GetDescs(const char *table, TIArray<FLDDesc> *descs);

	virtual bool Execute(const char *stmt, int errorType, const char *name = NULL, const char *oper = NULL,
		const TIArray<TCACParameter> *params = NULL);
	virtual int ExecCount(const char *stmt, int errorType, const char *name = NULL, const char *oper = NULL,
		const TIArray<TCACParameter> *params = NULL);

	virtual TCACQuery *NewQuery(const char *expr);
	virtual void CondActive(mstr &m, const char *field) const;

	static bool CheckDI(const char *name, const char *oper, DBIResult result, int errorType);
	static void CheckDI(const char *name, const char *oper, DBIResult result) { CheckDI(name, oper, result, ET_FATAL); }

	void Prepare(const char *expr, phDBIStmt statement);
	static void CloseCursor(const char *name, phDBICur cursor)
		{ CheckDI(name, "DbiCloseCursor", DbiCloseCursor(cursor), ET_ERROR); }

	static void Initialize(const char *tempdir1, const char *tempdir2, bool tAutoDown);
	static void Shutdown();

protected:
	virtual const char *GetNextFormat() const;
	virtual const char *GetNowFormat() const;
	virtual const char *GetExecPrefix() const;

	hDBIDb handle;

private:
	static int use_count;
	static bool autoDown;
	static bool bde_init;
};
#endif  // __BORLANDC__
