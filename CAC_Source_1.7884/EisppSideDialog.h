//{{TDialog = TEisppSideDialog}}
class TEisppSideDialog : public TQuickDialog
{
public:
	TEisppSideDialog(TWindow* parent, sNPRAKTSTS *tNPR, int resId = IDD_EISPP_SIDE);

protected:
	sNPRAKTSTS *npr;
	TFzlListBox *fzls;
	TEdit *text;

//{{TEisppSideDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TEisppSideDialogVIRTUAL_END}}

//{{TEisppSideDialogRSP_TBL_BEGIN}}
	void OKBNClicked();
	void IgnoreBNClicked();
	void CancelBNClicked();
	void EvClose();
//{{TEisppSideDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TEisppSideDialog);
};  //{{TEisppSideDialog}}