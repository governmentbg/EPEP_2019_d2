//{{TPenalityListWindow Implementation}}

TPenalityListWindow::TPenalityListWindow(TWindow* parent, int resourceId)
:
	TGroupListWindow(parent, resourceId, new TLongAliasArray, 172)
{
	Add(
		new TGroupListColumn("", group_inactive, 12, LVCFMT_CENTER | LVCFMT_NO_SEARCH),
		new TGroupListColumn("Име", "F_NAME", 148),
		NULL
	);
}

void TPenalityListWindow::Subject()
{
	subject_reality(this, (TPenalityType *) Group(), "F_PENALITIES");
}

void TPenalityListWindow::Print()
{
	print_realticles((TLongAliasArray *) array, "налагане на глоби");
}

bool TPenalityListWindow::AddGroup(TGroup *group)
{
	return edit_reality(this, (TPenalityType *) group, false, IDD_EDIT_PENALITY_TYPE);
}

bool TPenalityListWindow::EditGroup(TGroup *group)
{
	return edit_reality(this, (TPenalityType *) group, true, IDD_EDIT_PENALITY_TYPE);
}

TGroup *TPenalityListWindow::NewGroup() const
{
	return new TPenalityType;
}
