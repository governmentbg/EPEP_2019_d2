//{{TFloatHeritageDialog Implementation}}

TFloatHeritageDialog::TFloatHeritageDialog(TWindow* parent, TFloatHeritageGroup *group)
:
	TFloatDialog(parent, group, IDD_FLOAT_HERITAGE)
{
	heritages = new TCharAutoListFace(this, IDC_FLOAT_HERITAGE_HERITAGES, "F_HERITAGES", Result, RESULT_HERITAGE_STATES,
		SIZE_OF_RESULTS);
	minNo = new TLongFace(this, IDC_FLOAT_HERITAGE_MIN_NO, "F_MIN_NO", SIZE_OF_NO);
	maxNo = new TLongFace(this, IDC_FLOAT_HERITAGE_MAX_NO, "F_MAX_NO", SIZE_OF_NO);
	minYear = new TYearFace(this, IDC_FLOAT_HERITAGE_MIN_YEAR, "F_MIN_YEAR");
	maxYear = new TYearFace(this, IDC_FLOAT_HERITAGE_MAX_YEAR, "F_MAX_YEAR");
	reqKinds = new TCharAutoListFace(this, IDC_FLOAT_HERITAGE_REQ_KINDS, "F_SOURCE_KINDS", Kind, KIND_HERITAGE_INDOCS,
		SIZE_OF_KINDS);
	minFromDate = new TDateFace(this, IDC_FLOAT_HERITAGE_MIN_FROM_DATE, "F_MIN_DATE", false);
	maxFromDate = new TDateFace(this, IDC_FLOAT_HERITAGE_MAX_FROM_DATE, "F_MAX_DATE", false);
}

bool TFloatHeritageDialog::IsValid()
{
	bool isValid = TFloatDialog::IsValid();

	if (isValid)
	{
		if (invalid_range(minNo, maxNo))
			isValid = error("Началния номер е по-голям от крайния.");
		else if (invalid_range(minYear, maxYear))
			isValid = error("Началната година е по-голяма от крайната.");
		else if (invalid_range(minFromDate, maxFromDate))
		{
			isValid = error("Началната дата на съдебен акт е по-голяма от крайната.");
		}
	}

	return isValid;
}
