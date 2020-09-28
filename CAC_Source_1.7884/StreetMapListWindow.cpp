//{{TStreetMapListWindow Implementation}}

TStreetMapListWindow::TStreetMapListWindow(TWindow* parent, TUser *tUser, int resourceId)
:
	TGroupListWindow(parent, resourceId, NULL, 320), user(tUser), uclp(0)
{
	Add(new TGroupListColumn("Улица/номера", group_print, 308));
	initialRefresh = false;
	sortcol = 1;
}

void TStreetMapListWindow::SetUCLP(long tUCLP)
{
	if (uclp != tUCLP)
	{
		uclp = tUCLP;
		Refresh();
	}
}

void TStreetMapListWindow::Print()
{
	print_street_map(user->ucn);
}

void TStreetMapListWindow::Copy()
{
	if (copy_street_map(this, user, (TGArray<TStreetMap> *) array))
		Refresh();
}

TGroup *TStreetMapListWindow::NewGroup() const
{
	TStreetMap *streetMap = new TStreetMap;

	strcpy(streetMap->ucn, user->ucn);
	streetMap->uclp = uclp;
	return streetMap;
}

bool TStreetMapListWindow::AddGroup(TGroup *group)
{
	return add_street_map(this, (TStreetMap *) group);
}

bool TStreetMapListWindow::EditGroup(TGroup *group)
{
	return TNumapDialog(this, (TStreetMap *) group).Execute() == IDOK;
}
