//{{TFloatDestroyDialog Implementation}}

TFloatDestroyDialog::TFloatDestroyDialog(TWindow* parent, TFloatDestroyGroup *group)
:
	TFloatDialog(parent, group, IDD_FLOAT_DESTROY)
{
	types = new TTypeListFace(this, IDC_FLOAT_DESTROY_TYPES, "F_TYPES", TYPE_LAWSUITS, false);
	minKillDate = new TDateFace(this, IDC_FLOAT_DESTROY_MIN_KILL_DATE, "F_MIN_KILL_DATE", false);
	maxKillDate = new TDateFace(this, IDC_FLOAT_DESTROY_MAX_KILL_DATE, "F_MAX_KILL_DATE", false);
	minKillerNo = new TLongFace(this, IDC_FLOAT_DESTROY_MIN_KILLER_NO, "F_MIN_KILLER_NO", SIZE_OF_NO);
	maxKillerNo = new TLongFace(this, IDC_FLOAT_DESTROY_MAX_KILLER_NO, "F_MAX_KILLER_NO", SIZE_OF_NO);
	minKillerDate = new TDateFace(this, IDC_FLOAT_DESTROY_MIN_KILLER_DATE, "F_MIN_KILLER_DATE", false);
	maxKillerDate = new TDateFace(this, IDC_FLOAT_DESTROY_MAX_KILLER_DATE, "F_MAX_KILLER_DATE", false);
	minRemainderNo = new TLongFace(this, IDC_FLOAT_DESTROY_MIN_REMAINDER_NO, "F_MIN_REMAINDER_NO", SIZE_OF_NO);
	maxRemainderNo = new TLongFace(this, IDC_FLOAT_DESTROY_MAX_REMAINDER_NO, "F_MAX_REMAINDER_NO", SIZE_OF_NO);
	minRemainderYear = new TYearFace(this, IDC_FLOAT_DESTROY_MIN_REMAINDER_YEAR, "F_MIN_REMAINDER_YEAR");
	maxRemainderYear = new TYearFace(this, IDC_FLOAT_DESTROY_MAX_REMAINDER_YEAR, "F_MAX_REMAINDER_YEAR");
	indexes = new TArchiveIndexListBox(this, IDC_FLOAT_DESTROY_INDEXES, true);
	new TRadioFace(this, IDC_FLOAT_DESTROY_BY_TYN, "F_BY_TYN");
	new TRadioFace(this, IDC_FLOAT_DESTROY_BY_KTYN, "F_BY_KTYN");
}

bool TFloatDestroyDialog::IsValid()
{
	bool isValid = TFloatDialog::IsValid();

	if (isValid)
	{
		if (invalid_range(minKillDate, maxKillDate))
			isValid = error("Началната дата на унищожаване е по-голяма от крайната.");
		else if (invalid_range(minKillerNo, maxKillerNo))
			isValid = error("Началния номер на протокол за унищожаване е по-голям от крайния.");
		else if (invalid_range(minKillerDate, maxKillerDate))
			isValid = error("Началната дата на протокол за унищожаване е по-голяма от крайната.");
		else if (invalid_range(minRemainderNo, maxRemainderNo))
			isValid = error("Началния номер на тома със запазени документи е по-голям от крайния.");
		else if (invalid_range(minRemainderYear, maxRemainderYear))
			isValid = error("Началната година на тома със запазени документи по-голяма от крайната.");
	}

	return isValid;
}

void TFloatDestroyDialog::Action(bool execute)
{
	msql m;

	indexes->GetMark(&FloatDestroyGroup()->indexes);
	FloatDestroyGroup()->FormCriteria(m);

	if (execute)
		query_kill(this, str(m));
	else
		Message(m);
}
