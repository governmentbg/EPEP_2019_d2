class TNprFzlPneListDialog : public TFzlGroupListDialog
{
public:
	TNprFzlPneListDialog(TWindow* parent, FZL *fzl, SBE *sbe, int resId = IDD_NPRFZLPNE_LIST);

protected:
	TNprFzlPneListWindow *nprfzlpnes;

//{{TNprFzlPneListDialogRSP_TBL_BEGIN}}
	void AddBNEnable(TCommandEnabler &tce);
//{{TNprFzlPneListDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TNprFzlPneListDialog);
};
