//{{TDangerListWindow Implementation}}

TDangerListWindow::TDangerListWindow(TWindow* parent, int resourceId)
:
	TGroupListWindow(parent, resourceId, new TGSArray<TDanger>, 248)
{
	Add(
		new TGroupListColumn("", group_inactive, 12, LVCFMT_CENTER | LVCFMT_NO_SEARCH),
		new TGroupListColumn("»ÏÂ", danger_indent_name, 224),
		NULL
	);
}

void TDangerListWindow::Subject()
{
	subject_danger(this, (TDanger *) Group());
}

void TDangerListWindow::Print()
{
	print_dangers((TGArray<TDanger> *) array);
}

TGroup *TDangerListWindow::NewGroup() const
{
	return new TDanger;
}

bool TDangerListWindow::EditGroup(TGroup *group)
{
	return TEditDangerDialog(this, (TDanger *) group).Execute() == IDOK;
}
