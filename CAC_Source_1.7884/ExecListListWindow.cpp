//{{TExecListListWindow Implementation}}

TExecListListWindow::TExecListListWindow(TWindow* parent, int resourceId)
:
	TDoubleListWindow(parent, resourceId, new TGSArray<TPrintExecList>, 432)
{
	Add(
		new TGroupListColumn("", generic_null, 0),
		new TGroupListColumn("Дело", execlist_lawsuit, 80),
		new TGroupListColumn("Лист", execlist_exnodate, PRINT_EXEC_LIST_NO ? 80 : 60),
		new TGroupListColumn("Лица", execlist_debtors_line_1, PRINT_EXEC_LIST_NO ? 260 : 280),
		NULL
	);
}

void TExecListListWindow::Search(TExecList *execList)
{
	msql m(execList->Name);

	// put these into a search group?..
	m.AddLong("F_EXEC_LIST_KIND", EXEC_LIST_COUNTRY);
	m.Add(execList, "F_TYPE");
	if (execList->no)
		m.Add(execList, "F_NO");
	if (execList->year)
		m.Add(execList, "F_YEAR");
	if (!execList->execListDate.Empty())
		m.Add(execList, "F_EXEC_LIST_DATE");

	FlushVisuals();
	array->Flush();

	TQuery q(str(m));
	while (q.Read())
	{
		TPrintExecList *tExecList = new TPrintExecList(q);

		tExecList->CheckSend();
		tExecList->LoadCollects();
		InsertGroup(tExecList);
	}

	Refresh(GetClientRect().Width());

	if (array->Count())
	{
		SetVisualIndex(0);
		allowDeselect = true;
		Deselect();
		allowDeselect = false;
	}

	UpdateVisual();
}

void TExecListListWindow::DrawSecond(TListWindItem &lwi, TGroup *group)
{
	TPrintExecList *execList = TYPECHECK_PTR_CAST(TPrintExecList, group);

	if (execList->sent)
	{
		lwi.iSubItem = 1;
		lwi.SetText("Изпратен");
		SetItem(lwi);
	}

	lwi.iSubItem = 3;
	lwi.SetText(execList->debtext2);
	SetItem(lwi);
}

void TExecListListWindow::SetupWindow()
{
	TDoubleListWindow::SetupWindow();
	Refresh(GetClientRect().Width());
	UpdateVisual();
}
