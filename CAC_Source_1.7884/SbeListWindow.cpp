//{{TSbeListWindow Implementation}}

TSbeListWindow::TSbeListWindow(TWindow* parent, int resourceId, NPR *tNPR, TLawsuit *tLawsuit, TDatedContainer *tContainer,
	TIntegerArray *tLset, TGArray<SBE> *sbes, TSbeAliasFace *tAlias)
:
	TGroupListWindow(parent, resourceId, sbes, 264), npr(tNPR), lawsuit(tLawsuit), container(tContainer), lset(tLset),
		alias(tAlias)
{
	loadData = dynamic = false;
	Add(new TGroupListColumn("Събитие", group_print, 252));
}

bool TSbeListWindow::PostDialog(SBE *sbe, bool edit)
{
	if (EisVal->Contains(nmk_sbeednfzl, sbe->vid))
	{
		FZL *fzl = (*sbe->npr->dlo->fzl)[0];

		if (fzl->Includes("MPP"))
		{
			MPP *mppre;

			if (fzl->mpp->Count() == 2)
			{
				mppre = (*fzl->mpp)[0];
				mppre->flags |= XML_INCLUDED;
			}
			else
			{
				if (fzl->mpp->Count() < 1)	// shorter to add here instead of AddGroup()
				{
					MPP *mpp = fzl->mpp->NewObject();
					mpp->dta = sbe->dkpdta;
					fzl->mpp->Add(mpp);
				}

				mppre = sbe->vid == sbe_izmmpp ? fzl->mpp->NewObject() : new MPP;
				fzl->mpp->Insert(mppre, 0);
			}

			// 2014:143 PRQ: 6115 p.17.1
			mppre->FindAttrib("mppdta")->flags &= ~XML_REQUIRED;
			mppre->FindAttrib("mppstr")->flags &= ~XML_REQUIRED;

			return TEditMppDialog(this, fzl, npr->nmr).Execute() == IDOK;
		}

		// 2012:332 LRQ/LPR; may be unneeded
		if (sbe_zamiana_nkz.Contains(sbe->vid))
		{
			if (fzl->nkz->Count() < 1)
				fzl->nkz->Add(fzl->nkz->NewObject());

			return TEditNkzDialog(this, fzl, (*fzl->nkz)[0], sbe).Execute() == IDOK;
		}

		bool hasNkzs = false;
		bool hasNprFzlPnes = sbe->npr->dlo->Includes("NPRFZLPNE");

		if (fzl->Includes("NKZ"))
		{
			if (sbe->sbh->stn == psd_osaditelna || (fzl->nkz->flags & XML_REQUIRED))
				hasNkzs = true;
			else
				fzl->nkz->Clear();
		}

		if (hasNkzs || hasNprFzlPnes)
		{
			TFzlNkzPneListDialog(this, fzl, sbe, hasNkzs, hasNprFzlPnes, edit).Execute();
			return fzl->nkz->Count() > 0 || sbe->npr->dlo->nprfzlpne->Count() > 0;
		}
	}
	else if (sbe->vid == sbe_vklnzv)
	{
		TXArray<FZL> *fzl = sbe->npr->dlo->fzl;

		TFzlPneListDialog(this, sbe, container, edit, PNE_NMR_BLOCKED).Execute();

		for (int i = 0; i < fzl->Count(); i++)
			if ((*fzl)[i]->sid >= sbe->npr->fzlmin)
				return true;

		return false;
	}
	else if (sbe->vid == sbe_ovadlo)
	{
		TFzlPneListDialog(this, sbe, container, edit, PNE_NMR_BLOCKED).Execute();
		return true;
	}
	else if (sbe->Includes("FZL"))
	{
		TSbeFzlListDialog(this, sbe, edit).Execute();
		return sbe->npr->dlo->fzl->Count() > 0;
	}

	return true;
}

