//{{TDialog = TFzlPneListDialog}}
class TFzlPneListDialog : public TEisDialog
{
public:
	TFzlPneListDialog(TWindow* parent, SBE *sbe, TRCDKeyContainer *container, bool tEdit, int tPneMode,
		int resId = IDD_FZL_PNE_LIST);

protected:
	bool edit;
	int pneMode;
	int fzlmin;
	long vid;			// 2013:015 LPR: for ovadlo
	TEisNmrFace *nprnmr;
	TFzlListWindow *fzls;
	TPneListWindow *pnes;

	DEFINE_GROUP_TYPE(NPR, Npr)

//{{TFzlPneListDialogVIRTUAL_BEGIN}}
	virtual bool CanClose();
//{{TFzlPneListDialogVIRTUAL_END}}

//{{TFzlPneListDialogRSP_TBL_BEGIN}}
	void AddFzlBNClicked();
	void AddFzlBNEnable(TCommandEnabler &tce);
	void LinksBNClicked();
	void LinksBNEnable(TCommandEnabler &tce);
	void EditFzlBNClicked();
	void EditFzlBNEnable(TCommandEnabler &tce);
	void DeleteFzlBNClicked();
	void DeleteFzlBNEnable(TCommandEnabler &tce);
	bool FzlsLVNItemChanging(TLwNotify &lwn);
	void AddPneBNClicked();
	void AddPneBNEnable(TCommandEnabler &tce);
	void EditPneBNClicked();
	void EditPneBNEnable(TCommandEnabler &tce);
	void DeletePneBNClicked();
	void DeletePneBNEnable(TCommandEnabler &tce);
	bool PnesLVNItemChanging(TLwNotify &lwn);
	long EvFocusFace(TParam1 wParam, TParam2 lParam);
	void CancelBNClicked();
	void EvClose();
//{{TFzlPneListDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TFzlPneListDialog);
};    //{{TFzlPneListDialog}}
