//{{TInformedListWindow Implementation}}

TInformedListWindow::TInformedListWindow(TWindow* parent, int resourceId)
:
	TGroupListWindow(parent, resourceId, new TGSArray<TInformed>, 104)
{
	Add(new TGroupListColumn("Вид дело  Номер/Година", group_print, 104));
	initialRefresh = false;
	*informed = '\0';
}

void TInformedListWindow::SetInformed(const char *tInformed)
{
	if (strcmp(informed, tInformed))
	{
		strcpy(informed, tInformed);
		Refresh();
	}
}

void TInformedListWindow::Import()
{
	import_informed(this, informed);
	Refresh();
}

void TInformedListWindow::Print()
{
	print_informeds(this, array, informed);
}

TGroup *TInformedListWindow::NewGroup() const
{
	TInformed *tInformed = new TInformed;
	strcpy(tInformed->ucn, informed);
	return tInformed;
}

bool TInformedListWindow::AddGroup(TGroup *group)
{
	return TEditInformedDialog(this, (TInformed *) group, false).Execute() == IDOK;
}

bool TInformedListWindow::EditGroup(TGroup *group)
{
	return TEditInformedDialog(this, (TInformed *) group, true).Execute() == IDOK;
}
