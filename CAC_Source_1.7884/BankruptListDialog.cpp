DEFINE_RESPONSE_TABLE1(TBankruptListDialog, TGroupListDialog)
//{{TBankruptListDialogRSP_TBL_BEGIN}}
	EV_CBN_SELCHANGE(IDC_BANKRUPT_LIST_CREDITOR, CreditorCBNSelChange),
	EV_BN_CLICKED(IDQUERY, QueryBNClicked),
	EV_COMMAND_ENABLE(IDQUERY, QueryBNEnable),
	EV_BN_CLICKED(IDRIGHT, RightBNClicked),
	EV_BN_CLICKED(IDLOAD, LoadBNClicked),
//{{TBankruptListDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TBankruptListDialog Implementation}}

TBankruptListDialog::TBankruptListDialog(TWindow* parent)
:
	TGroupListDialog(parent, IDD_BANKRUPT_LIST)
{
	creditor = new TSenderAliasFace(this, IDC_BANKRUPT_LIST_CREDITOR, "F_CREDITOR", UCN_CREDITOR);
	bankrupts = new TBankruptListWindow(this, IDC_GROUP_LIST_WINDOW);
	new TTwinButton(this, IDQUERY, IDRIGHT);
}

void TBankruptListDialog::SetupWindow()
{
	TGroupListDialog::SetupWindow();
	CreditorCBNSelChange();
}

void TBankruptListDialog::CreditorCBNSelChange()
{
	bankrupts->SetCreditor(creditor->S());
}

void TBankruptListDialog::QueryBNClicked()
{
	bankrupts->Query(false);
}

void TBankruptListDialog::QueryBNEnable(TCommandEnabler &tce)
{
	tce.Enable(bankrupts->GetItemCount() > 0);
}

void TBankruptListDialog::RightBNClicked()
{
	bankrupts->Query(true);
}

void TBankruptListDialog::LoadBNClicked()
{
	bankrupts->Load();
}
