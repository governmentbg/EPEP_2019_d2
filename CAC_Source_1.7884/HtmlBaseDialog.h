//{{TDialog = THtmlBaseDialog}}
class THtmlBaseDialog : public TShowDialog
{
public:
	THtmlBaseDialog(TWindow* parent, int showMax, const char *tName, bool tRemove, int resId);
	THtmlBaseDialog(TWindow* parent, int showMax, char *tText, int resId);
	virtual ~THtmlBaseDialog();

	HRESULT pCallBack(const char *url) { return CallBack(url); }

protected:
	IWebBrowser2 *iBrowser;
	HRESULT hrOleInitialize;
	char name[SIZE_OF_PATH];
	char *text;
	bool remove;

	virtual void Resize();
	virtual HRESULT CallBack(const char *) { return S_OK; }

	void ResizeBrowser();
	void FocusBrowser();
	bool Navigate(const char *name);

//{{THtmlBaseDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{THtmlBaseDialogVIRTUAL_END}}

//{{THtmlBaseDialogRSP_TBL_BEGIN}}
	void PrintBNClicked();
	void PrintDirectBNClicked();
	void ViewBNClicked();
	void PageSetupBNClicked();
	void EditBNClicked();
	void SaveBNClicked();
	void RefreshBNClicked();
	void BrowserBNKillFocus();
	long EvFocusFace(TParam1, TParam2);
//{{THtmlBaseDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(THtmlBaseDialog);
};    //{{THtmlBaseDialog}}
