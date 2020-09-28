//{{TEditCompanyDialog Implementation}}

TEditCompanyDialog::TEditCompanyDialog(TWindow* parent, TDamn *damn, bool edit, bool full, bool stat, bool tReReg)
:
	TEditJudicialDialog(parent, damn, edit, full, stat, IDD_EDIT_COMPANY), reReg(tReReg)
{
	new TCharAliasFace(this, IDC_EDIT_COMPANY_TYPE, "F_UCN_TYPE", UCNType, NULL);
	new TStringFace(this, IDC_EDIT_COMPANY_UCN, "F_UCN", SIZE_OF_UCN);
	companyType = new TCharAliasFace(this, IDC_EDIT_COMPANY_COMPANY_TYPE, "F_COMPANY_TYPE", CompanyType, NULL);
	name = new TDamnNameFace(this, IDC_EDIT_COMPANY_NAME, "F_NAME", SIZE_OF_DAMN_NAME);
	bulstat = new TBulStatFace(this, IDC_EDIT_COMPANY_BULSTAT, "F_BULSTAT");
	reRegister = new TDateFace(this, IDC_EDIT_COMPANY_REREGISTER, "F_REREGISTER", false);
}

void TEditCompanyDialog::SetupWindow()
{
	TEditJudicialDialog::SetupWindow();

	companyType->EnableWindow(full);
	bulstat->EnableWindow(!reReg);
	reRegister->EnableWindow(edit);
}

bool TEditCompanyDialog::IsValid()
{
	bool isValid = TEditJudicialDialog::IsValid();

	if (isValid)
	{
		if (reReg && reRegister->Date().Empty())
			isValid = ask("Датата на пререгистрация в АВ не е попълнена. Желаете ли да продължите?");
	}

	return isValid;
}
