//{{TDialog = TOpenResultDialog}}
class TOpenResultDialog : public TSessionResultDialog
{
public:
	TOpenResultDialog(TWindow* parent, TSession *session, TLawsuit *lawsuit);

protected:
	TStatic *others;
	TStatic *header1;
	TStatic *header2;
	TCheckFace *flagsClosedD;	// 2007:240
#if COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
	TCheckFace *flagsProceed;
	TCheckFace *cutDown;
	TRadioFace *cutDownP1;		// 2014:163
	TRadioFace *cutDownP2;		// 2014:163
#endif  // AREAL || MILITARY || REGIONAL

	DEFINE_GROUP_TYPE(TSession, Session)
	virtual bool IsValid();

//{{TOpenResultDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TOpenResultDialogVIRTUAL_END}}

//{{TOpenResultDialogRSP_TBL_BEGIN}}
	void OKBNClicked();
	void ResultCBNSelChange();
	void CutDownBNClicked();
	void RightBNClicked();
	void JudgeCBNSelChange();
//{{TOpenResultDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TOpenResultDialog);
};    //{{TOpenResultDialog}}
