#include "database.h"

// ----- TCACQuery -----------------------------------------------------------
TCACQuery::TCACQuery(const char *expr)
	: TVObject(expr), descs(NULL), count(0), buffer(NULL)
{
}

TCACQuery::~TCACQuery()
{
}

pFLDDesc TCACQuery::Find(const char *name) const
{
	int l = 0, h = count - 1;

	while (l <= h)
	{
		int i = (l + h) / 2;
		int res = strcmpi(descs[i].szName, name);

		if (!res)
			return descs + i;

		if (res < 0)
			l = i + 1;
		else
			h = i - 1;
	}

	return NULL;
}

static int pFLDDesc_compare(const pFLDDesc desc1, const pFLDDesc desc2)
{
	return strcmpi(desc1->szName, desc2->szName);
}

void TCACQuery::Open()
{
	qsort(descs, count, sizeof(FLDDesc), (int (*)(const void *, const void *)) pFLDDesc_compare);
}

void TCACQuery::Close()
{
	delete[] descs;
	descs = NULL;
	delete[] buffer;
	buffer = NULL;
}

// ----- TODBCQuery ----------------------------------------------------------
TODBCQuery::TODBCQuery(const char *expr, TODBCDataBase *tDataBase)
	: TCACQuery(expr), dataBase(tDataBase), hstmt(SQL_NULL_HSTMT), states(NULL)
{
	Open();
}

TODBCQuery::~TODBCQuery()
{
	Close();
}

bool TODBCQuery::Read() const
{
	SQLRETURN result = SQLFetch(hstmt);
	return result == SQL_NO_DATA_FOUND ? false : dataBase->CheckRC(Name, "четене", hstmt, result, ET_READ);
}

bool TODBCQuery::GetField(pFLDDesc desc, pBYTE data) const
{
	if (states[desc->iFldNum] != SQL_NULL_DATA)
	{
		// copy the trailing '\0' for char *
		memcpy(data, buffer + desc->iOffset, states[desc->iFldNum] + (desc->iFldType == SQL_CHAR));
		return true;
	}

	return false;
}

bool TODBCQuery::GetText(const pFLDDesc desc, mstr *m, bool zero) const
{
	if (states[desc->iFldNum] != SQL_NULL_DATA)
	{
		mbk_resize(m, states[desc->iFldNum] + zero);
		memcpy(m->ncstr(), buffer + desc->iOffset, states[desc->iFldNum] + zero);
		return true;
	}

	return false;
}

static inline unsigned char Query_hexvalue(BYTE c)
{
	if (c >= '0' && c <= '9')
		return (unsigned char) (c - '0');

	if (c >= 'a' && c <= 'f')
		return (unsigned char) (c - 'a');

	if (c <= 'A' || c >= 'F')
		fatal("Query invalid hex digit %d", (int) c);

	return (unsigned char) (c - 'A');
}

bool TODBCQuery::GetBLOB(const pFLDDesc desc, memblock_t *m) const
{
#if 20150285
	SQLLEN returnLen;

	dataBase->CheckRC(Name, "SQLGetData", hstmt, SQLGetData(hstmt, desc->iFldNum, desc->iFldType, &returnLen, 0,
		&returnLen), ET_FATAL);

	if (returnLen != SQL_NULL_DATA)
	{
		mbk_resize(m, returnLen);
		dataBase->CheckRC(Name, "SQLGetData", hstmt, SQLGetData(hstmt, desc->iFldNum, desc->iFldType, m->data, returnLen,
			&returnLen), ET_FATAL);
		return true;
	}
#else
	size_t size = states[desc->iFldNum];

	if (size != SQL_NULL_DATA)
	{
		// TODO: warning! assuming offset fits in 16-bit iOffset
		const BYTE *data = buffer + desc->iOffset;

		if (size < 2 || memcmp(data, "\\x", 2))
			fatal("TODBCQuery::GetBLOB supports only hex");

		if (size > 2)
		{
			if (size % 2)
				fatal("TODBCQuery::GetBLOB invalid hex len %u", (unsigned) size);

			mbk_resize(m, size / 2 - 1);
			const BYTE *s_end = data + size;
			unsigned char *t = m->data;

			for (const unsigned char *s = data + 2; s < s_end; s += 2)
				*t++ = (unsigned char) ((Query_hexvalue(*s) << 4) + Query_hexvalue(s[1]));
		}

		return true;
	}
#endif

	return false;
}

