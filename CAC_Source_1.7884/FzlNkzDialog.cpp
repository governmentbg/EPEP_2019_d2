
TfzlNkzDialog::TfzlNkzDialog(TWindow *parent, FZL *fzl, bool edit, int resId)
:	TEisDialog(parent, fzl, edit, resId)
{
      nprnmrLabel = new TStatic(this, IDC_NPRNMR_LABEL);
	nprnmr = new TStringFace(this, IDC_NPRNMR, "nprnmr", SIZE_OF_EISPP_NMR);
	fzlFaceLabel = new TStatic(this, IDC_FZL_FACE_LABEL);
	fzlegnLabel = new TStatic(this, IDC_FZL_EGN_LABEL);
	fzlegn = new TStringFace(this, IDC_FZL_EGN, "fzlegn", SIZE_OF_UCN);
	fzllncLabel = new TStatic(this, IDC_FZL_LNC_LABEL);
	fzllnc = new TStringFace(this, IDC_FZL_LNC, "fzllnc", SIZE_OF_UCN);
	fzlimecyrLabel = new TStatic(this, IDC_FZL_FULL_NAME_CRL_LABEL);
	fzlimecyr = new TStringFace(this, IDC_FZL_FULL_NAME_CRL, SIZE_OF_EISPP_STRING)
	fzlsLabel = new TStatic(this, IDC_FZLS_LABEL);
//	fzls = new TGroupListWindow(this, IDC_FZLS, fzlsArray, 180); // korekciq ... 
	addFzl = new TPushButton(this, IDC_FZL_ADD);
	editFzl = new TPushButton(this, IDC_FZL_EDIT);
	delFzl = new TPushButton(this, IDC_FZL_DEL);
	nkzsLabel = new TStatic(this, IDC_FZL_NKZ_S_LABEL);
//	nkzs = new TGroupListWindow(this, IDC_FZL_NKZ_S, nkzsArray, 180); // korekciq ... 
	addNkz = new TPushButton(this, IDC_FZL_NKZ_ADD);
	editNkz = new TPushButton(this, IDC_FZL_NKZ_EDIT);
	delNkz = new TPushButton(this, IDC_FZL_NKZ_DEL);
}