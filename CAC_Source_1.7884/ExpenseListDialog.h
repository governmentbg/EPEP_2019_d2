//{{TDialog = TExpenseListDialog}}
class TExpenseListDialog : public TGroupListDialog
{
public:
	TExpenseListDialog(TWindow* parent, TSideWind *tSideWind, TRCDKeyContainer *tContainer, TSession *session,
		TLawsuit *lawsuit, int resId);

protected:
	TSideWind *sideWind;
	TRCDKeyContainer *container;
	TExpenseListWindow *expenses;
	TButton *penality;
	TStatic *header;

//{{TExpenseListDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TExpenseListDialogVIRTUAL_END}}

//{{TExpenseListDialogRSP_TBL_BEGIN}}
	void PenalityBNClicked();
	void RewardBNClicked();
	void RewardBNEnable(TCommandEnabler &tce);
//{{TExpenseListDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TExpenseListDialog);
};    //{{TExpenseListDialog}}
