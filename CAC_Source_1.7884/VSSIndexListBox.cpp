//{{TVSSIndexListBox Implementation}}

TVSSIndexListBox::TVSSIndexListBox(TWindow* parent, int resourceId)
:
	TCloneListBox(parent, resourceId, NULL, TVSSIndex::Clone)
{
	build_ctypes(types, TYPE_LAWSUITS, false);
	loadData = false;
}

void TVSSIndexListBox::SetCrit(const CDate &tStart, const CDate &tFinal)
{
	if (start != tStart || final != tFinal)
	{
		start = tStart;
		final = tFinal;
		Refresh();
	}
}

void TVSSIndexListBox::Refresh()
{
	array->Flush();
	array->Add(new TVSSIndex());
	TVSSIndexSet::AddMatching(*array, COURT_TYPE + '0', types, '\0', start, final, false);
	TCloneListBox::Refresh();
}
