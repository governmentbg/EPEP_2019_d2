//{{TFzlDialog Implementation}}

TFzlDialog::TFzlDialog(TWindow* parent, FZL *tFZL, TXMLGroup *group, sign edit, const char *nmrnpr, int resId)
:
	TEisDialog(parent, group, edit, nmrnpr, resId), fzl(tFZL)
{
	new TStringFace(this, IDC_FZL_EGN, "fzlegn", SIZE_OF_UCN);
	new TStringFace(this, IDC_FZL_LNC, "fzllnc", SIZE_OF_UCN);
	imecyr = new TStringFace(this, IDC_FZL_IMECYR, "fzlimecyr", SIZE_OF_EISPP_STRING);
}

void TFzlDialog::SetupWindow()
{
	TEisDialog::SetupWindow();

	mstr m;

	*fzl >> *this;
	fzl->Print(m);
	imecyr->SetS(str(m));
}
