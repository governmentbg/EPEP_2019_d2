DEFINE_RESPONSE_TABLE1(TPenalityListDialog, TGroupListDialog)
//{{TPenalityListDialogRSP_TBL_BEGIN}}
	EV_BN_CLICKED(IDSUBJECT, SubjectBNClicked),
	EV_BN_CLICKED(IDPRINT, PrintBNClicked),
	EV_COMMAND_ENABLE(IDSUBJECT, SelectEnable),
//{{TPenalityListDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TPenalityListDialog Implementation}}

TPenalityListDialog::TPenalityListDialog(TWindow* parent, int resId)
:
	TGroupListDialog(parent, resId)
{
	penalities = new TPenalityListWindow(this, IDC_GROUP_LIST_WINDOW);
}

void TPenalityListDialog::SetupWindow()
{
	TGroupListDialog::SetupWindow();
	SetCaption("��������� �� �������� �� �����");
}

void TPenalityListDialog::SubjectBNClicked()
{
	penalities->Subject();
}

void TPenalityListDialog::PrintBNClicked()
{
	penalities->Print();
}
