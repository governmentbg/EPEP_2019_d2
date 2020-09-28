//{{TDialog = TBanConSideDialog}}
class TBanConSideDialog : public TQuickDialog
{
public:
	TBanConSideDialog(TWindow* parent, TGArray<TBankWind> *tBankWinds, int resId = IDD_BANCONSIDE);

protected:
	TBanConSideListBox *bankWinds;

//{{TBanConSideDialogRSP_TBL_BEGIN}}
	void CertificateBNClicked();
	void QueryInconsBNClicked();
	void QueryBNClicked();
	void QueryBNEnable(TCommandEnabler &tce);
	void PrintBNClicked();
//{{TBanConSideDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TBanConSideDialog);
};    //{{TBanConSideDialog}}
