//{{TDialog = TEditMotivesDialog}}
class TEditMotivesDialog : public TWhateverTextBaseDialog
{
public:
	TEditMotivesDialog(TJudicialActDialog *parent, TDecision *decision, bool tEdit, char tMotivesKind, const char *title,
		const char *what);

protected:
	bool edit;
	TElectricTrouble trouble;
	char decisionKind;
	char motivesKind;
	TJudicialActDialog *judicialActDialog;

	DEFINE_GROUP_TYPE(TDatedContainer, Container)

//{{TEditMotivesDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TEditMotivesDialogVIRTUAL_END}}

//{{TEditMotivesDialogRSP_TBL_BEGIN}}
	void OKBNClicked();
	void AttachBNClicked();
	void SwapBNClicked();
	void RightBNClicked();
//{{TEditMotivesDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TEditMotivesDialog);
};    //{{TEditMotivesDialog}}
