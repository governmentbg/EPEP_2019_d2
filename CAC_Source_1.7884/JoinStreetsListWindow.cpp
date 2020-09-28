TJoinStreetsListWindow::TJoinStreetsListWindow(TWindow* parent, int resourceId, TGSArray<TJoinStreet> *streets)
	: TGroupListWindow(parent, resourceId, streets, 180)
{
	Add(
		new TGroupListColumn("Код", "F_STREET", 20, LVCFMT_RIGHT | LVCFMT_NO_SEARCH),
		new TGroupListColumn("Име", "F_NAME", 148),
		NULL
	);

	loadData = dynamic = false;
}

void TJoinStreetsListWindow::Join(TJoinStreet *target, TJoinStreetsListWindow *targets)
{
	if (join(Street(), target))
	{
		targets->SetVisualIndex(-1);
		RemoveGroup(IDC_MANUALLY_ADD);
	}
}
