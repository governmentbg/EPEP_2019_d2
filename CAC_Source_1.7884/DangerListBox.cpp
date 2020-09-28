//{{TDangerListBox Implementation}}

TDangerListBox::TDangerListBox(TWindow* parent, int resourceId)
:
	TCloneListBox(parent, resourceId, NULL, TDanger::Clone)
{
	sortcol = 0;
}

void TDangerListBox::Criteria(msql &m, TGroup *group)
{
	m.Begin(group->Name);
	m.Add("NOT");
	m.printf("'' || F_DANGER LIKE '%s'", DANGER_LIKE_SECTION);
}
