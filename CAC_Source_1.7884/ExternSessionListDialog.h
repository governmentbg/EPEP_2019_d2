//{{TDialog = TExternSessionListDialog}}
class TExternSessionListDialog : public TGroupListDialog
{
public:
	TExternSessionListDialog(TWindow* parent);

protected:
	TExternSessionListGroup group;
	TCharAliasFace *type;
	TDateFace *minDate;
	TDateFace *maxDate;
	TExternSessionListWindow *sessions;

//{{TExternSessionListDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TExternSessionListDialogVIRTUAL_END}}

//{{TExternSessionListDialogRSP_TBL_BEGIN}}
	void TypeCBNSelChange();
	void SearchBNClicked();
	void RightBNClicked();
	void DeleteBNClicked();
	void AssignBNEnable(TCommandEnabler &tce);
	void AssignBNClicked();
	void ReAssignBNEnable(TCommandEnabler &tce);
	void ReAssignBNClicked();
	void QueryBNEnable(TCommandEnabler &tce);
	void QueryBNClicked();
//{{TExternSessionListDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TExternSessionListDialog);
};    //{{TExternSessionListDialog}}
