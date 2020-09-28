//{{TDialog = TEditSetSessionDialog}}
class TEditSetSessionDialog : public TResetSessionDialog
{
public:
	TEditSetSessionDialog(TWindow* parent, TSession *session, TLawsuit *tLawsuit, const char *kinds);

protected:
//{{TEditSetSessionDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TEditSetSessionDialogVIRTUAL_END}}

//{{TEditSetSessionDialogRSP_TBL_BEGIN}}
	void OKBNClicked();
//{{TEditSetSessionDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TEditSetSessionDialog);
};    //{{TEditSetSessionDialog}}
