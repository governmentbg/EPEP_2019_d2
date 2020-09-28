//{{TReasonListWindow Implementation}}

TReasonListWindow::TReasonListWindow(TWindow* parent, int resourceId)
:
	TGroupListWindow(parent, resourceId, new TLongAliasArray, 172)
{
	Add(
		new TGroupListColumn("", group_inactive, 12, LVCFMT_CENTER | LVCFMT_NO_SEARCH),
		new TGroupListColumn("Име", "F_NAME", 148),
		NULL
	);
}

void TReasonListWindow::Subject()
{
	subject_reality(this, (TReasonType *) Group(), "F_REASONS");
}

void TReasonListWindow::Print()
{
	print_realticles((TLongAliasArray *) array, "назначаване на представители");
}

bool TReasonListWindow::AddGroup(TGroup *group)
{
	return edit_reality(this, (TReasonType *) group, false, IDD_EDIT_REASON_TYPE);
}

bool TReasonListWindow::EditGroup(TGroup *group)
{
	return edit_reality(this, (TReasonType *) group, true, IDD_EDIT_REASON_TYPE);
}

TGroup *TReasonListWindow::NewGroup() const
{
	return new TReasonType;
}
