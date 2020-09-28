//{{TEditPrepareAssignmentDialog Implementation}}

TEditPrepareAssignmentDialog::TEditPrepareAssignmentDialog(TWindow* parent, TAssignment *assignment, TDistribution *distribution, int resId)
:
	TGroupDialog(parent, assignment, resId)
{
	label = new TStatic(this, IDC_EDIT_PREPARE_ASSIGNMENT_LABEL);
	left = new TStatic(this, IDC_EDIT_PREPARE_ASSIGNMENT_LEFT);
	right = new TStatic(this, IDC_EDIT_PREPARE_ASSIGNMENT_RIGHT);
	subject = new TSubjectAliasFace(this, IDC_EDIT_PREPARE_ASSIGNMENT_SUBJECT, "F_SUBJECT", assignment->lawKind, &distribution->subjects);
	judge = new TPersonDistribAliasFace(this, IDC_EDIT_PREPARE_ASSIGNMENT_JUDGE, "F_JUDGE", new TUserAliasGroup, assignment->lawKind);
}

void TEditPrepareAssignmentDialog::SetupWindow()
{
	TGroupDialog::SetupWindow();

	mstr m;
	// 2007:247 LRQ: display indoc fields
	m.printf("%s %ld/%ldг. от %Dг.", Kind->Seek(Assignment()->indocKind), Assignment()->key.no, Assignment()->key.year,
		&Assignment()->indocDate);
	label->SetCaption(str(m));
	left->SetCaption(str(Assignment()->left));
	right->SetCaption(str(Assignment()->right));
}

bool TEditPrepareAssignmentDialog::IsValid()
{
	bool isValid = TGroupDialog::IsValid();

	if (isValid)
	{
		if (atob(judge) && !subject->L())
			isValid = error("Има попълнен съдия, но не и предмет.");
	}

	return isValid;
}
