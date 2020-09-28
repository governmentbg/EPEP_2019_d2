TBookEuherDialog::TBookEuherDialog(TWindow* parent, TBookEuherGroup *group)
	: TBookDialog(parent, group, Type, IDD_BOOK_EUHER, false)
{
	new TRadioFace(this, IDC_BOOK_EUHER_ORDER_BY_LEAVER, "F_ORDER_BY_LEAVER");
	new TRadioFace(this, IDC_BOOK_EUHER_ORDER_BY_EUHER_NO, "F_ORDER_BY_EUHER_NO");
}

void TBookEuherDialog::SetupWindow()
{
	TBookDialog::SetupWindow();
	types->SetS(C2S[TYPE_CITIZEN_LAW]);
}
