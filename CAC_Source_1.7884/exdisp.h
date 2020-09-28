EXTERN_C const CLSID CLSID_WebBrowser;
EXTERN_C const IID DIID_DWebBrowserEvents;

typedef enum {
	navOpenInNewWindow = 0x1,
	navNoHistory = 0x2,
	navNoReadFromCache = 0x4,
	navNoWriteTocache = 0x8,
	navAllowAutosearch = 0x10,
	navBrowserBar = 0x20,
	navHyperLink = 0x40
} BrowserNavConstants;

EXTERN_C const IID IID_IWebBrowser;
#undef INTERFACE
#define INTERFACE IWebBrowser
DECLARE_INTERFACE_(IWebBrowser,IDispatch)
{
	STDMETHOD(QueryInterface)(THIS_ REFIID,PVOID*) PURE;
	STDMETHOD_(ULONG,AddRef)(THIS) PURE;
	STDMETHOD_(ULONG,Release)(THIS) PURE;
	STDMETHOD(GetTypeInfoCount)(THIS_ UINT*) PURE;
	STDMETHOD(GetTypeInfo)(THIS_ UINT,LCID,LPTYPEINFO*) PURE;
	STDMETHOD(GetIDsOfNames)(THIS_ REFIID,LPOLESTR*,UINT,LCID,DISPID*) PURE;
	STDMETHOD(Invoke)(THIS_ DISPID,REFIID,LCID,WORD,DISPPARAMS*,VARIANT*,EXCEPINFO*,UINT*) PURE;

	STDMETHOD(GoBack)(THIS) PURE;
	STDMETHOD(GoForward)(THIS) PURE;
	STDMETHOD(GoHome)(THIS) PURE;
	STDMETHOD(GoSearch)(THIS) PURE;
	STDMETHOD(Navigate)(THIS_ BSTR,VARIANT*,VARIANT*,VARIANT*,VARIANT*) PURE;
	STDMETHOD(Refresh)(THIS) PURE;
	STDMETHOD(Refresh2)(THIS_ VARIANT*) PURE;
	STDMETHOD(Stop)(THIS) PURE;
	STDMETHOD(get_Application)(THIS_ IDispatch**) PURE;
	STDMETHOD(get_Parent)(THIS_ IDispatch**) PURE;
	STDMETHOD(get_Container)(THIS_ IDispatch**) PURE;
	STDMETHOD(get_Document)(THIS_ IDispatch**) PURE;
	STDMETHOD(get_TopLevelContainer)(THIS_ VARIANT_BOOL*) PURE;
	STDMETHOD(get_Type)(THIS_ BSTR*) PURE;
	STDMETHOD(get_Left)(THIS_ long*) PURE;
	STDMETHOD(put_Left)(THIS_ long) PURE;
	STDMETHOD(get_Top)(THIS_ long*) PURE;
	STDMETHOD(put_Top)(THIS_ long) PURE;
	STDMETHOD(get_Width)(THIS_ long*) PURE;
	STDMETHOD(put_Width)(THIS_ long) PURE;
	STDMETHOD(get_Height)(THIS_ long*) PURE;
	STDMETHOD(put_Height)(THIS_ long) PURE;
	STDMETHOD(get_LocationName)(THIS_ BSTR*) PURE;
	STDMETHOD(get_LocationURL)(THIS_ BSTR*) PURE;
	STDMETHOD(get_Busy)(THIS_ VARIANT_BOOL*) PURE;
};

