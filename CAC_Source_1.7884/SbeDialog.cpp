DEFINE_RESPONSE_TABLE1(TSbeDialog, TEisDialog)
//{{TSbeDialogRSP_TBL_BEGIN}}
	EV_CBN_SELCHANGE(IDC_SBE_SELECT, SelectCBNSelChange),
	EV_BN_CLICKED(IDOK, OKBNClicked),
//{{TSbeDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TSbeDialog Implementation}}

TSbeDialog::TSbeDialog(TWindow* parent, SBE *sbe, TRCDKeyContainer *tOrigin, TGArray<SBE> *sbes, TXArray<FZL> *tFzls,
	TDatedContainer *tContainer, sign edit, int resId)
:
	TEisDialog(parent, sbe, edit, sbe->npr->nmr, resId), fzls(tFzls), origin(tOrigin), container(tContainer)
{
	sbevid = new TEisAliasFace(this, IDC_SBE_SBEVID, "sbevid", nmk_sbevid, sbe);
	sbedkpvid = new TEisAliasFace(this, IDC_SBE_DKPVID, "sbedkpvid", nmk_sbedkpvid, sbe);
	sbedkpnmr = new TStringFace(this, IDC_SBE_DKPNMR, "sbedkpnmr", SIZE_OF_EISPP_STRING);
	sbedkpdta = new TDateFace(this, IDC_SBE_DKPDTA, "sbedkpdta", false);
	sbedkpstr = new TEisAliasFace(this, IDC_SBE_DKPSTR, "sbedkpstr", EISPP_DEPART, sbe);
	sbhvid = new TEisAliasFace(this, IDC_SBE_SBHVID, "sbhvid", nmk_sbhvid, sbe);

	if (sbe->sbhnmk == nmk_grj)
		sbhstn = new TGrjChjAliasFace(this, IDC_SBE_SBHSTN, "sbhstn", &grj_excl_bg_unkn, sbe);
	else
		sbhstn = new TEisAliasFace(this, IDC_SBE_SBHSTN, "sbhstn", sbe->sbhnmk, sbe);

	if (sbe_destroy_srk.Contains(sbe->vid) && !spr_2nd_instance.Contains(sbe->npr->dlo->sprvid))
		sbe->srk->Force();

	srkvid = new TEisAliasFace(this, IDC_SBE_SRKVID, "srkvid", nmk_srkvid, sbe);
	srkden = new TLongFace(this, IDC_SBE_SRKDEN, "srkden", 3);
	srksdc = new TLongFace(this, IDC_SBE_SRKSDC, "srksdc", 3);
	srkmsc = new TLongFace(this, IDC_SBE_SRKMSC, "srkmsc", 3);
	srkgdn = new TLongFace(this, IDC_SBE_SRKGDN, "srkgdn", 3);
	srkdta = new TDateFace(this, IDC_SBE_SRKDTA, "srkdta", false);
	dlosig = new TEisAliasFace(this, IDC_SBE_DLOSIG, "dlosig", nmk_dlosig, sbe);
	dlonmr = new TStringFace(this, IDC_SBE_DLONMR, "dlonmr", SIZE_OF_EISPP_STRING);
	dlogdn = new TYearFace(this, IDC_SBE_DLOGDN, "dlogdn");
	dlostr = new TEisAliasFace(this, IDC_SBE_DLOSTR, "dlostr", EISPP_DEPART, sbe);
	dloncnone = new TEisAliasFace(this, IDC_SBE_DLONCNONE, "dloncnone", nmk_dloncnone, sbe);
	dlosprvid = new TEisAliasFace(this, IDC_SBE_DLOSPRVID, "dlosprvid", nmk_dlosprvid, sbe);
	dlosts = new TEisAliasFace(this, IDC_SBE_DLOSTS, "dlosts", nmk_dlosts, sbe);
	dlodtasts = new TDateFace(this, IDC_SBE_DLODTASTS, "dlodtasts", false);
	dvjvid = new TEisAliasFace(this, IDC_SBE_DVJVID, "dvjvid", nmk_dvjvid, sbe);
	dvjdta = new TDateFace(this, IDC_SBE_DVJDTA, "dvjdta", false);
	dvjprc = new TEisAliasFace(this, IDC_SBE_DVJPRC, "dvjprc", nmk_dvjprc, sbe);
	selectLabel = new TStatic(this, IDC_SBE_SELECT_LABEL);

	if (sbe->Includes("DLOOSN") || sbe->vid == sbe_plcdlo)
	{
		selectConnect = new TConnectComboBox(this, IDC_SBE_SELECT, sbe, origin);
		selectFzl = NULL;
	}
	else if (EisVal->Contains(nmk_sbeednfzl, sbe->vid))
	{
		selectConnect = NULL;
		selectFzl = new TFzlComboBox(this, IDC_SBE_SELECT, sbe, fzls, sbes, NULL);
	}
	else
	{
		new TEisAliasFace(this, IDC_SBE_SELECT, "EMPTY", 0, sbe);
		selectConnect = NULL;
		selectFzl = NULL;
	}

	sbedkpdtaLabel = new TStatic(this, IDC_SBE_DKPDTA_LABEL);
}

