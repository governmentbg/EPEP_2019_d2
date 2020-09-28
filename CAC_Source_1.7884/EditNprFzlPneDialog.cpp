DEFINE_RESPONSE_TABLE1(TEditNprFzlPneDialog, TFzlDialog)
//{{TEditNprFzlPneDialogRSP_TBL_BEGIN}}
	EV_CBN_SELCHANGE(IDC_EDIT_NPRFZLPNE_PNESID, PneSidCBNSelChange),
	EV_BN_CLICKED(IDOK, OKBNClicked),
//{{TEditNprFzlPneDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TEditNprFzlPneDialog Implementation}}

TEditNprFzlPneDialog::TEditNprFzlPneDialog(TWindow* parent, FZL *fzl, NPRFZLPNE *nprfzlpne, TGArray<PNE> *pnes, sign edit,
	 SBE *tSbe, TXArray<NPRFZLPNE> *tFreeLinks, int resId)
:
	TFzlDialog(parent, fzl, nprfzlpne, edit, tSbe->npr->nmr, resId), npr(tSbe->npr), freeLinks(tFreeLinks), sbe(tSbe)
{
	pnesid = new TPneAliasFace(this, IDC_EDIT_NPRFZLPNE_PNESID, pnes);
	scqvid = new TEisAliasFace(this, IDC_EDIT_NPRFZLPNE_SCQVID, "scqvid", nmk_scqvid, nprfzlpne);
	scqstr = new TEisAliasFace(this, IDC_EDIT_NPRFZLPNE_SCQSTR, "scqstr", EISPP_DEPART, nprfzlpne);
	scqrlq = new TEisAliasFace(this, IDC_EDIT_NPRFZLPNE_SCQRLQ, "scqrlq", nmk_fzlpnerlq, nprfzlpne);
	scqosn = new TEisAliasFace(this, IDC_EDIT_NPRFZLPNE_SCQOSN, "scqosn", npr->scqnmk, nprfzlpne);
	sbcple = new TEisAliasFace(this, IDC_EDIT_NPRFZLPNE_SBCPLE, "sbcple", nmk_sbcple, nprfzlpne);
	sbcrcd = new TEisAliasFace(this, IDC_EDIT_NPRFZLPNE_SBCRCD, "sbcrcd", nmk_sbcrcd, nprfzlpne);
	sbcspj = new TEisAliasFace(this, IDC_EDIT_NPRFZLPNE_SBCSPJ, "sbcspj", nmk_sbcspj, nprfzlpne);
	sbcobr = new TEisAliasFace(this, IDC_EDIT_NPRFZLPNE_SBCOBR, "sbcobr", nmk_sbcobr, nprfzlpne);
	sbctrd = new TEisAliasFace(this, IDC_EDIT_NPRFZLPNE_SBCTRD, "sbctrd", nmk_sbctrd, nprfzlpne);
	sbcznq = new TEisAliasFace(this, IDC_EDIT_NPRFZLPNE_SBCZNQ, "sbcznq", nmk_sbcznq, nprfzlpne);
	sbcetn = new TEisAliasFace(this, IDC_EDIT_NPRFZLPNE_SBCETN, "sbcetn", nmk_sbcetn, nprfzlpne);
	sbcrge = new TEisAliasFace(this, IDC_EDIT_NPRFZLPNE_SBCRGE, "sbcrge", nmk_sbcrge, nprfzlpne);
	prevNprFzlPne << nprfzlpne;

	if (!nprfzlpne->sbc->ple)
		nprfzlpne->sbc->ple = psl_pln_palnoleten;
}

void TEditNprFzlPneDialog::SetupWindow()
{
	TFzlDialog::SetupWindow();

	pnesid->SetL(NprFzlPne()->pnesid);

	if (sbe->sbh->stn == psd_opravdatelna)
	{
		scqvid->SetL(scq_opravdava);
		scqvid->EnableWindow(false);
	}
}

void TEditNprFzlPneDialog::PneSidCBNSelChange()
{
	if (freeLinks)
	{
		NPRFZLPNE *nprfzlpne = find_nprfzlpne(freeLinks, pnesid->L());

		if (nprfzlpne)
			*nprfzlpne >> this;
	}
}

static bool sbc_diff(TEisAliasFace *alias, long value)
{
	return alias->IsWindowEnabled() && alias->L() != value;
}

void TEditNprFzlPneDialog::OKBNClicked()
{
	SBC *sbc = prevNprFzlPne.sbc;

	if (sbc_diff(sbcple, sbc->ple) || sbc_diff(sbcrcd, sbc->rcd) || sbc_diff(sbcspj, sbc->spj) ||
		sbc_diff(sbcobr, sbc->obr) || sbc_diff(sbctrd, sbc->trd) || sbc_diff(sbcznq, sbc->znq) ||
		sbc_diff(sbcetn, sbc->etn) || sbc_diff(sbcrge, sbc->rge))
	{
		save = true;
	}

	NprFzlPne()->pnesid = pnesid->L();
	TFzlDialog::OKBNClicked();
}
