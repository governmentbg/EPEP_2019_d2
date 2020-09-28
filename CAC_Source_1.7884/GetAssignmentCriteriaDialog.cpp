//{{TGetAssignmentCriteriaDialog Implementation}}

TGetAssignmentCriteriaDialog::TGetAssignmentCriteriaDialog(TWindow* parent, TAssignmentGroup *group, int resId)
:
	TPeriodDialog(parent, group, resId)
{
	kinds = new TCharListFace(this, IDC_GET_ASSIGNMENT_CRITERIA_KINDS, "F_KINDS", Kind, Default->lawKinds, SIZE_OF_LAW_KINDS);
}

bool TGetAssignmentCriteriaDialog::IsValid()
{
	bool isValid = TPeriodDialog::IsValid();

	if (isValid)
	{
		if (!*kinds->S())
			isValid = error("Не е избран нито един вид дело.");
#if COURT_TYPE == COURT_AREAL
		else if (strchr(kinds->S(), KIND_FIRM_LAW) && strlen(kinds->S()) > 1)
			isValid = error("Фирмените дела не могат да се комбинират с другите видове.");
#endif  // AREAL			
	}

	return isValid;
}
