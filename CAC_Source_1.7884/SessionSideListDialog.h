//{{TDialog = TSessionSideListDialog}}
class TSessionSideListDialog : public TDecisionSideListDialog
{
public:
	TSessionSideListDialog(TWindow* parent, TSession *session, TLawsuit *lawsuit, long sideMode,
		int resId = IDD_DECISION_SIDE_LIST);  // 2018-10-17: SESSION_SIDE_LIST -> SetCaption() 

protected:
	TButton *messages;	// 2003:310 To set title to "WALNS"

	DEFINE_SIDES_TYPE(TSession, Session)

//{{TSessionSideListDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TSessionSideListDialogVIRTUAL_END}}
};    //{{TSessionSideListDialog}}
