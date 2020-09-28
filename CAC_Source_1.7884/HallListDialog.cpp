DEFINE_RESPONSE_TABLE1(THallListDialog, TGroupListDialog)
//{{THallListDialogRSP_TBL_BEGIN}}
	EV_BN_CLICKED(IDLOADWEEK, LoadWeekBNClicked),
	EV_BN_CLICKED(IDLOADDATE, LoadDateBNClicked),
	EV_BN_CLICKED(IDPRINT, PrintBNClicked),
	EV_COMMAND_ENABLE(IDLOADWEEK, SelectEnable),
	EV_COMMAND_ENABLE(IDLOADDATE, SelectEnable),
//{{THallListDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{THallListDialog Implementation}}

THallListDialog::THallListDialog(TWindow* parent, int resId)
:
	TGroupListDialog(parent, resId)
{
	halls = new THallListWindow(this, IDC_GROUP_LIST_WINDOW);
}

void THallListDialog::LoadWeekBNClicked()
{
	halls->LoadWeek();
}

void THallListDialog::LoadDateBNClicked()
{
	halls->LoadDate();
}

void THallListDialog::PrintBNClicked()
{
	halls->Print();
}
