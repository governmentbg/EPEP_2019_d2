DEFINE_RESPONSE_TABLE1(TPerformAssignmentDialog, TAssignmentDialog)
	EV_BN_CLICKED(IDVIEW, ViewBNClicked),
	EV_BN_CLICKED(IDHISTORY, HistoryBNClicked),
	EV_BN_CLICKED(IDRIGHT, RightBNClicked),
END_RESPONSE_TABLE;

TPerformAssignmentDialog::TPerformAssignmentDialog(TWindow* parent, TGSArray<TAssignment> *array, TDistribution *distribution, const char *kinds, int resId)
:
	TAssignmentDialog(parent, resId)
{
	load = new TUserLoadListBox(this, IDC_PERFORM_ASSIGNMENT_LOAD, &distribution->users, true);
	autoSize = assignments = new TPerformAssignmentListWindow(this, IDC_PERFORM_ASSIGNMENT_ASSIGNMENTS, array, distribution, kinds, load);
	autoDown[3] = new TButton(this, IDVIEW);
	autoDown[4] = new TTwinButton(this, IDHISTORY, IDRIGHT);
	autoDown[6] = new TStatic(this, IDC_PERFORM_ASSIGNMENT_LABEL);
}

void TPerformAssignmentDialog::Resize()
{
	TAssignmentDialog::Resize();
	TAssignmentDialog::Resize(load, RESIZE_WIDTH | RESIZE_DOWN);
}

void TPerformAssignmentDialog::SetupWindow()
{
	TAssignmentDialog::SetupWindow();

	char title[0x100];

	GetWindowText(title, sizeof title);
	for (const char *s = assignments->kinds; *s; s++)
		strcatell(title, Kind->Seek(*s), s == assignments->kinds ? "" : s[1] ? ", " : " è ", sizeof title);
	SetCaption(title);

	// reflect the distributions done by assignment list window
	load->Refresh();
}

void TPerformAssignmentDialog::ViewBNClicked()
{
	assignments->distribution->Display();
}

void TPerformAssignmentDialog::HistoryBNClicked()
{
	TAssignmentGroup group;

	group.minDate += -3;
	strcpy(group.kinds, assignments->kinds);
	query_distrib(this, &group);
}

void TPerformAssignmentDialog::RightBNClicked()
{
	TAssignmentGroup group;

	group.minDate += -3;
	strcpy(group.kinds, assignments->kinds);
	if (get_period(this, &group))
		query_distrib(this, &group);
}
