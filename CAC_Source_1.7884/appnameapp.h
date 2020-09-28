// 2014:183 LPR: for powershell update
extern bool WinXPOrLater;
// 2013:260 LPR: for powershell update
extern bool Win7OrLater;
// 2014:185 LPR: for powershell update
extern bool Windows64Bit;
// 2007:127 LPR: rect not overlapping menu
extern TRect AdjustedRect;
// 2009:173 LPR: shortcuts, un/remaximize
extern WORD MainShowWindow;
// 2010:0097 LPR: themes
extern void WINAPI (*SetThemeAppProperties)(DWORD);
extern HRESULT WINAPI (*EnableThemeDialogTexture)(HWND, DWORD);
extern DWORD WINAPI (*GetProcessId)(HANDLE);
// 2015:030 LPR: outlined from config +related
typedef enum _NETSETUP_NAME_TYPE
{
	NetSetupUnknown = 0,
	NetSetupMachine,
	NetSetupWorkgroup,
	NetSetupDomain,
	NetSetupNonExistentDomain,
	NetSetupDnsMachine
} NETSETUP_NAME_TYPE;
extern NET_API_STATUS WINAPI (*NetValidateName)(LPCWSTR, LPCWSTR, LPCWSTR, LPCWSTR, NETSETUP_NAME_TYPE);
typedef struct _DOMAIN_CONTROLLER_INFOA {
	LPSTR DomainControllerName;
	LPSTR DomainControllerAddress;
	ULONG DomainControllerAddressType;
	GUID DomainGuid;
	LPSTR DomainName;
	LPSTR DnsForestName;
	ULONG Flags;
	LPSTR DcSiteName;
	LPSTR ClientSiteName;
} DOMAIN_CONTROLLER_INFOA, *PDOMAIN_CONTROLLER_INFOA;
extern DWORD WINAPI (*DsGetDcNameA)(LPCSTR, LPCSTR, GUID *, LPCSTR, ULONG, PDOMAIN_CONTROLLER_INFOA *);
extern int AppMainBottom;	// main initial window bottom pos
extern TWindow *AppMainWindow;
extern HICON AppNameIcon;
extern TPostGresExtraDataBase *ExtraDataBase;	// 2015:282
extern BOOL WINAPI (*PathIsRelativeA)(LPCTSTR lpszPath);	// 2017:292
