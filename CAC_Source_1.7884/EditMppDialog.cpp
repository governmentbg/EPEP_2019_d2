DEFINE_RESPONSE_TABLE1(TEditMppDialog, TFzlDialog)
//{{TEditMppDialogRSP_TBL_BEGIN}}
	EV_CBN_SELCHANGE(IDC_EDIT_MPP_VID, VidCBNSelChange),
	EV_CBN_SELCHANGE(IDC_EDIT_MPP_PRE_VID, PreVidCBNSelChange),
	EV_BN_CLICKED(IDOK, OKBNClicked),
//{{TEditMppDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TEditMppDialog Implementation}}

TEditMppDialog::TEditMppDialog(TWindow *parent, FZL *fzl, const char *nmrnpr, int resId)
:
	TFzlDialog(parent, fzl, (*fzl->mpp)[1], -1, nmrnpr, resId), mppre((*fzl->mpp)[0]), hasMppre(false)
{
	if (mppre->flags & XML_INCLUDED)
	{
		pre_vid = new TEisAliasFace(this, IDC_EDIT_MPP_PRE_VID, "mppvid", nmk_mppvid, mppre);
		pre_rzm = new TMoneyFace(this, IDC_EDIT_MPP_PRE_RZM, "mpprzm", SIZE_OF_SUM);
		pre_ste = new TEisAliasFace(this, IDC_EDIT_MPP_PRE_STE, "mppste", nmk_mppste, mppre);
		pre_dta = new TDateFace(this, IDC_EDIT_MPP_PRE_DTA, "mppdta", false);
		pre_str = new TEisAliasFace(this, IDC_EDIT_MPP_PRE_STR, "mppstr", EISPP_DEPART, mppre);
		hasMppre = true;
	}

	vid = new TEisAliasFace(this, IDC_EDIT_MPP_VID, "mppvid", nmk_mppvid, Mpp());
	rzm = new TMoneyFace(this, IDC_EDIT_MPP_RZM, "mpprzm", SIZE_OF_SUM);
	ste = new TEisAliasFace(this, IDC_EDIT_MPP_STE, "mppste", nmk_mppste, Mpp());
	dta = new TDateFace(this, IDC_EDIT_MPP_DTA, "mppdta", false);
	str = new TEisAliasFace(this, IDC_EDIT_MPP_STR, "mppstr", EISPP_COURT, Mpp());
}

bool TEditMppDialog::IsValid()
{
	bool isValid = TFzlDialog::IsValid();

	if (isValid)
	{
		if (vid->L() == mpp_garancia && rzm->M() == 0.0)
			isValid = error("Размерът е задължителен при парична гаранция.");
		else if (hasMppre)
		{
			if (pre_vid->L() == mpp_garancia && pre_rzm->M() == 0.0)
				isValid = error("Предишния размер е задължителен при предишна парична гаранция.");
		}
	}

	return isValid;
}

void TEditMppDialog::SetupWindow()
{
	TFzlDialog::SetupWindow();

	if (hasMppre)
	{
		pre_vid->SetL(mppre->vid);
		pre_rzm->SetM(mppre->rzm);
		pre_ste->SetL(sts_otmenena);
		pre_ste->EnableWindow(false);
		pre_dta->SetDate(mppre->dta);
		pre_dta->EnableWindow(true);
		pre_str->SetL(mppre->str);
		ste->SetL(sts_nalojena);
		ste->EnableWindow(false);
		PreVidCBNSelChange();
	}
	else
		ste->SetL(Mpp()->ste);

	vid->SetL(Mpp()->vid);
	rzm->SetM(Mpp()->rzm);
	dta->SetDate(Mpp()->dta);
	Court->Seek(COURT_CODE);
	str->SetL(Court->eispp);
	str->EnableWindow(false);

	VidCBNSelChange();
}

void TEditMppDialog::OKBNClicked()
{
	if (IsValid())
	{
		if (hasMppre)
		{
			mppre->vid = pre_vid->L();
			mppre->rzm = pre_rzm->M();
			mppre->ste = pre_ste->L();
			mppre->dta = pre_dta->Date();
			mppre->str = pre_str->L();
		}

		Mpp()->vid = vid->L();
		Mpp()->rzm = rzm->M();
		Mpp()->ste = ste->L();
		Mpp()->dta = dta->Date();
		Mpp()->str = str->L();

		CmOk();
	}
}

void TEditMppDialog::VidCBNSelChange()
{
	if (vid->L() == mpp_garancia)
		EnableEis(rzm);
	else
	{
		rzm->EnableWindow(false);
		rzm->SetM(0.0);
	}
}

void TEditMppDialog::PreVidCBNSelChange()
{
	if (pre_vid->L() == mpp_garancia)
		EnableEis(pre_rzm);
	else
	{
		pre_rzm->EnableWindow(false);
		pre_rzm->SetM(0.0);
	}
}
