//{{TElectricItemListWindow Implementation}}

TElectricItemListWindow::TElectricItemListWindow(TWindow* parent, int resourceId, TRCDKeyContainer *tContainer,
	TGSArray<TElectric> *items, const TLawsuit *tLawsuit)
:
	TGroupListWindow(parent, resourceId, items, 442), container(tContainer), lawsuit(tLawsuit)
{
	Add(
		new TGroupListColumn("", electric_extra_user_print, 12),
		new TGroupListColumn("��������", electric_extra_type_print, 52),
		new TGroupListColumn("���", electric_extra_ext_print, 36),
		new TGroupListColumn("��������", group_print, 312),
		NULL
	);

	loadData = dynamic = false;
}

void TElectricItemListWindow::ViewGroup()
{
	TElectric *electric = Electric();

	if (electric)
		electric_action(electric->extraName, electric->extraType, ACTION_VIEW);
}

TGroup *TElectricItemListWindow::NewGroup() const
{
	TElectric *electric = new TElectric;

	*electric << container;
	electric->extraType = TYPE_IMAGE;
	electric->extraNo = 0;	// avoid conflicts w/ inReg
	return electric;
}

bool TElectricItemListWindow::AddGroup(TGroup *group)
{
	return attach_extra(this, (TElectric *) group);
}

bool TElectricItemListWindow::EditGroup(TGroup *group)
{
	return attach_extra(this, (TElectric *) group);
}

bool TElectricItemListWindow::AskDelete(TGroup *group)
{
	TElectric *electric = (TElectric *) group;
	return ask("������� �� �� �������� ������������� %s ��������?", Electrics[electric->extraType].what);
}

bool TElectricItemListWindow::DeleteGroup(TGroup *group)
{
	return ((TElectric *) group)->ElectricDelete(lawsuit, container);
}
