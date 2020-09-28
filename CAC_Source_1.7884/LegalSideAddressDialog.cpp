DEFINE_RESPONSE_TABLE1(TLegalSideAddressDialog, TQuickDialog)
//{{TLegalSideAddressDialogRSP_TBL_BEGIN}}
	EV_COMMAND_ENABLE(IDADDRESS, AddressBNEnable),
	EV_BN_CLICKED(IDADDRESS, AddressBNClicked),
//{{TLegalSideAddressDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TLegalSideAddressDialog Implementation}}

TLegalSideAddressDialog::TLegalSideAddressDialog(TWindow* parent, TGArray<TLegalSide> &tLaids)
:
	TQuickDialog(parent, IDD_LEGAL_SIDE_ADDRESS)
{
	laids = new TLegalSideAddressListBox(this, IDC_LEGAL_SIDE_ADDRESS_LAIDS, tLaids);
}

void TLegalSideAddressDialog::AddressBNEnable(TCommandEnabler &tce)
{
	tce.Enable(laids->Group());
}

void TLegalSideAddressDialog::AddressBNClicked()
{
	laids->Address();
}
