//{{TEditNamedDialog Implementation}}

TEditNamedDialog::TEditNamedDialog(TWindow* parent, TNamedGroup *ucnGroup, bool edit, bool tFull, int resId)
:
	TEditDialog(parent, ucnGroup, edit, resId), full(tFull)
{
}

void TEditNamedDialog::SetupWindow()
{
	TEditDialog::SetupWindow();
	name->EnableWindow(full);
}
