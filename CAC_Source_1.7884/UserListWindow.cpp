DEFINE_RESPONSE_TABLE1(TUserListWindow, TGroupListWindow)
//{{TUserListWindowRSP_TBL_BEGIN}}
	EV_WM_LBUTTONDBLCLK,
//{{TUserListWindowRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TUserListWindow Implementation}}

TUserListWindow::TUserListWindow(TWindow* parent, int resourceId)
:
	TGroupListWindow(parent, resourceId, NULL, 236), post('\0')
{
	Add(
		new TGroupListColumn("", group_inactive, 12, LVCFMT_CENTER | LVCFMT_NO_SEARCH),
		new TGroupListColumn("Име", group_print, 148),
		new TGroupListColumn("Потребител", "F_LOGIN", 64),
		NULL
	);

	initialRefresh = false;
	abcskip = ABC_SKIP_LOWER;
	sortcol = 2;
}

void TUserListWindow::SetPost(char tPost)
{
	if (post != tPost)
	{
		post = tPost;
		Refresh();
	}
}

void TUserListWindow::Copy()
{
	TUser *copy = User();
	TUser *user = (TUser *) NewGroup();

	if (add_user(this, user, copy))
	{
		SetVisualIndex(InsertGroup(user));
		UpdateVisual();
	}
	else
		delete user;
}

void TUserListWindow::Right()
{
	TUser *user = User();
	TUser tUser;

	tUser << user;
	if (TEditUserDialog(this, &tUser).Execute() == IDOK)
	{
		if (tUser.post == user->post)
		{
			*user << tUser;
			RedrawGroup();
		}
		else
			RemoveGroup(IDRIGHT);
	}
}

void TUserListWindow::Region()
{
	map_user(this, User());
}

void TUserListWindow::List(long active)
{
	print_users(this, array, post, active);
}

void TUserListWindow::Criteria(msql &m, TGroup *group)
{
	m.Begin(group->Name);
	m.AddChar("F_POST", post);
	m.Add("NOT F_UCN");
	m.printf("IN ('%s', '%s')", EMPTY, INTER);
}

TGroup *TUserListWindow::NewGroup() const
{
	TUser *user = new TUser;
	user->post = post;
	return user;
}

bool TUserListWindow::AddGroup(TGroup *group)
{
	TUser *user = (TUser *) group;
	return add_user(this, user, NULL) && user->post == post;
}

bool TUserListWindow::EditGroup(TGroup *group)
{
	TUser *user = (TUser *) group;
	TCitizen citizen;

	citizen.ucnType = UCN_CITIZEN_UCN;
	strcpy(citizen.ucn, user->ucn);

	if (edit_ucn(this, &citizen, NULL))
	{
		user->Fixup(true);
		return true;
	}

	return false;
}

void TUserListWindow::EvLButtonDblClk(uint modKeys, TPoint& point)
{
	TListWindow::EvLButtonDblClk(modKeys, point);
	ParentNotify(IDRIGHT);
}
