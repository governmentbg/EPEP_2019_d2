//{{TEditNprFzlStaDialog Implementation}}

TEditNprFzlStaDialog::TEditNprFzlStaDialog(TWindow *parent, FZL *fzl, NPR *npr, int resId)
:
	TFzlDialog(parent, fzl, fzl, -1, npr->nmr, resId)
{
	osn = new TEisAliasFace(this, IDC_EDIT_NPRFZLSTA_OSN, "nprfzlosn", npr->nprfzlnmk, fzl);
	dta = new TDateFace(this, IDC_EDIT_NPRFZLSTA_DTA, "nprfzldta", false);
	kcv = new TEisAliasFace(this, IDC_EDIT_NPRFZLSTA_KCV, "nprfzlkcv", nmk_nprfzlkcv, fzl);
	sts = new TEisAliasFace(this, IDC_EDIT_NPRFZLSTA_STS, "nprfzlsts", nmk_nprfzlsts, fzl);
}
