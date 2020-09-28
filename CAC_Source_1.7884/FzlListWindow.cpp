//{{TFzlListWindow Implementation}}

TFzlListWindow::TFzlListWindow(TWindow* parent, int resourceId, SBE *tSbe, TRCDKeyContainer *tContainer)
:
	TXMLGroupListWindow(parent, resourceId, tSbe->npr->dlo->fzl, 228), npr(tSbe->npr), sbe(tSbe), container(tContainer)
{
	Add(new TGroupListColumn("Име", fzl_name, 216));
}

int TFzlListWindow::Count()
{
	int count = 0;

	for (int i = 0; i < array->Count(); i++)
		if (FzlAt(i)->sid >= npr->fzlmin)
			count++;

	return count;
}

void TFzlListWindow::Links()
{
	TNprFzlPneListDialog(this, Fzl(), sbe).Execute();
}

bool TFzlListWindow::IsValid()
{
	for (int i = 0; i < array->Count(); i++)
	{
		FZL *fzl = FzlAt(i);

		if (fzl->sid >= npr->fzlmin)
		{
			const char *invalid = fzl->Invalid();
			mstr m;

			fzl->Print(m);

			if (invalid)
				return error("%s: %s", str(m), invalid);

			if (!HasLinks(fzl))
				return error("Лицето %s няма връзки с престъпления.", str(m));
		}
	}

	return true;
}

bool TFzlListWindow::HasLinks(FZL *fzl)
{
	for (int i = 0; i < npr->dlo->nprfzlpne->Count(); i++)
		if ((*npr->dlo->nprfzlpne)[i]->fzlsid == fzl->sid)
			return true;

	return false;
}

TGroup *TFzlListWindow::NewGroup() const
{
	FZL *fzl = (FZL *) TXMLGroupListWindow::NewGroup();

	*fzl << npr;
	fzl->sid = npr->Generate();
	return fzl;
}

bool TFzlListWindow::AddGroup(TGroup *group)
{
	FZL *fzl = (FZL *) group;

	if (TEditFzlDialog(this, fzl, false, true, npr->nmr).Execute() == IDOK)
	{
		fzl->dirty = true;
		SetFocus();
		return true;
	}

	return false;
}

bool TFzlListWindow::EditGroup(TGroup *group)
{
	bool edit = sbe->vid == sbe_edit_fzlpne;

	if (TEditFzlDialog(this, (FZL *) group, true, !edit, npr->nmr).Execute() == IDOK)
	{
		if (edit)
		{
			SBE *zvk = new SBE(sbe_edit_fzl);
			SBE *sbe = new SBE(sbe_edit_fzl);
			FZL *fzl = zvk->npr->dlo->fzl->NewObject();

			*zvk->npr << npr;
			*fzl << Fzl();
			fzl->sid = 0;
			zvk->npr->dlo->fzl->Add(fzl);

			*sbe->npr << npr;
			fzl = sbe->npr->dlo->fzl->NewObject();
			*fzl << group;
			sbe->npr->dlo->fzl->Add(fzl);

			EISPAKET(sbe).Update(container, zvk, mnu_reg_korfzl);
		}

		return true;
	}

	return false;
}

bool TFzlListWindow::DeleteGroup(TGroup *group)
{
	if (HasLinks((FZL *) group))
		return error("Лицето има връзки с престъпления.");

	return TXMLGroupListWindow::DeleteGroup(group);
}
