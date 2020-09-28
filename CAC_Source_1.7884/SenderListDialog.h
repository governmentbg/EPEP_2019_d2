//{{TDialog = TSenderListDialog}}
class TSenderListDialog : public TGroupListDialog
{
public:
	TSenderListDialog(TWindow* parent, int resId = IDD_SENDER_LIST);

protected:
	TCharAliasFace *ucnType;
	TSenderListWindow *senders;
	mstr ucnTypes;

	bool ucnTypeIn(const char *ucnTypes) { return strchr(ucnTypes, ucnType->C()); } 	

//{{TSenderListDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TSenderListDialogVIRTUAL_END}}

//{{TSenderListDialogRSP_TBL_BEGIN}}
	void UCNTypeCBNSelChange();
	void AddBNEnable(TCommandEnabler &tce);
	void EditBNEnable(TCommandEnabler &tce);
	void DeleteBNEnable(TCommandEnabler &tce);
	void ListBNClicked();
//{{TSenderListDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TSenderListDialog);
};    //{{TSenderListDialog}}
