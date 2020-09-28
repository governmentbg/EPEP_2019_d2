//{{TDialog = TEditSurroundDialog}}
class TEditSurroundDialog : public TEditIncomingDialog
{
public:
	TEditSurroundDialog(TWindow* parent, TSurround *surround, const char *kinds);

protected:
	TStringFace *text;
	TUserAliasFace *judge;
	TCharAliasFace *lawsuitType;

	DEFINE_GROUP_TYPE(TSurround, Surround)
	virtual bool IsValid();

//{{TEditSurroundDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TEditSurroundDialogVIRTUAL_END}}

//{{TEditSurroundDialogRSP_TBL_BEGIN}}
	void KindCBNSelChange();
//{{TEditSurroundDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TEditSurroundDialog);
};    //{{TEditSurroundDialog}}