bool TODBCQuery::GetStamp(const pFLDDesc desc, ISetDateTime &stamp) const
{
	if (states[desc->iFldNum] != SQL_NULL_DATA)
	{
		const TIMESTAMP_STRUCT *times = (const TIMESTAMP_STRUCT *) (buffer + desc->iOffset);

		stamp.set(times->year, times->month, times->day, times->hour, times->minute, times->second);
		return true;
	}

	return false;
}

void TODBCQuery::Restart()
{
	Close();
	Open();
}

void TODBCQuery::Open()
{
	FLDDesc *desc;
	SQLSMALLINT columns;
	UINT16 recsize = 0;
	SQLSMALLINT index;

	hstmt = dataBase->AllocStmt();
	dataBase->CheckRC(Name, "SQLExecDirect", hstmt, SQLExecDirect(hstmt, (SQLCHAR *) Name, SQL_NTS), ET_FATAL);
	dataBase->CheckRC(Name, "SQLNumResultCols", hstmt, SQLNumResultCols(hstmt, &columns), ET_FATAL);
	count = columns;
	descs = new FLDDesc[count];
	states = new SQLLEN[count + 1];

	for (index = 1, desc = descs; index <= count; index++, desc++)
	{
		SQLSMALLINT unused;
		SQLSMALLINT type;
		SQLULEN size;
		SQLSMALLINT nullable;

		dataBase->CheckRC(Name, "SQLDescribeCol", hstmt, SQLDescribeCol(hstmt, index, (SQLCHAR *) desc->szName,
			sizeof desc->szName, &unused, &type, &size, &unused, &nullable), ET_FATAL);

		dataBase->AssignDesc(desc, type, size);
		desc->iFldNum = index;

		if (desc->iLen)
		{
			desc->iOffset = recsize;
			recsize += desc->iLen;
		}
	}

	TCACQuery::Open();
	buffer = new BYTE[recsize];

	for (index = 0, desc = descs; index < count; index++, desc++)
	{
		if (desc->iLen)
		{
			// ODBC docs say the state only may be bound, but that didn't work
			dataBase->BindColumn(hstmt, desc->iFldNum, desc->iFldType, buffer + desc->iOffset, desc->iLen,
				states + desc->iFldNum);
		}
	}
}

void TODBCQuery::Close()
{
	delete[] states;
	states = NULL;

	if (hstmt != SQL_NULL_HSTMT)
	{
		dataBase->FreeStmt(hstmt);
		hstmt = SQL_NULL_HSTMT;
	}

	TCACQuery::Close();
}

unsigned TODBCQuery::_Count()
{
	mstr m;
	unsigned count;

	// 2015:189 LPR: fallback to read on UNION SELECT
	if (strstr(Name, " UNION "))
	{
		count = 0;	// no Restart(), assuming fresh query

		while (Read())
			count++;

		Restart();	// cleanup
	}
	else
	{
		m.cpy("SELECT COUNT(*)");
	#if RANGECHECK
		if (strindex(Name, " FROM ") == INT_MAX)
			fatal("%s: invalid getRecsCount", Name);
	#endif  // RANGECHECK
		m.cat(Name + strindex(Name, " FROM "));
		// 2014:216 FB2: no ORDER for COUNT
		// 2015:177 LPR: avoid msql here
		const char *order = strstr(str(m), " ORDER BY ");

		if (order)
			m.cut(order);

		count = dataBase->GetLong(str(m), "COUNT");
	}

	return count;
}

#ifdef __BORLANDC__
// ----- TBDEQuery -----------------------------------------------------------
TBDEQuery::TBDEQuery(const char *expr, TBDEDataBase *tDataBase)
	: TCACQuery(expr), dataBase(tDataBase), statement(0), cursor(0)
{
	Open();
}

TBDEQuery::~TBDEQuery()
{
	Close();
}

bool TBDEQuery::Read() const
{
	DBIResult result = DbiGetNextRecord(cursor, dbiNOLOCK, buffer, 0);
	return result == DBIERR_EOF ? false : dataBase->CheckDI(Name, "четене", result, ET_READ);
}

bool TBDEQuery::GetField(pFLDDesc desc, pBYTE data) const
{
	BOOL bIsNULL;
	dataBase->CheckDI(desc->szName, "DbiGetField", DbiGetField(cursor, desc->iFldNum, buffer, data, &bIsNULL));
	return !bIsNULL;
}