bool TSbeDialog::IsValid()
{
	bool isValid;

	if (Sbe()->vid == sbe_psd && sbedkpdta->Date().Empty())
	     isValid = error("Поле дата на влизане на присъдата в сила е празно.");
	else
	{
		isValid = TEisDialog::IsValid();

		if (isValid)
		{
			bool srkL = srkden->L() || srksdc->L() || srkmsc->L() || srkgdn->L();

			if (Sbe()->vid == sbe_psd && sbedkpdta->Date() > Today)
				isValid = error("Дата на влизане на присъдата в сила не може да бъде по-голяма от текущата дата.");
			// 2016:160 IRQ: den/sdc/msc/gdn are special for srk_objalvane
			else if (srkvid->L() == srk_objalvane)
			{
				if (srkL)
					Sbe()->srk->FindAttrib("srkden")->flags &= ~(XML_REQUIRED | XML_NULLABLE);
				else
					Sbe()->srk->FindAttrib("srkden")->flags |= XML_REQUIRED | XML_NULLABLE;
			}
			else if (srkvid->L() && !srk_vid_dta_only.Contains(srkvid->L()) && !srkL)
				isValid = error("Не е въведена продължителност на срока.");
		}
	}

	return isValid;
}

void TSbeDialog::SetupWindow()
{
	TEisDialog::SetupWindow();

	*Sbe()->npr >> this;

	if (selectConnect)
		selectLabel->SetCaption(selectConnect->Label());
	else if (selectFzl)
		selectLabel->SetCaption("Избор на лице");

	SelectCBNSelChange();

	if (container && PRINT_JUDGEMENTS)
	{
		TLong *eventNo = (TLong *) container->Find("F_EVENT_NO");

		if (eventNo && eventNo->L())
		{
			char s[SIZE_OF_EVENT_NO];
			itoa(eventNo->L(), s, 10);
			sbedkpnmr->SetS(s);
		}

		sbedkpnmr->EnableWindow(false);
	}

	// 2013:035 checked in eispp
	//if (edit)
	//	dlosprvid->EnableWindow(false);

	long vid = Sbe()->vid;

	if (vid == sbe_psd)
	{
		sbedkpdtaLabel->SetCaption("Дата на влизане на присъдата в сила");
		sbedkpdta->SetS("");
	}
	else if (vid == sbe_pklrbtdlo)
	{
		TDate *finished = static_cast<TDate *> (origin->Find("F_FINISHED"));

		if (finished)
			dlodtasts->SetDate(finished->Date());
	}
	// 2015:071
	//if (!Sbe()->sbh->stn && sbhstn->IsWindowEnabled() && (vid == sbe_pispsd || vid == sbe_psd || vid == sbe_pkenprsze))
	//	sbhstn->SetL(psd_osaditelna);
	// 2015:138 TRQ: for all events
	if (!Sbe()->sbh->stn && sbhstn->IsWindowEnabled() && Sbe()->sbh->vid == sbx_tip_prisada)
		sbhstn->SetL(psd_osaditelna);
}

void TSbeDialog::SelectCBNSelChange()
{
	if (Sbe()->vid == sbe_plcdlo)
	{
		TConnect *connect = selectConnect->Connect();

		if (connect)
		{
			//sbedkpstr->SetL(selectConnect->Eispp());	// 2013:045 FIX: desc -1
			Sbe()->dvjdlo->stripr = selectConnect->Eispp();
			strcpy(Sbe()->dvjdlo->dvn, connect->connectNo);
			dlogdn->SetL(connect->connectYear);
			dlonmr->SetS(connect->connectNo);
			dlostr->SetL(selectConnect->Eispp());	// 2013:029
			XferKind->Seek(connect->xferKind);
			dlosig->SetL(XferKind->eispp);
			ConnectType->Seek(connect->connectType);
			Sbe()->npr->dlo->vid = ConnectType->eispp;
		}
	}
	else if (sbe_dlosnsbhstn.Contains(Sbe()->vid))
		sbhstn->SetL(selectConnect->Eispp());
}

void TSbeDialog::OKBNClicked()
{
	*Sbe()->npr << this;

	if (IsValid())
	{
		// 2012:339 LRQ: 0 if needed
		if (Sbe()->Includes("sbedkpnmr") && !any(Sbe()->dkpnmr))
			strcpy(Sbe()->dkpnmr, "0");

		if (Sbe()->Includes("DLOOSN"))
		{
			TConnect *connect = selectConnect->Connect();
			DLOOSN *dloosn = Sbe()->npr->dlo->dloosn->NewObject();
			const char *errorMsg = dloosn->LoadConnect(connect);
			msql m;

			if (!errorMsg)
			{
				msql m;

				if (dloosn->IsValid(m))
				{
					Sbe()->npr->dlo->dloosn->Flush();
					Sbe()->npr->dlo->dloosn->Add(dloosn);
				}
				else
					errorMsg = str(m);
			}

			if (errorMsg)
			{
				delete dloosn;
				error("Дело по което се образува: %s.", errorMsg);
				return;
			}
		}
		else if (selectFzl && fzls != Sbe()->npr->dlo->fzl)
		{
			FZL *fzl = Sbe()->npr->dlo->fzl->NewObject();

			*fzl << selectFzl->Fzl();
			Sbe()->npr->dlo->fzl->Flush();
			Sbe()->npr->dlo->fzl->Add(fzl);
		}

		Sbe()->dirty = true;

		if (!save || Sbe()->npr->Save(edit))
			TGroupDialog::OKBNClicked();
	}
}
