DEFINE_RESPONSE_TABLE1(TExternSessionListDialog, TGroupListDialog)
//{{TExternSessionListDialogRSP_TBL_BEGIN}}
	EV_CBN_SELCHANGE(IDC_EXTERN_SESSION_LIST_TYPE, TypeCBNSelChange),
	EV_BN_CLICKED(IDSEARCH, SearchBNClicked),
	EV_BN_CLICKED(IDRIGHT, RightBNClicked),
	EV_BN_CLICKED(IDDELETE, DeleteBNClicked),
	EV_COMMAND_ENABLE(IDASSIGN, AssignBNEnable),
	EV_BN_CLICKED(IDASSIGN, AssignBNClicked),
	EV_COMMAND_ENABLE(IDC_EXTERN_SESSION_LIST_REASSIGN, ReAssignBNEnable),
	EV_BN_CLICKED(IDC_EXTERN_SESSION_LIST_REASSIGN, ReAssignBNClicked),
	EV_COMMAND_ENABLE(IDC_EXTERN_SESSION_LIST_QUERY, QueryBNEnable),
	EV_BN_CLICKED(IDC_EXTERN_SESSION_LIST_QUERY, QueryBNClicked),
//{{TExternSessionListDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TExternSessionListDialog Implementation}}

TExternSessionListDialog::TExternSessionListDialog(TWindow* parent)
:
	TGroupListDialog(parent, IDD_EXTERN_SESSION_LIST)
{
	type = new TCharAliasFace(this, IDC_EXTERN_SESSION_LIST_TYPE, "F_TYPE", Type, Default->colleges);
	minDate = new TDateFace(this, IDC_EXTERN_SESSION_LIST_MIN_DATE, "F_MIN_DATE");
	new TDateFaceUpDown(this, IDC_EXTERN_SESSION_LIST_MIN_DATE_UPDN, minDate);
	maxDate = new TDateFace(this, IDC_EXTERN_SESSION_LIST_MAX_DATE, "F_MAX_DATE", false);
	new TDateFaceUpDown(this, IDC_EXTERN_SESSION_LIST_MAX_DATE_UPDN, maxDate);
	sessions = new TExternSessionListWindow(this, IDC_GROUP_LIST_WINDOW, group);
	new TTwinButton(this, IDADD, IDRIGHT);
}

void TExternSessionListDialog::SetupWindow()
{
	TGroupListDialog::SetupWindow();

	group >> this;
	SearchBNClicked();
}

void TExternSessionListDialog::TypeCBNSelChange()
{
	SearchBNClicked();
}

void TExternSessionListDialog::SearchBNClicked()
{
	group << this;
	sessions->Search();
}

void TExternSessionListDialog::RightBNClicked()
{
	sessions->Right();
}

void TExternSessionListDialog::DeleteBNClicked()
{
	TGroupListDialog::DeleteBNClicked();
}

void TExternSessionListDialog::AssignBNEnable(TCommandEnabler &tce)
{
	tce.Enable(can_func(RIGHT_DISTRIB));
}

void TExternSessionListDialog::AssignBNClicked()
{
	sessions->Assign();
}

void TExternSessionListDialog::ReAssignBNEnable(TCommandEnabler &tce)
{
	tce.Enable(can_func(RIGHT_DISTRIB));
}

void TExternSessionListDialog::ReAssignBNClicked()
{
	sessions->ReAssign();
}

void TExternSessionListDialog::QueryBNEnable(TCommandEnabler &tce)
{
	tce.Enable(can_func(RIGHT_DISTRIB));
}

void TExternSessionListDialog::QueryBNClicked()
{
	sessions->Query();
}
