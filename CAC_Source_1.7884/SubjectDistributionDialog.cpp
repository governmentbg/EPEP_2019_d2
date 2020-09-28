DEFINE_RESPONSE_TABLE1(TSubjectDistributionDialog, TGroupDialog)
//{{TSubjectDistributionDialogRSP_TBL_BEGIN}}
	EV_BN_CLICKED(IDOK, OKBNClicked),
//{{TSubjectDistributionDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TSubjectDistributionDialog Implementation}}

TSubjectDistributionDialog::TSubjectDistributionDialog(TWindow* parent, TSubject *subject, TGArray<TUserAliasGroup> *tUsers)
:
	TGroupDialog(parent, subject, IDD_SUBJECT_DISTRIBUTION), array(tUsers)
{
	new TStringFace(this, IDC_SUBJECT_DISTRIBUTION_ALIAS, "F_ALIAS", SIZE_OF_NAME);
	users = new TUserListBox(this, IDC_SUBJECT_DISTRIBUTION_USERS, POST_VISIBLE_JUDGES, USER_NOLIMIT);
}

void TSubjectDistributionDialog::SetupWindow()
{
	TGroupDialog::SetupWindow();
	users->SetMark(array);
}

void TSubjectDistributionDialog::OKBNClicked()
{
	if (IsValid())
	{
		*Subject() << this;
		users->GetMark(array);
		CmOk();
	}
}
