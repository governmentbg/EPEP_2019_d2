TBook251Dialog::TBook251Dialog(TWindow* parent, TInDocBookGroup *group, int resId)
	: TGroupDialog(parent, group, resId)
{
	minDate = new TDateFace(this, IDC_BOOK_251_MIN_DATE, "F_MIN_DATE", false);
	new TDateFaceUpDown(this, IDC_BOOK_251_MIN_DATE_UPDN, minDate);
	maxDate = new TDateFace(this, IDC_BOOK_251_MAX_DATE, "F_MAX_DATE", false);
	new TDateFaceUpDown(this, IDC_BOOK_251_MAX_DATE_UPDN, maxDate);
}

bool TBook251Dialog::IsValid()
{
	bool isValid = TGroupDialog::IsValid();

	if (isValid)
	{
		if (minDate->Date().Empty() || maxDate->Date().Empty())
			isValid = error("Невалидна или непопълнена дата.");
		else if (invalid_range(minDate, maxDate))
			isValid = error("Началната дата е по-голяма от крайната.");
	}

	return isValid;
}
