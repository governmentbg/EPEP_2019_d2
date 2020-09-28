DEFINE_RESPONSE_TABLE1(TCurrencyListDialog, TGroupListDialog)
//{{TUserListDialogRSP_TBL_BEGIN}}
	EV_COMMAND_ENABLE(IDDELETE, DeleteBNEnable),
	EV_BN_CLICKED(IDPRINT, PrintBNClicked),
//{{TUserListDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TCurrencyListDialog Implementation}}

TCurrencyListDialog::TCurrencyListDialog(TWindow* parent, int resId)
:
	TGroupListDialog(parent, resId)
{
	currences = new TCurrencyListWindow(this, IDC_GROUP_LIST_WINDOW);
}

void TCurrencyListDialog::DeleteBNEnable(TCommandEnabler &tce)
{
	TCurrency *currency = (TCurrency *) currences->Group();
	tce.Enable(currency && strcmp(currency->currency, CURRENCY_BGN));
}

void TCurrencyListDialog::PrintBNClicked()
{
	currences->Print();
}
