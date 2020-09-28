//{{TQueryCorruptDialog Implementation}}

TQueryCorruptDialog::TQueryCorruptDialog(TWindow* parent, TCorruptGroup *group)
:
	TMonthPeriodDialog(parent, group, IDD_QUERY_CORRUPT)
{
	forExport = new TCheckFace(this, IDC_QUERY_CORRUPT_FOR_EXPORT, "F_FOR_EXPORT");
}
