//{{TDialog = TInRegNewXDialog}}
class TInRegNewXDialog : public TEditIncomingDialog
{
public:
	TInRegNewXDialog(TWindow* parent, TInReg *inReg, const char *kinds, int resId = IDD_INREG_NEW_X);

protected:
	char types[SIZE_OF_TYPES];

	TCharAliasFace *lawsuitType;
	TLongFace *lawsuitNo;
	TYearFace *lawsuitYear;
	TDateFace *sessionDate;
	TCharAliasFace *decisionKind;
	TLongFace *decisionNo;
	TYearFace *decisionYear;

	void SetDirection();

	DEFINE_GROUP_TYPE(TInReg, InReg)
	virtual bool IsValid();

//{{TInRegNewXDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TInRegNewXDialogVIRTUAL_END}}

//{{TInRegNewXDialogRSP_TBL_BEGIN}}
	void KindCBNSelChange();
	void LawsuitTypeCBNSelChange();
	void OKBNClicked();
	void DecisionKindCBNSelChange();
	void DecisionNoKillFocus();
	void DecisionYearKillFocus();
//{{TInRegNewXDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TInRegNewXDialog);
};    //{{TInRegNewXDialog}}
