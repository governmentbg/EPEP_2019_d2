DEFINE_RESPONSE_TABLE1(TAssignmentDialog, TResizeDialog)
//{{TAssignmentDialogRSP_TBL_BEGIN}}
	EV_LVN_ITEMCHANGING(IDC_PREPARE_ASSIGNMENT_ASSIGNMENTS, AssignmentsLVNItemChanging),
	EV_BN_CLICKED(IDOK, OKBNClicked),
	EV_BN_CLICKED(IDADD, AddBNClicked),
	EV_COMMAND_ENABLE(IDEDIT, EditBNEnable),
//{{TAssignmentDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TAssignmentDialog Implementation}}

TAssignmentDialog::TAssignmentDialog(TWindow* parent, int resId)
:
	TResizeDialog(parent, SHOW_MAXIMIZED, resId), assignments(NULL)
{
	autoDown[0] = new TButton(this, IDOK);
	autoDown[1] = new TButton(this, IDCANCEL);
	autoDown[2] = new TButton(this, IDADD);
}

void TAssignmentDialog::OKBNClicked()
{
	if (assignments->IsValid())
		CmOk();
}

bool TAssignmentDialog::AssignmentsLVNItemChanging(TLwNotify &lwn)
{
	return assignments->ItemChanging(lwn);
}

void TAssignmentDialog::AddBNClicked()
{
	assignments->Add();
}

void TAssignmentDialog::EditBNClicked()
{
	assignments->Edit();
}

void TAssignmentDialog::EditBNEnable(TCommandEnabler &tce)
{
	tce.Enable(assignments->Group());
}
