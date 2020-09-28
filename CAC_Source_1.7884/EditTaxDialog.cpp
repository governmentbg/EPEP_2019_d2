//{{TEditTaxDialog Implementation}}

TEditTaxDialog::TEditTaxDialog(TWindow* parent, TExpense *expense, bool edit, int resId)
:
	TEditDialog(parent, expense, edit, resId)
{
	new TMoneyFace(this, IDC_EDIT_PENALITY_SUM_1, "F_SUM_1", SIZE_OF_SUM);
	paidDate = new TDateFace(this, IDC_EDIT_PENALITY_PAID_DATE, "F_PAID_DATE", false);
	new TStringFace(this, IDC_EDIT_PENALITY_PAID_NOTE, "F_PAID_NOTE", SIZE_OF_EXPENSE_NOTE);
	new TStringFace(this, IDC_EDIT_PENALITY_TEXT, "F_TEXT", SIZE_OF_EXPENSE_TEXT);
}

void TEditTaxDialog::SetupWindow()
{
	TEditDialog::SetupWindow();

	if (paidDate->Date().Empty())
		paidDate->SetDate(Expense()->date);
}

bool TEditTaxDialog::IsValid()
{
	bool isValid = TEditDialog::IsValid();

	if (isValid)
	{
		if (paidDate->Date().Empty())
			isValid = error("Липсва дата на внасяне.");
		else if (paidDate->Date() > Today)
			isValid = error("Дата на внасяне и текуща дата не си съответстват.");
	}

	return isValid;
}
