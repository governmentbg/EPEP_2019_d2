#include "cac.h"

// ----- TCACDataBase --------------------------------------------------------
TCACDataBase::TCACDataBase(const char *name)
	: TVObject(name)
{
}

TCACDataBase::~TCACDataBase()
{
}

bool TCACDataBase::ExecProc(mstr &m, int errorType, const char *name, const char *oper, const TIArray<TCACParameter> *params)
{
	mbk_insert(&m, GetExecPrefix(), 0, strlen(GetExecPrefix()));
	return Execute(ncstr(m), errorType, name, oper, params);
}

long TCACDataBase::GetLong(const char *expr, const char *field)
{
	TCACQuery *query = NewQuery(expr, false);
	long value = 0;

	if (query->Read())
		query->GetField(query->Find(field), (pBYTE) &value);

	delete query;
	return value;
}

bool TCACDataBase::GetString(const char *expr, const char *field, char *value)
{
	TCACQuery *query = NewQuery(expr, false);
	bool result = query->Read() && query->GetField(query->Find(field), (pBYTE) value);

	delete query;
	return result;
}

long TCACDataBase::GetNext(const char *generator, const char *field)
{
	mstr m;

	m.printf(GetNextFormat(), generator, field);
	long value = GetLong(str(m), field);

	if (value <= 0)
		fatal("%s: generator reached limit", generator);

	return value;
}

void TCACDataBase::GetNow(CDateTime *stamp)
{
	TCACQuery *query = NewQuery(GetNowFormat(), false);

	if (!query->Read() || !query->GetStamp(query->Find("NOW"), stamp))
		fatal("%s: unable to get data", query->Name);

	delete query;
}

TCACDataBase *TCACDataBase::DefaultDB = NULL;

char *TCACDataBase::MaxDisplay(const char *name)
{
	char *tName = strnew(name);

	if (strlen(tName) > 2000 + 3)
		strcpy(tName + 2000, "...");

	return tName;
}

const char *TCACDataBase::WrongField(const char *name, const char *marker, char *message)
{
	char *s;
	
	if (strlen(name) >= 3 && !strncmpi(name, "T_", 2) && (s = strstr(message, marker)) != NULL)
	{
		s += strlen(marker) - 2;

		if (!strncmpi(s + 1, name + 1, strlen(name) - 1))	// constraint is for our table
		{
			char *end = s + strlen(name);

			s = end - 1;
			*s = 'F';

			while (isalpha(*end) || *end == '_')
				end++;

			*end = '\0';
			return strupr(s);
		}
	}

	return NULL;
}

void TCACDataBase::RegularError(const TRegularError *regerr, const char *name, const char *oper, const char *marker,
	char *errorText)
{
	static constant FIELD_FMT = "%s: поле ";
	const FIELD_LEN = 9;
	const char *field;

	if (strncmp(regerr->format, FIELD_FMT, FIELD_LEN) || (field = WrongField(name, marker, errorText)) == NULL)
		error(regerr->format, name, oper, regerr->code);
	else
	{
		mstr m;

		m.printf(FIELD_FMT, name);
		m.cat(field);
		m.printf(regerr->format + FIELD_LEN - 1, oper, regerr->code);
		error("%s", str(m));
	}
}

// ----- TODBCDataBase -------------------------------------------------------
#ifndef SQL_SUCCEEDED
#define SQL_SUCCEEDED(result) (((result) & (~1)) == 0)
#endif

TODBCDataBase::TODBCDataBase(const char *name, bool (*tRegularError)(const char *, const char *, TODBCError &, int))
	: TCACDataBase(name), hdbc(SQL_NULL_HDBC), connected(false), RegularError(tRegularError)
{
	if (!use_count++)
	{
		CheckRC("ODBC", "SQLAllocHandle(henv)", SQL_HANDLE_ENV, henv,	// henv == NULL
			SQLAllocHandle(SQL_HANDLE_ENV, NULL, &henv), ET_FATAL);

		CheckRC("ODBC", "SQLSetEnvAttr", SQL_HANDLE_ENV, henv,
			SQLSetEnvAttr(henv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER) SQL_OV_ODBC3, 0), ET_FATAL);
	}
}

TODBCDataBase::~TODBCDataBase()
{
	Disconnect();

	if (--use_count == 0 && autoDown)
		Shutdown();
}

bool TODBCDataBase::Connect(const char *userName, const char *password, int errorType)
{
	CheckRC(Name, "SQLAllocHandle(hdbc)", SQL_HANDLE_ENV, henv, SQLAllocHandle(SQL_HANDLE_DBC, henv, &hdbc), ET_FATAL);
	SetSystemDSN();
	connected = CheckRC(Name, "SQLConnect", SQL_HANDLE_DBC, hdbc, SQLConnect(hdbc, (SQLCHAR *) Name, SQL_NTS,
		(SQLCHAR *) userName, SQL_NTS, (SQLCHAR *) password, SQL_NTS), errorType);
	return connected;
}

