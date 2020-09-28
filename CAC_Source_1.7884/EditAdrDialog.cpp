DEFINE_RESPONSE_TABLE1(TEditAdrDialog, TEisDialog)
//{{TEditAdrDialogRSP_TBL_BEGIN}}
	EV_BN_CLICKED(IDOK, OKBNClicked),
	EV_CBN_SELCHANGE(IDC_EDIT_ADR_DRJ, DrjCBNSelChange),
	EV_CBN_SELCHANGE(IDC_EDIT_ADR_AREA, AreaCBNSelChange),
	EV_CBN_SELCHANGE(IDC_EDIT_ADR_REGION, RegionCBNSelChange),
//{{TEditAdrDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TEditAdrDialog Implementation}}

TEditAdrDialog::TEditAdrDialog(TWindow* parent, PNE *pne, const char *nmrnpr, int resId)
:
	TEisDialog(parent, pne, -1, nmrnpr, resId), adr(pne->adr)
{
	pnenmr = new TEisNmrFace(this, IDC_EDIT_ADR_PNENMR, "F_PNENMR", EISPP_PNE_KERNEL);
	tip = new TEisAliasFace(this, IDC_EDIT_ADR_TIP, "adrtip", nmk_adrtip, pne);
	drj = new TEisAliasFace(this, IDC_EDIT_ADR_DRJ, "adrdrj", nmk_grj, pne);
	area = new TEisAreaAliasFace(this, IDC_EDIT_ADR_AREA, "F_AREA", pne);
	region = new TEisRegionAliasFace(this, IDC_EDIT_ADR_REGION, "F_REGION", area, pne);
	nsmbgr = new TFindEisPlaceAliasFace(this, IDC_EDIT_ADR_NSMBGR, "adrnsmbgr", region, pne);
	nsmchj = new TStringFace(this, IDC_EDIT_ADR_NSMCHJ, "adrnsmchj", SIZE_OF_EISPP_STRING);
	rjn = new TEisAliasFace(this, IDC_EDIT_ADR_RJN, "adrrjn", nmk_raion, pne);
	pstkod = new TLongFace(this, IDC_EDIT_ADR_PSTKOD, "adrpstkod", SIZE_OF_PSTKOD);
	krdtxt = new TStringFace(this, IDC_EDIT_ADR_KRDTXT, "adrkrdtxt", SIZE_OF_EISPP_STRING);
	nmr = new TStringFace(this, IDC_EDIT_ADR_NMR, "adrnmr", SIZE_OF_EISPP_STRING);
	blk = new TStringFace(this, IDC_EDIT_ADR_BLK, "adrblk", SIZE_OF_EISPP_STRING);
	vhd = new TStringFace(this, IDC_EDIT_ADR_VHD, "adrvhd", SIZE_OF_EISPP_STRING);
	etj = new TStringFace(this, IDC_EDIT_ADR_ETJ, "adretj", SIZE_OF_EISPP_STRING);
	apr = new TStringFace(this, IDC_EDIT_ADR_APR, "adrapr", SIZE_OF_EISPP_STRING);
	mstvid = new TEisAliasFace(this, IDC_EDIT_ADR_MSTVID, "adrmstvid", nmk_adrmstvid, pne);
	msttxt = new TStringFace(this, IDC_EDIT_ADR_MSTTXT, "adrmsttxt", SIZE_OF_EISPP_STRING);
	kodpdl = new TLongFace(this, IDC_EDIT_ADR_KODPDL, "adrkodpdl", SIZE_OF_KODPDL);
	loc = new TEisAliasFace(this, IDC_EDIT_ADR_LOC, "adrloc", nmk_adrloc, pne);
}

bool TEditAdrDialog::IsValid()
{
	bool isValid = TEisDialog::IsValid();

	if (isValid)
	{
		if (drj->L())
			if (!(drj->L() == grj_BGR ? nsmbgr->L() : *nsmchj->S()))
				isValid = error("Населеното място е задължително.");
	}

	return isValid;
}

void TEditAdrDialog::SetupWindow()
{
	long place = adr->nsmbgr;

	TEisDialog::SetupWindow();

	if (!adr->drj)
	{
		msql m(EisPlace->Name);

		drj->SetL(grj_BGR);

		m.AddLong("F_UCLP", DEFAULT_UCLP);
		TQuery q(str(m));

		if (q.Read())
		{
			*EisPlace << q;
			place = EisPlace->code;
		}
	}

	if (place)
	{
		msql m(EisVal->Name);

		EisPlace->Seek(place);
		m.AddLong("F_CODE", EisPlace->nom);
		m.AddLong("NOT F_NOM", nmk_obstina);

		TQuery q(str(m));
		if (!q.Read())
			fatal("%s: unable to get data for %d", EisVal->Name, EisPlace->nom);

		*EisVal << q;
		area->SetL(EisVal->nom);
		region->Reset(EisPlace->nom);
		nsmbgr->Reset(EisPlace->code);
	}

	DrjCBNSelChange();
}

void TEditAdrDialog::OKBNClicked()
{
	if (drj->L())
	{
		if (drj->L() == grj_BGR)
			nsmchj->SetS("");
		else
		{
			rjn->SetL(0);
			pstkod->SetL(0);
			kodpdl->SetL(0);
			nsmbgr->SetL(0);
		}
	}

	TEisDialog::OKBNClicked();
}

void TEditAdrDialog::DrjCBNSelChange()
{
	bool any = drj->L();
	bool bgr = drj->L() == grj_BGR;

	area->EnableWindow(bgr);
	region->EnableWindow(bgr);
	EnableEis(nsmbgr, bgr);
	nsmbgr->ShowWindow(bgr);
	EnableEis(nsmchj, any && !bgr);
	nsmchj->ShowWindow(!bgr);
	EnableEis(rjn, bgr);
	EnableEis(pstkod, bgr);
	EnableEis(krdtxt, any);
	EnableEis(nmr, any);
	EnableEis(blk, any);
	EnableEis(vhd, any);
	EnableEis(etj, any);
	EnableEis(apr, any);
	EnableEis(mstvid, any);
	EnableEis(msttxt, any);
	EnableEis(kodpdl, bgr);
	EnableEis(loc, any);
}

void TEditAdrDialog::AreaCBNSelChange()
{
	region->Reset(0);
	RegionCBNSelChange();
}

void TEditAdrDialog::RegionCBNSelChange()
{
	nsmbgr->Reset(0);
}
