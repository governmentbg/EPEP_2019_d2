//{{TDialog = TEditDialog}}
class TEditDialog : public TGroupDialog
{
public:
	TEditDialog(TWindow* parent, TGroup *group, bool tEdit, int resId);

protected:
	bool edit;

//{{TEditDialogRSP_TBL_BEGIN}}
	void OKBNClicked();
//{{TEditDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TEditDialog);
};    //{{TEditDialog}}
