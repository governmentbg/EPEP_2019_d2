//{{TSchedListWindow Implementation}}

TSchedListWindow::TSchedListWindow(TWindow* parent, int resourceId, TSched *tSched, const char *tScheds)
:
	TGroupListWindow(parent, resourceId, NULL, 284), sched(tSched), scheds(tScheds)
{
	Add(
		new TGroupListColumn("��� ����", sched_sched, 164),
		new TGroupListColumn("������� ����", "F_START", 60, LVCFMT_RIGHT),
		new TGroupListColumn("������ ����", "F_FINAL", 60, LVCFMT_RIGHT),
		NULL
	);
}

TSched *TSchedListWindow::NewGroup() const
{
	TSched *tSched = new TSched;
	*tSched << sched;
	return tSched;
}

bool TSchedListWindow::AddGroup(TGroup *group)
{
	TSched *tSched = (TSched *) group;

	if (TAddSchedDialog(this, tSched, scheds).Execute() == IDOK)
	{
		log(LOG_ADD, CM_CLERK_EDIT_SCHEDS, tSched, "�� ����");
		return true;
	}

	return false;
}

bool TSchedListWindow::EditGroup(TGroup *group)
{
	TSched *tSched = (TSched *) group;

	if (TEditSchedDialog(this, tSched).Execute() == IDOK)
	{
		log(LOG_EDIT, CM_CLERK_EDIT_SCHEDS, tSched, "�� ����");
		return true;
	}

	return false;
}

bool TSchedListWindow::AskDelete(TGroup *)
{
	return ask("������� �� �� �������� ��������� ����?");
}

bool TSchedListWindow::DeleteGroup(TGroup *group)
{
	if (TGroupListWindow::DeleteGroup(group))
	{
		log(LOG_DELETE, CM_CLERK_EDIT_SCHEDS, (TSched *) group, "�� ����");
		return true;
	}

	return false;
}
