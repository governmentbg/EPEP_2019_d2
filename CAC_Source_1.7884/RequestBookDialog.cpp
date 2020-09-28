TRequestBookDialog::TRequestBookDialog(TWindow* parent, TRequestBookGroup *group)
	: TBookInstanceDialog(parent, group, Type, IDD_REQUEST_BOOK)
{
}

bool TRequestBookDialog::IsValid()
{
	bool isValid = TBookInstanceDialog::IsValid();

	if (isValid)
	{
		if (byNoYear->GetCheck() == BF_CHECKED)
		{
			if (!year->L())
				isValid = error("�������� � ������������.");
		}
		else if (minDate->Date().Empty() || maxDate->Date().Empty())
			isValid = error("��������� � �������� ���� �� ������������.");
	}

	return isValid;
}

void TRequestBookDialog::Action(bool execute)
{
	if (execute && (REPEAT_BOOKS_B & REPEAT_BOOK_B_REQUEST))
		request_book(this, (TRequestBookGroup *) Group());
	else
		TFloatDialog::Action(execute);
}
