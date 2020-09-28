//{{TDialog = TScaleDialog}}
class TScaleDialog : public TResourceDialog
{
public:
	TScaleDialog(TWindow* parent, int resId);
	virtual ~TScaleDialog();

protected:
	UINT timerId;

	static bool MatchesId(TWindow *window, void *id);
	static BOOL CALLBACK CreatePush(HWND hWnd, LPARAM lParam);

	virtual bool IsValid();
	virtual bool IsClean();

//{{TScaleDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
	virtual TResult EvCommand(uint id, THandle hWndCtl, uint notifyCode);
	virtual TResult EvNotify(uint id, TNotify far& info);
//{{TScaleDialogVIRTUAL_END}}

//{{TScaleDialogRSP_TBL_BEGIN}}
	void OKBNClicked();
	void CancelBNClicked();
	void EvClose();
//{{TScaleDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TScaleDialog);
};    //{{TScaleDialog}}
