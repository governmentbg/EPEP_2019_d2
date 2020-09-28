//{{TArchiveIndexListBox Implementation}}

TArchiveIndexListBox::TArchiveIndexListBox(TWindow* parent, int resourceId, bool tEmpty)
:
	TCloneListBox(parent, resourceId, NULL, TArchiveIndex::Clone), empty(tEmpty)
{
}

void TArchiveIndexListBox::SetMark(TGArray<TArchiveIndex> *indexes)
{
	for (int i = 0; i < indexes->Count(); i++)
		for (int j = 0; j < array->Count(); j++)
			if (!strcmp((*indexes)[i]->archiveIndex, ArchiveIndexAt(j)->archiveIndex))
				SetSel(j, true);
}

void TArchiveIndexListBox::LoadData(const char *s)
{
	TCloneListBox::LoadData(s);

	if (empty)
		InsertEmpty();
}

void TArchiveIndexListBox::GetGroupText(mstr &m, TGroup *group, int column)
{
	if (empty)
		TCloneListBox::GetGroupText(m, group, column);
	else
	{
		TArchiveIndex *archiveIndex = (TArchiveIndex *) group;
		m.printf("%-15s %s", archiveIndex->archiveIndex, archiveIndex->name);
	}
}