void TODBCDataBase::Disconnect()
{
	if (connected)
	{
		CheckRC(Name, "SQLDisconnect", SQL_HANDLE_DBC, hdbc, SQLDisconnect(hdbc), ET_ERROR);
		connected = false;
	}

	if (hdbc != SQL_NULL_HDBC)
	{
		CheckRC(Name, "SQLFreeHandle(hdbc)", SQL_HANDLE_DBC, hdbc, SQLFreeHandle(SQL_HANDLE_DBC, hdbc), ET_ERROR);
		hdbc = SQL_NULL_HDBC;
	}
}

bool TODBCDataBase::GetDescs(const char *table, TIArray<FLDDesc> *descs)
{
	bool primaryKey = false;

	SQLHSTMT hstmt = AllocStmt();
	SQLRETURN result = SQLColumns(hstmt, NULL, 0, NULL, 0, (SQLCHAR *) table, SQL_NTS, NULL, 0);
	SQLCHAR name[DBIMAXNAMELEN + 1];
	SQLSMALLINT type;
	SQLUINTEGER size;

	// Get all columns
	CheckRC(table, "SQLColumns", hstmt, result, ET_FATAL);
	BindColumn(hstmt, 4, SQL_C_CHAR, name, sizeof name);
	BindColumn(hstmt, 5, SQL_C_SSHORT, &type, sizeof type);
	BindColumn(hstmt, 7, SQL_C_SLONG, &size, sizeof size);

	while (SQL_SUCCEEDED(result = SQLFetch(hstmt)))
	{
		pFLDDesc desc = new FLDDesc;

		strcpy(desc->szName, (const char *) name);
		AssignDesc(desc, type, size);
		desc->iUnUsed[0] = FALSE;
		descs->Add(desc);
	}

	if (result != SQL_NO_DATA_FOUND)
		CheckRC(table, "SQLFetch", hstmt, result, ET_FATAL);

	FreeStmt(hstmt);

	// Mark primary columns
	hstmt = AllocStmt();
	result = SQLPrimaryKeys(hstmt, NULL, 0, NULL, 0, (SQLCHAR *) table, SQL_NTS);

	CheckRC(table, "SQLPrimaryKeys", hstmt, result, ET_FATAL);
	BindColumn(hstmt, 4, SQL_C_CHAR, name, sizeof name);

	while (SQL_SUCCEEDED(result = SQLFetch(hstmt)))
	{
		int i;

		for (i = 0; i < descs->Count(); i++)
		{
			pFLDDesc desc = (*descs)[i];

			if (!strcmpi(desc->szName, (const char *) name))
			{
				desc->iUnUsed[0] = TRUE;
				primaryKey = true;
				break;
			}
		}

		if (i == descs->Count())
			fatal("%s: missing primary column %s", table, name);
	}

	if (result != SQL_NO_DATA_FOUND)
		CheckRC(table, "SQLFetch", hstmt, result, ET_FATAL);

	FreeStmt(hstmt);
	return primaryKey;
}

bool TODBCDataBase::Execute(const char *stmt, int errorType, const char *name, const char *oper,
	const TIArray<TCACParameter> *params)
{
	if (!name)
		name = stmt;

	SQLHSTMT hstmt = AllocStmt();
	BindParams(hstmt, params);
	SQLRETURN result = SQLExecDirect(hstmt, (SQLCHAR *) stmt, SQL_NTS);

	if (result == SQL_NO_DATA_FOUND)
		result = 0;
	else
		CheckRC(name, oper ? oper : "SQLExecDirect", hstmt, result, errorType);

	FreeStmt(hstmt);
	return !result;
}

int TODBCDataBase::ExecCount(const char *stmt, int errorType, const char *name, const char *oper,
	const TIArray<TCACParameter> *params)
{
	if (!name)
		name = stmt;

	SQLHSTMT hstmt = AllocStmt();
	BindParams(hstmt, params);
	SQLRETURN result = SQLExecDirect(hstmt, (SQLCHAR *) stmt, SQL_NTS);
	SQLINTEGER count;

	if (result == SQL_NO_DATA_FOUND)
		count = 0;
	else if (!CheckRC(name, oper ? oper : "SQLExecDirect", hstmt, result, errorType))
		count = -1;
	else
		CheckRC(name, "SQLRowCount", hstmt, SQLRowCount(hstmt, &count), ET_FATAL);

	FreeStmt(hstmt);
	return count;
}

TCACQuery *TODBCDataBase::NewQuery(const char *expr, bool)
{
	return new TODBCQuery(expr, this);
}

void TODBCDataBase::Initialize(bool tAutoDown)
{
	autoDown = tAutoDown;
}

