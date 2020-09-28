//{{TSelectUCLPDialog Implementation}}

TSelectUCLPDialog::TSelectUCLPDialog(TWindow* parent, TUCLP *uclp, int resId)
:
	TSelectDialog(parent, uclp, resId)
{
	glist = new TSelectUCLPListBox(this, IDC_SELECT_UCLP_UCLPS, uclp->name);
}
