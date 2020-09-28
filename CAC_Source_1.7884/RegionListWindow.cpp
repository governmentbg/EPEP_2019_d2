//{{TRegionListWindow Implementation}}

TRegionListWindow::TRegionListWindow(TWindow* parent, int resourceId, const long &tNewArea, const long &newRegion)
:
	TCadrListWindow(parent, resourceId, newRegion), area(0), newArea(tNewArea)
{
}

TGroup *TRegionListWindow::NewGroup() const
{
	TRegion *region = new TRegion;

	region->area = area;
	region->region = 0;

	return region;
}

bool TRegionListWindow::AddGroup(TGroup *group)
{
	return TEditRegionDialog(this, (TRegion *) group, false).Execute() == IDOK;
}

bool TRegionListWindow::EditGroup(TGroup *group)
{
	return TEditRegionDialog(this, (TRegion *) group, true).Execute() == IDOK;
}

void TRegionListWindow::Activate()
{
	if (area != newArea)
	{
		area = newArea;
		TCadrListWindow::Activate();
	}
}
