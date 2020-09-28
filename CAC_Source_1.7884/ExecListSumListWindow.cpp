//{{TExecListSumListWindow Implementation}}

TExecListSumListWindow::TExecListSumListWindow(TWindow* parent, int resourceId, TExecList *tExecList)
:
	TDoubleListWindow(parent, resourceId, &tExecList->sums, 432), execList(tExecList)
{
	Add(
		new TGroupListColumn("", generic_null, 0),
		new TGroupListColumn("Вид задължение", execlistsum_alias, 68),
		new TGroupListColumn("Сума", execlistsum_sum, 60, LVCFMT_RIGHT),
		new TGroupListColumn(execList->execListKind == EXEC_LIST_PRIVATE ? "Задължени лица" : "По сметка на",
			execlistsum_persons_line_1, 292),
		NULL
	);
}

bool TExecListSumListWindow::AddGroup(TGroup *group)
{
	return edit_exec_list_sum(this, execList, (TExecListSum *) group, false);
}

bool TExecListSumListWindow::EditGroup(TGroup *group)
{
	return edit_exec_list_sum(this, execList, (TExecListSum *) group, true);
}

bool TExecListSumListWindow::AskDelete(TGroup *)
{
	return ask("Желаете ли да изтриете избраното задължение?");
}

bool TExecListSumListWindow::DeleteGroup(TGroup *)
{
	return true;
}

TGroup *TExecListSumListWindow::NewGroup() const
{
	TExecListSum *sum = new TExecListSum;
	*sum << execList;
	return sum;
}

void TExecListSumListWindow::DrawSecond(TListWindItem &lwi, TGroup *group)
{
	mstr m;

	lwi.iSubItem = 3;
	execlistsum_persons_line_2(m, group);
	lwi.SetText(str(m));
	SetItem(lwi);
}

void TExecListSumListWindow::SetupWindow()
{
	TDoubleListWindow::SetupWindow();
	Refresh(GetClientRect().Width());
}
