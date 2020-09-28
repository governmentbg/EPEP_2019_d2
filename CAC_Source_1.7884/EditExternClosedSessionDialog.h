//{{TDialog = TEditExternClosedSessionDialog}}
class TEditExternClosedSessionDialog : public TEditExternSessionDialog
{
public:
	TEditExternClosedSessionDialog(TWindow* parent, TExternSession *session, bool edit);

protected:
	TIndexFace *index;

//{{TEditExternClosedSessionDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TEditExternClosedSessionDialogVIRTUAL_END}}

//{{TEditExternSessionRSP_TBL_BEGIN}}
	void OKBNClicked();
//{{TEditExternSessionRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TEditExternClosedSessionDialog);
};    //{{TEditExternClosedSessionDialog}}
