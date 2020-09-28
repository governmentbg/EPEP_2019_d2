//{{TDialog = TEisppLawsListDialog}}
class TEisppLawsListDialog : public TQuickDialog
{
public:
	TEisppLawsListDialog(TWindow* parent, TLawsuit *lawsuit, int resId = IDD_EISPP_LAWS_LIST);

protected:
	TEisppLawsListBox *eisppLaws;

//{{TEisppLawsListDialogRSP_TBL_BEGIN}}
	void QueryBNClicked();
	void QueryBNEnable(TCommandEnabler &tce);
//{{TEisppLawsListDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TEisppLawsListDialog);
};    //{{TEisppLawsListDialog}}
