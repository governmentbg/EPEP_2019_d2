//{{TEditSessionPriceDialog Implementation}}

TEditSessionPriceDialog::TEditSessionPriceDialog(TWindow* parent, TSessionPrice *tSessionPrice, bool edit, int resId)
:
	TEditDialog(parent, tSessionPrice, edit, resId)
{
	date = new TDateFace(this, IDC_EDIT_SESSION_PRICE_DATE, "F_DATE", true);
	new TMoneyFace(this, IDC_EDIT_SESSION_PRICE_PRICE, "F_PRICE", SIZE_OF_SESSION_PRICE);
}

void TEditSessionPriceDialog::SetupWindow()
{
	TEditDialog::SetupWindow();
	date->EnableWindow(!edit);
}
