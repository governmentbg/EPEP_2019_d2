//{{TUnfilteredActsDialog Implementation}}

TUnfilteredActsDialog::TUnfilteredActsDialog(TWindow* parent, TUnfilteredActsGroup *group)
:
	TGroupDialog(parent, group, IDD_UNFILTERED_ACTS)
{
	types = new TTypeListFace(this, IDC_UNFILTERED_ACTS_TYPES, "F_TYPES", TYPE_LAWSUITS, true);
	minDate = new TDateFace(this, IDC_UNFILTERED_ACTS_MIN_DATE, "F_MIN_DATE", true);
	maxDate = new TDateFace(this, IDC_UNFILTERED_ACTS_MAX_DATE, "F_MAX_DATE", true);
}

bool TUnfilteredActsDialog::IsValid()
{
	bool isValid = TGroupDialog::IsValid();

	if (isValid)
	{
		if (invalid_range(minDate, maxDate))
			isValid = error("Началната дата на съдебен акт е по-голяма от крайната.");
	}

	return isValid;
}
