DEFINE_RESPONSE_TABLE1(TSenderListDialog, TGroupListDialog)
//{{TSenderListDialogRSP_TBL_BEGIN}}
	EV_CBN_SELCHANGE(IDC_SENDER_LIST_UCN_TYPE, UCNTypeCBNSelChange),
	EV_COMMAND_ENABLE(IDADD, AddBNEnable),
	EV_COMMAND_ENABLE(IDEDIT, EditBNEnable),
	EV_COMMAND_ENABLE(IDDELETE, DeleteBNEnable),
	EV_BN_CLICKED(IDLIST, ListBNClicked),
//{{TSenderListDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TSenderListDialog Implementation}}

TSenderListDialog::TSenderListDialog(TWindow* parent, int resId)
:
	TGroupListDialog(parent, resId)
{
	ucnTypes.cat(UCN_EDITABLE_SENDERS);

#if COURT_TYPE == COURT_AREAL
	if (!COURT_LOCK)
		ucnTypes.cach(UCN_CREDITOR);
#endif  // AREAL

	if (COURT_UCLP == BUILDIN_UCLP)
		ucnTypes.cach(UCN_INFORMED);

	ucnType = new TCharAliasFace(this, IDC_SENDER_LIST_UCN_TYPE, "F_UCN_TYPE", UCNType, str(ucnTypes));
	senders = new TSenderListWindow(this, IDC_GROUP_LIST_WINDOW);
}

void TSenderListDialog::SetupWindow()
{
	TGroupListDialog::SetupWindow();
	UCNTypeCBNSelChange();
}

void TSenderListDialog::UCNTypeCBNSelChange()
{
	senders->SetUCNType(ucnType->C());
}

void TSenderListDialog::AddBNEnable(TCommandEnabler &tce)
{
	if (!ucnType->GetDroppedState())
		tce.Enable(ucnTypeIn(UCN_ADD_ABLE_SENDERS));
}

void TSenderListDialog::EditBNEnable(TCommandEnabler &tce)
{
	if (!ucnType->GetDroppedState())
		tce.Enable((ucnTypeIn(UCN_EDIT_NAME_SENDERS) || ucnTypeIn(UCN_EDIT_ADDR_SENDERS)) && senders->Group());
}

void TSenderListDialog::DeleteBNEnable(TCommandEnabler &tce)
{
	AddBNEnable(tce);
}

void TSenderListDialog::ListBNClicked()
{
	senders->List();
}
