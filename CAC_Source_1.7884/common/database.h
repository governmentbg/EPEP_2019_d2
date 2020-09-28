#include "common.h"

#ifdef __BORLANDC__
#include <idapi.h>
#else  // __BORLANDC__
#include "idapiv3.h"
#endif  // __BORLANDC__
#include <sql.h>
#include <sqlext.h>

#ifdef __BORLANDC__
#include "odbcv3.h"
#include "odbcinstv3.h"
#else
#include <odbcinst.h>
#endif  // __BORLANDC__

#include "password.h"
#include "cacdatabase.h"
#include "cacconnect.h"
#include "cacquery.h"
