DEFINE_RESPONSE_TABLE1(TDangerListDialog, TGroupListDialog)
//{{TDangerListDialogRSP_TBL_BEGIN}}
	EV_BN_CLICKED(IDSUBJECT, SubjectBNClicked),
	EV_COMMAND_ENABLE(IDSUBJECT, SubjectBNEnable),
	EV_BN_CLICKED(IDPRINT, PrintBNClicked),
//{{TDangerListDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TDangerListDialog Implementation}}

TDangerListDialog::TDangerListDialog(TWindow* parent, int resId)
:
	TGroupListDialog(parent, resId)
{
	dangers = new TDangerListWindow(this, IDC_GROUP_LIST_WINDOW);
}

void TDangerListDialog::SubjectBNClicked()
{
	dangers->Subject();
}

void TDangerListDialog::SubjectBNEnable(TCommandEnabler &tce)
{
	TDanger *danger = dangers->Danger();
	tce.Enable(danger && danger->danger % DANGER_MASK_ARTICLE);
}

void TDangerListDialog::PrintBNClicked()
{
	dangers->Print();
}
