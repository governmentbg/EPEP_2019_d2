//{{TSelectUCLPListBox Implementation}}

TSelectUCLPListBox::TSelectUCLPListBox(TWindow* parent, int resourceId, char *tName)
:
	TCloneListBox(parent, resourceId, NULL, TUCLP::Clone), name(tName)
{
	sortcol = 0;
}

void TSelectUCLPListBox::Criteria(msql &m, TGroup *group)
{
	m.Begin(group->Name);
	m.AddLike("F_NAME", name, true);
}

void TSelectUCLPListBox::GetGroupText(mstr &m, TGroup *group, int column)
{
	TUCLP *uclp = (TUCLP *) group;
	TArea area;
	TRegion region;

	TCloneListBox::GetGroupText(m, group, column);
	m.cat(" - ");
	area.area = uclp->area;

	if (area.Get())
		m.cat(area.name);
	else
		m.printf("%ld", area.area);

	m.cat(", ");
	region.area = uclp->area;
	region.region = uclp->region;

	if (region.Get())
		m.cat(region.name);
	else
		m.printf("%ld", region.region);
}
