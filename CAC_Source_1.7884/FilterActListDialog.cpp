DEFINE_RESPONSE_TABLE1(TFilterActListDialog, TGroupListDialog)
//{{TFilterActListDialogRSP_TBL_BEGIN}}
	EV_COMMAND_ENABLE(IDFILTER, SelectEnable),
	EV_BN_CLICKED(IDFILTER, FilterBNClicked),
	EV_COMMAND_ENABLE(IDQUERY, SelectEnable),
	EV_BN_CLICKED(IDQUERY, QueryBNClicked),
	EV_BN_CLICKED(IDREFRESH, RefreshBNClicked),
//{{TFilterActListDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TFilterActListDialog Implementation}}

TFilterActListDialog::TFilterActListDialog(TWindow* parent, TUnfilteredActsGroup *unfiltered)
:
	TGroupListDialog(parent, IDD_FILTER_ACT_LIST)
{
	acts = new TFilterActListWindow(this, IDC_GROUP_LIST_WINDOW, unfiltered);
}

void TFilterActListDialog::FilterBNClicked()
{
	acts->Filter();
}

void TFilterActListDialog::QueryBNClicked()
{
	acts->Query();
}

void TFilterActListDialog::RefreshBNClicked()
{
	acts->Refresh();
}
