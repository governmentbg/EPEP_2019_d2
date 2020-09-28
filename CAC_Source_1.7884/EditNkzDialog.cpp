DEFINE_RESPONSE_TABLE1(TEditNkzDialog, TEditDsmgRzmDialog)
//{{TEditNkzDialogRSP_TBL_BEGIN}}
	EV_CBN_SELCHANGE(IDC_EDIT_NKZ_VID, VidCBNSelChange),
	EV_CBN_SELCHANGE(IDC_EDIT_NKZ_NCN, NcnCBNSelChange),
//{{TEditNkzDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TEditNkzDialog Implementation}}

TEditNkzDialog::TEditNkzDialog(TWindow *parent, FZL *fzl, NKZ *nkz, SBE *tSbe, int resId)
:
	TEditDsmgRzmDialog(parent, fzl, nkz, tSbe->npr->nmr, resId), sbe(tSbe)
{
	tip = new TEisAliasFace(this, IDC_EDIT_NKZ_TIP, "nkztip", nmk_nkztip, nkz);
	vid = new TEisAliasFace(this, IDC_EDIT_NKZ_VID, "nkzvid", nmk_nkzvid, nkz);
	rzm = new TMoneyFace(this, IDC_EDIT_NKZ_RZM, "nkzrzm", SIZE_OF_SUM);
	den = new TLongFace(this, IDC_EDIT_NKZ_DEN, "nkzden", SIZE_OF_EISPP_RZM);
	sdc = new TLongFace(this, IDC_EDIT_NKZ_SDC, "nkzsdc", SIZE_OF_EISPP_RZM);
	msc = new TLongFace(this, IDC_EDIT_NKZ_MSC, "nkzmsc", SIZE_OF_EISPP_RZM);
	gdn = new TLongFace(this, IDC_EDIT_NKZ_GDN, "nkzgdn", SIZE_OF_EISPP_RZM);
	ncn = new TEisAliasFace(this, IDC_EDIT_NKZ_NCN, "nkzncn", nmk_nkzncn, nkz);
	iptsrkden = new TLongFace(this, IDC_EDIT_NKZ_IPTSRKDEN, "iptsrkden", SIZE_OF_EISPP_RZM);
	iptsrksdc = new TLongFace(this, IDC_EDIT_NKZ_IPTSRKSDC, "iptsrksdc", SIZE_OF_EISPP_RZM);
	iptsrkmsc = new TLongFace(this, IDC_EDIT_NKZ_IPTSRKMSC, "iptsrkmsc", SIZE_OF_EISPP_RZM);
	iptsrkgdn = new TLongFace(this, IDC_EDIT_NKZ_IPTSRKGDN, "iptsrkgdn", SIZE_OF_EISPP_RZM);
	iptsrkdta = new TDateFace(this, IDC_EDIT_NKZ_IPTSRKDTA, "iptsrkdta", false);
	rjm = new TEisAliasFace(this, IDC_EDIT_NKZ_RJM, "nkzrjm", nmk_nkzrjm, nkz);
	akt = new TEisAliasFace(this, IDC_EDIT_NKZ_AKT, "nkzakt", nmk_nkzakt, nkz);
	aktdta = new TDateFace(this, IDC_EDIT_NKZ_AKTDTA, "nkzaktdta", false);
}

void TEditNkzDialog::AdjustDate(TDateFace *dta)
{
	if ((sbe->vid == sbe_pispsd || sbe->vid == sbe_pkenprsze) && dta->IsWindowEnabled() && dta->Date().Empty())
		dta->SetDate(sbe->dkpdta);
}

bool TEditNkzDialog::IsValid()
{
	bool isValid = TEditDsmgRzmDialog::IsValid();

	if (isValid)
	{
		if (ncn->L() == izt_osaden_efektivno && EisVal->Contains(nmk_nkzvid_rjm, vid->L()) && !rjm->L())
			isValid = error("Режим на изтърпяване на наказанието е задължителен при ефективно лишаване от свобода.");
		else if (EisVal->Contains(nmk_nkzncn_iptsrk, ncn->L()))
		{
			if (!iptsrkden->L() && !iptsrksdc->L() && !iptsrkmsc->L() && !iptsrkgdn->L())
				isValid = error("Размер на изпитателен срок е задължителен при условно изтърпяване на наказанието.");
			else if (iptsrkdta->Date().Empty())
			{
				isValid = error("Дата, от която започва да тече изпитателния срок, е задължителна при "
					"условно изтърпяване на наказанието.");
			}
		}
	}

	return isValid;
}

void TEditNkzDialog::SetupWindow()
{
	TEditDsmgRzmDialog::SetupWindow();

	AdjustDate(aktdta);
	NcnCBNSelChange();
}

void TEditNkzDialog::NcnCBNSelChange()
{
	if (EisVal->Contains(nmk_nkzncn_iptsrk, ncn->L()))
	{
		EnableEis(iptsrkden);
		EnableEis(iptsrksdc);
		EnableEis(iptsrkmsc);
		EnableEis(iptsrkgdn);
		EnableEis(iptsrkdta);
	}
	else
	{
		iptsrkden->EnableWindow(false);
		iptsrkden->SetL(0);
		iptsrksdc->EnableWindow(false);
		iptsrksdc->SetL(0);
		iptsrkmsc->EnableWindow(false);
		iptsrkmsc->SetL(0);
		iptsrkgdn->EnableWindow(false);
		iptsrkgdn->SetL(0);
		iptsrkdta->EnableWindow(false);
		iptsrkdta->SetS("");
	}

	AdjustDate(iptsrkdta);
}
