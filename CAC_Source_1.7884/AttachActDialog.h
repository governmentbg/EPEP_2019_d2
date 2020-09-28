//{{TDialog = TAttachActDialog}}
class TAttachActDialog : public THtmlBaseDialog
{
public:
	TAttachActDialog(TWindow* parent, TIndexContainer *tContainer, const char *name, int resId = IDD_ATTACH_ACT);

protected:
	TIndexContainer *container;
	TElectricTrouble trouble;

//{{TAttachActDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TAttachActDialogVIRTUAL_END}}

//{{TAttachActDialogRSP_TBL_BEGIN}}
	void OKBNClicked();
	void AttachBNClicked();
	void SwapBNClicked();
//{{TAttachActDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TAttachActDialog);
};    //{{TAttachActDialog}}
