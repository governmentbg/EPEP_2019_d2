//{{TStreetListWindow Implementation}}

TStreetListWindow::TStreetListWindow(TWindow* parent, int resourceId, const long &tNewUCLP)
:
	TCadrListWindow(parent, resourceId, ZERO_LONG), uclp(0), newUCLP(tNewUCLP)
{
}

TGroup *TStreetListWindow::NewGroup() const
{
	TStreet *street = new TStreet;

	street->uclp = uclp;
	street->street = 0;

	return street;
}

bool TStreetListWindow::AddGroup(TGroup *group)
{
	return add_street(this, (TStreet *) group);
}

bool TStreetListWindow::EditGroup(TGroup *group)
{
	return edit_street(this, (TStreet *) group);
}

bool TStreetListWindow::AskDelete(TGroup *group)
{
	static constant tables[] = { "T_STREET_MAP", "T_ADDRESS", "T_REGISTERED_MAIL", "T_SUBPOENA", "T_ANNOUNCE", NULL };
	const char *table = check_exists(tables, group, "F_UCLP, F_STREET");

	if (table)
		return error("Улицата се използва в %s.", table);

	return TCadrListWindow::AskDelete(group);
}

void TStreetListWindow::Activate()
{
	if (uclp != newUCLP)
	{
		uclp = newUCLP;
		Refresh();
	}
}
