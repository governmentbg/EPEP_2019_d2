DEFINE_RESPONSE_TABLE1(TStatQueryThirdDialog, TGroupListDialog)
//{{TSubjectListDialogRSP_TBL_BEGIN}}
	EV_BN_CLICKED(IDLAUNCH, LaunchBNClicked),
	EV_COMMAND_ENABLE(IDLAUNCH, LaunchBNEnable),
//{{TSubjectListDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TStatQueryThirdDialog Implementation}}

TStatQueryThirdDialog::TStatQueryThirdDialog(TWindow* parent)
:
	TGroupListDialog(parent, IDD_STAT_QUERY_THIRD)
{
	files = new TStatQueryListWindow(this, IDC_GROUP_LIST_WINDOW);
}

void TStatQueryThirdDialog::LaunchBNClicked()
{
	files->Launch();
}

void TStatQueryThirdDialog::LaunchBNEnable(TCommandEnabler &tce)
{
	tce.Enable(files->GetItemCount());
}
