//{{TDialog = TLoginDialog}}
class TLoginDialog : public TGroupDialog
{
public:
	TLoginDialog(TWindow* parent, TUser *user, int resId = IDD_LOGIN);
	virtual ~TLoginDialog();

protected:
	BOOL caps;
	TStatic *capsLock;

//{{TLoginDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TLoginDialogVIRTUAL_END}}

//{{TLoginDialogRSP_TBL_BEGIN}}
	void OKBNClicked();
	void EvTimer(uint timerId);
//{{TLoginDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TLoginDialog);
};    //{{TLoginDialog}}
