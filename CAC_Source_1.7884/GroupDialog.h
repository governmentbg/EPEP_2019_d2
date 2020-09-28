//{{TDialog = TGroupDialog}}
class TGroupDialog : protected TQuickDialog
{
public:
	TGroupDialog(TWindow* parent, TGroup *tGroup, int resId);
	virtual ~TGroupDialog();

//{{TGroupDialogVIRTUAL_BEGIN}}
	TQuickDialog::Execute;
//{{TGroupDialogRSP_TBL_END}}

private:
	TGroup *group;

protected:
	TGroup *Group() { return group; }

//{{TGroupDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TGroupDialogVIRTUAL_END}}

//{{TGroupDialogRSP_TBL_BEGIN}}
	void OKBNClicked();
//{{TGroupDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TGroupDialog);
};    //{{TGroupDialog}}
