//{{TFloatUnappealedDialog Implementation}}
TFloatUnappealedDialog::TFloatUnappealedDialog(TWindow* parent, TFloatUnappealedGroup *group)
:
	TFloatSubjectDialog(parent, group, IDD_FLOAT_UNAPPEALED)
{
	lawKinds = new TCharAutoListFace(this, IDC_FLOAT_UNAPPEALED_LAW_KINDS, "F_LAW_KINDS", Kind, Default->xKinds,
		SIZE_OF_KINDS);
	decKinds = new TCharListFace(this, IDC_FLOAT_UNAPPEALED_DEC_KINDS, "F_DEC_KINDS", Kind, KIND_VISIBLE_ENDOCS,
		SIZE_OF_KINDS);
	judges = new TUserListBox(this, IDC_FLOAT_UNAPPEALED_JUDGES, POST_VISIBLE_JUDGES, USER_VIMIT);
	compositions = new TCharListFace(this, IDC_FLOAT_UNAPPEALED_COMPOSITIONS, "F_COMPOSITIONS", CharComposition,
		QUERY_COMPOSITIONS, SIZE_OF_COMPOSITIONS);
	minForceDate = new TDateFace(this, IDC_FLOAT_UNAPPEALED_MIN_FORCE_DATE, "F_MIN_FORCE_DATE", false);
	maxForceDate = new TDateFace(this, IDC_FLOAT_UNAPPEALED_MAX_FORCE_DATE, "F_MAX_FORCE_DATE", false);
	minFromDate = new TDateFace(this, IDC_FLOAT_UNAPPEALED_MIN_FROM_DATE, "F_MIN_FROM_DATE", false);
	maxFromDate = new TDateFace(this, IDC_FLOAT_UNAPPEALED_MAX_FROM_DATE, "F_MAX_FROM_DATE", false);
	decFinal = new TCheckFace(this, IDC_FLOAT_UNAPPEALED_DEC_FINAL, "F_DEC_FINAL");
}

bool TFloatUnappealedDialog::IsValid()
{
	bool isValid = TFloatSubjectDialog::IsValid();

	if (isValid)
	{
		if (invalid_range(minForceDate, maxForceDate))
			isValid = error("Началната дата на влизане в законна сила е по-голяма от крайната.");
		else if (invalid_range(minFromDate, maxFromDate))
			isValid = error("Началната дата на съдебен акт е по-голяма от крайната.");
	}

	return isValid;
}

void TFloatUnappealedDialog::Action(bool execute)
{
	msql m;
	TFloatUnappealedGroup *unappealedGroup = FloatUnappealedGroup();

	TFloatSubjectDialog::GetSubjects(unappealedGroup->newSubjects, unappealedGroup->oldSubjects);
	judges->GetMark(&unappealedGroup->judges);
	unappealedGroup->FormCriteria(m);

	if (execute)
		query_unappealed(this, str(m), unappealedGroup->minForceDate, unappealedGroup->maxForceDate);
	else
		Message(m);
}
