//{{TDialog = TFindReplaceDialog}}
class TFindReplaceDialog : public TNTDialog
{
public:
	TFindReplaceDialog(TWindow* parent, TFilterDocument *tFD, TIArray<TReplacePair> *tPairs);
	virtual ~TFindReplaceDialog();

protected:
	TFilterDocument *fd;
	const char *name;
	bool locked;
	UINT timer;

	TStringFace *find;
	TStringFace *replace;
	TFindReplaceListBox *pairs;

	bool IsValid();
	bool IsOpen();

//{{TFindReplaceDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TFindReplaceDialogVIRTUAL_END}}

//{{TFindReplaceDialogRSP_TBL_BEGIN}}
	void AddBNClicked();
	void AddBNEnable(TCommandEnabler &tce);
	void DeleteBNClicked();
	void DeleteBNEnable(TCommandEnabler &tce);
	void PairsCBNSelChange();
	void EvClose();
	void EvTimer(uint);
	virtual TResult EvCommand(uint id, THandle hWndCtl, uint notifyCode);
//{{TFindReplaceDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TFindReplaceDialog);
};    //{{TFindReplaceDialog}}
