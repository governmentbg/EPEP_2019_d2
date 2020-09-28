//{{TDialog = TGroupListDialog}}
class TGroupListDialog : public TQuickDialog
{
public:
	TGroupListDialog(TWindow* parent, int resId);

protected:
	TGroupListWindow *glist;

	void SelectEnable(TCommandEnabler &tce);

//{{TGroupListDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TGroupListDialogVIRTUAL_END}}

//{{TGroupListDialogRSP_TBL_BEGIN}}
	void AddBNClicked();
	void DeleteBNClicked();
	void EditBNClicked();
	bool ListLVNItemChanging(TLwNotify &lwn);
//{{TGroupListDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TGroupListDialog);
};    //{{TGroupListDialog}}
