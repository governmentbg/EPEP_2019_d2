//{{TPneListWindow Implementation}}

TPneListWindow::TPneListWindow(TWindow* parent, int resourceId, SBE *sbe, int tPneMode, TRCDKeyContainer *tContainer)
:
	TXMLGroupListWindow(parent, resourceId, sbe->npr->dlo->pne, 228), npr(sbe->npr), vid(sbe->vid), container(tContainer),
		pneMode(tPneMode)
{
	Add(new TGroupListColumn("Квалификация", group_print, 216));
}

bool TPneListWindow::IsValid()
{
	for (int i = 0; i < array->Count(); i++)
	{
		PNE *pne = (PNE *) (*array)[i];

		if (!HasLinks(pne))
		{
			mstr m;
			pne->Print(m);
			return error("Престъплението %s няма връзки с лица.", str(m));
		}
	}

	return true;
}

bool TPneListWindow::HasLinks(PNE *pne)
{
	for (int i = 0; i < npr->dlo->nprfzlpne->Count(); i++)
		if ((*npr->dlo->nprfzlpne)[i]->pnesid == pne->sid)
			return true;

	return false;
}

TGroup *TPneListWindow::NewGroup() const
{
	PNE *pne = (PNE *) TXMLGroupListWindow::NewGroup();

	*pne << npr;
	pne->sid = npr->Generate();
	return pne;
}

bool TPneListWindow::AddGroup(TGroup *group)
{
	PNE *pne = (PNE *) group;

	if (TEditPneDialog(this, pne, false, pneMode, npr->nmr).Execute() == IDOK)
	{
		pne->dirty = true;
		SetFocus();
		return true;
	}

	return false;
}

bool TPneListWindow::EditGroup(TGroup *group)
{
	bool edit = vid == sbe_edit_fzlpne;

	if (TEditPneDialog(this, (PNE *) group, true, pneMode, npr->nmr).Execute() == IDOK)
	{
		if (edit)
		{
			SBE *zvk = new SBE(sbe_edit_pne_zvk);
			SBE *sbe = new SBE(sbe_edit_pne_reg);
			PNE_ZVK *pne_zvk = new PNE_ZVK;
			PNE *pne;

			*zvk->npr << npr;
			*pne_zvk << Pne();
			pne_zvk->sid = 0;
			zvk->npr->dlo->pne->Add((PNE *) pne_zvk);

			*sbe->npr << npr;
			pne = sbe->npr->dlo->pne->NewObject();
			*pne << group;
			sbe->npr->dlo->pne->Add(pne);

			EISPAKET(sbe).Update(container, zvk, mnu_reg_korpne);
		}

		return true;
	}

	return false;
}

bool TPneListWindow::DeleteGroup(TGroup *group)
{
	if (HasLinks((PNE *) group))
		return error("Престъплението има връзки с лица.");

	return TXMLGroupListWindow::DeleteGroup(group);
}
