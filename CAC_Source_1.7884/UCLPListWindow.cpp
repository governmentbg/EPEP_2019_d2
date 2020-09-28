//{{TUCLPListWindow Implementation}}

TUCLPListWindow::TUCLPListWindow(TWindow* parent, int resourceId, const long &tNewArea, const long &tNewRegion, const long &newUCLP)
:
	TCadrListWindow(parent, resourceId, newUCLP), area(0), region(0), newArea(tNewArea), newRegion(tNewRegion)
{
}

TGroup *TUCLPListWindow::NewGroup() const
{
	TUCLP *uclp = new TUCLP;

	uclp->area = area;
	uclp->region = region;
	uclp->uclp = 0;

	return uclp;
}

bool TUCLPListWindow::AddGroup(TGroup *group)
{
	TUCLP *uclp = (TUCLP *) group;
	return add_uclp(this, uclp, true) && uclp->area == area && uclp->region == region;
}

bool TUCLPListWindow::EditGroup(TGroup *group)
{
	TUCLP *uclp = (TUCLP *) group;

	// 2010:277 LPR: remove if moved to another area/region
	if (edit_uclp(this, uclp, true))
	{
		if (uclp->area == area && uclp->region == region)
			return true;

		RemoveGroup(IDEDIT);
	}

	return false;
}

bool TUCLPListWindow::AskDelete(TGroup *group)
{
	static constant tables[] = { "T_STREET", "T_STREET_MAP", "T_COURT", "T_ADDRESS", "T_REGISTERED_MAIL", "T_SUBPOENA",
		"T_ANNOUNCE", NULL };
	const char *table = check_exists(tables, group, "F_UCLP");

	if (table)
		return error("Населеното място се използва в %s.", table);

	return TCadrListWindow::AskDelete(group);
}

void TUCLPListWindow::Activate()
{
	if (area != newArea || region != newRegion)
	{
		area = newArea;
		region = newRegion;
		TCadrListWindow::Activate();
	}
}
