TCombineCorruptListWindow::TCombineCorruptListWindow(TWindow* parent, int resourceId, TCombineCorruptGroup *tGroup)
:
	TGroupListWindow(parent, resourceId, &tGroup->array, 244), group(tGroup)
{
	Add(new TGroupListColumn("", group_print, 232));
	loadData = dynamic = false;
}

TGroup *TCombineCorruptListWindow::NewGroup() const
{
	return new TCombineCorruptItem;
}

bool TCombineCorruptListWindow::AddGroup(TGroup *tGroup)
{
	char name[SIZE_OF_PATH];

	if (!GetOpenFileName(this, name, "Хипертекстови файлове", OUT_PATH, "Corrupt_*.htm", sizeof name))
		return false;

	TCombineCorruptGroup meta;
	TCombineCorruptItem *item = (TCombineCorruptItem *) tGroup;

	if (!item->Read(name, meta))
		return false;

	if (array->Count() == 0)
		*group << meta;
	else if (meta.minMonth != group->minMonth || meta.minYear != group->minYear ||
		meta.maxMonth != group->maxMonth || meta.maxYear != group->maxYear)
	{
		return error("Периодът не съвпада.", name);
	}

	for (int i = 0; i < array->Count(); i++)
	{
		const TCombineCorruptItem *tItem = (const TCombineCorruptItem *) (*array)[i];

		if (tItem->courtCode == item->courtCode)
			return error("Вече има файл за %s.", Court->Seek(item->courtCode));
	}

	return true;
}
