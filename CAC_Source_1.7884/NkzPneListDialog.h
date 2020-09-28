//{{TDialog = TNkzPneListDialog}}
class TNkzPneListDialog : public TFzlGroupListDialog
{
public:
	TNkzPneListDialog(TWindow* parent, NPRFZLPNE *tNPRFZLPNE, FZL *fzl, NPR *npr, bool tEdit,
		int resId = IDD_NKZPNE_LIST);

protected:
	bool edit;
	NPRFZLPNE *nprfzlpne;
	TNkzPneListWindow *nkzpnes;

//{{TNkzPneListDialogVIRTUAL_BEGIN}}
	virtual bool CanClose();
//{{TNkzPneListDialogVIRTUAL_END}}

//{{TNkzPneListDialogRSP_TBL_BEGIN}}
	void DeleteBNEnable(TCommandEnabler &tce);
	void CancelBNClicked();
	void EvClose();
//{{TNkzPneListDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TNkzPneListDialog);
};    //{{TNkzPneListDialog}}