void TODBCDataBase::Shutdown()
{
	if (henv != SQL_NULL_HENV && CheckRC("ODBC", "SQLFreeHandle(henv)", SQL_HANDLE_ENV, henv,
		SQLFreeHandle(SQL_HANDLE_ENV, henv), ET_ERROR, NotARegularError))
	{
		henv = SQL_NULL_HENV;
	}
}

void TODBCDataBase::SetSystemDSN()
{
	if (!CheckInst(NULL, "SQLSetConfigMode(SYS)", SQLSetConfigMode(ODBC_SYSTEM_DSN)))
		failure();
}

bool TODBCDataBase::CheckInst(const char *name, const char *oper, BOOL result)
{
	if (result)
		return true;

	DWORD errorCode;
	char errorMsg[SQL_MAX_MESSAGE_LENGTH];
	WORD unused;
	mstr m;

	for (WORD iError = 1; iError <= 8; iError++)
	{
		if (SQLInstallerError(iError, &errorCode, errorMsg, sizeof errorMsg - 1, &unused) == SQL_SUCCESS)
		{
			m.sep(",\n\t");
			m.printf("%s (%d)", errorMsg, errorCode);
		}
	}

	if (!any(m))
		m.cat("грешка при изпълнение на заявката");

	return name ? error("%s: %s: %s.", name, oper, m.str()) : error("%s: %s.", oper, m.str());
}

void TODBCDataBase::BindParams(SQLHSTMT hstmt, const TIArray<TCACParameter> *params)
{
	if (params)
	{
		for (SQLUSMALLINT i = 0; i < params->Count(); )
		{
			TCACParameter *param = (*params)[i++];

			CheckRC(Name, "SQLBindParameter", hstmt, SQLBindParameter(hstmt, (SQLUSMALLINT) i, SQL_PARAM_INPUT,
				SQL_C_BINARY, SQL_VARBINARY, param->size, 0, param->data, param->size, &param->size), ET_FATAL);
		}
	}
}

SQLHSTMT TODBCDataBase::AllocStmt()
{
	SQLHSTMT hstmt;
	CheckRC(Name, "SQLAllocHandle(hstmt)", SQL_HANDLE_DBC, hdbc, SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt), ET_FATAL);
	return hstmt;
}

void TODBCDataBase::FreeStmt(SQLHSTMT hstmt)
{
	CheckRC(Name, "SQLFreeHandle(hstmt)", hstmt, SQLFreeHandle(SQL_HANDLE_STMT, hstmt), ET_FATAL);
}

void TODBCDataBase::BindColumn(SQLHSTMT hstmt, SQLUSMALLINT column, SQLSMALLINT type, SQLPOINTER data, SQLINTEGER size,
	SQLINTEGER *state)
{
	static SQLINTEGER unused;	// N.B. must be static
	CheckRC(Name, "SQLBindCol", hstmt, SQLBindCol(hstmt, column, type, data, size, state ? state : &unused), ET_FATAL);
}

void TODBCDataBase::AssignDesc(pFLDDesc desc, SQLSMALLINT type, SQLUINTEGER size)
{
	switch (type)
	{
		case SQL_VARCHAR :
		case SQL_LONGVARCHAR :
		case SQL_DECIMAL :
		case SQL_NUMERIC : desc->iFldType = SQL_CHAR; break;
		case SQL_FLOAT   : desc->iFldType = SQL_DOUBLE; break;
		case SQL_BIGINT  : desc->iFldType = SQL_INTEGER; break;
		case SQL_LONGVARBINARY :
		case SQL_VARBINARY :
		case SQL_BINARY  : desc->iFldType = SQL_BINARY; break;
		default : desc->iFldType = type;
	}

	switch (desc->iFldType)
	{
		case SQL_INTEGER	: desc->iLen = sizeof(long); break;
		case SQL_SMALLINT	: desc->iLen = sizeof(short); break;
		case SQL_REAL	: desc->iLen = sizeof(float); break;
		case SQL_DOUBLE	: desc->iLen = sizeof(double); break;
		case SQL_TYPE_DATE : desc->iLen = sizeof(DATE_STRUCT); break;
		case SQL_TYPE_TIME : desc->iLen = sizeof(TIME_STRUCT); break;
		case SQL_TYPE_TIMESTAMP : desc->iLen = sizeof(TIMESTAMP_STRUCT); break;
		case SQL_CHAR     : desc->iLen = (UINT16) (size + 1); break;
		case SQL_BINARY : desc->iLen = 0; break;	// reports short "size"
		default : fatal("ODBC: unsupported data type %d", (int) type);
	}
}

bool TODBCDataBase::NotARegularError(const char *, const char *, TODBCError &, int)
{
	return false;
}

struct TODBCTransErrorCode
{
	const char *sqlState;
	int errorCode;
};

