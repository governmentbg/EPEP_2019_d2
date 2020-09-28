//{{TSelectStreetDialog Implementation}}

TSelectStreetDialog::TSelectStreetDialog(TWindow* parent, TStreet *street, int resId)
:
	TSelectDialog(parent, street, resId)
{
	uclp = new TUCLPAliasFace(this, IDC_SELECT_STREET_UCLP, "F_UCLP", street->uclp);
	glist = streets = new TStreetListBox(this, IDC_SELECT_STREET_STREETS, street->uclp);
}