bool TSbeListWindow::OvaDlo(SBE *sbe, SBE *sbe1)
{
	TLawsuit lawsuit;
	msql m;

	strcpy(PrimaryTitle, "Дело за присъединяване");

	if (_primary_group)
		_primary_group->key.no = 0;

	if (!get_group(this, &lawsuit, C2S[TYPE_PUNISHMENT_LAW], NULL, has_eispp_nmr))
		return false;

	sbe1->npr->key = lawsuit.key;

	if (!sbe1->npr->Get())
		return false;

	sbe1->ApplyDesc(sbe_pklova);
	sbe1->LoadFzlPneNpr();

	if (!sbe1->npr->dlo->IsValid(m))
		return error("%s", str(m));

	if (lawsuit.key == sbe->npr->key || !strcmp(lawsuit.eispp_nmr, sbe->npr->nmr))
		return error("Присъединяване на дело към същото дело или НП.");

	DLO *dlo = sbe->npr->dlo;
	DLO *dlo1 = sbe1->npr->dlo;

	if (!ask("Желаете ли да присъедините %s %ld/%ld към %s %ld/%ldг?", Kind->Seek(lawsuit.kind), lawsuit.key.no,
		lawsuit.key.year, EisVal->Seek(nmk_dlosig, dlo->sig), sbe->npr->key.no, sbe->npr->key.year))
	{
		return false;
	}

	for (int i = 0; i < dlo1->fzl->Count(); i++)
	{
		FZL *fzl = dlo->fzl->NewObject();

		*fzl << (*dlo1->fzl)[i];
		fzl->sid += sbe->npr->fzlmin;
		fzl->key = sbe->npr->key;

		if (fzl->Insert())
		{
			dlo->fzl->ApplyDescs(fzl);
			dlo->fzl->Add(fzl);
		}
	}

	for (int i = 0; i < dlo1->pne->Count(); i++)
	{
		PNE *pne = dlo->pne->NewObject();

		*pne << (*dlo1->pne)[i];
		pne->sid += sbe->npr->fzlmin;
		pne->key = sbe->npr->key;

		if (pne->Insert())
		{
			dlo->pne->ApplyDescs(pne);
			dlo->pne->Add(pne);
		}
	}

	for (int i = 0; i < dlo1->nprfzlpne->Count(); i++)
	{
		NPRFZLPNE *nprfzlpne1 = (*dlo1->nprfzlpne)[i];
		NPRFZLPNE *nprfzlpne = dlo->nprfzlpne->NewObject();

		*nprfzlpne << nprfzlpne1;
		nprfzlpne->fzlsid += sbe->npr->fzlmin;
		nprfzlpne->pnesid += sbe->npr->fzlmin;
		nprfzlpne->key = sbe->npr->key;

		if (nprfzlpne->Insert())
		{
			nprfzlpne->scq->vid = scq_svarzva_s_np;
			dlo->nprfzlpne->Add(nprfzlpne);
			nprfzlpne1->scq->vid = scq_obedinjava_drugo_np;
			nprfzlpne1->scq->rlq = rlq_obedinen_kam_drugo_np;
		}
	}

	return true;
}

bool TSbeListWindow::AddGroup(TGroup *group)
{
	SBE *sbe = (SBE *) group;

	*sbe->npr << npr;
	sbe->ApplyDesc(alias->L());

	if (strchr(KIND_SESSIONS_ENDOCS, container->kind))
	{
		TIndexContainer *tContainer = (TIndexContainer *) container;

		Kind->Seek(tContainer->IndexKind());
		sbe->dkpvid = Kind->eispp;
		sbe->dkpdta = strchr(KIND_DECISIONS, container->kind) ? ((TDecision *) container)->fromDate : container->date;

		if (sbe->Includes("srkdta"))
			sbe->srk->dta = sbe->dkpdta;	// fixed | default
	}

	if (sbe_loadfzlpne.Find(sbe->vid) != INT_MAX)
	{
		sbe->LoadFzlPneNpr();
		sbe->npr->fzlmin = npr->Generate();
	}

	if (TSbeDialog(this, sbe, lawsuit, (TGArray<SBE> *) array, npr->dlo->fzl, container).Execute() == IDOK)
	{
		if (sbe->vid == sbe_ovadlo)
		{
			SBE *sbe1 = sbe->sbe->NewObject();

			if (!OvaDlo(sbe, sbe1))
			{
				delete sbe1;
				return false;
			}

			strcpy(sbe1->elementType, "sbereg");
			sbe1->npr->sid = 0;
			sbe1->dkpdta = sbe->dkpdta;
			sbe->sbe->Add(sbe1);
		}

		return PostDialog(sbe, false);
	}

	return false;
}

bool TSbeListWindow::EditGroup(TGroup *group)
{
	SBE *sbe = (SBE *) group;

	if (TSbeDialog(this, sbe, lawsuit, NULL, sbe->npr->dlo->fzl, container).Execute() == IDOK)
	{
		PostDialog(sbe, true);
		return true;
	}

	return false;
}

bool TSbeListWindow::DeleteGroup(TGroup *)
{
	return true;
}

TGroup *TSbeListWindow::NewGroup() const
{
	return new SBE;
}
