#include "cac.h"

// ----- TCACConnect ---------------------------------------------------------
TCACConnect::TCACConnect(const char *name)
	: TVObject(name)
{
}

// ----- static --------------------------------------------------------------
static bool get_profile(const char *Name, const char *key, char *value)
{
	if (SQLGetPrivateProfileString(Name, key, "", value, DBIMAXSCFLDLEN, "ODBC.INI") <= 0)
		*value = '\0';

	return *value;
}

static void check_profile(const char *Name, const char *key, char *value, const char *mandatory)
{
	if (!get_profile(Name, key, value) || strcmp(value, mandatory))
		fatal("%s: полето %s трябва задължително да е %s.", Name, key, mandatory);
}

static bool write_profile(const char *Name, const char *key, const char *value)
{
	return TODBCDataBase::CheckInst(Name, "SQLWritePrivateProfileString()",
		SQLWritePrivateProfileString(Name, key, value, "ODBC.INI"));
}

static void append_zero(mstr &m)
{
	mbk_append(&m, "", 1);
}

#define MAX_PORT 6

static const char *invalid_port(const char *port)
{
	if (strlen(port) >= MAX_PORT)
		return "Прекалено дълъг номер на порт";

	for (const char *s = port; *s; s++)
		if (!isdigit(*s))
			return "Невалиден номер на порт - изискват се само цифри";

	return atol(port) > 65535 ? "Прекалено голям номер на порт" : NULL;
}

// ----- TODBCConnect --------------------------------------------------------
bool TODBCConnect::Exist(const char *name, char *value)
{
	TODBCDataBase::SetSystemDSN();
	return get_profile(name, "Driver", value);
}

bool TODBCConnect::Delete(char *value)
{
	TODBCDataBase::SetSystemDSN();
	bool exist = get_profile(Name, "Driver", value);
	mstr m;

	if (DeleteDSN(m))
	{
		if (get_profile(Name, "Driver", value))
			return error("Грешка при изтриване. Може би са необходими администраторски права.");

		info("%s: %s.", Name, exist ? "името е изтрито" : "името е изтрито (или не е било дефинирано)");
		return true;
	}

	return false;
}

bool TODBCConnect::DeleteDSN(mstr &m)
{
	m.clear();
	m.printf("DSN=%s", Name);
	append_zero(m);
	return ConfigDSN("SQLConfigDataSource(ADD)", ODBC_REMOVE_SYS_DSN, m.str());
}

bool TODBCConnect::ConfigDSN(const char *oper, WORD request, const char *attributes)
{
	return TODBCDataBase::CheckInst(Name, oper, SQLConfigDataSource(NULL, request, DriverName(), attributes));
}

// ----- TPostGresConnect ----------------------------------------------------
TCACDataBase *TPostGresConnect::NewDataBase() const
{
	return new TPostGresDataBase(Name);
}

bool TPostGresConnect::Load(char *value)
{
	if (!Exist(Name, value))
		return false;

	const char *base = basename(value);

	if (strlen(base) != 15 || strncmpi(base, "psqlodbc3", 9) || !isdigit(base[9]) || strncmpi(base + 10, "a.dll", 4))
		return false;

	check_profile(Name, "LFConversion", value, "0");
	check_profile(Name, "MaxVarcharSize", value, "255");
	check_profile(Name, "MaxLongVarcharSize", value, "8190");

	get_profile(Name, "Servername", value);
	mstr m(value);

	if (get_profile(Name, "Port", value))
	{
		m.sep(":");
		m.cat(value);
	}

	m.sep(":");
	get_profile(Name, "Database", value);	// invalid names will block on Save
	m.cat(value);

	if (m.len() >= DBIMAXSCFLDLEN)
		fatal("%s: combined database name too long", Name);

	strcpy(value, m.str());		
	return true;
}

const char *TPostGresConnect::SyntaxError(char *value)
{
	static constant COLON_ERROR = "Синтаксисът е  сървър:база-данни  или  сървър:порт:база-данни,\n\n"
			"където \"сървър\" може да бъде име или IP адрес";

	char *colon = strrchr(value, ':');

	if (!colon)
		return COLON_ERROR;

	if (strlen(colon) < 5 || strncmp(colon + 1, "CAC_", 4))
		return "Името на базата данни трябва да започва със CAC_ (латински C и A)";

	*colon = '\0';
	colon = strrchr(value, ':');

	if (colon)
	{
		*colon++ = '\0';

		const char *invalid = invalid_port(colon);

		if (invalid)
			return invalid;
	}

	return !*value || strchr(value, ':') ? COLON_ERROR : NULL;
}

