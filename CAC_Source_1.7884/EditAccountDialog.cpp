//{{TEditAccountDialog Implementation}}

TEditAccountDialog::TEditAccountDialog(TWindow* parent, TAccount *account, bool edit)
:
	TEditDialog(parent, account, edit, IDD_EDIT_ACCOUNT)
{
	name = new TSenderNameFace(this, IDC_EDIT_ACCOUNT_NAME, "F_NAME_STRING", SIZE_OF_NAME);
	accountType = new TCharAliasFace(this, IDC_EDIT_ACCOUNT_ACCOUNT_TYPE, "F_ACCOUNT_TYPE", AccountType, NULL);
	new TBankFace(this, IDC_EDIT_ACCOUNT_BANK_ACCOUNT, "F_BANK_ACCOUNT", SIZE_OF_BANK_ACCOUNT);
	new TBankFace(this, IDC_EDIT_ACCOUNT_BIC, "F_BIC", SIZE_OF_BIC);
	new TStringFace(this, IDC_EDIT_ACCOUNT_BANK_NAME, "F_BANK_NAME", SIZE_OF_EXEC_TEXT);
	judicial = new TCheckFace(this, IDC_EDIT_ACCOUNT_JUDICIAL, "F_FLAGS_JUDICIAL"); 
	new TLongFace(this, IDC_EDIT_ACCOUNT_ORDER, "F_ORDER", SIZE_OF_ORDER);
}

void TEditAccountDialog::SetupWindow()
{
	TEditDialog::SetupWindow();

	if (Account()->ucnType != UCN_INSTANCE_SELF)
	{
		name->EnableWindow(true);
		judicial->EnableWindow(true);

		if (!edit)
			accountType->EnableWindow(true);
	}
}
