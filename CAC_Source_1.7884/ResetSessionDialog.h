//{{TDialog = TResetSessionDialog}}
class TResetSessionDialog : public TSetSessionBaseDialog
{
public:
	TResetSessionDialog(TWindow* parent, TSession *session, TLawsuit *tawsuit, const char *kinds);

protected:
//{{TResetSessionDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TResetSessionDialogVIRTUAL_END}}

//{{TResetSessionDialogRSP_TBL_BEGIN}}
	void OKBNClicked();
//{{TResetSessionDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TResetSessionDialog);
};    //{{TResetSessionDialog}}
