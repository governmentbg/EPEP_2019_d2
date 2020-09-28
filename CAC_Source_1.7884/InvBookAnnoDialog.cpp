DEFINE_RESPONSE_TABLE1(TInvBookAnnoDialog, TFloatDialog)
	EV_BN_CLICKED(IDC_INV_ANNO_BOOK_SERIAL_NO_COLUMN, SerialNoColumnBNClicked),
END_RESPONSE_TABLE;

TInvBookAnnoDialog::TInvBookAnnoDialog(TWindow* parent, TInvBookAnnoGroup *group)
	: TFloatDialog(parent, group, IDD_INV_ANNO_BOOK)
{
	senders = new TSenderListBox(this, IDC_INV_ANNO_BOOK_COURT, C2S[UCN_INSTANCE_EXTERNAL]);
	minDate = new TDateFace(this, IDC_INV_ANNO_BOOK_MIN_DATE, "F_MIN_DATE");
	new TDateFaceUpDown(this, IDC_INV_ANNO_BOOK_MIN_DATE_UPDN, minDate);
	maxDate = new TDateFace(this, IDC_INV_ANNO_BOOK_MAX_DATE, "F_MAX_DATE");
	new TDateFaceUpDown(this, IDC_INV_ANNO_BOOK_MAX_DATE_UPDN, maxDate);
	serialNoColumn = new TCheckFace(this, IDC_INV_ANNO_BOOK_SERIAL_NO_COLUMN, "F_SERIAL_NO_COLUMN");
	autoSerialNo = new TLongFace(this, IDC_INV_ANNO_BOOK_AUTO_SERIAL_NO, "F_AUTO_SERIAL_NO", SIZE_OF_EVENT_NO);
}

bool TInvBookAnnoDialog::IsValid()
{
	bool isValid = TFloatDialog::IsValid();

	if (isValid)
	{
		if (invalid_range(minDate, maxDate))
			isValid = error("Началната дата  е по-голяма от крайната.");
		else if (minDate->Date().Empty() || maxDate->Date().Empty())
			isValid = error("Невалидна или непопълнена дата.");
	}

	return isValid;
}

void TInvBookAnnoDialog::Action(bool execute)
{
	senders->GetMark(&InvBookAnnoGroup()->senders);
	TFloatDialog::Action(execute);
}

void TInvBookAnnoDialog::SerialNoColumnBNClicked()
{
	autoSerialNo->EnableWindow(serialNoColumn->GetCheck() == BF_CHECKED);
}
