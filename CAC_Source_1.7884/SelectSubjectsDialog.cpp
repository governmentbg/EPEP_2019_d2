DEFINE_RESPONSE_TABLE1(TSelectSubjectsDialog, TQuickDialog)
//{{TSelectSubjectsDialogRSP_TBL_BEGIN}}
	EV_BN_CLICKED(IDOK, OKBNClicked),
//{{TSelectSubjectsDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TSelectSubjectsDialog Implementation}}

TSelectSubjectsDialog::TSelectSubjectsDialog(TWindow* parent, TGArray<TSubject> *tSubjects, const char *kinds, bool saas,
	bool hidden)
:
	TQuickDialog(parent, IDD_SELECT_SUBJECTS), array(tSubjects)
{
	subjects = new TSelectSubjectsListBox(this, IDC_SELECT_SUBJECTS_SUBJECTS, kinds, saas, hidden);
}

void TSelectSubjectsDialog::SetupWindow()
{
	TQuickDialog::SetupWindow();
	subjects->SetMark(array);
}

void TSelectSubjectsDialog::OKBNClicked()
{
	subjects->GetMark(array);
	CmOk();
}
