DEFINE_RESPONSE_TABLE1(TReasonListDialog, TGroupListDialog)
//{{TReasonListDialogRSP_TBL_BEGIN}}
	EV_BN_CLICKED(IDSUBJECT, SubjectBNClicked),
	EV_BN_CLICKED(IDPRINT, PrintBNClicked),
	EV_COMMAND_ENABLE(IDSUBJECT, SelectEnable),
//{{TReasonListDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TReasonListDialog Implementation}}

TReasonListDialog::TReasonListDialog(TWindow* parent, int resId)
:
	TGroupListDialog(parent, resId)
{
	reasons = new TReasonListWindow(this, IDC_GROUP_LIST_WINDOW);
}

void TReasonListDialog::SetupWindow()
{
	TGroupListDialog::SetupWindow();
	SetCaption("Основания за назначаване на служебни представители");
}

void TReasonListDialog::SubjectBNClicked()
{
	reasons->Subject();
}

void TReasonListDialog::PrintBNClicked()
{
	reasons->Print();
}
