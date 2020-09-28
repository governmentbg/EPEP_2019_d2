DEFINE_RESPONSE_TABLE1(TSelectOldSubjectsDialog, TQuickDialog)
//{{TSelectOldSubjectsDialogRSP_TBL_BEGIN}}
	EV_BN_CLICKED(IDOK, OKBNClicked),
//{{TSelectOldSubjectsDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TSelectOldSubjectsDialog Implementation}}

TSelectOldSubjectsDialog::TSelectOldSubjectsDialog(TWindow* parent, TGArray<TOldSubject> *tOldSubjects, const char *kinds)
:
	TQuickDialog(parent, IDD_SELECT_OLD_SUBJECTS), array(tOldSubjects)
{
	oldSubjects = new TOldSubjectListBox(this, IDC_SELECT_OLD_SUBJECTS_OLD_SUBJECTS, kinds);
}

void TSelectOldSubjectsDialog::SetupWindow()
{
	TQuickDialog::SetupWindow();
	oldSubjects->SetMark(array);
}

void TSelectOldSubjectsDialog::OKBNClicked()
{
	oldSubjects->GetMark(array);
	CmOk();
}
