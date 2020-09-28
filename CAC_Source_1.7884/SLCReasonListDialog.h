//{{TDialog = TSLCReasonListDialog}}
class TSLCReasonListDialog : public TGroupListDialog
{
public:
	TSLCReasonListDialog(TWindow* parent, int resId = IDD_SLC_REASON_LIST);

protected:
	TCharAliasFace *slcType;
	TSLCReasonListWindow *reasons;

	void ChangeEnable(TCommandEnabler &tce);

//{{TSLCReasonListDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TSLCReasonListDialogVIRTUAL_END}}

//{{TSLCReasonListDialogRSP_TBL_BEGIN}}
	void SLCTypeCBNSelChange();
	void PrintBNClicked();
//{{TSLCReasonListDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TSLCReasonListDialog);
};    //{{TSLCReasonListDialog}}
