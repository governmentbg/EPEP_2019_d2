DEFINE_RESPONSE_TABLE1(TInvBookDeliverDialog, TFloatDialog)
	EV_BN_CLICKED(IDC_INV_DELIVERER_BOOK_SERIAL_NO, SerialNoBNClicked),
END_RESPONSE_TABLE;

TInvBookDeliverDialog::TInvBookDeliverDialog(TWindow* parent, TInvBookDeliverGroup *group)
	: TFloatDialog(parent, group, IDD_INV_DELIVERER_BOOK)
{
	deliverer = new TUserAliasFace(this, IDC_INV_DELIVERER_BOOK_DELIVERS, "F_DELIVERER_UCN", C2S[POST_DELIVERER]);
	minDate = new TDateFace(this, IDC_INV_DELIVERER_BOOK_MIN_DATE, "F_MIN_DATE");
	new TDateFaceUpDown(this, IDC_INV_DELIVERER_BOOK_MIN_DATE_UPDN, minDate);
	maxDate = new TDateFace(this, IDC_INV_DELIVERER_BOOK_MAX_DATE, "F_MAX_DATE");
	new TDateFaceUpDown(this, IDC_INV_DELIVERER_BOOK_MAX_DATE_UPDN, maxDate);
	new TCheckFace(this, IDC_INV_DELIVERER_BOOK_NOTIFIEDS, "F_NOTIFIEDS");
	serialNo = new TCheckFace(this, IDC_INV_DELIVERER_BOOK_SERIAL_NO, "F_SERIAL_NO");
	autoSerialNo = new TLongFace(this, IDC_INV_DELIVERER_BOOK_AUTO_SERIAL_NO, "F_AUTO_SERIAL_NO", SIZE_OF_NO);
}

bool TInvBookDeliverDialog::IsValid()
{
	bool isValid = TFloatDialog::IsValid();

	if (isValid)
	{
		if (!atob(deliverer))
			isValid = error("Не е избран призовкар.");
		else if (invalid_range(minDate, maxDate))
			isValid = error("Началната дата  е по-голяма от крайната.");
		else if (minDate->Date().Empty() || maxDate->Date().Empty())
			isValid = error("Невалидна или непопълнена дата.");
	}

	return isValid;
}

void TInvBookDeliverDialog::Action(bool execute)
{
	if (execute && (REPEAT_BOOKS_B & REPEAT_BOOK_B_INV_DELIV))
		invDeliver_book(this, (TInvBookDeliverGroup *) Group());
	else
		TFloatDialog::Action(execute);
}

void TInvBookDeliverDialog::SetupWindow()
{
	TFloatDialog::SetupWindow();

	if (Default->post == POST_DELIVERER)
	{
		deliverer->SetS(Default->ucn);
		deliverer->EnableWindow(false);
	}

	SerialNoBNClicked();
}

void TInvBookDeliverDialog::SerialNoBNClicked()
{
	autoSerialNo->EnableWindow(serialNo->GetCheck() == BF_CHECKED);
}
