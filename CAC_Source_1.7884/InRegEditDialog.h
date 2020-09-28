//{{TDialog = TInRegEditDialog}}
class TInRegEditDialog : public TEditIncomingDialog
{
public:
	TInRegEditDialog(TWindow* parent, TInReg *inReg);

protected:
	TStringFace *text;
	TSenderAliasFace *receiver;	// 2016:084 +related

//{{TInRegEditDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TInRegEditDialogVIRTUAL_END}}

//{{TInRegEditDialogRSP_TBL_BEGIN}}
	void OKBNClicked();
//{{TInRegEditDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TInRegEditDialog);
};    //{{TInRegEditDialog}}
