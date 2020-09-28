//{{TBankruptListWindow Implementation}}

TBankruptListWindow::TBankruptListWindow(TWindow* parent, int resourceId)
:
	TGroupListWindow(parent, resourceId, new TGArray<TBankrupt>, 292)
{
	Add(
		new TGroupListColumn("Име", bankrupt_name, 208),
		new TGroupListColumn("Булстат", "F_BULSTAT", 72),
		NULL
	);

	*creditor = '\0';
	initialRefresh = false;
	sortcol = 1;
}

void TBankruptListWindow::SetCreditor(const char *tCreditor)
{
	if (strcmp(creditor, tCreditor))
	{
		strcpy(creditor, tCreditor);
		Refresh();
	}
}

void TBankruptListWindow::Query(bool select)
{
	TPeriodGroup period("T_PERIOD");
	int index = select ? GetVisualIndex() : BANKRUPT_SELECT_ALL;
	TQueryBankruptDialog(this, &period, (TGArray<TBankrupt> *) Array(), index).Execute();
}

void TBankruptListWindow::Load()
{
	TGArray<TBankrupt> bankrupts;

	import_bankrupts(this, creditor, &bankrupts);
	DrawLockCount();

	for (int i = 0; i < bankrupts.Count(); i++)
		InsertGroup(bankrupts[i]);

	UpdateVisual();
	bankrupts.Clear();
}

TGroup *TBankruptListWindow::NewGroup() const
{
	TBankrupt *bankrupt = new TBankrupt;
	strcpy(bankrupt->creditor, creditor);
	return bankrupt;
}

bool TBankruptListWindow::AddGroup(TGroup *group)
{
	return add_bankrupt(this, (TBankrupt *) group);
}

bool TBankruptListWindow::EditGroup(TGroup *group)
{
	return edit_bankrupt(this, (TBankrupt *) group);
}
