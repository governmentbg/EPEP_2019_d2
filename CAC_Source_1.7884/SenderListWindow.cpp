//{{TSenderListWindow Implementation}}

TSenderListWindow::TSenderListWindow(TWindow* parent, int resourceId)
:
	TGroupListWindow(parent, resourceId, NULL, 180), ucnType('\0')
{
	Add(new TGroupListColumn("Èìå", "F_NAME", 168));
	initialRefresh = false;
	sortcol = 1;
}

void TSenderListWindow::SetUCNType(char tUCNType)
{
	if (ucnType != tUCNType)
	{
		ucnType = tUCNType;
		Refresh();
	}
}

void TSenderListWindow::List()
{
	print_senders(array, ucnType);
}

TGroup *TSenderListWindow::NewGroup() const
{
	TSender *sender = new TSender;
	sender->ucnType = ucnType;
	return sender;
}

bool TSenderListWindow::AddGroup(TGroup *group)
{
	return add_sender(this, (TSender *) group, ucnType);
}

bool TSenderListWindow::EditGroup(TGroup *group)
{
	return edit_sender(this, (TSender *) group);
}

bool TSenderListWindow::DeleteGroup(TGroup *group)
{
	TAddress address;

	address << group;
	address.Delete(true);
	return group->Delete(true);
}
