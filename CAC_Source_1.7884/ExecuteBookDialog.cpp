TExecuteBookDialog::TExecuteBookDialog(TWindow* parent, TExecuteBookGroup *group, int resId)
	: TFloatDialog(parent, group, resId)
{
	allColumns = new TCheckFace(this, IDC_EXECUTE_BOOK_ALL_COLUMNS, "F_ALL_COLUMNS");
	minDate = new TDateFace(this, IDC_EXECUTE_BOOK_MIN_DATE, "F_MIN_DATE");
	new TDateFaceUpDown(this, IDC_EXECUTE_BOOK_MIN_DATE_UPDN, minDate);
	maxDate = new TDateFace(this, IDC_EXECUTE_BOOK_MAX_DATE, "F_MAX_DATE");
	new TDateFaceUpDown(this, IDC_EXECUTE_BOOK_MAX_DATE_UPDN, maxDate);
	new TLongFace(this, IDC_EXECUTE_BOOK_AUTO_SERIAL_NO, "F_AUTO_SERIAL_NO", SIZE_OF_NO);
}

bool TExecuteBookDialog::IsValid()
{
	bool isValid = TFloatDialog::IsValid();

	if (isValid)
	{
		if (minDate->Date().Empty() || maxDate->Date().Empty())
			isValid = error("Началната и крайната дата са задължителни.");
		else if (invalid_range(minDate, maxDate))
			isValid = error("Началната дата е по-голяма от крайната.");
	}

	return isValid;
}
