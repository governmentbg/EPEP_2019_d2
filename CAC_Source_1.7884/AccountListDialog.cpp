DEFINE_RESPONSE_TABLE1(TAccountListDialog, TGroupListDialog)
//{{TAccountListDialogRSP_TBL_BEGIN}}
	EV_CBN_SELCHANGE(IDC_SENDER_LIST_UCN_TYPE, UCNTypeCBNSelChange),
	EV_COMMAND_ENABLE(IDADD, AddBNEnable),
	EV_COMMAND_ENABLE(IDDELETE, DeleteBNEnable),
	EV_BN_CLICKED(IDPRINT, PrintBNClicked),
//{{TAccountListDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TAccountListDialog Implementation}}

TAccountListDialog::TAccountListDialog(TWindow* parent)
:
	TGroupListDialog(parent, IDD_ACCOUNT_LIST)
{
	ucnType = new TCharAliasFace(this, IDC_ACCOUNT_LIST_UCN_TYPE, "F_UCN_TYPE", UCNType, UCN_ACCOUNTS);
	accounts = new TAccountListWindow(this, IDC_GROUP_LIST_WINDOW);
}

void TAccountListDialog::SetupWindow()
{
	TGroupListDialog::SetupWindow();

	ucnType->SetC(UCN_EXEC_LIST);
	UCNTypeCBNSelChange();
}

void TAccountListDialog::UCNTypeCBNSelChange()
{
	accounts->SetUCNType(ucnType->C());
}

void TAccountListDialog::AddBNEnable(TCommandEnabler &tce)
{
	tce.Enable(ucnType->C() != UCN_INSTANCE_SELF);
}

void TAccountListDialog::DeleteBNEnable(TCommandEnabler &tce)
{
	tce.Enable(ucnType->C() != UCN_INSTANCE_SELF);
}

void TAccountListDialog::PrintBNClicked()
{
	accounts->Print();
}
