//{{TEditBanConSistDialog Implementation}}

TEditBanConSistDialog::TEditBanConSistDialog(TWindow* parent, TBanConSist *tBanConSist, bool edit, bool tActive, int resId)
:
	TEditDialog(parent, tBanConSist, edit, resId), active(tActive)
{
	banConSist = new TLongFace(this, IDC_EDIT_BANCONSIST_BANCONSIST, "F_BANCONSIST_LONG", SIZE_OF_BANCONSIST);
	activeFlag = new TCheckFace(this, IDC_EDIT_BANCONSIST_ACTIVE, "F_FLAGS_ACTIVE");
	new TStringFace(this, IDC_EDIT_BANCONSIST_TEXT, "F_TEXT", SIZE_OF_BANCONSIST_TEXT);
}

void TEditBanConSistDialog::SetupWindow()
{
	TEditDialog::SetupWindow();

	banConSist->EnableWindow(!edit);

	if (active)
	{
		activeFlag->SetCheck(BF_CHECKED);
		activeFlag->EnableWindow(false);
	}
}
