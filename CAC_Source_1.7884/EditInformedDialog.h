//{{TDialog = TEditInformedDialog}}
class TEditInformedDialog : public TGroupDialog
{
public:
	TEditInformedDialog(TWindow* parent, TInformed *informed, bool edit, int resId = IDD_EDIT_INFORMED);

protected:
	bool edit;

//{{TEditInformedDialogRSP_TBL_BEGIN}}
	void OKBNClicked();
//{{TEditInformedDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TEditInformedDialog);
};    //{{TEditInformedDialog}}
