//{{TDialog = TFzlNkzPneListDialog}}
class TFzlNkzPneListDialog : public TFzlDialog
{
public:
	TFzlNkzPneListDialog(TWindow* parent, FZL *fzl, SBE *sbe, bool tHasNkzs, bool tHasNprFzlPnes, bool tEdit);

protected:
	bool edit;
	long sbhstn;	// 2013:028 LPR: for nprfzlpne scqvid
	bool hasNkzs;
	bool hasNprFzlPnes;
	TNkzListWindow *nkzs;
	TFzlNprPneListWindow *nprfzlpnes;

	DEFINE_GROUP_TYPE(NPR, Npr)

//{{TFzlNkzPneListDialogVIRTUAL_BEGIN}}
	virtual bool CanClose();
	virtual void SetupWindow();
//{{TFzlNkzPneListDialogVIRTUAL_END}}

//{{TFzlNkzPneListDialogRSP_TBL_BEGIN}}
	void AddNkzBNClicked();
	void EditNkzBNClicked();
	void DeleteNkzBNClicked();
	void AddNkzBNEnable(TCommandEnabler &tce);
	void EditNkzBNEnable(TCommandEnabler &tce);
	void DeleteNkzBNEnable(TCommandEnabler &tce);
	bool NkzsLVNItemChanging(TLwNotify &lwn);
	void AddNprFzlPneBNClicked();
	void EditNprFzlPneBNClicked();
	void DeleteNprFzlPneBNClicked();
	void AddNprFzlPneBNEnable(TCommandEnabler &tce);
	void EditNprFzlPneBNEnable(TCommandEnabler &tce);
	void DeleteNprFzlPneBNEnable(TCommandEnabler &tce);
	bool NprFzlPnesLVNItemChanging(TLwNotify &lwn);
	void CancelBNClicked();
	void EvClose();
//{{TFzlNkzPneListDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TFzlNkzPneListDialog);
};    //{{TFzlNkzPneListDialog}}
