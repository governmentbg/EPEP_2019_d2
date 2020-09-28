//{{TSelectDebtorListBox Implementation}}

TSelectDebtorListBox::TSelectDebtorListBox(TWindow* parent, int resourceId, TGArray<TExecDebtor> *debtors)
:
	TCloneListBox(parent, resourceId, debtors, TExecDebtor::Clone)
{
	loadData = dynamic = false;
}

void TSelectDebtorListBox::Select(const char *ucn, char ucnType)
{
	TCloneListBox::SetupWindow();

	for (int i = 0; i < array->Count(); i++)
	{
		TExecDebtor *debtor = (TExecDebtor *) (*array)[i];

		if (!strcmp(debtor->ucn, ucn) && debtor->ucnType == ucnType)
		{
			SetSelIndex(i);
			break;
		}
	}
}

void TSelectDebtorListBox::GetGroupText(mstr &m, TGroup *group, int)
{
	TExecDebtor *debtor = (TExecDebtor *) group;
	m.cat(debtor->name);
}
