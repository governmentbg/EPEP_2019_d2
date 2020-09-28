//{{TDialog = TOutRegEditDialog}}
class TOutRegEditDialog : public TOutRegSendBaseDialog
{
public:
	TOutRegEditDialog(TWindow* parent, TOutReg *outReg, TInReg *inReg, TConnect **connects);

protected:
	TButton *ok;
	TButton *enter;
	TCheckFace *resend;

	DEFINE_GROUP_TYPE(TOutReg, OutReg)

//{{TOutRegEditDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TOutRegEditDialogVIRTUAL_END}}

//{{TOutRegEditDialogRSP_TBL_BEGIN}}
	void OKBNClicked();
	void PrintBNClicked();
//{{TOutRegEditDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TOutRegEditDialog);
};    //{{TOutRegEditDialog}}
