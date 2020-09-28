#undef TListWindow

//{{TTestListWindow Implementation}}

TTestListWindow::TTestListWindow(TWindow* parent, int resourceId, TListWEmpty *tEmptyRect)
:
	TNTListWindow(parent, resourceId), emptyRect(tEmptyRect)
{
}

void TTestListWindow::SetupWindow()
{
	TNTListWindow::SetupWindow();

	TListWindColumn lwc;

	lwc.SetFormat(TListWindColumn::Left);

	lwc.SetText("col0");
	lwc.SetWidth(0);
	InsertColumn(0, lwc);

	lwc.SetWidth(40);
	lwc.SetText("col1");
	InsertColumn(1, lwc);

	TListWindItem lwi;

	lwi.iItem = 0;
	lwi.iSubItem = 0;
	lwi.mask = 0;
	lwi.SetText("text0");
	InsertItem(lwi);

	lwi.iSubItem = 1;
	lwi.SetText("text1");
	SetItem(lwi);

	emptyRect->flag = GetItemRect(0, emptyRect, Bounds);
}
