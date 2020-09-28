//{{TEditAreaDialog Implementation}}

TEditAreaDialog::TEditAreaDialog(TWindow* parent, TArea *tArea, bool edit)
:
	TEditDialog(parent, tArea, edit, IDD_EDIT_AREA)
{
	area = new TLongFace(this, IDC_EDIT_AREA_AREA, "F_AREA_LONG", SIZE_OF_AREA);
	new TStringFace(this, IDC_EDIT_AREA_NAME, "F_NAME", SIZE_OF_UCLP_NAME);
}

void TEditAreaDialog::SetupWindow()
{
	TEditDialog::SetupWindow();
	area->EnableWindow(!edit);
}
