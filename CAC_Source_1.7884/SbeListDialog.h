//{{TDialog = TSbeListDialog}}
class TSbeListDialog : public TGroupListDialog
{
public:
	TSbeListDialog(TWindow* parent, NPR *tNPR, TLawsuit *tLawsuit, TDatedContainer *tContainer, TIntegerArray *lset,
		TGArray<SBE> *tSbes, int resId = IDD_SBE_LIST);

protected:
	NPR *npr;
	TLawsuit *lawsuit;
	TDatedContainer *container;
	TIntegerArray *lset;

	TSbeAliasFace *sbe;
	TSbeListWindow *sbes;

	void SbeEnable(TCommandEnabler &tce, long nmk);

//{{TSbeListDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TSbeListDialogVIRTUAL_END}}

//{{TSbeListDialogRSP_TBL_BEGIN}}
	void AddBNClicked();
	void AddBNEnable(TCommandEnabler &tce);
	void EditBNClicked();
	void EditBNEnable(TCommandEnabler &tce);
	void DeleteBNClicked();
	void DeleteBNEnable(TCommandEnabler &tce);
//{{TSbeListDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TSbeListDialog);
};  //{{TSbeListDialog}}
