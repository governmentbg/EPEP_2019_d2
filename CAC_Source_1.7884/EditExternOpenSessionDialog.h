//{{TDialog = TEditExternOpenSessionDialog}}
class TEditExternOpenSessionDialog : public TEditExternSessionDialog
{
public:
	TEditExternOpenSessionDialog(TWindow* parent, TExternSession *session, bool edit);

protected:
	TTimeFace *time;

	virtual bool IsValid();

//{{TEditExternOpenSessionDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TEditExternOpenSessionDialogVIRTUAL_END}}

//{{TEditExternOpenSessionRSP_TBL_BEGIN}}
	void KindCBNSelChange();
	void TypeCBNSelChange();
//{{TEditExternSessionRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TEditExternOpenSessionDialog);
};    //{{TEditExternOpenSessionDialog}}
