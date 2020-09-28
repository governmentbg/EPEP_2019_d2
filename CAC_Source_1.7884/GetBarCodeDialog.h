//{{TDialog = TGetBarCodeDialog}}
class TGetBarCodeDialog : public TQuickDialog
{
public:
	TGetBarCodeDialog(TWindow* parent, long *tCourt, long *tRequestId, char *tCES, int resId = IDD_GET_BARCODE);

protected:
	long *court, *requestId;
	char *ces;
	TBarCodeFace *barcode;

	virtual bool IsValid();

//{{TGetBarCodeDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TGetBarCodeDialogVIRTUAL_END}}

//{{TGetBarCodeDialogRSP_TBL_BEGIN}}
	void OKBNClicked();
	void EvTimer(uint timerId);
//{{TGetBarCodeDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TGetBarCodeDialog);
};    //{{TGetBarCodeDialog}}
