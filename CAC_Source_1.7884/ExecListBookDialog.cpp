TExecListBookDialog::TExecListBookDialog(TWindow* parent, TFloatExecListGroup *group, int resId)
	: TFloatDialog(parent, group, resId)
{
	types = new TCharListFace(this, IDC_EXEC_LIST_BOOK_TYPES, "F_TYPES", Type, QUERY_COLLEGES, SIZE_OF_TYPES);
	minExecListDate = new TDateFace(this, IDC_EXEC_LIST_BOOK_MIN_EXEC_LIST_DATE, "F_MIN_EXEC_LIST_DATE");
	new TDateFaceUpDown(this, IDC_EXEC_LIST_BOOK_MIN_EXEC_LIST_DATE_UPDN, minExecListDate);
	maxExecListDate = new TDateFace(this, IDC_EXEC_LIST_BOOK_MAX_EXEC_LIST_DATE, "F_MAX_EXEC_LIST_DATE");
	new TDateFaceUpDown(this, IDC_EXEC_LIST_BOOK_MAX_EXEC_LIST_DATE_UPDN, maxExecListDate);
	execListAlls = new TRadioFace(this, IDC_EXEC_LIST_BOOK_EXEC_LIST_ALLS, "F_EXEC_LIST_ALLS");
	execListJudicials = new TRadioFace(this, IDC_EXEC_LIST_BOOK_EXEC_LIST_JUDICIAL, "F_EXEC_LIST_JUDICIALS");
	new TRadioFace(this, IDC_EXEC_LIST_BOOK_EXEC_LIST_COUNTRY, "F_EXEC_LIST_COUNTRY");
	new TRadioFace(this, IDC_EXEC_LIST_BOOK_EXEC_LIST_PRIVATE, "F_EXEC_LIST_PRIVATE");
	voluntary = new TCheckFace(this, IDC_EXEC_LIST_BOOK_VOLUNTARY, "F_VOLUNTARY");
}

bool TExecListBookDialog::IsValid()
{
	bool isValid = TFloatDialog::IsValid();

	if (isValid)
	{
		if (!*types->S())
			isValid = error("Не е избран нито един вид дело.");
		else if (invalid_range(minExecListDate, maxExecListDate))
			isValid = error("Началната дата на съдебния акт е по-голяма от крайната.");
	}

	return isValid;
}

void TExecListBookDialog::SetupWindow()
{
	TFloatDialog::SetupWindow();

	if (EXEC_LIST_NUMBERING & EXEC_LIST_NUMBERING_EXEC_KIND)
	{
		execListAlls->EnableWindow(false);
		execListAlls->SetCheck(BF_UNCHECKED);
		execListJudicials->SetCheck(BF_CHECKED);
	}
}
