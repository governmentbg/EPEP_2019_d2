TStatQueryListWindow::TStatQueryListWindow(TWindow* parent, int resourceId)
:
	TGroupListWindow(parent, resourceId, new TGArray<TStatQueryThirdGroup>, 324)
{
	Add(new TGroupListColumn("", group_print, 312));
	loadData = false;
}


TGroup *TStatQueryListWindow::NewGroup() const
{
	return new TStatQueryThirdGroup;
}

bool TStatQueryListWindow::AddGroup(TGroup *group)
{
	TStatQueryThirdGroup *third = (TStatQueryThirdGroup *) group;
	TUserFile uf("�������");
	FILE *f;

	if (!GetOpenFileName(this, third->name, "�������� �������", OUT_PATH, "*DOP.TXT", sizeof third->name) ||
		(f = uf.Open(third->name, "r")) == NULL)
	{
		return false;
	}

	if (!third->Read(f))
		error("%s: ������ ��� ������ �� ��������� ���.", third->name);

	for (int i = 0; i < array->Count(); i++)
	{
		TStatQueryThirdGroup *tThird = (TStatQueryThirdGroup *) GroupAt(i);

		if (third->minDate != tThird->minDate || third->maxDate != tThird->maxDate)
			return error("��� �������������� � �������.");

		if (third->created != tThird->created)
			return error("��� �������������� ��� �������� �� ���������.");

		if (third->court == tThird->court)
		{
			mstr m;
			return error("���� ��� ������� ���� �� %s.", get_sencor_name(UCN_INSTANCE_LOWER, third->court, m));
		}
	}

	uf.Close();
	return true;
}

void TStatQueryListWindow::Launch()
{
	stat_query_third((TGArray<TStatQueryThirdGroup> *) array);
}
