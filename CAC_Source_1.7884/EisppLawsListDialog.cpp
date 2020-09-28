DEFINE_RESPONSE_TABLE1(TEisppLawsListDialog, TQuickDialog)
//{{TEisppLawsListDialogRSP_TBL_BEGIN}}
	EV_COMMAND_ENABLE(IDQUERY, QueryBNEnable),
	EV_BN_CLICKED(IDQUERY, QueryBNClicked),
//{{TEisppLawsListDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TEisppLawsListDialog Implementation}}

TEisppLawsListDialog::TEisppLawsListDialog(TWindow* parent, TLawsuit *lawsuit, int resId)
:
	TQuickDialog(parent, resId)
{
	eisppLaws = new TEisppLawsListBox(this, IDC_EISPP_LAWS_LIST_LIST, lawsuit);
}

void TEisppLawsListDialog::QueryBNClicked()
{
	eisppLaws->Query();
}

void TEisppLawsListDialog::QueryBNEnable(TCommandEnabler &tce)
{
	tce.Enable(eisppLaws->Group());
}
