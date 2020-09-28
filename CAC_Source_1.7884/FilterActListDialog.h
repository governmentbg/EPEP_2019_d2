//{{TDialog = TFilterActListDialog}}
class TFilterActListDialog : public TGroupListDialog
{
public:
	TFilterActListDialog(TWindow* parent, TUnfilteredActsGroup *unfiltered);

protected:
	TFilterActListWindow *acts;

//{{TFilterActListDialogRSP_TBL_BEGIN}}
	void FilterBNClicked();
	void QueryBNClicked();
	void RefreshBNClicked();
//{{TFilterActListDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TFilterActListDialog);
};    //{{TFilterActListDialog}}
