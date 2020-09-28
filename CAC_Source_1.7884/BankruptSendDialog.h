class TBankruptSendDialog : public TScaleDialog
{
public:
	TBankruptSendDialog(TWindow* parent, TLawsuit *tLawsuit, int resId = IDD_BANKRUPT_SEND);
	virtual ~TBankruptSendDialog();

protected:
	TLawsuit *lawsuit;
	TStatic *header;
	TBankruptSendListBox *events;

//{{TBankruptSendDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TBankruptSendDialogVIRTUAL_END}}

//{{TBankruptSendDialogRSP_TBL_BEGIN}}
	void ExportBNClicked();
	void QueryBNClicked();
DECLARE_RESPONSE_TABLE(TBankruptSendDialog);
//{{TBankruptSendDialogRSP_TBL_END}}
};    //{{TBankruptSendDialog}}
