TRecusalBookDialog::TRecusalBookDialog(TWindow* parent, TRecusalBookGroup *group, int resId)
	: TFloatDialog(parent, group, resId)
{
	types = new TCharListFace(this, IDC_RECUSAL_BOOK_TYPES, "F_TYPES", Type, Default->colleges, SIZE_OF_TYPES);
	compositions = new TCharAutoListFace(this, IDC_RECUSAL_BOOK_COMPOSITIONS, "F_COMPOSITIONS", CharComposition,
		QUERY_COMPOSITIONS, SIZE_OF_COMPOSITIONS);
	minDate = new TDateFace(this, IDC_RECUSAL_BOOK_MIN_DATE, "F_MIN_DATE");
	new TDateFaceUpDown(this, IDC_RECUSAL_BOOK_MIN_DATE_UPDN, minDate);
	maxDate = new TDateFace(this, IDC_RECUSAL_BOOK_MAX_DATE, "F_MAX_DATE");
	new TDateFaceUpDown(this, IDC_RECUSAL_BOOK_MAX_DATE_UPDN, maxDate);
	autogen = new TLongFace(this, IDC_RECUSAL_BOOK_AUTOGEN, "F_AUTO_SERIAL_NO", SIZE_OF_NO);
	filtered = new TCheckFace(this, IDC_RECUSAL_BOOK_FILTERED, "F_FILTERED");
	recujed = new TCheckFace(this, IDC_RECUSAL_BOOK_RECUJED, "F_RECUJED");
}

bool TRecusalBookDialog::IsValid()
{
	bool isValid = TFloatDialog::IsValid();

	if (isValid)
	{
		if (!*types->S())
			isValid = error("Ќе е избран нито един вид дело.");
		else if (invalid_range(minDate, maxDate))
			isValid = error("Ќачалната дата на заседание е по-гол€ма от крайната.");
	}

	return isValid;
}
