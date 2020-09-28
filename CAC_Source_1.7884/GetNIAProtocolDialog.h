//{{TDialog = TGetNIAProtocolDialog}}
class TGetNIAProtocolDialog : public TGroupDialog
{
public:
	TGetNIAProtocolDialog(TWindow* parent, TNIAProtocol *protocol, nia_match_func tMatch,
		int resId = IDD_GET_NIA_PROTOCOL);

protected:
	nia_match_func match;

	TStatic *typeLabel;
	TCharAliasFace *type;
	TLongFace *no;
	TYearFace *year;

	DEFINE_GROUP_TYPE(TNIAProtocol, Protocol)
	virtual bool IsValid();

//{{TGetNIAProtocolDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TGetNIAProtocolDialogVIRTUAL_END}}

//{{TGetNIAProtocolDialogRSP_TBL_BEGIN}}
	void OKBNClicked();
//{{TGetNIAProtocolDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TGetNIAProtocolDialog);
};    //{{TGetNIAProtocolDialog}}
