//{{TExportArchivesListWindow Implementation}}

TExportArchivesListWindow::TExportArchivesListWindow(TWindow* parent, int resourceId)
:
	TGroupListWindow(parent, resourceId, NULL, 348)
{
	Add(
		new TGroupListColumn("Вид дело", keygroup_key_type, 60),
		new TGroupListColumn("Номер/Година", keygroup_key_no_year, 76, LVCFMT_RIGHT),
		new TGroupListColumn("Предмет на делото", lawsuit_subject, 204),
		NULL
	);

	initialRefresh = false;
}

void TExportArchivesListWindow::Export()
{
	export_archives(this, array);
}

TGroup *TExportArchivesListWindow::NewGroup() const
{
	return new TLawsuit;
}

bool TExportArchivesListWindow::AddGroup(TGroup *group)
{
	TLawsuit *lawsuit = (TLawsuit *) group;

	if (!get_group(this, lawsuit, TYPE_LAWSUITS, NULL, extern_archable, GET_GROUP_ARCHFN))
		return false;

	for (int i = 0; i < array->Count(); i++)
		if (LawsuitAt(i)->key == lawsuit->key)
			return error("Делото вече е включено в списъка.");

	return true;
}

bool TExportArchivesListWindow::DeleteGroup(TGroup *)
{
	return true;
}
