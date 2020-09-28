//{{TProveSideListDialog Implementation}}

TProveSideListDialog::TProveSideListDialog(TWindow* parent, TProve *prove, long sideMode, int resId)
:
	TSideWindListDialog(parent, prove, NULL, sideMode | SIDE_LIST_DATE, "доказателството", resId)
{
}
