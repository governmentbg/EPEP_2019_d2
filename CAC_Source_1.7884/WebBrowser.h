struct IWebBrowser2;

#define IWebBrowser2_Refresh(p) (p)->lpVtbl->Refresh(p)
#define IWebBrowser2_GoForward(p) (p)->lpVtbl->GoForward(p)
#define IWebBrowser2_GoBack(p) (p)->lpVtbl->GoBack(p)
#define IWebBrowser2_Stop(p) (p)->lpVtbl->Stop(p)
#define IWebBrowser2_Quit(p) (p)->lpVtbl->Quit(p)
#define IWebBrowser2_Release(p) (p)->lpVtbl->Release(p)
#define IWebBrowser2_QueryInterface(p,i,o) (p)->lpVtbl->QueryInterface((p),(i),(o))
#define IWebBrowser2_Navigate2(p,u,ve1,ve2,ve3,ve4) (p)->lpVtbl->Navigate2((p),(u),(ve1),(ve2),(ve3),(ve4))
#define IWebBrowser2_ExecWB(p,c,o,vi,vo) (p)->lpVtbl->ExecWB((p),(c),(o),(vi),(vo))
#define IWebBrowser2_get_Document(p, t) (p)->lpVtbl->get_Document(p, t)

void IWebBrowser2_Resize(IWebBrowser2 *iBrowser, RECT *rect);
void IWebBrowser2_NavigateURL(IWebBrowser2 *iBrowser, BSTR url);
void IWebBrowser2_Print(IWebBrowser2 *iBrowser, BOOL direct);
void IWebBrowser2_PrintPreview(IWebBrowser2 *iBrowser);
void IWebBrowser2_PageSetup(IWebBrowser2 *iBrowser);
void IWebBrowser2_SaveAs(IWebBrowser2 *iBrowser, BSTR url);
void IWebBrowser2_SetFontSize(IWebBrowser2 *iBrowser, long lVal);
HWND IWebBrowser2_GetHWnd(IWebBrowser2 *iBrowser);
void IWebBrowser2_Activate(IWebBrowser2 *iBrowser);

IWebBrowser2 *CreateEmbeddedWebControl(THtmlBaseDialog *pHtmlDialog);
void CloseEmbeddedWebControl(IWebBrowser2 *iBrowser);
