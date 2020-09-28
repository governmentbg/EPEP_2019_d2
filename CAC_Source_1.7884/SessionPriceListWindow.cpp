//{{TSessionPriceListWindow Implementation}}

TSessionPriceListWindow::TSessionPriceListWindow(TWindow* parent, int resourceId)
:
	TGroupListWindow(parent, resourceId, new TGSArray<TSessionPrice>, 132)
{
	Add(
		new TGroupListColumn("������� ����", "F_DATE", 60),
		new TGroupListColumn("������ ������", "F_PRICE", 60),
		NULL
	);
}

TGroup *TSessionPriceListWindow::NewGroup() const
{
	return new TSessionPrice;
}

bool TSessionPriceListWindow::AddGroup(TGroup *group)
{
	return TEditSessionPriceDialog(this, (TSessionPrice *) group, false).Execute() == IDOK;
}

bool TSessionPriceListWindow::EditGroup(TGroup *group)
{
	return TEditSessionPriceDialog(this, (TSessionPrice *) group, true).Execute() == IDOK;
}

void TSessionPriceListWindow::Print()
{
	price_list(this, array);
}
