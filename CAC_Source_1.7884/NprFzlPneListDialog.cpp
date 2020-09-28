DEFINE_RESPONSE_TABLE1(TNprFzlPneListDialog, TFzlGroupListDialog)
//{{TNprFzlPneListDialogRSP_TBL_BEGIN}}
	EV_COMMAND_ENABLE(IDADD, AddBNEnable),
//{{TNprFzlPneListDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TNprFzlPneListDialog Implementation}}

TNprFzlPneListDialog::TNprFzlPneListDialog(TWindow* parent, FZL *fzl, SBE *sbe, int resId)
:
	TFzlGroupListDialog(parent, fzl, sbe->npr, resId)
{
	nprfzlpnes = new TNprFzlPneListWindow(this, IDC_GROUP_LIST_WINDOW, fzl, sbe, NULL);
}

void TNprFzlPneListDialog::AddBNEnable(TCommandEnabler &tce)
{
	tce.Enable(nprfzlpnes->Array()->Count() < npr->dlo->pne->Count());
}
