//{{TEditStreetDialog Implementation}}

TEditStreetDialog::TEditStreetDialog(TWindow* parent, TStreet *tStreet, bool edit, int resId)
:
	TEditDialog(parent, tStreet, edit, resId)
{
	uclp = new TUCLPAliasFace(this, IDC_EDIT_STREET_UCLP, "F_UCLP", tStreet->uclp);
	street = new TLongFace(this, IDC_EDIT_STREET_STREET, "F_STREET_LONG", SIZE_OF_STREET);
	new TStringFace(this, IDC_EDIT_STREET_NAME, "F_NAME", SIZE_OF_STREET_NAME);
}

void TEditStreetDialog::SetupWindow()
{
	TEditDialog::SetupWindow();
	street->EnableWindow(!edit);
}
