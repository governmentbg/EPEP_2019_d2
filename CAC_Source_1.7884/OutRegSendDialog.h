//{{TDialog = TOutRegSendDialog}}
class TOutRegSendDialog : public TOutRegSendBaseDialog
{
public:
	TOutRegSendDialog(TWindow* parent, TOutReg *outReg, TReceiver *tReceiver, const char *kinds, TInReg *inReg,
		TRCDKeyContainer *tEvent, TConnect **connects = NULL);

protected:
	char types[SIZE_OF_COLLEGES];
	// 2008:028 to check kind for SLOWDOWN; 2008:059 inReg -> kind only
	char inRegKind;
	TComboBox *fixeiver;		// 2011:150 LPR: hide only
	long subject;			// 2014:003 select ADMIN for regional
	TRCDKeyContainer *event;	// 2014:073 +related: for OutRegL12

	DEFINE_GROUP_TYPE(TOutReg, OutReg)
	virtual bool IsValid();

//{{TOutRegSendDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TOutRegSendDialogVIRTUAL_END}}

//{{TOutRegSendDialogRSP_TBL_BEGIN}}
	void EnterBNClicked();
	void PrintBNClicked();
	void KindCBNSelChange();
//{{TOutRegSendDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TOutRegSendDialog);
};    //{{TOutRegSendDialog}}
