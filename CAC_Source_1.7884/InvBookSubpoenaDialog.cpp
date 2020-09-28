DEFINE_RESPONSE_TABLE1(TInvBookSubpoenaDialog, TFloatDialog)
	EV_BN_CLICKED(IDC_INV_SUBPOENA_BOOK_BY_SUBP_DATE, RadioBNClicked),
	EV_BN_CLICKED(IDC_INV_SUBPOENA_BOOK_BY_RECEIVED, RadioBNClicked),
	EV_CBN_SELCHANGE(IDC_INV_SUBPOENA_BOOK_SUB_KIND, TypeBNSelChange),
	EV_BN_CLICKED(IDC_INV_SUBPOENA_BOOK_NEW_FORMAT, NewFormatBNClicked),
END_RESPONSE_TABLE;

TInvBookSubpoenaDialog::TInvBookSubpoenaDialog(TWindow* parent, TInvBookSubpoenaGroup *group)
	: TFloatDialog(parent, group, IDD_INV_SUBPOENA_BOOK)
{
	build_ctypes(types, TYPE_ORIGINS, true);
	type = new TCharAliasFace(this, IDC_INV_SUBPOENA_BOOK_SUB_KIND, "F_TYPE", Type, types);
	dateLabel = new TStatic(this, IDC_INV_SUBPOENA_BOOK_DATE_LABEL);
	minDate = new TDateFace(this, IDC_INV_SUBPOENA_BOOK_MIN_DATE, "F_MIN_DATE");
	new TDateFaceUpDown(this, IDC_INV_SUBPOENA_BOOK_MIN_DATE_UPDN, minDate);
	maxDate = new TDateFace(this, IDC_INV_SUBPOENA_BOOK_MAX_DATE, "F_MAX_DATE");
	new TDateFaceUpDown(this, IDC_INV_SUBPOENA_BOOK_MAX_DATE_UPDN, maxDate);
	compositions = new TCharAutoListFace(this, IDC_INV_SUBPOENA_BOOK_COMPOSITIO, "F_COMPOSITIONS", CharComposition,
		QUERY_COMPOSITIONS, SIZE_OF_COMPOSITIONS);
	bySubpoenaDate = new TRadioFace(this, IDC_INV_SUBPOENA_BOOK_BY_SUBP_DATE, "F_BY_SUBPOENA_DATE");
	bySubpoenaReceived = new TRadioFace(this, IDC_INV_SUBPOENA_BOOK_BY_RECEIVED, "F_BY_RECEIVED_DATE");
	deliverers = new TUserListBox(this, IDC_INV_SUBPOENA_BOOK_DELIVERERS, C2S[POST_DELIVERER], USER_EMPTY);
	newFormat = new TCheckFace(this, IDC_INV_SUBPOENA_BOOK_NEW_FORMAT, "F_NEW_FORMAT");
	autogen = new TLongFace(this, IDC_INV_SUBPOENA_BOOK_AUTO_SERIAL_NO, "F_AUTO_SERIAL_NO", SIZE_OF_EVENT_NO);
}

bool TInvBookSubpoenaDialog::IsValid()
{
	bool isValid = TFloatDialog::IsValid();

	if (isValid)
	{
		if (invalid_range(minDate, maxDate))
			isValid = error("Началната дата е по-голяма от крайната.");
	}

	return isValid;
}

void TInvBookSubpoenaDialog::Action(bool execute)
{
	InvBookSubpoenaGroup()->type = type->C();
	deliverers->GetMark(&InvBookSubpoenaGroup()->deliverers);
	TFloatDialog::Action(execute);
}

void TInvBookSubpoenaDialog::RadioBNClicked()
{
	dateLabel->SetCaption(bySubpoenaReceived->GetCheck() == BF_CHECKED ? " Дата на връчване " : " Дата на изготвяне ");
}

void TInvBookSubpoenaDialog::TypeBNSelChange()
{
	if (type->C() == TYPE_REQUEST)
		compositions->SetSel(compositions->GetSelIndex(), false);

	compositions->EnableWindow(type->C() != TYPE_REQUEST);
}

void TInvBookSubpoenaDialog::NewFormatBNClicked()
{
	if (newFormat->GetCheck() == BF_CHECKED)
	{
		bySubpoenaDate->EnableWindow(false);
		bySubpoenaReceived->EnableWindow(false);
		bySubpoenaDate->SetCheck(BF_CHECKED);
		bySubpoenaReceived->SetCheck(BF_UNCHECKED);
		RadioBNClicked();
		autogen->EnableWindow(true);
	}
	else
	{
		bySubpoenaDate->EnableWindow(true);
		bySubpoenaReceived->EnableWindow(true);
		autogen->EnableWindow(false);
	}
}
