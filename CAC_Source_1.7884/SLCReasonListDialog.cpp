DEFINE_RESPONSE_TABLE1(TSLCReasonListDialog, TGroupListDialog)
//{{TSLCReasonListDialogRSP_TBL_BEGIN}}
	EV_CBN_SELCHANGE(IDC_SLC_REASON_LIST_SLC_TYPE, SLCTypeCBNSelChange),
	EV_COMMAND_ENABLE(IDEDIT, ChangeEnable),
	EV_COMMAND_ENABLE(IDDELETE, ChangeEnable),
	EV_BN_CLICKED(IDPRINT, PrintBNClicked),
//{{TSLCReasonListDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TSLCReasonListDialog Implementation}}

TSLCReasonListDialog::TSLCReasonListDialog(TWindow* parent, int resId)
:
	TGroupListDialog(parent, resId)
{
	slcType = new TCharAliasFace(this, IDC_SLC_REASON_LIST_SLC_TYPE, "F_SLC_TYPE", SLCType, NULL);
	reasons = new TSLCReasonListWindow(this, IDC_GROUP_LIST_WINDOW);
}

void TSLCReasonListDialog::SetupWindow()
{
	TGroupListDialog::SetupWindow();
	SLCTypeCBNSelChange();
}

void TSLCReasonListDialog::SLCTypeCBNSelChange()
{
	reasons->SetSLCType(slcType->C());
}

void TSLCReasonListDialog::ChangeEnable(TCommandEnabler &tce)
{
	TSLCReason *reason = static_cast<TSLCReason *> (reasons->Group());
	tce.Enable(reason && !(reason->flags & SLC_REASON_BOOK));
}

void TSLCReasonListDialog::PrintBNClicked()
{
	reasons->Print();
}
