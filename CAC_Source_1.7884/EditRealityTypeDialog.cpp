//{{TEditRealityTypeDialog Implementation}}

TEditRealityTypeDialog::TEditRealityTypeDialog(TWindow* parent, TRealityType *tRealityType, bool edit, int resId)
:
	TEditDialog(parent, tRealityType, edit, resId)
{
	realityType = new TLongFace(this, IDC_EDIT_REALITY_TYPE_REALITY, tRealityType->longValueName, SIZE_OF_REALITY_TYPE);
	new TStringFace(this, IDC_EDIT_REALITY_TYPE_NAME, "F_NAME", SIZE_OF_REALITY_NAME);
	new TCheckFace(this, IDC_EDIT_REALITY_TYPE_ACTIVE, "F_FLAGS_ACTIVE");
	new TCheckFace(this, IDC_EDIT_REALITY_TYPE_HIREJ, "F_FLAGS_HIREJ");
	new TCheckFace(this, IDC_EDIT_REALITY_TYPE_BOOK, "F_FLAGS_BOOK");
}

void TEditRealityTypeDialog::SetupWindow()
{
	TEditDialog::SetupWindow();
	realityType->EnableWindow(!edit);
}
