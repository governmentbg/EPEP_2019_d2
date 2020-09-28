//{{TStreetMapListBox Implementation}}

TStreetMapListBox::TStreetMapListBox(TWindow* parent, int resourceId)
:
	TGroupListBox(parent, resourceId, NULL), user(NULL)
{
	qsMode = QUICK_SEARCH_ABLE;
	initialRefresh = false;
}

void TStreetMapListBox::SetUser(const char *tUser)
{
	user = tUser;
	Refresh();
}

TGroup *TStreetMapListBox::NewGroup() const
{
	return new TStreetMap;
}

void TStreetMapListBox::Criteria(msql &m, TGroup *group)
{
	m.Begin(group->Name);
	m.AddString("F_UCN", user);
	m.AddOrder("F_UCLP, F_STREET");
}
