#include <errno.h>
#include <io.h>
#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <time.h>
#include <windows.h>

#include <idapi.h>
#include <idapi.h>
#include <sql.h>
#include <sqlext.h>
#include "odbcv3.h"
#include "odbcinst.h"

#include <owl/button.h>
#include <owl/dialog.h>
#include <owl/edit.h>
#include <owl/combobox.h>

typedef const char *const constant;
typedef int sign;

#include "mconnect.h"

#include "vprinter.h"
#include "object.h"
#include "memblock.h"
#include "mstr.h"
#include "message.h"
#include "array.h"
#include "dbistr.h"
#include "cacdatabase.h"
#include "query.h"
#include "CACTarget.h"
#include "CACConnect.h"
#include "connect.rh"

enum
{
	ALIAS_SIZE  = 24,
	NAME_SIZE   = 41
};

enum
{
	// 1 = BDE only
	// 2 = BDE + ODBC/IBase-FBird
	// 3 = BDE + ODBC/IBase-FBird + ODBC/PostgreSQL
	DBT_COUNT = 2
};

#include "ConnectDialog.h"
#include "WaitDialog.h"
