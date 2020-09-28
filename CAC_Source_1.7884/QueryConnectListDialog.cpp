DEFINE_RESPONSE_TABLE1(TQueryConnectListDialog, TQuickDialog)
//{{TQueryConnectListDialogRSP_TBL_BEGIN}}
	EV_BN_CLICKED(IDOK, OKBNClicked),
	EV_COMMAND_ENABLE(IDOK, OKBNEnable),
	EV_BN_CLICKED(IDPRINT, PrintBNClicked),
//{{TQueryConnectListDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TQueryConnectListDialog Implementation}}

TQueryConnectListDialog::TQueryConnectListDialog(TWindow* parent, TConnect *connect, int resId)
:
	TQuickDialog(parent, resId)
{
	connects = new TQueryConnectListBox(this, IDC_QUERY_CONNECT_LIST_CONNECTS, connect);
}

void TQueryConnectListDialog::OKBNClicked()
{
	query_key(this, ((TConnect *) connects->Group())->key);
}

void TQueryConnectListDialog::OKBNEnable(TCommandEnabler &tce)
{
	tce.Enable(connects->Group());
}

void TQueryConnectListDialog::PrintBNClicked()
{
	connects->Print();
}
