DEFINE_RESPONSE_TABLE1(TUserListDialog, TGroupListDialog)
//{{TUserListDialogRSP_TBL_BEGIN}}
	EV_CBN_SELCHANGE(IDC_USER_LIST_POST, PostCBNSelChange),
	EV_COMMAND_ENABLE(IDDELETE, DeleteBNEnable),
	EV_BN_CLICKED(IDCOPY, CopyBNClicked),
	EV_COMMAND_ENABLE(IDCOPY, SelectEnable),
	EV_BN_CLICKED(IDRIGHT, RightBNClicked),
	EV_COMMAND_ENABLE(IDRIGHT, SelectEnable),
	EV_BN_CLICKED(IDREGION, RegionBNClicked),
	EV_COMMAND_ENABLE(IDREGION, RegionBNEnable),
	//EV_BN_CLICKED(IDASSIGN, AssignBNClicked),
	//EV_COMMAND_ENABLE(IDASSIGN, AssignBNEnable),
	EV_BN_CLICKED(IDLIST, ListBNClicked),
	EV_BN_CLICKED(IDQUERY, QueryBNClicked),
//{{TUserListDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TUserListDialog Implementation}}

TUserListDialog::TUserListDialog(TWindow* parent, int resId)
:
	TGroupListDialog(parent, resId)
{
	post = new TCharAliasFace(this, IDC_USER_LIST_POST, "F_POST", Post, POST_VISIBLES);
	users = new TUserListWindow(this, IDC_GROUP_LIST_WINDOW);
	new TTwinButton(this, IDLIST, IDQUERY);
}

void TUserListDialog::SetupWindow()
{
	TGroupListDialog::SetupWindow();
	PostCBNSelChange();
}

void TUserListDialog::PostCBNSelChange()
{
	users->SetPost(post->C());
}

void TUserListDialog::DeleteBNEnable(TCommandEnabler &tce)
{
	TUser *user = (TUser *) users->Group();
	tce.Enable(user && !is_internal(user->ucn) && strcmp(user->ucn, Default->ucn));
}

void TUserListDialog::CopyBNClicked()
{
	users->Copy();
}

void TUserListDialog::RightBNClicked()
{
	users->Right();
}

void TUserListDialog::RegionBNClicked()
{
	users->Region();
}

void TUserListDialog::RegionBNEnable(TCommandEnabler &tce)
{
	if (!post->GetDroppedState())
		tce.Enable(post->C() == POST_DELIVERER && users->User());
}

//void TUserListDialog::AssignBNClicked()
//{
//	users->Assign();
//}

//void TUserListDialog::AssignBNEnable(TCommandEnabler &tce)
//{
//	if (!post->GetDroppedState())
//		tce.Enable(post->C() != POST_DELIVERER && users->User());
//}

void TUserListDialog::ListBNClicked()
{
	users->List(FLAG_ACTIVE);
}

void TUserListDialog::QueryBNClicked()
{
	users->List(FLAG_NULL);
}
