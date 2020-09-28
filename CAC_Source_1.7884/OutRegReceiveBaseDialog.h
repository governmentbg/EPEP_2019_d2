//{{TDialog = TOutRegReceiveBaseDialog}}
class TOutRegReceiveBaseDialog : public TTroubleDialog
{
public:
	TOutRegReceiveBaseDialog(TWindow* parent, TOutReg *outReg, const char *tResults,
		int resId = IDD_OUTREG_RECEIVE_BASE);

protected:
	TStatic *header1;
	TStatic *header2;
	TDateFace *returned;
	TCharAliasFace *result;
	TStringFace *remark;

	TCheckFace *registry;
	TCheckFace *restart;

	bool oldRestart;
	bool higher;
	const char *results;

	DEFINE_GROUP_TYPE(TOutReg, OutReg)
	virtual bool IsValid();

	void Print(long flags);

//{{TOutRegReceiveBaseDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();		// 2007:242 LPR: to init headers
//{{TOutRegReceiveBaseDialogVIRTUAL_END}}

//{{TOutRegReceiveBaseDialogRSP_TBL_BEGIN}}
	void OKBNClicked();
	void PrintBNClicked();
	void RightBNClicked();
//{{TOutRegReceiveBaseDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TOutRegReceiveBaseDialog);
};    //{{TOutRegReceiveBaseDialog}}
