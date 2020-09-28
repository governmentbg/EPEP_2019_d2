DEFINE_RESPONSE_TABLE1(TBanConSideDialog, TQuickDialog)
//{{TBanConSideDialogRSP_TBL_BEGIN}}
	EV_BN_CLICKED(IDC_BANCONSIDE_CERTIFICATE, CertificateBNClicked),
	EV_BN_CLICKED(IDC_BANCONSIDE_QUERY_INCONS, QueryInconsBNClicked),
	EV_BN_CLICKED(IDQUERY, QueryBNClicked),
	EV_COMMAND_ENABLE(IDQUERY, QueryBNEnable),
	EV_BN_CLICKED(IDPRINT, PrintBNClicked),
//{{TBanConSideDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TBanConSideDialog Implementation}}

TBanConSideDialog::TBanConSideDialog(TWindow* parent, TGArray<TBankWind> *tBankWinds, int resId)
:
	TQuickDialog(parent, resId)
{
	bankWinds = new TBanConSideListBox(this, IDC_BANCONSIDE_BANK_WINDS, tBankWinds);
}

void TBanConSideDialog::CertificateBNClicked()
{
	bankWinds->Cert("ÓÄÎÑÒÎÂÅÐÅÍÈÅ");
}

void TBanConSideDialog::QueryInconsBNClicked()
{
	bankWinds->Cert("ÑÏÐÀÂÊÀ");
}

void TBanConSideDialog::QueryBNClicked()
{
	bankWinds->Query();
}

void TBanConSideDialog::QueryBNEnable(TCommandEnabler &tce)
{
	tce.Enable(bankWinds->Group());
}

void TBanConSideDialog::PrintBNClicked()
{
	bankWinds->Print();
}
