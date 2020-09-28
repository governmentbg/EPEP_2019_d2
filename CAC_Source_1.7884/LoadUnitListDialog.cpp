//{{TLoadUnitListDialog Implementation}}

TLoadUnitListDialog::TLoadUnitListDialog(TWindow* parent, TLoadGroup *loadGroup, TLoadGroup *invisible, int resId)
:
	TGroupListDialog(parent, resId)
{
	units = new TLoadUnitListWindow(this, IDC_GROUP_LIST_WINDOW, loadGroup, invisible);
}
