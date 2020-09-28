//{{TFzlGroupListDialog Implementation}}

TFzlGroupListDialog::TFzlGroupListDialog(TWindow* parent, FZL *tFZL, NPR *tNPR, int resId)
:
	TGroupListDialog(parent, resId), fzl(tFZL), npr(tNPR)
{
	nprnmr = new TEisNmrFace(this, IDC_EIS_NPRNMR, "nprnmr");
	new TUCNFace(this, IDC_FZL_EGN, "fzlegn", UCN_CITIZEN_UCN);
	new TLNCFace(this, IDC_FZL_LNC, "fzllnc");
	imecyr = new TStringFace(this, IDC_FZL_IMECYR, "fzlimecyr", SIZE_OF_EISPP_STRING);
}

void TFzlGroupListDialog::SetupWindow()
{
	TGroupListDialog::SetupWindow();

	mstr m;

	nprnmr->SetS(npr->nmr);
	*fzl >> *this;
	fzl->Print(m);
	imecyr->SetS(str(m));
}
