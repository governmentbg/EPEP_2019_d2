DEFINE_RESPONSE_TABLE1(TAddressListDialog, TGroupDialog)
//{{TAddressListDialogRSP_TBL_BEGIN}}
	EV_LVN_ITEMCHANGING(IDC_ADDRESS_LIST_ADDRESSES, AddressesLVNItemChanging),
//{{TAddressListDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TAddressListDialog Implementation}}

TAddressListDialog::TAddressListDialog(TWindow* parent, TUCNGroup *ucnGroup, int addrMode, int resId)
:
	TGroupDialog(parent, ucnGroup, resId)
{
	name = new TStatic(this, IDC_ADDRESS_LIST_NAME);
	addresses = new TAddressListWindow(this, IDC_ADDRESS_LIST_ADDRESSES, ucnGroup, addrMode);
}

void TAddressListDialog::SetupWindow()
{
	TGroupDialog::SetupWindow();

	mstr m;

	UCNGroup()->Print(m);
	m.compress();
	ucngroup_extra(m, UCNGroup());
	name->SetText(str(m));
}

bool TAddressListDialog::AddressesLVNItemChanging(TLwNotify &lwn)
{
	return addresses->ItemChanging(lwn);
}
