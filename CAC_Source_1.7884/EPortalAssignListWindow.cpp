//{{TEPortalAssignListWindow Implementation}}

TEPortalAssignListWindow::TEPortalAssignListWindow(TWindow* parent, int resourceId, const TSideWind *tSideWind,
	long tEPortalMode)
:
	TGroupListWindow(parent, resourceId, NULL, 392), sideWind(tSideWind), eportalMode(tEPortalMode)
{
	Add(
		new TGroupListColumn("Вид дело", assign_law_type, 100),
		new TGroupListColumn("Номер/година", assign_law_no_year, 60, LVCFMT_RIGHT),
		new TGroupListColumn("Участие", assign_law_involvement, 136),
		new TGroupListColumn("Електронно призоваване", assign_eportal_subpoena, 84),
		NULL
	);
}

void TEPortalAssignListWindow::Active()
{
	TEPortAssign *assign = (TEPortAssign *) Group();
	assign->flags ^= EPORTAL_SUBPOENA;

	if (assign->Update("F_FLAGS"))
		RedrawGroup();
}

TGroup *TEPortalAssignListWindow::NewGroup() const
{
	TEPortAssign *assign = new TEPortAssign;
	assign->key = sideWind->key;
	return assign;
}

bool TEPortalAssignListWindow::AddGroup(TGroup *group)
{
	return get_eportal_assign(this, (TEPortAssign *) group, eportalMode, sideWind) && group->Insert();
}
