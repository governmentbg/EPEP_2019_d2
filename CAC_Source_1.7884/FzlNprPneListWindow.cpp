//{{TFzlNprPneListWindow Implementat*ion}}

TFzlNprPneListWindow::TFzlNprPneListWindow(TWindow* parent, int resourceId, FZL *tFZL, SBE *tSbe)
:
	TXMLGroupListWindow(parent, resourceId, tSbe->npr->dlo->nprfzlpne, 228), fzl(tFZL), npr(tSbe->npr), sbe(tSbe),
		freeLinks("NPRFZLPNE"), freePnes("PNE")
{
	loadData = false;

	NPRFZLPNE nprfzlpne;
	msql m;
	PNE pne;

	if (!npr->key.no)
	{
		// 2012:331 FIX: loaded from XML, no key
		npr->key.no = atoi(npr->dlo->nmr);
		npr->key.year = npr->dlo->gdn;
		npr->key.type = TYPE_PUNISHMENT_LAW;
	}

	nprfzlpne << npr;
	nprfzlpne.fzlsid = fzl->sid;
	nprfzlpne.FloatCriteria(m);
	nprfzlpne.LoadArray(&freeLinks, str(m), npr->dlo->nprfzlpne);

	for (int i = 0; i < npr->dlo->nprfzlpne->Count(); i++)
	{
		NPRFZLPNE *nprfzlpne = (*npr->dlo->nprfzlpne)[i];
		int index = find_nprfzlpne_index(&freeLinks, nprfzlpne->pnesid);

		if (index != INT_MAX)
			freeLinks.Remove(index);

		// 2013:036 FIX: loaded from XML, no key
		nprfzlpne->key = npr->key;
	}

	pne.LoadFloat(&freePnes, npr, npr->dlo->pne);
	// 2012:331 FIX: remove loaded pnes as well
	for (int i = 0; i < npr->dlo->pne->Count(); i++)
	{
		int index = find_pne_index(&freePnes, (*npr->dlo->pne)[i]->sid);

		if (index != INT_MAX)
			freePnes.Remove(index);
	}

	Add(new TGroupListColumn("Квалификация", group_print, 216));
}

void TFzlNprPneListWindow::GetGroupText(mstr &m, TGroup *group, int column)
{
	if (column == 1)
		print_nprfzlpne(m, group, npr);
}

TGroup *TFzlNprPneListWindow::NewGroup() const
{
	NPRFZLPNE *nprfzlpne = npr->dlo->nprfzlpne->NewObject();
	*nprfzlpne << fzl;
	return nprfzlpne;
}

bool TFzlNprPneListWindow::PostDialog(NPRFZLPNE *nprfzlpne, bool edit)
{
	if (nprfzlpne->Includes("NKZPNE"))
	{
		// 2012:335 LPR/LRQ
		if (nprfzlpne->scq->vid == scq_osajda || (nprfzlpne->scq->nkzpne->flags & XML_REQUIRED))
		{
			TNkzPneListDialog(this, nprfzlpne, fzl, npr, edit).Execute();
			return nprfzlpne->scq->nkzpne->Count() > 0;
		}
		else
			nprfzlpne->scq->nkzpne->Clear();
	}

	return true;
}

bool TFzlNprPneListWindow::AddGroup(TGroup *group)
{
	TGArray<PNE> pnes;

	for (int i = 0; i < freeLinks.Count(); i++)
		pnes.Add(find_pne(&freePnes, freeLinks[i]->pnesid));

	NPRFZLPNE *nprfzlpne = (NPRFZLPNE *) group;
	*nprfzlpne << freeLinks[0];

	if (sbe->sbh->stn == psd_osaditelna)
		nprfzlpne->scq->vid = scq_osajda;

	// 2012:331 LPR: do not alter links if Add was abandoned at PostDialog
	if (TEditNprFzlPneDialog(this, fzl, nprfzlpne, &pnes, -1, sbe, &freeLinks).Execute() == IDOK &&
		PostDialog(nprfzlpne, false))
	{
		npr->dlo->pne->Add(find_pne(&freePnes, nprfzlpne->pnesid));
		freePnes.Detach(find_pne_index(&freePnes, nprfzlpne->pnesid));
		freeLinks.Remove(find_nprfzlpne_index(&freeLinks, nprfzlpne->pnesid));
		return true;
	}

	return false;
}

bool TFzlNprPneListWindow::EditGroup(TGroup *group)
{
	NPRFZLPNE *nprfzlpne = (NPRFZLPNE *) Group();
	TGArray<PNE> pnes;

	pnes.Add(find_pne(npr->dlo->pne, nprfzlpne->pnesid));

	if (TEditNprFzlPneDialog(this, fzl, (NPRFZLPNE *) group, &pnes, -1, sbe, NULL).Execute() == IDOK)
	{
		PostDialog(nprfzlpne, true);
		return true;
	}

	return false;
}

bool TFzlNprPneListWindow::DeleteGroup(TGroup *group)
{
	NPRFZLPNE *nprfzlpne = npr->dlo->nprfzlpne->NewObject();

	*nprfzlpne << group;
	freePnes.Add(find_pne(npr->dlo->pne, nprfzlpne->pnesid));
	npr->dlo->pne->Detach(find_pne_index(npr->dlo->pne, nprfzlpne->pnesid));
	freeLinks.Add(nprfzlpne);
	return true;
}
