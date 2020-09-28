TInDocBookDialog::TInDocBookDialog(TWindow* parent, TInDocBookGroup *group)
	: TBookInstanceDialog(parent, group, CollegeRight, IDD_INDOC_BOOK)
{
	inRequest = new TCheckFace(this, IDC_INDOC_BOOK_BY_REQUEST, "F_REQUEST");
	inSurround = new TCheckFace(this, IDC_INDOC_BOOK_BY_INREG, "F_INREG");
	inInReg = new TCheckFace(this, IDC_INDOC_BOOK_BY_SURROUND, "F_SURROUND");
	new TCheckFace(this, IDC_INDOC_BOOK_PRINT_RIGHT, "F_PRINT_RIGHT");
	new TRadioFace(this, IDC_INDOC_BOOK_BY_DIRECTION, "F_DIRECTION");
	new TRadioFace(this, IDC_INDOC_BOOK_BY_JUDGE, "F_JUDGE");
}

bool TInDocBookDialog::IsValid()
{
	bool isValid = TBookInstanceDialog::IsValid();

	if (isValid)
	{
		if (inRequest->GetCheck() == BF_UNCHECKED && inSurround->GetCheck() == BF_UNCHECKED &&
			inInReg->GetCheck() == BF_UNCHECKED)
		{
			isValid = error("Не е избран нито един вид документ.");
		}
	}

	return isValid;
}

void TInDocBookDialog::Action(bool execute)
{
	if (execute && (REPEAT_BOOKS_B && REPEAT_BOOK_B_INDOC))
		indoc_book(this, (TInDocBookGroup *) Group());
	else
		TFloatDialog::Action(execute);
}
