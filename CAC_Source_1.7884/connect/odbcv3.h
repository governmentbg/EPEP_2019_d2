typedef void FAR       *SQLHANDLE;

#define SQL_HANDLE_ENV  1
#define SQL_HANDLE_DBC  2
#define SQL_HANDLE_STMT 3
#define SQL_ATTR_ODBC_VERSION 200
#define SQL_OV_ODBC3    3UL

#define SQL_TYPE_DATE 91
#define SQL_TYPE_TIME 92
#define SQL_TYPE_TIMESTAMP 93

#ifdef __cplusplus
extern "C"
{
#endif
SQLRETURN SQL_API SQLAllocHandle(SQLSMALLINT HandleType, SQLHANDLE InputHandle, SQLHANDLE *OutputHandlePtr);
SQLRETURN SQL_API SQLCloseCursor(SQLHSTMT StatementHandle);
SQLRETURN SQL_API SQLEndTran(SQLSMALLINT HandleType, SQLHANDLE Handle, SQLSMALLINT CompletionType);
SQLRETURN SQL_API SQLFreeHandle(SQLSMALLINT HandleType, SQLHANDLE Handle);
SQLRETURN SQL_API SQLGetDiagRec(SQLSMALLINT HandleType, SQLHANDLE Handle, SQLSMALLINT RecNumber, SQLCHAR *SQLState,
	SQLINTEGER *NativeErrorPtr, SQLCHAR *MessageText, SQLSMALLINT BufferLength, SQLSMALLINT *TextLengthPtr);
SQLRETURN SQL_API SQLSetEnvAttr(SQLHENV EnvironmentHandle, SQLINTEGER Attribute, SQLPOINTER ValuePtr,
	SQLINTEGER StringLength);
#ifdef __cplusplus
}
#endif
