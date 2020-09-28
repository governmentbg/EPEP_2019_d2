DEFINE_RESPONSE_TABLE1(TEditFzlDialog, TEisDialog)
//{{TEditFzlDialogRSP_TBL_BEGIN}}
	EV_EN_KILLFOCUS(IDC_EDIT_FZL_EGN, EgnKillFocus),
	EV_EN_SETFOCUS(IDC_EDIT_FZL_IMECYR, ImeCyrSetFocus),
	EV_EN_SETFOCUS(IDC_EDIT_FZL_IMELAT, ImeLatSetFocus),
	EV_BN_CLICKED(IDOK, OKBNClicked),
//{{TEditFzlDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TEditFzlDialog Implementation}}

TEditFzlDialog::TEditFzlDialog(TWindow *parent, FZL *fzl, bool edit, bool tFull, const char *nmrnpr, int resId)
:
	TEisDialog(parent, fzl, edit, nmrnpr, resId), full(tFull)
{
	egn = new TUCNFace(this, IDC_EDIT_FZL_EGN, "fzlegn", UCN_CITIZEN_UCN);
	lnc = new TLNCFace(this, IDC_EDIT_FZL_LNC, "fzllnc");
	pol = new TEisAliasFace(this, IDC_EDIT_FZL_POL, "fzlpol", nmk_fzlpol, fzl);
	dtarjd = new TDateFace(this, IDC_EDIT_FZL_DTARJD, "F_DTARJD", -1);
	dtapnl = new TDateFace(this, IDC_EDIT_FZL_DTAPNL, "F_DTAPNL", -1);
	ime = new TFzlNameFace(this, IDC_EDIT_FZL_IME, "fzlime", true, "Невалидно име");
	prz = new TFzlNameFace(this, IDC_EDIT_FZL_PRZ, "fzlprz", true, "Невалидно презиме");
	fma = new TFzlNameFace(this, IDC_EDIT_FZL_FMA, "fzlfma", true, "Невалидна фамилия");
	imecyr = new TFzlNameFace(this, IDC_EDIT_FZL_IMECYR, "fzlimecyr", -1, "Невалидни имена на кирилица");
	imelat = new TFzlNameFace(this, IDC_EDIT_FZL_IMELAT, "fzlimelat", false, "Невалидни имена на латиница");
	grjbgr = new TEisAliasFace(this, IDC_EDIT_FZL_GRJBGR, "fzlgrjbgr", nmk_grj, fzl);
	grjchj = new TGrjChjAliasFace(this, IDC_EDIT_FZL_GRJCHJ, "fzlgrjchj", &grj_excl_bulgaria, fzl);
}

bool TEditFzlDialog::IsValid()
{
	bool isValid = TEisDialog::IsValid();

	if (isValid)
	{
		*Group() << this;
		const char *invalid = ((FZL *) Group())->Invalid();

		if (invalid)
			isValid = error("%s", invalid);
	}

	return isValid;
}

void TEditFzlDialog::SetupWindow()
{
	TEisDialog::SetupWindow();

	EgnKillFocus();

	if (!edit)
		grjbgr->SetL(grj_BGR);

	if (!full)
	{
		egn->EnableWindow(false);

		if (any(Fzl()->lnc))
			lnc->EnableWindow(false);
	}
}

void TEditFzlDialog::EgnKillFocus()
{
	const char *ucn = egn->S();

	if (ucn_valid(UCN_CITIZEN_UCN, ucn))
	{
		if (*ucn)
		{
			dtarjd->SetDate(CDate(ucn));
			dtarjd->EnableWindow(false);
			pol->SetL(ucn[8] % 2 ? pol_jena : pol_maj);
			pol->EnableWindow(false);
		}
		else
		{
			EnableEis(dtarjd);
			EnableEis(pol);
		}
	}
}

void TEditFzlDialog::ImeCyrSetFocus()
{
	if (!*imecyr->S())
	{
		mstr m;

		*Fzl() << this;
		Fzl()->Print(m);
		imecyr->SetS(str(m));
	}
}

void TEditFzlDialog::ImeLatSetFocus()
{
	if (!*imelat->S())
	{
		char s[SIZE_OF_EISPP_STRING];

		ImeCyrSetFocus();
		transliterate(imecyr->S(), s, LENGTH_OF_EISPP_STRING);
		imelat->SetS(s);
	}
}

void TEditFzlDialog::OKBNClicked()
{
	EgnKillFocus();
	ImeCyrSetFocus();
	TEisDialog::OKBNClicked();
}