static TODBCTransErrorCode ODBCTransErrorCodes[] =
{
	{ "01004", -802 },	// string_data_right_truncation
	{ "08000", 10014 },	// connection_exception
	{ "08006", 10014 },	// sqlclient_unable_to_establish_sqlconnection
	{ "08004", 10014 },	// sqlserver_rejected_establishment_of_sqlconnection
	{ NULL, 0 }
};

bool TODBCDataBase::CheckRC(const char *name, const char *oper, SQLSMALLINT type, SQLHANDLE handle, SQLRETURN result,
	int errorType, bool (*regularError)(const char *, const char *, TODBCError &, int))
{
#if RANGECHECK
	if (errorType < ET_SILENT || errorType > ET_COUNT)
		fatal("%s: %s: invalid error type %d", name, oper, errorType);
#endif  // RANGECHECK

	if (SQL_SUCCEEDED(result))
		return true;

	if (errorType != ET_SILENT)
	{
		SQLSMALLINT recNumber = 1;
		TODBCError odbcError;
		SQLSMALLINT unused;
		mstr m;

		while (SQLGetDiagRec(type, handle, recNumber, odbcError.sqlState, &odbcError.Code, odbcError.Msg,
			sizeof odbcError.Msg, &unused) == SQL_SUCCESS)
		{
			if (errorType >= ET_READ)
			{
				if (regularError(name, oper, odbcError, errorType))
					return false;

				errorType = ET_FATAL;
			}

			m.sep(",\n\t");
			m.printf("%s (%s", odbcError.Msg, odbcError.sqlState);

			const TODBCTransErrorCode *trans;

			for (trans = ODBCTransErrorCodes; trans->sqlState; trans++)
			{
				if (!strcmp(trans->sqlState, (const char *) odbcError.sqlState))
				{
					m.printf(", %d", trans->errorCode);
					break;
				}
			}

			m.cat(")");
			recNumber++;
		}

		if (recNumber == 1)
		{
			m.cpy("Invalid ODBC state or handle");
			odbcError.Code = result;
		}

		char *tName = MaxDisplay(name);
		error("%s: грешка при изпълнение на заявката (%d).\n\n%s: %s.", tName, odbcError.Code, oper, str(m));
		delete[] tName;

		if (errorType == ET_FATAL)
			failure();
	}

	return false;
}

int TODBCDataBase::use_count = 0;
bool TODBCDataBase::autoDown = true;
SQLHENV TODBCDataBase::henv = SQL_NULL_HENV;

// ----- TPostGresDataBase ---------------------------------------------------
TPostGresDataBase::TPostGresDataBase(const char *name) : TODBCDataBase(name, PostGresRegularError)
{
}

const char *TPostGresDataBase::Protocol() const
{
	return "ODBC";
}

const char *TPostGresDataBase::BaseType() const
{
	return "PostgreSQL";
}