void TPostGresConnect::SyntaxWarn(char *value, mstr &m)
{
	char *colon = strrchr(value, ':');

	if (strcmp(colon + 1, "CAC_REGCOURT"))
		m.cat("База данни, различна от CAC_REGCOURT, може да се използва само за тестови цели");

	*colon = '\0';
	colon = strrchr(value, ':');

	if (colon && strcmp(colon + 1, "5432"))
	{
		m.sep(".\n\n");
		m.cat("Препоръчва се използването на стандартния порт");
	}
}

bool TPostGresConnect::Save(char *value)
{
	mstr m;
	DeleteDSN(m);

	static constant params_1[] = { "CommLog=0", "Debug=0", "Fetch=100", "Optimizer=0", "Ksqo=0", "UniqueIndex=1",
		"UseDeclareFetch=1", "UnknownSizes=0", "TextAsLongVarchar=1", "UnknownsAsLongVarchar=0", "BoolsAsChar=1",
		"Parse=0", "CancelAsFreeStmt=0", "MaxVarcharSize=255", "MaxLongVarcharSize=8190", "ExtraSysTablePrefixes=dd_;",
		"Description=", NULL };

	for (constant *param = params_1; *param; param++)
	{
		m.cat(*param);
		append_zero(m);
	}

	char *colon = strrchr(value, ':');

	*colon = '\0';
	m.printf("Database=%s", colon + 1);
	append_zero(m);
	colon = strrchr(value, ':');

	if (colon)
		*colon++ = '\0';
	else
		colon = "";

	m.printf("Servername=%s", value);
	append_zero(m);
	m.printf("Port=%s", colon);
	append_zero(m);

	static constant params_2[] = { "ReadOnly=0", "ShowOidColumn=0", "FakeOidIndex=0", "RowVersioning=0",
		"ShowSystemTables=0", "Protocol=7.4-1", "ConnSettings=", "DisallowPremature=0", "UpdatableCursors=0",
		"LFConversion=0", "TrueIsMinus1=0", "BI=0", "AB=0", "ByteaAsLongVarBinary=0", "UseServerSidePrepare=1",
		"LowerCaseIdentifier=0", "GssAuthUseGSS=0", "SSLmode=disable", "KeepaliveTime=-1", "KeepaliveInterval=-1",
		"PreferLibpq=-1", "XaOpt=1", NULL };

	for (constant *param = params_2; *param; param++)
	{
		m.cat(*param);
		append_zero(m);
	}

	return ConfigDSN("SQLConfigDataSource(ADD)", ODBC_ADD_SYS_DSN, m.str());
}

const char *TPostGresConnect::DriverName()
{
	return "PostgreSQL ODBC Driver(ANSI)";
}

// ----- InterBase/Firebird --------------------------------------------------
static const char *ibase_fbird_syntax_error(char *value)
{
	const char *s = strchr(value, ':');

	if (!s || !isalpha(s[1]) || s[2] != ':' || (s[3] != '\\' && s[3] != '/'))
	{
		return "Синтаксисът е:\n\nсървър:устройство:\\директории\\база-данни.gdb\n\n"
			"където \"сървър\" може да бъде име или IP адрес, а \"устройство\" е буква (A-Z)";
	}

	if ((s = strrchr(basename(value), '.')) == NULL || strcmpi(s + 1, "gdb"))
		return "Името на базата данни трябва да завършва на .gdb (включително и за Firebird)";

	return NULL;
}

static void ibase_fbird_syntax_warn(char *value, mstr &m)
{
	if (strcmpi(basename(value), "AppName.gdb"))
		m.cat("База данни, различна от AppName.gdb, може да се използва само за тестови цели");
}

// ----- TFireBirdConnect ----------------------------------------------------
TCACDataBase *TFireBirdConnect::NewDataBase() const
{
	return new TFireBirdDataBase(Name);
}

bool TFireBirdConnect::Load(char *value)
{
	if (!Exist(Name, value) || strcmpi(basename(value), "OdbcFBcc.dll"))
		return false;

	check_profile(Name, "CharacterSet", value, "WIN1251");
	check_profile(Name, "Dialect", value, "1");
	check_profile(Name, "ReadOnly", value, "N");
	get_profile(Name, "Dbname", value);
	return true;
}

const char *TFireBirdConnect::SyntaxError(char *value)
{
	return ibase_fbird_syntax_error(value);
}

void TFireBirdConnect::SyntaxWarn(char *value, mstr &m)
{
	ibase_fbird_syntax_warn(value, m);
}

