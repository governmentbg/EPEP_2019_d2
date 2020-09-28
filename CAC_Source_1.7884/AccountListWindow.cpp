//{{TAccountListWindow Implementation}}

TAccountListWindow::TAccountListWindow(TWindow* parent, int resourceId)
:
	TGroupListWindow(parent, resourceId, new TCharAliasArray, 180), ucnType('\0')
{
	Add(new TGroupListColumn("Име", "F_NAME", 168));
	initialRefresh = false;
}

void TAccountListWindow::SetUCNType(char tUCNType)
{
	if (ucnType != tUCNType)
	{
		ucnType = tUCNType;
		Refresh();
	}
}

TGroup *TAccountListWindow::NewGroup() const
{
	TAccount *account = new TAccount;
	account->ucnType = ucnType;
	return account;
}

void TAccountListWindow::Criteria(msql &m, TGroup *group)
{
	m.Begin(group->Name);
	m.AddChar("F_UCN_TYPE", ucnType);
}

bool TAccountListWindow::AddGroup(TGroup *group)
{
	return TEditAccountDialog(this, TYPECHECK_PTR_CAST(TAccount, group), false).Execute() == IDOK;
}

bool TAccountListWindow::EditGroup(TGroup *group)
{
	return TEditAccountDialog(this, TYPECHECK_PTR_CAST(TAccount, group), true).Execute() == IDOK;
}

bool TAccountListWindow::AskDelete(TGroup *group)
{
	TExecListSum sum;
	strcpy(sum.sender, TYPECHECK_PTR_CAST(TAccount, group)->ucn);

	if (check_exists(sum.Name, &sum, "F_SENDER"))
		return error("Адресантът се използва.");

	return TGroupListWindow::AskDelete(group);
}
