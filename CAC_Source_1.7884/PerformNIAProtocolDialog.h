//{{TDialog = TPerformNIAProtocolDialog}}
class TPerformNIAProtocolDialog : public TGroupDialog
{
public:
	TPerformNIAProtocolDialog(TWindow* parent, TNIAProtocol *protocol, int resId = IDD_PERFORM_NIA_PROTOCOL);

protected:
	TLongFace *niaNo;
	TSelectExecListListBox *execLists;

	DEFINE_GROUP_TYPE(TNIAProtocol, NIAProtocol)

//{{TPerformNIAProtocolDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TPerformNIAProtocolDialogVIRTUAL_END}}

//{{TPerformNIAProtocolDialogRSP_TBL_BEGIN}}
	void PrintBNEnable(TCommandEnabler &tce);
	void PrintBNClicked();
	void EnterBNEnable(TCommandEnabler &tce);
	void EnterBNClicked();
//{{TPerformNIAProtocolDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TPerformNIAProtocolDialog);
};    //{{TPerformNIAProtocolDialog}}
