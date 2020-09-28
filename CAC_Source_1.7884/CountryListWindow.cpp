//{{TCountryListWindow Implementation}}

TCountryListWindow::TCountryListWindow(TWindow* parent, int resourceId)
:
	TGroupListWindow(parent, resourceId, NULL, 172)
{
	Add(
		new TGroupListColumn("Код", "F_COUNTRY", 24),
		new TGroupListColumn("Име", "F_NAME", 136),
		NULL
	);

	sortcol = 2;
}

bool TCountryListWindow::AddGroup(TGroup *group)
{
	return TEditCountryDialog(this, (TCountry *) group, false).Execute() == IDOK;
}

bool TCountryListWindow::EditGroup(TGroup *group)
{
	return TEditCountryDialog(this, (TCountry *) group, true).Execute() == IDOK;
}
