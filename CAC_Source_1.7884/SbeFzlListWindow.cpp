//{{TSbeFzlListWindow Implementation}}

TSbeFzlListWindow::TSbeFzlListWindow(TWindow* parent, int resourceId, TXArray<FZL> *fzl, TXArray<NPRFZLPNE> *tAllLinks,
	TFzlComboBox *tCombo, SBE *tSbe)
:
	TXMLGroupListWindow(parent, resourceId, fzl, 228), allLinks(tAllLinks), combo(tCombo), npr(tSbe->npr), sbe(tSbe)
{
	loadData = dynamic = false;

	Add(new TGroupListColumn("Èìå", group_print, 216));
}

bool TSbeFzlListWindow::PostDialog(FZL *fzl, bool edit)
{
	if (fzl->Includes("NPRFZLSTA"))
		if (TEditNprFzlStaDialog(this, fzl, npr).Execute() == IDCANCEL)
			return false;

	if (npr->Includes("NPRFZLPNE"))
	{
		TFzlNprPneListDialog(this, fzl, sbe, allLinks, edit).Execute();

		for (int i = 0; i < npr->dlo->nprfzlpne->Count(); i++)
			if ((*npr->dlo->nprfzlpne)[i]->fzlsid == fzl->sid)
				return true;

		return false;
	}

	return true;
}

bool TSbeFzlListWindow::AddGroup(TGroup *group)
{
	*group << combo->Group();
	return PostDialog((FZL *) group, false);
}

bool TSbeFzlListWindow::EditGroup(TGroup *group)
{
	return PostDialog((FZL *) group, true);
}

bool TSbeFzlListWindow::DeleteGroup(TGroup *)
{
	return true;
}

TGroup *TSbeFzlListWindow::NewGroup() const
{
	return ((TXArray<FZL> *) array)->NewObject();
}
