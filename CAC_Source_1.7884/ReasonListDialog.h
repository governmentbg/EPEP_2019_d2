//{{TDialog = TReasonListDialog}}
class TReasonListDialog : public TGroupListDialog
{
public:
	TReasonListDialog(TWindow* parent, int resId = IDD_REALITY_LIST);

protected:
	TReasonListWindow *reasons;

//{{TReasonListDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TReasonListDialogVIRTUAL_END}}

//{{TReasonListDialogRSP_TBL_BEGIN}}
	void SubjectBNClicked();
	void PrintBNClicked();
//{{TReasonListDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TReasonListDialog);
};    //{{TReasonListDialog}}
