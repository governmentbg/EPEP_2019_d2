DEFINE_RESPONSE_TABLE1(TSubjectListDialog, TGroupListDialog)
//{{TSubjectListDialogRSP_TBL_BEGIN}}
	EV_CBN_SELCHANGE(IDC_SUBJECT_LIST_KIND, KindCBNSelChange),
	//EV_COMMAND_ENABLE(IDCOPY, SelectEnable),
	EV_BN_CLICKED(IDCOPY, CopyBNClicked),
	EV_BN_CLICKED(IDPRINT, PrintBNClicked),
	EV_BN_CLICKED(IDRIGHT, RightBNClicked),
	EV_COMMAND_ENABLE(IDDELETE, DeleteBNEnable),
	EV_COMMAND_ENABLE(IDADD, AddBNEnable),
	EV_COMMAND_ENABLE(IDCOPY, CopyBNEnable),
//{{TSubjectListDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TSubjectListDialog Implementation}}

TSubjectListDialog::TSubjectListDialog(TWindow* parent)
:
	TGroupListDialog(parent, IDD_SUBJECT_LIST)
{
	kind = new TCharAliasFace(this, IDC_SUBJECT_LIST_KIND, "F_KIND", Kind, KIND_LAWSUITS);
	subjects = new TSubjectListWindow(this, IDC_GROUP_LIST_WINDOW);
	new TTwinButton(this, IDPRINT, IDRIGHT);
}

void TSubjectListDialog::SetupWindow()
{
	TGroupListDialog::SetupWindow();
	KindCBNSelChange();
}

void TSubjectListDialog::KindCBNSelChange()
{
	subjects->SetKind(kind->C());
}

void TSubjectListDialog::CopyBNClicked()
{
	subjects->Copy();
}

void TSubjectListDialog::PrintBNClicked()
{
	subjects->Print();
}

void TSubjectListDialog::RightBNClicked()
{
	subjects->PrintOld();
}

void TSubjectListDialog::DeleteBNEnable(TCommandEnabler &tce)
{
	TSubject *subject = subjects->Subject();
	// 2016:062
	//tce.Enable(TESTVER && subject && (subject->subject != SUBJECT_A_SUBJECT || subject->type != TYPE_PUNISHMENT_LAW));
	tce.Enable(subject && subject->flagsX == SUBJECT_X_ADDED);
}

void TSubjectListDialog::AddBNEnable(TCommandEnabler &tce)
{
	tce.Enable(TESTVER);
}

void TSubjectListDialog::CopyBNEnable(TCommandEnabler &tce)
{
	tce.Enable(TESTVER);
}
