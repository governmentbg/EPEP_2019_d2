//{{TEditNkzPneDialog Implementation}}

TEditNkzPneDialog::TEditNkzPneDialog(TWindow *parent, FZL *fzl, NKZPNE *nkzpne, const char *nmrnpr, int resId)
:
	TEditDsmgRzmDialog(parent, fzl, nkzpne, nmrnpr, resId)
{
	vid = new TEisAliasFace(this, IDC_EDIT_NKZPNE_VID, "nkzpnevid", nmk_nkzvid, nkzpne);
	rzm = new TMoneyFace(this, IDC_EDIT_NKZPNE_RZM, "nkzpnerzm", SIZE_OF_SUM);
	den = new TLongFace(this, IDC_EDIT_NKZPNE_DEN, "nkzpneden", SIZE_OF_EISPP_RZM);
	sdc = new TLongFace(this, IDC_EDIT_NKZPNE_SDC, "nkzpnesdc", SIZE_OF_EISPP_RZM);
	msc = new TLongFace(this, IDC_EDIT_NKZPNE_MSC, "nkzpnemsc", SIZE_OF_EISPP_RZM);
	gdn = new TLongFace(this, IDC_EDIT_NKZPNE_GDN, "nkzpnegdn", SIZE_OF_EISPP_RZM);
}
