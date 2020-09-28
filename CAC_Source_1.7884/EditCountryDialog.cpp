//{{TEditCountryDialog Implementation}}

TEditCountryDialog::TEditCountryDialog(TWindow* parent, TCountry *tCountry, bool edit)
:
	TEditDialog(parent, tCountry, edit, IDD_EDIT_COUNTRY)
{
	country = new TLatinFace(this, IDC_EDIT_COUNTRY_COUNTRY, "F_COUNTRY_STRING", SIZE_OF_COUNTRY);
	new TStringFace(this, IDC_EDIT_COUNTRY_NAME, "F_NAME", SIZE_OF_UCLP_NAME);
}

void TEditCountryDialog::SetupWindow()
{
	TEditDialog::SetupWindow();
	country->EnableWindow(!edit);
}
