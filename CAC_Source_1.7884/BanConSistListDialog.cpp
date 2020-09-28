DEFINE_RESPONSE_TABLE1(TBanConSistListDialog, TQuickDialog)
//{{TBanConSistListDialogRSP_TBL_BEGIN}}
	EV_BN_CLICKED(IDADD, AddBNClicked),
	EV_COMMAND_ENABLE(IDADD, AddBNEnable),
	EV_BN_CLICKED(IDEDIT, EditBNClicked),
	EV_COMMAND_ENABLE(IDEDIT, EditBNEnable),
	EV_BN_CLICKED(IDDELETE, DeleteBNClicked),
	EV_COMMAND_ENABLE(IDDELETE, DeleteBNEnable),
	EV_BN_CLICKED(IDPRINT, PrintBNClicked),
	EV_BN_CLICKED(IDOK, OKBNClicked),
	EV_BN_CLICKED(IDENTER, EnterBNClicked),
	EV_BN_CLICKED(IDEXEC, ExecBNClicked),
	EV_LVN_ITEMCHANGING(IDC_BANCONSIST_LIST_BANCONSISTS, BanConSistLVNItemChanging),
//{{TBanConSistListDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TBanConSistListDialog Implementation}}

TBanConSistListDialog::TBanConSistListDialog(TWindow* parent, TText *tField, int resId)
:
	TQuickDialog(parent, resId), field(tField)
{
	banConSists = new TBanConSistListWindow(this, IDC_BANCONSIST_LIST_BANCONSISTS, field != NULL);
}

void TBanConSistListDialog::Action(int command)
{
	TBanConSist *banConSist = (TBanConSist *) banConSists->Group();

	if (banConSist)
		field->SetS(str(banConSist->text));

	CloseDialog(command);
}

void TBanConSistListDialog::AddBNClicked()
{
	banConSists->Add();
}

void TBanConSistListDialog::AddBNEnable(TCommandEnabler &tce)
{
	tce.Enable(can_func(RIGHT_NOM));
}

void TBanConSistListDialog::EditBNClicked()
{
	banConSists->Edit();
}

void TBanConSistListDialog::EditBNEnable(TCommandEnabler &tce)
{
	tce.Enable(can_func(RIGHT_NOM) && banConSists->Group());
}

void TBanConSistListDialog::DeleteBNClicked()
{
	banConSists->Delete();
}

void TBanConSistListDialog::DeleteBNEnable(TCommandEnabler &tce)
{
	tce.Enable(banConSists->Group());
}

void TBanConSistListDialog::PrintBNClicked()
{
	banConSists->Print();
}

void TBanConSistListDialog::OKBNClicked()
{
	Action(IDOK);
}

void TBanConSistListDialog::EnterBNClicked()
{
	Action(IDENTER);
}

void TBanConSistListDialog::ExecBNClicked()
{
	Action(IDEXEC);
}

bool TBanConSistListDialog::BanConSistLVNItemChanging(TLwNotify &lwn)
{
	return banConSists->ItemChanging(lwn);
}
