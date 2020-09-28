TExpenseBookDialog :: TExpenseBookDialog(TWindow *parent, TExpenseBookGroup *group, int resId )
	: TGroupDialog(parent, group, resId)
{

	types = new TCharListFace(this, IDC_BOOK_EXPENSE_TYPES, "F_TYPES", Type, QUERY_COLLEGES, SIZE_OF_TYPES);
	compositions = new TCharAutoListFace(this, IDC_BOOK_EXPENSE_COMPOSITION, "F_COMPOSITION",  CharComposition,
		QUERY_COMPOSITIONS, SIZE_OF_COMPOSITIONS);
	minDate = new TDateFace(this, IDC_BOOK_EXPENSE_BEG_DATE, "F_MIN_DATE", true);
	new TDateFaceUpDown(this, IDC_BOOK_EXPENSE_BEG_DATE_UPDN, minDate);
	maxDate = new TDateFace(this, IDC_BOOK_EXPENSE_END_DATE, "F_MAX_DATE", true);
	new TDateFaceUpDown(this, IDC_BOOK_EXPENSE_END_DATE_UPDN, maxDate);
	new TLongFace(this, IDC_BOOK_EXPENSE_AUTO_SERIAL_NO, "F_AUTO_SERIAL_NO", SIZE_OF_EVENT_NO);
	all = new TCheckFace(this, IDC_BOOK_EXPENSE_ALL, "F_ALL");
}

bool TExpenseBookDialog::IsValid()
{
	bool isValid = TGroupDialog::IsValid();

	if (isValid)
	{
		if (!*types->S())
			isValid = error("Не е избран нито един вид дело.");
		else if (invalid_range(minDate, maxDate))
			isValid = error("Началната дата е по-голяма от крайната.");
	}

	return isValid;
}