bool TFireBirdConnect::Save(char *value)
{
	mstr m;
	DeleteDSN(m);

	m.cat("Description=");
	append_zero(m);
	m.printf("Dbname=%s", value);
	append_zero(m);
	m.cat("Client=");
	append_zero(m);
	m.printf("User=%s", "????????");
	append_zero(m);

	static constant params[] = { "Role=", "CharacterSet=WIN1251", "JdbcDriver=", "ReadOnly=N",
		"NoWait=N", "LockTimeoutWaitTransactions=", "Dialect=1", "QuotedIdentifier=Y", "SensitiveIdentifier=N",
		"AutoQuotedIdentifier=N", "UseSchemaIdentifier=0", "SafeThread=Y", "Password=", NULL };

	for (constant *param = params; *param; param++)
	{
		m.cat(*param);
		append_zero(m);
	}

	return ConfigDSN("SQLConfigDataSource(ADD)", ODBC_ADD_SYS_DSN, m.str()) &&
		(TODBCDataBase::SetSystemDSN(), true) &&
		write_profile(Name, "JdbcDriver", "") &&
		write_profile(Name, "UseSchemaIdentifier", "0");
}

const char *TFireBirdConnect::DriverName()
{
	return "Firebird/InterBase(r) driver (BCC)";
}

// ----- TBDEConnect ---------------------------------------------------------
TCACDataBase *TBDEConnect::NewDataBase() const
{
	return new TBDEDataBase(Name);
}

bool TBDEConnect::Load(char *value)
{
	DBIResult result;
	DBDesc test;
	CFGDesc desc;

	if ((result = DbiGetDatabaseDesc((pCHAR) Name, &test)) != DBIERR_OBJNOTFOUND)
	{
		mstr path;
		hDBICur hcur;

		TBDEDataBase::CheckDI("DbiGetDatabaseDesc", NULL, result, ET_FATAL);
		path.printf("\\DATABASES\\%s\\DB OPEN\\", Name);

		if ((result = DbiOpenCfgInfoList(NULL, dbiREADONLY, cfgPersistent, path.ncstr(), &hcur)) != DBIERR_OBJNOTFOUND)
		{
			TBDEDataBase::CheckDI("DbiOpenCfgInfoList", NULL, result, ET_FATAL);

			while ((result = DbiGetNextRecord(hcur, dbiNOLOCK, (pBYTE) &desc, NULL)) == DBIERR_NONE)
			{
				if (!strcmp(desc.szNodeName, "SERVER NAME"))
				{
					if (strcmp(test.szDbType, "INTRBASE"))
						fatal("%s: invalid database type %s", Name, test.szDbType);
					break;
				}
			}

			if (result != DBIERR_EOF)
				TBDEDataBase::CheckDI("DbiGetNextRecord", NULL, result, ET_FATAL);

			// 2015:336 LPR: was missing
			TBDEDataBase::CloseCursor("DbiDatabaseDesc", &hcur);
		}
	}

	if (result != DBIERR_NONE)
		return false;

	strcpy(value, desc.szValue);
	return true;
}

const char *TBDEConnect::SyntaxError(char *value)
{
	return ibase_fbird_syntax_error(value);
}

void TBDEConnect::SyntaxWarn(char *value, mstr &m)
{
	ibase_fbird_syntax_warn(value, m);
}

bool TBDEConnect::Save(char *value)
{
	DBIResult result = DbiDeleteAlias(NULL, (pCHAR) Name);

	if (result != DBIERR_OBJNOTFOUND && !TBDEDataBase::CheckDI(Name, "DbiDeleteAlias", result, ET_ERROR))
		return false;

	mstr params;

	params.printf("SERVER NAME: %s;USER NAME: %s;LANGDRIVER: ancyrr", value, "????????");
	return TBDEDataBase::CheckDI(Name, "DbiAddAlias", DbiAddAlias(NULL, (pCHAR) Name, "INTRBASE", params.ncstr(), TRUE),
		ET_ERROR);
}

bool TBDEConnect::Delete(char *)
{
	DBIResult result;

	switch (result = DbiDeleteAlias(NULL, (pCHAR) Name))
	{
		case DBIERR_NONE : info("%s: името е изтрито.", Name); return true;
		case DBIERR_OBJNOTFOUND : info("%s: името не е дефинирано.", Name); return true;
		default : TBDEDataBase::CheckDI(Name, "DbiDeleteAlias", result, ET_ERROR);
	}

	return false;
}
