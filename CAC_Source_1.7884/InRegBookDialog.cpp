TInRegBookDialog::TInRegBookDialog(TWindow* parent, TInRegBookGroup *group)
	: TBookDialog(parent, group, CollegeRight, IDD_REGISTER_BOOK)
{
}

void TInRegBookDialog::Action(bool execute)
{
	if (execute && (REPEAT_BOOKS_B & REPEAT_BOOK_B_INREG))
		inreg_book(this, (TInRegBookGroup *) Group());
	else
		TFloatDialog::Action(execute);
}
