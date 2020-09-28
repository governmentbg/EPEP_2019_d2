//{{TDialog = TAttachActListDialog}}
class TAttachActListDialog : public TGroupListDialog
{
public:
	TAttachActListDialog(TWindow* parent, TGArray<TIndexContainer> *array);

protected:
	TAttachActListWindow *attaches;

//{{TAttachActListDialogRSP_TBL_BEGIN}}
	void AttachBNClicked();
//{{TAttachActListDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TAttachActListDialog);
};    //{{TAttachActListDialog}}
