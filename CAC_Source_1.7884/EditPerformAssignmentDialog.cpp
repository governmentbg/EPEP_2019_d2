DEFINE_RESPONSE_TABLE1(TEditPerformAssignmentDialog, TGroupDialog)
//{{TEditPerformAssignmentDialogRSP_TBL_BEGIN}}
	EV_CBN_SELCHANGE(IDC_EDIT_PERFORM_ASSIGNMENT_RESOLUTION, ResolutionCBNSelChange),
	EV_BN_CLICKED(IDOK, OKBNClicked),
//{{TEditPerformAssignmentDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TEditPerformAssignmentDialog Implementation}}

TEditPerformAssignmentDialog::TEditPerformAssignmentDialog(TWindow* parent, TAssignment *assignment, TDistribution *distribution, int resId)
:
	TGroupDialog(parent, assignment, resId)
{
	label = new TStatic(this, IDC_EDIT_PERFORM_ASSIGNMENT_LABEL);
	left = new TStatic(this, IDC_EDIT_PERFORM_ASSIGNMENT_LEFT);
	right = new TStatic(this, IDC_EDIT_PERFORM_ASSIGNMENT_RIGHT);
	resolution = new TCharAliasFace(this, IDC_EDIT_PERFORM_ASSIGNMENT_RESOLUTION, "F_RESOLUTION", Resolution, RESOLUTION_PERFASSIGNS);
	assign = new TPersonDistribAliasFace(this, IDC_EDIT_PERFORM_ASSIGNMENT_ASSIGN, "F_ASSIGN", new TUserAliasGroup, &distribution->users, ALIAS_OPTION);
	subject = new TSubjectAliasFace(this, IDC_EDIT_PERFORM_ASSIGNMENT_SUBJECT, "F_SUBJECT", assignment->lawKind, &distribution->subjects);
	text = new TStringFace(this, IDC_EDIT_PERFORM_ASSIGNMENT_TEXT, "F_TEXT_100", SIZE_OF_RESOLUTION_TEXT - 100);
}

void TEditPerformAssignmentDialog::SetupWindow()
{
	TGroupDialog::SetupWindow();

	// 2007:247 LPR: display Today date
	mstr m;

	m.printf("%s %ld/%ldг. от %Dг.", Kind->Seek(Assignment()->kind), Assignment()->key.no, Assignment()->key.year, &Today);
	label->SetCaption(str(m));
	left->SetCaption(str(Assignment()->left));
	right->SetCaption(str(Assignment()->right));

	text->SetS(str(Assignment()->text));
	ResolutionCBNSelChange();	// 2010:266
}

bool TEditPerformAssignmentDialog::IsValid()
{
	bool isValid = TGroupDialog::IsValid();

	if (isValid)
	{
		char tResolution = resolution->C();

		if (tResolution == RESOLUTION_ASSIGNED)
		{
			if (!atob(assign) || !subject->L())
				isValid = error("Предметът е задължителен при резолюция %s.", Resolution->Seek(tResolution));
			else if (atob(Assignment()->autoAssign) && strcmp(Assignment()->autoAssign, assign->S()) && !any(text))
				isValid = error("Текстът е задължителен при промяна на автоматично предложения съдия.");
		}
	}

	return isValid;
}

void TEditPerformAssignmentDialog::OKBNClicked()
{
	if (IsValid())
	{
		*Assignment() << this;
		Assignment()->text.cpy(text->S());
		CmOk();
	}
}

void TEditPerformAssignmentDialog::ResolutionCBNSelChange()
{
	if (resolution->C() == RESOLUTION_ASSIGNED)
	{
		assign->SetFlags(ALIAS_OPTION);
		assign->SetS(Assignment()->assign);
	}
	else
	{
		// USER_INTER == JUROR_PEDAGOG can't match
		assign->SetFlags(ALIAS_EMPTY | USER_INTER);
	}
}