bool TBDEQuery::GetText(const pFLDDesc desc, mstr *m, bool zero) const
{
	UINT32 blobSize;

	dataBase->CheckDI(desc->szName, "DbiOpenBlob", DbiOpenBlob(cursor, buffer, desc->iFldNum, dbiREADONLY));
	dataBase->CheckDI(desc->szName, "DbiGetBlobSize", DbiGetBlobSize(cursor, buffer, desc->iFldNum, &blobSize));

	if (blobSize)
	{
		UINT32 blobRead;

		mbk_resize(m, blobSize + zero);
		dataBase->CheckDI(desc->szName, "DbiGetBlob", DbiGetBlob(cursor, buffer, desc->iFldNum, 0, blobSize,
			(pBYTE) m->ncstr(), &blobRead));

		if (blobRead != blobSize)
			fatal("%s: BLOB %s read size error", Name, desc->szName);

		if (zero)
			m->ncstr()[blobRead] = '\0';

		return true;
	}

	return false;
}

bool TBDEQuery::GetBLOB(const pFLDDesc, memblock_t *) const
{
	fatal("TBDEQuery::GetBLOB not implemented");
	return false;
}

bool TBDEQuery::GetStamp(const pFLDDesc desc, ISetDateTime &stamp) const
{
	BOOL bIsNULL;
	TIMESTAMP times;

	dataBase->CheckDI(desc->szName, "DbiGetField", DbiGetField(cursor, desc->iFldNum, buffer, (pBYTE) &times, &bIsNULL));

	if (!bIsNULL)
	{
		DBIDATE date;
		TIME time;
		UINT16 day, month;
		INT16 year;
		UINT16 hour, minute, msec;

		dataBase->CheckDI(desc->szName, "DbiTimeStampDecode", DbiTimeStampDecode(times, &date, &time));
		dataBase->CheckDI(desc->szName, "DbiDateDecode", DbiDateDecode(date, &month, &day, &year));
		dataBase->CheckDI(desc->szName, "DbiTimeDecode", DbiTimeDecode(time, &hour, &minute, &msec));
		stamp.set(year, month, day, hour, minute, msec / 1000);
		return true;
	}

	return false;
}

void TBDEQuery::Restart()
{
	dataBase->CheckDI(Name, "restart", DbiSetToBegin(cursor));
}

void TBDEQuery::Open()
{
	dataBase->Prepare(Name, &statement);
	dataBase->CheckDI(Name, "DbiQExec", DbiQExec(statement, &cursor));

	if (cursor)
	{
		CURProps props;

		dataBase->CheckDI(Name, "DbiGetCursorProps", DbiGetCursorProps(cursor, &props));
		count = props.iFields;
		descs = new FLDDesc[count];
		dataBase->CheckDI(Name, "DbiGetFieldDescs", DbiGetFieldDescs(cursor, descs));
		TCACQuery::Open();
		buffer = new BYTE[props.iRecBufSize];
	}
}

void TBDEQuery::Close()
{
	if (cursor)
	{
		dataBase->CheckDI(Name, "DbiFreeBlob", DbiFreeBlob(cursor, buffer, 0));
		dataBase->CheckDI(Name, "DbiCloseCursor", DbiCloseCursor(&cursor), ET_ERROR);
		cursor = 0;		// force
	}

	if (statement)
	{
		dataBase->CheckDI(Name, "DbiQFree", DbiQFree(&statement), ET_ERROR);
		statement = 0;	// force
	}

	TCACQuery::Close();
}

unsigned TBDEQuery::_Count()
{
	unsigned long count;
	return dataBase->CheckDI(Name, "преброяване", DbiGetRecordCount(cursor, &count), ET_SILENT) ? count : 0;
}
#endif  // __BORLANDC__

// ----- TQuery --------------------------------------------------------------
TQuery::TQuery(const char *expr, TCACDataBase *dataBase)
	: TObject(expr)
{
	query = (dataBase ? dataBase : TCACDataBase::DefaultDB)->NewQuery(expr);
}

TQuery::~TQuery()
{
	delete query;
}

bool TQuery::Read() const
{
	return query->Read();
}

bool TQuery::GetField(const pFLDDesc desc, pBYTE data) const
{
	return query->GetField(desc, data);
}

bool TQuery::GetText(const pFLDDesc desc, mstr *m, bool zero) const
{
	return query->GetText(desc, m, zero);
}

bool TQuery::GetBLOB(const pFLDDesc desc, memblock_t *m) const
{
	return query->GetBLOB(desc, m);
}

bool TQuery::GetStamp(const pFLDDesc desc, ISetDateTime &stamp) const
{
	return query->GetStamp(desc, stamp);
}

void TQuery::Restart()
{
	query->Restart();
}

unsigned TQuery::_Count()
{
	return query->_Count();
}
