//{{TDialog = TInRegEPortalDialog}}
class TInRegEPortalDialog : public TEditIncomingDialog
{
public:
	TInRegEPortalDialog(TWindow* parent, TInReg *inReg, bool edit, bool hasRegistration, bool hasSides,
		TSideWind &tSideWind);

protected:
	bool hasRegistration;
	bool hasSides;
	TSideWind &sideWind;	// 2018-10-26: LPR: +related

	TStringFace *text;
	TButton *editSide;
	TStringFace *eportalMail;
	TStatic *eportalMailLabel;
	TRadioFace *registration;
	TRadioFace *grantAccess;
	TRadioFace *revokeAccess;
	TStatic *sideWindNames;

	void SideNamesChanged();

	DEFINE_GROUP_TYPE(TInReg, InReg)
	virtual bool IsValid();

//{{TInRegEPortalDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TInRegEPortalDialogVIRTUAL_END}}

//{{TInRegEPortalDialogRSP_TBL_BEGIN}}
	void EditSideBNClicked();
	void EditSideBNEnable(TCommandEnabler &tce);
	void EPortalModeBNClicked();
//{{TInRegEPortalDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TInRegEPortalDialog);
};    //{{TInRegEPortalDialog}}