EXTERN_C const IID IID_IWebBrowserApp;
#undef INTERFACE
#define INTERFACE IWebBrowserApp
DECLARE_INTERFACE_(IWebBrowserApp,IWebBrowser)
{
	STDMETHOD(QueryInterface)(THIS_ REFIID,PVOID*) PURE;
	STDMETHOD_(ULONG,AddRef)(THIS) PURE;
	STDMETHOD_(ULONG,Release)(THIS) PURE;
	STDMETHOD(GetTypeInfoCount)(THIS_ UINT*) PURE;
	STDMETHOD(GetTypeInfo)(THIS_ UINT,LCID,LPTYPEINFO*) PURE;
	STDMETHOD(GetIDsOfNames)(THIS_ REFIID,LPOLESTR*,UINT,LCID,DISPID*) PURE;
	STDMETHOD(Invoke)(THIS_ DISPID,REFIID,LCID,WORD,DISPPARAMS*,VARIANT*,EXCEPINFO*,UINT*) PURE;

	STDMETHOD(GoBack)(THIS) PURE;
	STDMETHOD(GoForward)(THIS) PURE;
	STDMETHOD(GoHome)(THIS) PURE;
	STDMETHOD(GoSearch)(THIS) PURE;
	STDMETHOD(Navigate)(THIS_ BSTR,VARIANT*,VARIANT*,VARIANT*,VARIANT*) PURE;
	STDMETHOD(Refresh)(THIS) PURE;
	STDMETHOD(Refresh2)(THIS_ VARIANT*) PURE;
	STDMETHOD(Stop)(THIS) PURE;
	STDMETHOD(get_Application)(THIS_ IDispatch**) PURE;
	STDMETHOD(get_Parent)(THIS_ IDispatch**) PURE;
	STDMETHOD(get_Container)(THIS_ IDispatch**) PURE;
	STDMETHOD(get_Document)(THIS_ IDispatch**) PURE;
	STDMETHOD(get_TopLevelContainer)(THIS_ VARIANT_BOOL*) PURE;
	STDMETHOD(get_Type)(THIS_ BSTR*) PURE;
	STDMETHOD(get_Left)(THIS_ long*) PURE;
	STDMETHOD(put_Left)(THIS_ long) PURE;
	STDMETHOD(get_Top)(THIS_ long*) PURE;
	STDMETHOD(put_Top)(THIS_ long) PURE;
	STDMETHOD(get_Width)(THIS_ long*) PURE;
	STDMETHOD(put_Width)(THIS_ long) PURE;
	STDMETHOD(get_Height)(THIS_ long*) PURE;
	STDMETHOD(put_Height)(THIS_ long) PURE;
	STDMETHOD(get_LocationName)(THIS_ BSTR*) PURE;
	STDMETHOD(get_LocationURL)(THIS_ BSTR*) PURE;
	STDMETHOD(get_Busy)(THIS_ VARIANT_BOOL*) PURE;

	STDMETHOD(Quit)(THIS) PURE;
	STDMETHOD(ClientToWindow)(THIS_ int*,int*) PURE;
	STDMETHOD(PutProperty)(THIS_ BSTR,VARIANT) PURE;
	STDMETHOD(GetProperty)(THIS_ BSTR,VARIANT*) PURE;
	STDMETHOD(get_Name)(THIS_ BSTR*) PURE;
	STDMETHOD(get_HWND)(THIS_ long*) PURE;
	STDMETHOD(get_FullName)(THIS_ BSTR*) PURE;
	STDMETHOD(get_Path)(THIS_ BSTR*) PURE;
	STDMETHOD(get_Visible)(THIS_ VARIANT_BOOL*) PURE;
	STDMETHOD(put_Visible)(THIS_ VARIANT_BOOL) PURE;
	STDMETHOD(get_StatusBar)(THIS_ VARIANT_BOOL*) PURE;
	STDMETHOD(put_StatusBar)(THIS_ VARIANT_BOOL) PURE;
	STDMETHOD(get_StatusText)(THIS_ BSTR*) PURE;
	STDMETHOD(put_StatusText)(THIS_ BSTR) PURE;
	STDMETHOD(get_ToolBar)(THIS_ int*) PURE;
	STDMETHOD(put_ToolBar)(THIS_ int) PURE;
	STDMETHOD(get_MenuBar)(THIS_ VARIANT_BOOL*) PURE;
	STDMETHOD(put_MenuBar)(THIS_ VARIANT_BOOL) PURE;
	STDMETHOD(get_FullScreen)(THIS_ VARIANT_BOOL*) PURE;
	STDMETHOD(put_FullScreen)(THIS_ VARIANT_BOOL) PURE;
};

