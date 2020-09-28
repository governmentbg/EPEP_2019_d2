DEFINE_RESPONSE_TABLE1(TSelectUserDialog, TGroupDialog)
//{{TSelectUserDialogRSP_TBL_BEGIN}}
	EV_COMMAND_ENABLE(IDOK, OKBNEnable),
	EV_BN_CLICKED(IDOK, OKBNClicked),
//{{TSelectUserDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TSelectUserDialog Implementation}}

TSelectUserDialog::TSelectUserDialog(TWindow* parent, const char *posts, long flags, TUser *user, const char *exclude,
	int resId)
:
	TGroupDialog(parent, user, resId)
{
	users = new TUserListBox(this, IDC_SELECT_USER_USERS, posts, flags, exclude);
}

void TSelectUserDialog::OKBNEnable(TCommandEnabler &tce)
{
	tce.Enable(users->Group());
}

void TSelectUserDialog::OKBNClicked()
{
	*Group() << users->Group();
	CmOk();
}
