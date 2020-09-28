//{{TDialog = TJoinUXNDialog}}
class TJoinUXNDialog : public TAlterUXNDialog
{
public:
	TJoinUXNDialog(TWindow* parent, TUCNGroup *source, TUCNGroup *target, int resId = IDD_JOIN_UXN);

protected:
	char targetUCNTypes[3];		// 2005:354 LRQ: allow real-to-real

//{{TJoinUXNDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TJoinUXNDialogVIRTUAL_END}}

//{{TJoinUXNDialogRSP_TBL_BEGIN}}
	void SourceTypeCBNSelChange();
//{{TJoinUXNDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TJoinUXNDialog);
};    //{{TJoinUXNDialog}}
