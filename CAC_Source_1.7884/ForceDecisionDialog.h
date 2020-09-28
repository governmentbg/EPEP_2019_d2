//{{TDialog = TForceDecisionDialog}}
class TForceDecisionDialog : public TForceDateDialog
{
public:
	TForceDecisionDialog(TWindow* parent, TDecision *decision, TLawsuit *lawsuit);

protected:
	DEFINE_GROUP_TYPE(TDecision, Decision)
	virtual bool IsValid();

//{{TForceDecisionDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TForceDecisionDialogVIRTUAL_END}}

//{{TForceDecisionDialogRSP_TBL_BEGIN}}
	void OKBNClicked();
//{{TForceDecisionDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TForceDecisionDialog);
};    //{{TForceDecisionDialog}}
