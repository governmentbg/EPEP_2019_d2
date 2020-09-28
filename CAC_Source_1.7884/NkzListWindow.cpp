//{{TNkzListWindow Implementation}}

TNkzListWindow::TNkzListWindow(TWindow* parent, int resourceId, FZL *tFZL, SBE *tSbe)
:
	TXMLGroupListWindow(parent, resourceId, tFZL->nkz, 228), fzl(tFZL), sbe(tSbe)
{
	loadData = false;
	Add(new TGroupListColumn("Наказание", group_print, 216));
}

TGroup *TNkzListWindow::NewGroup() const
{
	return fzl->nkz->NewObject();
}

bool TNkzListWindow::AddGroup(TGroup *group)
{
	NKZ *nkz = (NKZ *) group;

	if (TEditNkzDialog(this, fzl, nkz, sbe).Execute() == IDOK)
	{
		TXMLInt *tip = (TXMLInt *) nkz->FindAttrib("nkztip");

		// 2013:028 TRQ/LPR: auto copy
		if (nkz->tip == nakazanie_za_iztarpjavane && tip->lset.Contains(obsto_nakazanie))
		{
			NKZ *tNKZ = (NKZ *) NewGroup();

			*tNKZ << group;
			tNKZ->tip = obsto_nakazanie;
			InsertGroup(tNKZ);
		}

		return true;
	}

	return false;
}

bool TNkzListWindow::EditGroup(TGroup *group)
{
	return TEditNkzDialog(this, fzl, (NKZ *) group, sbe).Execute() == IDOK;
}

bool TNkzListWindow::DeleteGroup(TGroup *)
{
	return true;
}