EXTERN_C const IID IID_IWebBrowser2;
#undef INTERFACE
#define INTERFACE IWebBrowser2
DECLARE_INTERFACE_(IWebBrowser2,IWebBrowserApp)
{
	STDMETHOD(QueryInterface)(THIS_ REFIID,PVOID*) PURE;
	STDMETHOD_(ULONG,AddRef)(THIS) PURE;
	STDMETHOD_(ULONG,Release)(THIS) PURE;
	STDMETHOD(GetTypeInfoCount)(THIS_ UINT*) PURE;
	STDMETHOD(GetTypeInfo)(THIS_ UINT,LCID,LPTYPEINFO*) PURE;
	STDMETHOD(GetIDsOfNames)(THIS_ REFIID,LPOLESTR*,UINT,LCID,DISPID*) PURE;
	STDMETHOD(Invoke)(THIS_ DISPID,REFIID,LCID,WORD,DISPPARAMS*,VARIANT*,EXCEPINFO*,UINT*) PURE;

	STDMETHOD(GoBack)(THIS) PURE;
	STDMETHOD(GoForward)(THIS) PURE;
	STDMETHOD(GoHome)(THIS) PURE;
	STDMETHOD(GoSearch)(THIS) PURE;
	STDMETHOD(Navigate)(THIS_ BSTR,VARIANT*,VARIANT*,VARIANT*,VARIANT*) PURE;
	STDMETHOD(Refresh)(THIS) PURE;
	STDMETHOD(Refresh2)(THIS_ VARIANT*) PURE;
	STDMETHOD(Stop)(THIS) PURE;
	STDMETHOD(get_Application)(THIS_ IDispatch**) PURE;
	STDMETHOD(get_Parent)(THIS_ IDispatch**) PURE;
	STDMETHOD(get_Container)(THIS_ IDispatch**) PURE;
	STDMETHOD(get_Document)(THIS_ IDispatch**) PURE;
	STDMETHOD(get_TopLevelContainer)(THIS_ VARIANT_BOOL*) PURE;
	STDMETHOD(get_Type)(THIS_ BSTR*) PURE;
	STDMETHOD(get_Left)(THIS_ long*) PURE;
	STDMETHOD(put_Left)(THIS_ long) PURE;
	STDMETHOD(get_Top)(THIS_ long*) PURE;
	STDMETHOD(put_Top)(THIS_ long) PURE;
	STDMETHOD(get_Width)(THIS_ long*) PURE;
	STDMETHOD(put_Width)(THIS_ long) PURE;
	STDMETHOD(get_Height)(THIS_ long*) PURE;
	STDMETHOD(put_Height)(THIS_ long) PURE;
	STDMETHOD(get_LocationName)(THIS_ BSTR*) PURE;
	STDMETHOD(get_LocationURL)(THIS_ BSTR*) PURE;
	STDMETHOD(get_Busy)(THIS_ VARIANT_BOOL*) PURE;

	STDMETHOD(Quit)(THIS) PURE;
	STDMETHOD(ClientToWindow)(THIS_ int*,int*) PURE;
	STDMETHOD(PutProperty)(THIS_ BSTR,VARIANT) PURE;
	STDMETHOD(GetProperty)(THIS_ BSTR,VARIANT*) PURE;
	STDMETHOD(get_Name)(THIS_ BSTR*) PURE;
	STDMETHOD(get_HWND)(THIS_ long*) PURE;
	STDMETHOD(get_FullName)(THIS_ BSTR*) PURE;
	STDMETHOD(get_Path)(THIS_ BSTR*) PURE;
	STDMETHOD(get_Visible)(THIS_ VARIANT_BOOL*) PURE;
	STDMETHOD(put_Visible)(THIS_ VARIANT_BOOL) PURE;
	STDMETHOD(get_StatusBar)(THIS_ VARIANT_BOOL*) PURE;
	STDMETHOD(put_StatusBar)(THIS_ VARIANT_BOOL) PURE;
	STDMETHOD(get_StatusText)(THIS_ BSTR*) PURE;
	STDMETHOD(put_StatusText)(THIS_ BSTR) PURE;
	STDMETHOD(get_ToolBar)(THIS_ int*) PURE;
	STDMETHOD(put_ToolBar)(THIS_ int) PURE;
	STDMETHOD(get_MenuBar)(THIS_ VARIANT_BOOL*) PURE;
	STDMETHOD(put_MenuBar)(THIS_ VARIANT_BOOL) PURE;
	STDMETHOD(get_FullScreen)(THIS_ VARIANT_BOOL*) PURE;
	STDMETHOD(put_FullScreen)(THIS_ VARIANT_BOOL) PURE;

	STDMETHOD(Navigate2)(THIS_ VARIANT*,VARIANT*,VARIANT*,VARIANT*,VARIANT*) PURE;
	STDMETHOD(QueryStatusWB)(THIS_ OLECMDID,OLECMDF*) PURE;
	STDMETHOD(ExecWB)(THIS_ OLECMDID,OLECMDEXECOPT,VARIANT*,VARIANT*) PURE;
	STDMETHOD(ShowBrowserBar)(THIS_ VARIANT*,VARIANT*,VARIANT*) PURE;
	STDMETHOD(get_ReadyState)(THIS_ READYSTATE*) PURE;
	STDMETHOD(get_Offline)(THIS_ VARIANT_BOOL*) PURE;
	STDMETHOD(put_Offline)(THIS_ VARIANT_BOOL) PURE;
	STDMETHOD(get_Silent)(THIS_ VARIANT_BOOL*) PURE;
	STDMETHOD(put_Silent)(THIS_ VARIANT_BOOL) PURE;
	STDMETHOD(get_RegisterAsBrowser)(THIS_ VARIANT_BOOL*) PURE;
	STDMETHOD(put_RegisterAsBrowser)(THIS_ VARIANT_BOOL) PURE;
	STDMETHOD(get_RegisterAsDropTarget)(THIS_ VARIANT_BOOL*) PURE;
	STDMETHOD(put_RegisterAsDropTarget)(THIS_ VARIANT_BOOL) PURE;
	STDMETHOD(get_TheaterMode)(THIS_ VARIANT_BOOL*) PURE;
	STDMETHOD(put_TheaterMode)(THIS_ VARIANT_BOOL) PURE;
	STDMETHOD(get_AddressBar)(THIS_ VARIANT_BOOL*) PURE;
	STDMETHOD(put_AddressBar)(THIS_ VARIANT_BOOL) PURE;
	STDMETHOD(get_Resizable)(THIS_ VARIANT_BOOL*) PURE;
	STDMETHOD(put_Resizable)(THIS_ VARIANT_BOOL) PURE;
};

