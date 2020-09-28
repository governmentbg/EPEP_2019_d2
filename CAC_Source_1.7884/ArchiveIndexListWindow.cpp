//{{TArchiveIndexListWindow Implementation}}

TArchiveIndexListWindow::TArchiveIndexListWindow(TWindow* parent, int resourceId)
:
	TGroupListWindow(parent, resourceId, new TGSArray<TArchiveIndex>, 360)
{
	Add(
		new TGroupListColumn("", group_inactive, 12, LVCFMT_CENTER | LVCFMT_NO_SEARCH),
		new TGroupListColumn("Индекс", "F_ARCHIVE_INDEX", 80),
		new TGroupListColumn("Срок", "F_KEEP", 24),
		new TGroupListColumn("Име", "F_NAME", 232),
		NULL
	);
}

void TArchiveIndexListWindow::Subject()
{
	subject_archive(this, (TArchiveIndex *) Group());
}

void TArchiveIndexListWindow::Print()
{
	print_archive_indexes(array);
}

void TArchiveIndexListWindow::Empty(mstr &m)
{
	for (int i = 0; i < array->Count(); i++)
	{
		TArchiveIndex *index = (TArchiveIndex *) GroupAt(i);
		msql w(ArchiveSubject->Name);

		w.AddString("F_ARCHIVE_INDEX", index->archiveIndex);
		TQuery q(str(w));

		if (!q.Read())
		{
			m.sep(", ");
			m.cat(index->archiveIndex);
		}
	}
}

TGroup *TArchiveIndexListWindow::NewGroup() const
{
	return new TArchiveIndex;
}

bool TArchiveIndexListWindow::AddGroup(TGroup *group)
{
	return TEditArchiveIndexDialog(this, (TArchiveIndex *) group, false).Execute() == IDOK;
}

bool TArchiveIndexListWindow::EditGroup(TGroup *group)
{
	return TEditArchiveIndexDialog(this, (TArchiveIndex *) group, true).Execute() == IDOK;
}
