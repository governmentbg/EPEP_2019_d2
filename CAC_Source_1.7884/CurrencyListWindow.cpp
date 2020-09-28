//{{TCurrencyListWindow Implementation}}

TCurrencyListWindow::TCurrencyListWindow(TWindow* parent, int resourceId)
:
	TGroupListWindow(parent, resourceId, NULL, 172)
{
	Add(
		new TGroupListColumn("Код", "F_CURRENCY", 28),
		new TGroupListColumn("Име", "F_NAME", 132),
		NULL
	);

	sortcol = 2;
}

bool TCurrencyListWindow::AddGroup(TGroup *group)
{
	return TEditCurrencyDialog(this, (TCurrency *) group, false).Execute() == IDOK;
}

bool TCurrencyListWindow::EditGroup(TGroup *group)
{
	return TEditCurrencyDialog(this, (TCurrency *) group, true).Execute() == IDOK;
}

bool TCurrencyListWindow::AskDelete(TGroup *group)
{
	TCurrency *currency = (TCurrency *) group;
	TExecListSum sum;

	strcpy(sum.currency, currency->currency);
	return check_exists(sum.Name, &sum, "F_CURRENCY") ? error("Валутата се използва.") :
		TGroupListWindow::AskDelete(group);
}
