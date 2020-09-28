//{{TSelectExecKindDialog Implementation}}

TSelectExecKindDialog::TSelectExecKindDialog(TWindow* parent, TDecision *decision, int resId)
:
	TGroupDialog(parent, decision, resId)
{
	kind410money = new TRadioFace(this, IDC_SELECT_EXEC_KIND_410_MONEY, "F_EXEC_KIND_410_MONEY");
	kind410movable = new TRadioFace(this, IDC_SELECT_EXEC_KIND_410_MOVABLE, "F_EXEC_KIND_410_MOVABLE");
	kind417money = new TRadioFace(this, IDC_SELECT_EXEC_KIND_417_MONEY, "F_EXEC_KIND_417_MONEY");
	kind417movable = new TRadioFace(this, IDC_SELECT_EXEC_KIND_417_MOVABLE, "F_EXEC_KIND_417_MOVABLE");
	new TCheckFace(this, IDC_SELECT_EXEC_KIND_UCN_STAT, "F_UCN_STAT");
}

bool TSelectExecKindDialog::IsValid()
{
	bool isValid = TGroupDialog::IsValid();

	if (isValid)
	{
		if (kind410money->GetCheck() == BF_UNCHECKED && kind410movable->GetCheck() == BF_UNCHECKED &&
			kind417money->GetCheck() == BF_UNCHECKED && kind417movable->GetCheck() == BF_UNCHECKED)
		{
			isValid = error("Не е избран вид заповед.");
		}
	}

	return isValid;
}
