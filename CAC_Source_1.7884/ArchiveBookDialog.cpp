TArchiveBookDialog::TArchiveBookDialog(TWindow* parent, TArchiveBookGroup *group)
	: TBookDialog(parent, group, Type, IDD_ARCHIVE_BOOK)
{
	minYear = new TYearFace(this, IDC_ARCHIVE_BOOK_MIN_YEAR, "F_MIN_YEAR");
	maxYear = new TYearFace(this, IDC_ARCHIVE_BOOK_MAX_YEAR, "F_MAX_YEAR");
	newFormat = new TCheckFace(this, IDC_ARCHIVE_BOOK_NEW_FORMAT, "F_NEW_FORMAT");
	sides = new TCheckFace(this, IDC_ARCHIVE_BOOK_SIDES, "F_SIDES");
}

bool TArchiveBookDialog::IsValid()
{
	bool isValid = TBookDialog::IsValid();

	if (isValid)
	{
		if (invalid_range(minYear, maxYear))
			isValid = error("Началната година е по-голяма от крайната.");
		else if (minDate->Date().Empty() || maxDate->Date().Empty())
			isValid = error("Въведете начална/крайна дата");
	}

	return isValid;
}

void TArchiveBookDialog::SetupWindow()
{
	TBookDialog::SetupWindow();

	minDate->EnableWindow(true);
	maxDate->EnableWindow(true);
}
