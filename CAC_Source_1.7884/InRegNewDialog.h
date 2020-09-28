//{{TDialog = TInRegNewDialog}}
class TInRegNewDialog : public TInRegNewBaseDialog
{
public:
	TInRegNewDialog(TWindow* parent, TInReg *inReg, const char *kinds, const TLawsuit *lawsuit);

protected:
	TPushButton *search;		// 2016:097 +related

	char sesKinds[SIZE_OF_KINDS];

	virtual bool IsValid();

//{{TInRegNewDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TInRegNewDialogVIRTUAL_END}}

//{{TInRegNewDialogRSP_TBL_BEGIN}}
	void OKBNClicked();
	void KindCBNSelChange();
	void LawsuitTypeCBNSelChange();
	void EditBNClicked();
	void SessionKindCBNSelChange();
	void DecisionKindCBNSelChange();
	void SearchBNEnable(TCommandEnabler &tce);
	void SearchBNClicked();
//{{TInRegNewDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TInRegNewDialog);
};    //{{TInRegNewDialog}}
