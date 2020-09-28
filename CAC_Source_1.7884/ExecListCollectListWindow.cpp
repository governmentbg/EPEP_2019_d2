//{{TExecListCollectListWindow Implementation}}

TExecListCollectListWindow::TExecListCollectListWindow(TWindow* parent, int resourceId,
	TPrintExecList *tExecList)
:
	TGroupListWindow(parent, resourceId, tExecList->collects, 356), execList(tExecList)
{
	Add(
		new TGroupListColumn("Дата", "F_DATE", 52),
		new TGroupListColumn("Сума", "F_SUM", 60, LVCFMT_RIGHT),
		new TGroupListColumn("Задължено лице", collect_names, 232),
		NULL
	);
}

TGroup *TExecListCollectListWindow::NewGroup() const
{
	TExecListCollect *collect = new TExecListCollect;
	*collect << execList;
	return collect;
}

bool TExecListCollectListWindow::AddGroup(TGroup *group)
{
	return TEditExecListCollectDialog(this, execList, TYPECHECK_PTR_CAST(TExecListCollect, group), false).Execute() == IDOK;
}

bool TExecListCollectListWindow::EditGroup(TGroup *group)
{
	return TEditExecListCollectDialog(this, execList, TYPECHECK_PTR_CAST(TExecListCollect, group), true).Execute() == IDOK;
}
