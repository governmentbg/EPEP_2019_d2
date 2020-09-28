//{{THallListWindow Implementation}}

THallListWindow::THallListWindow(TWindow* parent, int resourceId)
:
	TGroupListWindow(parent, resourceId, new TGSArray<THall>, 172)
{
	Add(
		new TGroupListColumn("", group_inactive, 12, LVCFMT_NO_SEARCH),
		new TGroupListColumn("Èìå", "F_NAME", 148),
		NULL
	);
}

void THallListWindow::LoadWeek()
{
	TLoadWeek loadWeek;
	loadWeek.hall = Hall()->hall;
	TLoadWeekDialog(this, &loadWeek).Execute();
}

void THallListWindow::LoadDate()
{
	load_date(this, Hall());
}

void THallListWindow::Print()
{
	TPeriodGroup period("T_LOAD_HALL");

	period.minDate.day = period.minDate.month = 1;
	if (get_period(this, &period))
		print_halls(&period);
}

TGroup *THallListWindow::NewGroup() const
{
	return new THall;
}

bool THallListWindow::AddGroup(TGroup *group)
{
	return edit_hall(this, (THall *) group, false);
}

bool THallListWindow::EditGroup(TGroup *group)
{
	return edit_hall(this, (THall *) group, true);
}
