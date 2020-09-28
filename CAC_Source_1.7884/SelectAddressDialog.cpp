DEFINE_RESPONSE_TABLE1(TSelectAddressDialog, TAddressListDialog)
//{{TSelectAddressDialogRSP_TBL_BEGIN}}
	EV_BN_CLICKED(IDADD, AddBNClicked),
	EV_COMMAND_ENABLE(IDADD, AddBNEnable),
	EV_BN_CLICKED(IDEDIT, EditBNClicked),
	EV_COMMAND_ENABLE(IDEDIT, EditBNEnable),
	EV_BN_CLICKED(IDDELETE, DeleteBNClicked),
	EV_COMMAND_ENABLE(IDDELETE, DeleteBNEnable),
	EV_BN_CLICKED(IDC_SELECT_ADDRESS_FOREIGN, ForeignBNClicked),
	EV_COMMAND_ENABLE(IDC_SELECT_ADDRESS_FOREIGN, ForeignBNEnable),
	EV_BN_CLICKED(IDOK, OKBNClicked),
	EV_COMMAND_ENABLE(IDOK, OKBNEnable),
//{{TSelectAddressDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TSelectAddressDialog Implementation}}

TSelectAddressDialog::TSelectAddressDialog(TWindow* parent, TUCNGroup *ucnGroup, TAnyAddress **tAddress, int addrMode,
	bool intermediate, int resId)
:
	TAddressListDialog(parent, ucnGroup, addrMode, resId), address(tAddress), foreign(addrMode == ADDRESS_ALLOW_FOREIGN)
{
	buttons[false] = new TPushButton(this, IDADD);
	buttons[true] = new TPushButton(this, IDC_SELECT_ADDRESS_FOREIGN, IDCOPY);

	if (intermediate)
		buttons[IDCANCEL] = new TPushButton(this, IDOK);
	else
		buttons[IDCANCEL] = new TPushButton(this, IDCANCEL, IDEND);
}

void TSelectAddressDialog::AddAddress(bool foreign)
{
	addresses->Add(foreign);

	if (GetFocus() == addresses->HWindow)
		SetDefault(buttons[foreign], buttons[IDCANCEL]);
}

void TSelectAddressDialog::AddBNClicked()
{
	AddAddress(false);
}

void TSelectAddressDialog::AddBNEnable(TCommandEnabler &tce)
{
	tce.Enable(addresses->Count(false) < (COURT_LOCK ? 2 : 99));
}

void TSelectAddressDialog::EditBNClicked()
{
	addresses->Edit();
}

void TSelectAddressDialog::EditBNEnable(TCommandEnabler &tce)
{
	tce.Enable(addresses->Group());
}

void TSelectAddressDialog::DeleteBNClicked()
{
	addresses->Delete(IDDELETE);
}

void TSelectAddressDialog::DeleteBNEnable(TCommandEnabler &tce)
{
	TAnyAddress *address = addresses->Address();
	tce.Enable(address && address->address != 1);
}

void TSelectAddressDialog::ForeignBNClicked()
{
	AddAddress(true);
}

void TSelectAddressDialog::ForeignBNEnable(TCommandEnabler &tce)
{
	tce.Enable(foreign && addresses->Count(true) < (COURT_LOCK ? 1 : 99));
}

void TSelectAddressDialog::OKBNClicked()
{
	*address = addresses->OK();

	if (*address)
		CmOk();
}

void TSelectAddressDialog::OKBNEnable(TCommandEnabler &tce)
{
	TAnyAddress *address = addresses->Address();
	tce.Enable(address && (address->address > 0 || foreign));
}
