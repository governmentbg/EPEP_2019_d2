//{{TDialog = TFzlNprPneListDialog}}
class TFzlNprPneListDialog : public TFzlGroupListDialog
{
public:
	TFzlNprPneListDialog(TWindow* parent, FZL *fzl, SBE *sbe, TXArray<NPRFZLPNE> *allLinks, bool tEdit, 
		int resId = IDD_FZLNPRPNE_LIST);

protected:
	bool edit;
	TNprFzlPneComboBox *nprfzlpne;
	TNprFzlPneListWindow *nprfzlpnes;

//{{TNkzPneDialogVIRTUAL_BEGIN}}
	virtual bool CanClose();
//{{TNkzPneDialogVIRTUAL_END}}

//{{TNkzPneDialogRSP_TBL_BEGIN}}
	void AddBNClicked();
	void AddBNEnable(TCommandEnabler &tce);
	void DeleteBNClicked();
	void DeleteBNEnable(TCommandEnabler &tce);
	void CancelBNClicked();
	void EvClose();
//{{TNkzPneDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TFzlNprPneListDialog);
};    //{{TFzlNprPneListDialog}}
