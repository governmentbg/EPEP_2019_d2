#define ODBC_ADD_DSN 1
#define ODBC_CONFIG_DSN 2
#define ODBC_REMOVE_DSN 3
#define ODBC_ADD_SYS_DSN 4
#define ODBC_CONFIG_SYS_DSN 5
#define ODBC_REMOVE_SYS_DSN 6
#define ODBC_BOTH_DSN 0
#define ODBC_USER_DSN 1
#define ODBC_SYSTEM_DSN 2
#define INSTAPI __stdcall

#ifdef __cplusplus
extern "C"
{
#endif
BOOL INSTAPI SQLConfigDataSource(HWND hwndParent, WORD fRequest, LPCSTR lpszDriver, LPCSTR lpszAttributes);
int INSTAPI SQLGetPrivateProfileString(LPCSTR lpszSection, LPCSTR lpszEntry, LPCSTR lpszDefault, LPSTR RetBuffer,
	int cbRetBuffer, LPCSTR lpszFilename);
BOOL INSTAPI SQLGetConfigMode(UWORD *pwConfigMode);
SQLRETURN INSTAPI SQLInstallerError(WORD iError, DWORD *pfErrorCode, LPSTR lpszErrorMsg, WORD cbErrorMsgMax,
	WORD *pcbErrorMsg);
BOOL INSTAPI SQLSetConfigMode(UWORD wConfigMode);
BOOL INSTAPI SQLWritePrivateProfileString(LPCSTR lpszSection, LPCSTR lpszEntry, LPCSTR lpszString, LPCSTR lpszFilename);
#ifdef __cplusplus
}
#endif
