TAppealedBookDialog::TAppealedBookDialog(TWindow* parent, TAppealedBookGroup *group, int resId)
	: TFloatDialog(parent, group, resId)
{
	types = new TCharListFace(this, IDC_APPEALED_BOOK_TYPES, "F_TYPES", Type, QUERY_COLLEGES, SIZE_OF_TYPES);
	minDate = new TDateFace(this, IDC_APPEALED_BOOK_MIN_DATE, "F_MIN_DATE");
	new TDateFaceUpDown(this, IDC_APPEALED_BOOK_MIN_DATE_UPDN, minDate);
	maxDate = new TDateFace(this, IDC_APPEALED_BOOK_MAX_DATE, "F_MAX_DATE");
	new TDateFaceUpDown(this, IDC_APPEALED_BOOK_MAX_DATE_UPDN, maxDate);
	columns = new TCheckFace(this, IDC_APPEALED_BOOK_COLUMNS, "F_COLUMNS");
	rec = new TCheckFace(this, IDC_APPEALED_BOOK_REC, "F_RECAPITULATION");
	new TLongFace(this, IDC_APPEALED_BOOK_SERIAL_NO, "F_AUTO_SERIAL_NO", SIZE_OF_EVENT_NO);
}

bool TAppealedBookDialog::IsValid()
{
	bool isValid = TFloatDialog::IsValid();

	if (isValid)
	{
		if (!*types->S())
			isValid = error("Не е избран нито един вид дело.");
		else if (invalid_range(minDate, maxDate))
			isValid = error("Началната дата на съдебния акт е по-голяма от крайната.");
	}

	return isValid;
}
