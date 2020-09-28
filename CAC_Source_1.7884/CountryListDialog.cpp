//{{TCountryListDialog Implementation}}

TCountryListDialog::TCountryListDialog(TWindow* parent)
:
	TGroupListDialog(parent, IDD_COUNTRY_LIST)
{
	new TCountryListWindow(this, IDC_GROUP_LIST_WINDOW);
}
