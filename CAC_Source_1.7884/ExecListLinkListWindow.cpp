//{{TExecListLinkListWindow Implementation}}

TExecListLinkListWindow::TExecListLinkListWindow(TWindow* parent, int resourceId, TGArray<TLinkWind> *xLinks)
:
	TTryAddressSideListWindow(parent, resourceId, xLinks, 432)
{
	Add(
		new TGroupListColumn("�������", basewind_involvement, 84),
		new TGroupListColumn("���", ucngroup_ucn_type, 28),
		new TGroupListColumn("���", ucngroup_ucn, 48),
		new TGroupListColumn("���/�����", basewind_names, 260),
		NULL
	);
}

void TExecListLinkListWindow::DrawSecond(TListWindItem &lwi, TGroup *group)
{
	mstr m;

	lwi.iSubItem = 4;
	execlink_address(m, group);
	lwi.SetText(str(m));
	SetItem(lwi);
}
