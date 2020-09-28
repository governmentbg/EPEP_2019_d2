//{{TNprFzlPneListWindow Implementation}}

TNprFzlPneListWindow::TNprFzlPneListWindow(TWindow* parent, int resourceId, FZL *tFZL, SBE *tSbe, TNprFzlPneComboBox *tCombo)
:
	TXMLGroupListWindow(parent, resourceId, NULL, 228), fzl(tFZL), npr(tSbe->npr), allLinks(tSbe->npr->dlo->nprfzlpne),
		combo(tCombo), sbe(tSbe)
{
	Add(new TGroupListColumn("Квалификация", group_print, 216));
}

TNprFzlPneListWindow::~TNprFzlPneListWindow()
{
	array->Clear();
}

void TNprFzlPneListWindow::GetGroupText(mstr &m, TGroup *group, int column)
{
	if (column == 1)
		print_nprfzlpne(m, group, npr);
}

TGroup *TNprFzlPneListWindow::NewGroup() const
{
	NPRFZLPNE *nprfzlpne = allLinks->NewObject();
	*nprfzlpne << fzl;
	return nprfzlpne;
}

void TNprFzlPneListWindow::LoadData(const char *)
{
	for (int i = 0; i < allLinks->Count(); i++)
	{
		NPRFZLPNE *nprfzlpne = (*allLinks)[i];

		if (nprfzlpne->fzlsid == fzl->sid)
			InsertGroup(nprfzlpne);
	}
}

bool TNprFzlPneListWindow::AddGroup(TGroup *group)
{
	NPRFZLPNE *nprfzlpne = (NPRFZLPNE *) group;
	TGArray<PNE> pnes;

	if (combo)
	{
		*nprfzlpne << combo->Group();
		allLinks->ApplyDescs(nprfzlpne);	// fixup << values
		pnes.Add(find_pne(npr->dlo->pne, nprfzlpne->pnesid));
	}
	else
	{
		static constant xfer[] = { "sbcspj", "sbcobr", "sbctrd", "sbcznq", "sbcetn", "sbcrge", NULL };

		for (int i = 0; i < array->Count(); i++)
		{
			TGroup *tGroup = (*array)[i];

			for (constant *name = xfer; *name; name++)
			{
				long l = ((TEisAlias *) tGroup->Find(*name))->L();

				if (l)
					((TEisAlias *) group->Find(*name))->SetL(l);
			}
		}

		for (int i = 0; i < npr->dlo->pne->Count(); i++)
		{
			PNE *pne = (*npr->dlo->pne)[i];
			int n;

			for (n = 0; n < allLinks->Count(); n++)
			{
				NPRFZLPNE *tNprFzlPne = (*allLinks)[n];

				if (tNprFzlPne->fzlsid == fzl->sid && tNprFzlPne->pnesid == pne->sid)
					break;
			}

			if (n == allLinks->Count())
				pnes.Add(pne);
		}
	}

	if (TEditNprFzlPneDialog(this, fzl, nprfzlpne, &pnes, combo ? -1 : false, sbe, NULL).Execute() == IDOK)
	{
		allLinks->Add(nprfzlpne);
		return true;
	}

	return false;
}

bool TNprFzlPneListWindow::EditGroup(TGroup *group)
{
	NPRFZLPNE *nprfzlpne = (NPRFZLPNE *) Group();
	TGArray<PNE> pnes;

	pnes.Add(find_pne(npr->dlo->pne, nprfzlpne->pnesid));
	return TEditNprFzlPneDialog(this, fzl, (NPRFZLPNE *) group, &pnes, combo ? -1 : true, sbe, NULL).Execute() == IDOK;
}

bool TNprFzlPneListWindow::DeleteGroup(TGroup *group)
{
	int index = ((TIntegerArray *) allLinks)->Find((long) group);

	if (TXMLGroupListWindow::DeleteGroup(group))
	{
		allLinks->Detach(index);
		return true;
	}

	return false;
}