#define STDDISP(m) STDMETHOD_(void,m)

EXTERN_C const IID DIID_DWebBrowserEvents2;
#undef INTERFACE
#define INTERFACE DWebBrowserEvents2
DECLARE_INTERFACE_(DWebBrowserEvents2,IDispatch)
{
	STDMETHOD(QueryInterface)(THIS_ REFIID,PVOID*) PURE;
	STDMETHOD_(ULONG,AddRef)(THIS) PURE;
	STDMETHOD_(ULONG,Release)(THIS) PURE;
	STDMETHOD(GetTypeInfoCount)(THIS_ UINT*) PURE;
	STDMETHOD(GetTypeInfo)(THIS_ UINT,LCID,LPTYPEINFO*) PURE;
	STDMETHOD(GetIDsOfNames)(THIS_ REFIID,LPOLESTR*,UINT,LCID,DISPID*) PURE;
	STDMETHOD(Invoke)(THIS_ DISPID,REFIID,LCID,WORD,DISPPARAMS*,VARIANT*,EXCEPINFO*,UINT*) PURE;

	STDDISP(StatusTextChange)(THIS_ BSTR) PURE;
	STDDISP(ProgressChange)(THIS_ long,long) PURE;
	STDDISP(CommandStateChange)(THIS_ long,VARIANT_BOOL) PURE;
	STDDISP(DownloadBegin)(THIS) PURE;
	STDDISP(DownloadComplete)(THIS) PURE;
	STDDISP(TitleChange)(THIS_ BSTR) PURE;
	STDDISP(PropertyChange)(THIS_ BSTR) PURE;
	STDDISP(BeforeNavigate2)(THIS_ IDispatch*,VARIANT*,VARIANT*,VARIANT*,VARIANT*,VARIANT*,VARIANT_BOOL*) PURE;
	STDDISP(NewWindow2)(THIS_ IDispatch**,VARIANT_BOOL*) PURE;
	STDDISP(NavigateComplete)(THIS_ IDispatch*,VARIANT*) PURE;
	STDDISP(DocumentComplete)(THIS_ IDispatch*,VARIANT*) PURE;
	STDDISP(OnQuit)(THIS) PURE;
	STDDISP(OnVisible)(THIS_ VARIANT_BOOL) PURE;
	STDDISP(OnToolBar)(THIS_ VARIANT_BOOL) PURE;
	STDDISP(OnMenuBar)(THIS_ VARIANT_BOOL) PURE;
	STDDISP(OnStatusBar)(THIS_ VARIANT_BOOL) PURE;
	STDDISP(OnFullScreen)(THIS_ VARIANT_BOOL) PURE;
	STDDISP(OnTheaterMode)(THIS_ VARIANT_BOOL) PURE;
	STDDISP(WindowSetResizable)(THIS_ VARIANT_BOOL) PURE;
	STDDISP(WindowSetLeft)(THIS_ long) PURE;
	STDDISP(WindowSetTop)(THIS_ long) PURE;
	STDDISP(WindowSetWidth)(THIS_ long) PURE;
	STDDISP(WindowSetHeight)(THIS_ long) PURE;
	STDDISP(WindowClosing)(THIS_ VARIANT_BOOL,VARIANT_BOOL*) PURE;
	STDDISP(ClientToHostWindow)(THIS_ long*,long*) PURE;
	STDDISP(SetSecureLockIcon)(THIS_ long) PURE;
	STDDISP(FileDownload)(THIS_ VARIANT_BOOL*) PURE;
};

#undef STDDISP
