//{{TDialog = TNTDialog}}
class TNTDialog : public TDialog
{
public:
	TNTDialog(TWindow* parent, int resId);
	virtual ~TNTDialog();

protected:
	bool alreadyExcepted;
	bool exceptTerminate;

	void HandleException(const char *descript);

protected:
//{{TNTDialogVIRTUAL_BEGIN}}
	virtual bool DialogFunction(uint message, TParam1 wParam, TParam2 lParam);
	virtual TResult EvCommand(uint id, THandle hWndCtl, uint notifyCode);
//{{TNTDialogVIRTUAL_END}}
};    //{{TNTDialog}}
