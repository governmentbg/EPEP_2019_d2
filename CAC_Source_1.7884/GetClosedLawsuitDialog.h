//{{TDialog = TGetClosedLawsuitDialog}}
class TGetClosedLawsuitDialog : public TGetExternContainerDialog
{
public:
	TGetClosedLawsuitDialog(TWindow* parent, TRCDKeyGroup **grouptr, CDate *tSessionDate, const TExternSession *suggest,
		bool forceExcess);

protected:
	CDate *sessionDate;
	TCharAliasFace *sesKind;
	TDateFace *sesDate;

//{{TGetClosedLawsuitDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TGetClosedLawsuitDialogVIRTUAL_END}}

//{{TGetClosedLawsuitDialogRSP_TBL_BEGIN}}
	void OKBNClicked();
//{{TGetClosedLawsuitDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TGetClosedLawsuitDialog);
};    //{{TGetClosedLawsuitDialog}}
