TZZDNBookDialog::TZZDNBookDialog(TWindow* parent, TZZDNBookGroup *group)
	: TBookDialog(parent, group, Type, IDD_ZZDN_BOOK, true)
{
	autogen = new TLongFace(this, IDC_ZZDN_BOOK_AUTOGEN, "F_AUTO_SERIAL_NO", SIZE_OF_NO);
}

void TZZDNBookDialog::SetupWindow()
{
	TBookDialog::SetupWindow();

	type->SetXCrit(C2S[TYPE_CITIZEN_LAW]);
}