bool TPostGresDataBase::DefaultConnect(int errorType)
{
	unsigned char password[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
	DtorZero pwd((char *) password, sizeof password);

	calc_simple_xor(password, sizeof password);
	return Connect("????????", (char *) password, errorType);
}

void TPostGresDataBase::CondActive(mstr &m, const char *) const
{
	m.printf("& %d = %d", FLAG_ACTIVE, FLAG_ACTIVE);
}

const char *TPostGresDataBase::GetNextFormat() const
{
	return "SELECT NEXTVAL('%s') AS %s";
}

const char *TPostGresDataBase::GetNowFormat() const
{
	return "SELECT CURRENT_TIMESTAMP AS NOW";
}

const char *TPostGresDataBase::GetExecPrefix() const
{
	return "SELECT ";
}

struct TPostGresRegularError
{
	const char *sqlState;
	TCACDataBase::TRegularError regerr;
};

static const TPostGresRegularError PostGresReadRegularErrors[] =
{
	{ "40P01", { -913, DBISTR_RECLOCKED } },	// deadlock_detected
	{ NULL, { 0, NULL } }
};

static const TPostGresRegularError PostGresDeleteRegularErrors[] =
{
	{ "40P01", { -913, DBISTR_RECLOCKED } },			// deadlock_detected
	{ "2B000", { -803, DBISTR_DETAILRECORDSEXIST } },	// dependent_privilege_descriptors_still_exist
	{ "2BP01", { -803, DBISTR_DETAILRECORDSEXIST } },	// dependent_objects_still_exist
	{ "42501", { -551, DBISTR_NOTSUFFSQLRIGHTS } },		// insufficient_privilege
	{ NULL, { 0, NULL } }
};

static const TPostGresRegularError PostGresWriteRegularErrors[] =
{
	{ "40P01", { -913, DBISTR_RECLOCKED } },			// deadlock_detected
	{ "22000", { -802, DBISTR_REQDERR } },			// data_exception
	{ "22004", { -802, DBISTR_NULLERROR } },			// null_value_not_allowed
	{ "22003", { -802, DBISTR_RANGEERROR } },			// numeric_value_out_of_range
	{ "23000", { -803, DBISTR_INTEGRITY } },			// integrity_constraint_violation
	{ "23001", { -803, DBISTR_INTEGRITY } },			// restrict_violation
	{ "23502", { -803, DBISTR_NULLERROR } },			// not_null_violation
	{ "23503", { -803, DBISTR_FOREIGNKEY } },			// foreign_key_violation
	{ "23505", { -803, DBISTR_KEYVIOL } },			// unique_violation
	{ "23514", { -803, DBISTR_REQDERR } },			// check_violation
	{ "23P01", { -803, DBISTR_INTEGRITY } },			// exclusion_violation
	{ "2B000", { -803, DBISTR_DETAILRECORDSEXIST } },	// dependent_privilege_descriptors_still_exist
	{ "2BP01", { -803, DBISTR_DETAILRECORDSEXIST } },	// dependent_objects_still_exist
	{ "42501", { -551, DBISTR_NOTSUFFSQLRIGHTS } },		// insufficient_privilege
	{ NULL, { 0, NULL } }
};

static const TPostGresRegularError *const PostGresRegularErrors[3] =
	{ PostGresReadRegularErrors, PostGresDeleteRegularErrors, PostGresWriteRegularErrors };

bool TPostGresDataBase::PostGresRegularError(const char *name, const char *oper, TODBCError &odbcError, int errorType)
{
	for (const TPostGresRegularError *pgrerr = PostGresRegularErrors[errorType - ET_READ]; pgrerr->sqlState; pgrerr++)
	{
		if (!strcmp(pgrerr->sqlState, (const char *) odbcError.sqlState))
		{
			TCACDataBase::RegularError(&pgrerr->regerr, name, oper, "constraint \"c_", (char *) odbcError.Msg);
			return true;
		}
	}

	return false;
}

// ----- TPostGresExtraDataBase ----------------------------------------------
TPostGresExtraDataBase::TPostGresExtraDataBase(const char *name)
	: TPostGresDataBase(name)
{
}

bool TPostGresExtraDataBase::Connect(const char *userName, const char *password, int errorType)
{
	SQLSMALLINT unused;
	mstr m;

	CheckRC(Name, "SQLAllocHandle(hdbc)", SQL_HANDLE_ENV, henv, SQLAllocHandle(SQL_HANDLE_DBC, henv, &hdbc), ET_FATAL);
	m.printf("DRIVER=PostgreSQL ODBC Driver(ANSI);%s;", Name);
	m.printf("Username=%s;Password={%s}", userName, password);	// md5 hex:calc_md5(pass+user)
	connected = CheckRC(Name, "SQLConnect", SQL_HANDLE_DBC, hdbc, SQLDriverConnect(hdbc, NULL, (SQLCHAR *) str(m),
		SQL_NTS, NULL, 0, &unused, SQL_DRIVER_NOPROMPT), errorType);
	return connected;
}

bool TPostGresExtraDataBase::DefaultConnect(int errorType)
{
	unsigned char password[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
	DtorZero pwd((char *) password, sizeof password);

	calc_simple_xor(password, sizeof password);
	return Connect("????????", (char *) password, errorType);
}

// ----- InterBase/Firebird --------------------------------------------------
static constant IBASE_FBIRD_BASE_TYPE = "InterBase/Firebird";
static constant IBASE_FBIRD_USER_NAME = "????????";
static constant IBASE_FBIRD_PASSWORD = "????????";
static constant IBASE_FBIRD_NEXT_FORMAT = "SELECT GEN_ID(%s, 1) AS %s FROM RDB$DATABASE";
static constant IBASE_FBIRD_NOW_FORMAT = "SELECT CURRENT_TIMESTAMP AS NOW FROM RDB$DATABASE";
static constant IBASE_FBIRD_EXEC_PREFIX = "EXECUTE PROCEDURE ";

static void ibase_fbird_cond_active(mstr &m, const char *field)
{
	m.printf("< (CAST((%s / %d) AS INTEGER)) * %d", field, FLAG_ACTIVE << 1, FLAG_ACTIVE << 1);
}

// ----- TFireBirdDataBase ---------------------------------------------------
TFireBirdDataBase::TFireBirdDataBase(const char *name)
#if FIREBIRD_ODBC_CURSOR_CACHE
	: TODBCDataBase(name, FireBirdRegularError), used_handles(0)
#else
	: TODBCDataBase(name, FireBirdRegularError)
#endif  // FIREBIRD_ODBC_CURSOR_CACHE
{
}

const char *TFireBirdDataBase::Protocol() const
{
	return "ODBCC";
}

const char *TFireBirdDataBase::BaseType() const
{
	return IBASE_FBIRD_BASE_TYPE;
}

bool TFireBirdDataBase::DefaultConnect(int errorType)
{
	return Connect(IBASE_FBIRD_USER_NAME, IBASE_FBIRD_PASSWORD, errorType);
}

void TFireBirdDataBase::CondActive(mstr &m, const char *field) const
{
	ibase_fbird_cond_active(m, field);
}

#if FIREBIRD_ODBC_CURSOR_CACHE
SQLHSTMT TFireBirdDataBase::AllocStmt()
{
	return used_handles ? handles[--used_handles] : TODBCDataBase::AllocStmt();
}

void TFireBirdDataBase::FreeStmt(SQLHSTMT hstmt)
{
	if (used_handles < FIREBIRD_ODBC_CURSOR_CACHE)
	{
		CheckRC(Name, "SQLFreeStmt(params)", hstmt, SQLFreeStmt(hstmt, SQL_RESET_PARAMS), ET_FATAL);
		CheckRC(Name, "SQLFreeStmt(unbind)", hstmt, SQLFreeStmt(hstmt, SQL_UNBIND), ET_FATAL);
		CheckRC(Name, "SQLCloseCursor(hstmt)", hstmt, SQLCloseCursor(hstmt), ET_FATAL);
		handles[used_handles++] = hstmt;
	}
	else
		TODBCDataBase::FreeStmt(hstmt);
}
#endif  // FIREBIRD_ODBC_CURSOR_CACHE

const char *TFireBirdDataBase::GetNextFormat() const
{
	return IBASE_FBIRD_NEXT_FORMAT;
}

const char *TFireBirdDataBase::GetNowFormat() const
{
	return IBASE_FBIRD_NOW_FORMAT;
}

const char *TFireBirdDataBase::GetExecPrefix() const
{
	return IBASE_FBIRD_EXEC_PREFIX;
}

static const TCACDataBase::TRegularError FireBirdReadRegularErrors[] =
{
	{ -615, DBISTR_RECLOCKED },
	{ -693, DBISTR_RECLOCKED },
	{ -913, DBISTR_RECLOCKED },
	{ 0, NULL }
};

static const TCACDataBase::TRegularError FireBirdDeleteRegularErrors[] =
{
	{ -530, DBISTR_UNKNOWNSQL },
	{ -615, DBISTR_RECLOCKED },
	{ -693, DBISTR_RECLOCKED },
	{ -913, DBISTR_RECLOCKED },
	{ 0, NULL }
};

static const TCACDataBase::TRegularError FireBirdWriteRegularErrors[] =
{
	{ -297, DBISTR_REQDERR },
	{ -530, DBISTR_UNKNOWNSQL },
	{ -615, DBISTR_RECLOCKED },
	{ -625, DBISTR_REQDERR },
	{ -693, DBISTR_RECLOCKED },
	{ -803, DBISTR_KEYVIOL },
	{ -913, DBISTR_RECLOCKED },
	{ 0, NULL }
};

static const TCACDataBase::TRegularError *FireBirdRegularErrors[3] =
	{ FireBirdReadRegularErrors, FireBirdDeleteRegularErrors, FireBirdWriteRegularErrors };

bool TFireBirdDataBase::FireBirdRegularError(const char *name, const char *oper, TODBCError &odbcError, int errorType)
{
	for (const TRegularError *regerr = FireBirdRegularErrors[errorType - ET_READ]; regerr->code; regerr++)
	{
		if (regerr->code == odbcError.Code)
		{
			TCACDataBase::RegularError(regerr, name, oper, "constraint C_", (char *) odbcError.Msg);
			return true;
		}
	}

	return false;
}

// ----- TBDEDataBase --------------------------------------------------------
TBDEDataBase::TBDEDataBase(const char *name)
	: TCACDataBase(name), handle(0)
{
	use_count++;
}

TBDEDataBase::~TBDEDataBase()
{
	Disconnect();

	if (--use_count == 0 && autoDown)
		Shutdown();
}

const char *TBDEDataBase::Protocol() const
{
	return "BDE-M";
}

const char *TBDEDataBase::BaseType() const
{
	return IBASE_FBIRD_BASE_TYPE;
}

bool TBDEDataBase::Connect(const char *userName, const char *password, int errorType)
{
	FLDDesc desc;

	strcpy(desc.szName, "USER NAME");
	desc.iOffset = 0;
	desc.iLen = (UINT16) (strlen(userName) + 1);

	return CheckDI(Name, "DbiOpenDatabase", DbiOpenDatabase((pCHAR) Name, "INTRBASE", dbiREADWRITE, dbiOPENSHARED,
		(pCHAR) password, 1, &desc, (pBYTE) userName, &handle), errorType);
}

bool TBDEDataBase::DefaultConnect(int errorType)
{
	return Connect(IBASE_FBIRD_USER_NAME, IBASE_FBIRD_PASSWORD, errorType);
}

void TBDEDataBase::Disconnect()
{
	if (handle)
	{
		CheckDI(Name, "DbiCloseDatabase", DbiCloseDatabase(&handle), ET_ERROR);
		handle = 0;		// force
	}
}

bool TBDEDataBase::GetDescs(const char *table, TIArray<FLDDesc> *descs)
{
	bool primaryKey = false;

	hDBICur cursor;
	CURProps props;

	CheckDI(table, "DbiOpenTable", DbiOpenTable(handle, (pCHAR) table, NULL, 0, 0, NODEFAULTINDEX, dbiREADONLY,
		dbiOPENSHARED, xltFIELD, TRUE, 0, &cursor));
	CheckDI(table, "DbiGetCursorProps", DbiGetCursorProps(cursor, &props));

	// Get all columns
	pFLDDesc fDescs = new FLDDesc[props.iFields];
	CheckDI(table, "DbiGetFieldDescs", DbiGetFieldDescs(cursor, fDescs));

	for (int i = 0; i < props.iFields; i++)
	{
		pFLDDesc desc = new FLDDesc;

		memcpy(desc, fDescs + i, sizeof(FLDDesc));
		desc->iUnUsed[0] = FALSE;
		descs->Add(desc);
	}

	delete[] fDescs;

	// Mark primary columns
	pIDXDesc iDescs = new IDXDesc[props.iIndexes];
	CheckDI(table, "DbiGetIndexDescs", DbiGetIndexDescs(cursor, iDescs));

	for (int index = 0; index < props.iIndexes; index++)
	{
		pIDXDesc desc = iDescs + index;

		if (desc->bPrimary || (desc->bUnique && !strncmp(desc->szName, "I_", 2) &&
			!strcmp(table + 2, desc->szName + 2)))	// LPR: bPrimary may fail
		{
			for (int k = 0; k < desc->iFldsInKey; k++)
			{
				UINT16 kFieldNum = desc->aiKeyFld[k];
				int i;

				for (i = 0; i < descs->Count(); i++)
				{
					pFLDDesc desc = (*descs)[i];

					if (desc->iFldNum == kFieldNum)
					{
						desc->iUnUsed[0] = TRUE;
						break;
					}
				}

				if (i == descs->Count())
					fatal("%s: missing primary column %u", table, (unsigned) kFieldNum);
			}

			primaryKey = true;
			break;
		}
	}

	CloseCursor(table, &cursor);

	delete[] iDescs;
	return primaryKey;
}

bool TBDEDataBase::Execute(const char *stmt, int errorType, const char *name, const char *oper,
	const TIArray<TCACParameter> *params)
{
	if (!name)
		name = stmt;

	if (params && params->Count())
		fatal("%s: TBDEDataBase does not support params", name);

	return CheckDI(name, oper ? oper : "DbiQExecDirect", DbiQExecDirect(handle, qrylangSQL, (pCHAR) stmt, NULL), errorType);
}

int TBDEDataBase::ExecCount(const char *stmt, int errorType, const char *name, const char *oper,
	const TIArray<TCACParameter> *params)
{
	if (!name)
		name = stmt;

	if (params && params->Count())
		fatal("%s: TBDEDataBase does not support params", name);

	hDBIStmt statement;
	UINT32 count = 0;
	UINT16 unused;

	Prepare(stmt, &statement, false);

	// 2014:045 FIX: stmt -> name
	if (!CheckDI(name, oper ? oper : "DbiQExec", DbiQExec(statement, NULL), errorType))
		count = -1;
	else
		CheckDI(name, "DbiGetProp", DbiGetProp(statement, stmtROWCOUNT, &count, sizeof count, &unused));

	CheckDI(name, "DbiQFree", DbiQFree(&statement), ET_ERROR);
	return count;
}

TCACQuery *TBDEDataBase::NewQuery(const char *expr, bool simple)
{
	return new TBDEQuery(expr, this, simple);
}

void TBDEDataBase::CondActive(mstr &m, const char *field) const
{
	ibase_fbird_cond_active(m, field);
}

static const TCACDataBase::TRegularError BDEReadRegularErrors[] =
{
	{ DBIERR_ALREADYLOCKED, DBISTR_ALREADYLOCKED },
	{ DBIERR_FILELOCKED, DBISTR_FILELOCKED },
	{ DBIERR_NONE, NULL }
};

static const TCACDataBase::TRegularError BDEDeleteRegularErrors[] =
{
	{ DBIERR_RECLOCKFAILED, DBISTR_RECLOCKFAILED },
	{ DBIERR_DETAILRECORDSEXIST, DBISTR_DETAILRECORDSEXIST },
	{ DBIERR_NOTSUFFTABLERIGHTS, DBISTR_NOTSUFFTABLERIGHTS },
	{ DBIERR_NOTSUFFSQLRIGHTS, DBISTR_NOTSUFFSQLRIGHTS },
	{ DBIERR_UNKNOWNSQL, DBISTR_UNKNOWNSQL },
	{ DBIERR_NONE, NULL }
};

static const TCACDataBase::TRegularError BDEWriteRegularErrors[] =
{
	{ DBIERR_KEYVIOL, DBISTR_KEYVIOL },
	{ DBIERR_FILELOCKED, DBISTR_FILELOCKED },
	{ DBIERR_MINVALERR, DBISTR_MINVALERR },
	{ DBIERR_MAXVALERR, DBISTR_MAXVALERR },
	{ DBIERR_REQDERR, DBISTR_REQDERR },
	{ DBIERR_NOTSUFFTABLERIGHTS, DBISTR_NOTSUFFTABLERIGHTS },
	{ DBIERR_TABLEREADONLY, DBISTR_TABLEREADONLY },
	{ DBIERR_NODISKSPACE, DBISTR_NODISKSPACE },
	{ DBIERR_RECLOCKFAILED, DBISTR_RECLOCKFAILED },
	{ DBIERR_UNKNOWNSQL, DBISTR_REQDERR },
	{ DBIERR_NONE, NULL }
};

static const TCACDataBase::TRegularError *const BDERegularErrors[3] =
	{ BDEReadRegularErrors, BDEDeleteRegularErrors, BDEWriteRegularErrors };

bool TBDEDataBase::CheckDI(const char *name, const char *oper, DBIResult result, int errorType)
{
#if RANGECHECK
	if (errorType < ET_SILENT || errorType > ET_COUNT)
		fatal("%s: %s: invalid error type %d", name, oper, errorType);
#endif  // RANGECHECK

	if (result == DBIERR_NONE)
		return true;

	if (errorType != ET_SILENT)
	{
		mstr m;

		if (errorType >= ET_READ)
		{
			for (const TRegularError *regerr = BDERegularErrors[errorType - ET_READ]; regerr->code; regerr++)
			{
				if (regerr->code == result)
				{
					dbi_message(result, m);
					RegularError(regerr, name, oper, "constraint C_", ncstr(m));
					return false;
				}
			}

			errorType = ET_FATAL;
		}

		char *tName = MaxDisplay(name);

		dbi_message(result, m);
		error("%s: грешка при изпълнение на заявката (%d).\n\n%s: %s", tName, (int) result, oper, str(m));
		delete[] tName;

		if (errorType == ET_FATAL)
			failure();
	}

	return false;
}

void TBDEDataBase::Prepare(const char *stmt, phDBIStmt statement, bool simple)
{
	CheckDI(stmt, "DbiQAlloc", DbiQAlloc(handle, qrylangSQL, statement));

	if (simple)
	{
		CheckDI(stmt, "DbiSetProp", DbiSetProp(*statement, stmtUNIDIRECTIONAL, true));
		CheckDI(stmt, "DbiSetProp", DbiSetProp(*statement, stmtLIVENESS, wantLIVE));
	}
	else
		CheckDI(stmt, "DbiSetProp", DbiSetProp(*statement, stmtLIVENESS, (UINT32) false));

	CheckDI(stmt, "DbiQPrepare", DbiQPrepare(*statement, (pCHAR) stmt));
}

void TBDEDataBase::Initialize(const char *tempdir1, const char *tempdir2, bool tAutoDown)
{
	FMTDate fmtDate = { ".", 1, TRUE, FALSE, TRUE, TRUE };
	FMTTime fmtTime = { ':', FALSE, "", "", TRUE, FALSE };

	CheckDI("BDE", "DbiInit", DbiInit(NULL));

	if ((!tempdir1 || DbiSetPrivateDir((pCHAR) tempdir1) != DBIERR_NONE) && tempdir2 && strcmp(tempdir1, tempdir2))
		DbiSetPrivateDir((pCHAR) tempdir2);

	if (!CheckDI("BDE", "DbiSetDateFormat", DbiSetDateFormat(&fmtDate), ET_ERROR) ||
		!CheckDI("BDE", "DbiSetTimeFormat", DbiSetTimeFormat(&fmtTime), ET_ERROR))
	{
		Shutdown();
		failure();
	}

	autoDown = tAutoDown;
}

void TBDEDataBase::Shutdown()
{
	CheckDI("BDE", "DbiExit", DbiExit(), ET_ERROR);
}

const char *TBDEDataBase::GetNextFormat() const
{
	return IBASE_FBIRD_NEXT_FORMAT;
}

const char *TBDEDataBase::GetNowFormat() const
{
	return IBASE_FBIRD_NOW_FORMAT;
}

const char *TBDEDataBase::GetExecPrefix() const
{
	return  IBASE_FBIRD_EXEC_PREFIX;
}

int TBDEDataBase::use_count = 0;
bool TBDEDataBase::autoDown = true;
