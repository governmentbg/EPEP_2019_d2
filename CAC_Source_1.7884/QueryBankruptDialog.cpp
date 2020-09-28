DEFINE_RESPONSE_TABLE1(TQueryBankruptDialog, TQuickDialog)
//{{TQueryBankruptDialogRSP_TBL_BEGIN}}
	EV_BN_CLICKED(IDOK, OKBNClicked),
//{{TQueryBankruptDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TQueryBankruptDialog Implementation}}

TQueryBankruptDialog::TQueryBankruptDialog(TWindow* parent, TPeriodGroup *period, TGArray<TBankrupt> *tBankrupts, int select, int resId)
:
	TPeriodDialog(parent, period, resId)
{
	bankrupts = new TBankruptListBox(this, IDC_QUERY_BANKRUPT_BANKRUPTS, tBankrupts, select);
}

void TQueryBankruptDialog::OKBNClicked()
{
	if (IsValid())
	{
		TGArray<TBankrupt> tBankrupts;

		*Group() << this;
		bankrupts->GetMark(&tBankrupts);

		TBankruptQuery(this, (TPeriodGroup *) Group(), &tBankrupts).Execute();
	}
}
