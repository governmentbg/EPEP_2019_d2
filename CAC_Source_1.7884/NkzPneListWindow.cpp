//{{TNkzPneListWindow Implementat*ion}}

TNkzPneListWindow::TNkzPneListWindow(TWindow* parent, int resourceId, NPRFZLPNE *tNPRFZLPNE, FZL *tFZL, NPR *tNPR)
:
	TXMLGroupListWindow(parent, resourceId, tNPRFZLPNE->scq->nkzpne, 228), nprfzlpne(tNPRFZLPNE), fzl(tFZL), npr(tNPR)
{
	loadData = false;
	Add(new TGroupListColumn("Наказание", group_print, 216));
}

TGroup *TNkzPneListWindow::NewGroup() const
{
	return nprfzlpne->scq->nkzpne->NewObject();
}

bool TNkzPneListWindow::AddGroup(TGroup *group)
{
	return TEditNkzPneDialog(this, fzl, (NKZPNE *) group, npr->nmr).Execute() == IDOK;
}

bool TNkzPneListWindow::EditGroup(TGroup *group)
{
	return TEditNkzPneDialog(this, fzl, (NKZPNE *) group, npr->nmr).Execute() == IDOK;
}

bool TNkzPneListWindow::DeleteGroup(TGroup *)
{
	return true;
}
