//{{TAreaListWindow Implementation}}

TAreaListWindow::TAreaListWindow(TWindow* parent, int resourceId, const long &newArea)
:
	TCadrListWindow(parent, resourceId, newArea)
{
}

TGroup *TAreaListWindow::NewGroup() const
{
	TArea *area = new TArea;
	area->area = 0;
	return area;
}

bool TAreaListWindow::AddGroup(TGroup *group)
{
	return TEditAreaDialog(this, (TArea *) group, false).Execute() == IDOK;
}

bool TAreaListWindow::EditGroup(TGroup *group)
{
	return TEditAreaDialog(this, (TArea *) group, true).Execute() == IDOK;
}

void TAreaListWindow::Activate()
{
	if (!initialRefresh)
		TCadrListWindow::Activate();
}
