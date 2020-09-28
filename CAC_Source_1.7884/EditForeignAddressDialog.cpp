//{{TEditForeignAddressDialog Implementation}}

TEditForeignAddressDialog::TEditForeignAddressDialog(TWindow* parent, TForeignAddress *address, bool edit)
:
	TEditDialog(parent, address, edit, IDD_EDIT_FOREIGN_ADDRESS)
{
	new TCharAliasFace(this, IDC_EDIT_FOREIGN_ADDRESS_UCN_TYPE, "F_UCN_TYPE", UCNType, NULL);
	new TStringFace(this, IDC_EDIT_FOREIGN_ADDRESS_UCN, "F_UCN", SIZE_OF_UCN);
	new TStringAliasFace(this, IDC_EDIT_FOREIGN_ADDRESS_COUNTRY, "F_COUNTRY", Country, NULL);
	new TStringFace(this, IDC_EDIT_FOREIGN_ADDRESS_FOREIGN, "F_FOREIGN", SIZE_OF_FOREIGN_ADDRESS);
}
