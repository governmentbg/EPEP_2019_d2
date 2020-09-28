//{{TPrepareAssignmentDialog Implementation}}

TPrepareAssignmentDialog::TPrepareAssignmentDialog(TWindow* parent, TGSArray<TAssignment> *array, TAssignmentGroup *tCriteria, TDistribution *distribution, int resId)
:
	TAssignmentDialog(parent, resId), criteria(tCriteria)
{
	autoSize = assignments = new TPrepareAssignmentListWindow(this, IDC_PREPARE_ASSIGNMENT_ASSIGNMENTS, array, distribution, criteria->kinds);
}

void TPrepareAssignmentDialog::SetupWindow()
{
	TAssignmentDialog::SetupWindow();

	char title[0x100];

	GetWindowText(title, sizeof title);
	scprintf(title, " от %D до %D", &criteria->minDate, &criteria->maxDate);
	SetCaption(title);
}
