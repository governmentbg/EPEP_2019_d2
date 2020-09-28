//{{TDialog = TSbeFzlListDialog}}
class TSbeFzlListDialog : public TGroupListDialog
{
public:
	TSbeFzlListDialog(TWindow* parent, SBE *tSBE, bool tEdit, int resId = IDD_SBE_FZL_LIST);

protected:
	NPR *npr;
	bool edit;
	TXArray<FZL> include;
	TXArray<NPRFZLPNE> allLinks;
	TFzlComboBox *fzl;
	TSbeFzlListWindow *fzls;

//{{TSbeFzlListDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
	virtual bool CanClose();
//{{TSbeFzlListDialogVIRTUAL_END}}

//{{TSbeFzlListDialogRSP_TBL_BEGIN}}
	void AddBNClicked();
	void AddBNEnable(TCommandEnabler &tce);
	void EditBNClicked();
	void EditBNEnable(TCommandEnabler &tce);
	void DeleteBNClicked();
	void DeleteBNEnable(TCommandEnabler &tce);
	void CancelBNClicked();
	void EvClose();
//{{TSbeFzlListDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TSbeFzlListDialog);
};  //{{TSbeFzlListDialog}}
