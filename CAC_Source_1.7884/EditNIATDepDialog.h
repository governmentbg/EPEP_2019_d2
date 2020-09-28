//{{TDialog = TEditNIATDepDialog}}
class TEditNIATDepDialog : public TEditNamedDialog
{
public:
	TEditNIATDepDialog(TWindow* parent, TSender *sender, bool edit, int resId = IDD_EDIT_NIA_TDEP);

	DEFINE_GROUP_TYPE(TSender, Sender)

protected:
	TAreaAliasFace *area;

//{{TEditNIATDepDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TEditNIATDepDialogVIRTUAL_END}}

//{{TEditNIATDepDialogRSP_TBL_BEGIN}}
	void OKBNClicked();
//{{TEditNIATDepDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TEditNIATDepDialog);
};    //{{TEditNIATDepDialog}}
