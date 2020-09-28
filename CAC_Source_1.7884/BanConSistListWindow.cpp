DEFINE_RESPONSE_TABLE1(TBanConSistListWindow, TDoubleListWindow)
//{{TBanConSistListWindowRSP_TBL_BEGIN}}
	EV_WM_LBUTTONDBLCLK,
//{{TBanConSistListWindowRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TBanConSistListWindow Implementation}}

TBanConSistListWindow::TBanConSistListWindow(TWindow* parent, int resourceId, bool tActive)
:
	TDoubleListWindow(parent, resourceId, &banConSists, tActive ? 420 : 432), active(tActive), dirty(false)
{
	Add(new TGroupListColumn("", generic_null, 0));

	if (!active)
		Add(new TGroupListColumn("", group_inactive, 12, LVCFMT_CENTER | LVCFMT_NO_SEARCH));

	Add(new TGroupListColumn("Текст", banconsist_text1, 408));

	BanConSist->LoadData(&banConSists, NULL, active ? FLAG_ACTIVE : FLAG_NULL);

	qsMode = QUICK_SEARCH_AUTO;	// 2012:080 LRQ/LRQ: if (active) -> always
	allowDeselect = true;
}

void TBanConSistListWindow::Add()
{
	TBanConSist *banConSist = new TBanConSist;

	banConSist->text.cpy("Настоящото удостоверение да послужи");

	if (TEditBanConSistDialog(this, banConSist, false, active).Execute() == IDOK)
	{
		InsertGroup(banConSist);
		dirty = true;
	}
	else
		delete banConSist;
}

void TBanConSistListWindow::Edit()
{
	TBanConSist banConSist;

	banConSist << Group();

	if (TEditBanConSistDialog(this, &banConSist, true, active).Execute() == IDOK)
	{
		*Group() << banConSist;
		DrawGroupAt(Group(), GetVisualIndex(), false);
		dirty = true;
	}
}

void TBanConSistListWindow::Delete()
{
	if (ask("Желаете ли да изтриете избрания елемент?") && Group()->Delete(true))
		RemoveGroup(IDDELETE);
}

void TBanConSistListWindow::DrawSecond(TListWindItem &lwi, TGroup *group)
{
	TBanConSist *banConSist = (TBanConSist *) group;

	lwi.iSubItem = 1 + !active;
	lwi.SetText(banConSist->text2);
	SetItem(lwi);
}

void TBanConSistListWindow::SetupWindow()
{
	TDoubleListWindow::SetupWindow();
	Refresh(GetClientRect().Width());
}

void TBanConSistListWindow::CleanupWindow()
{
	TDoubleListWindow::CleanupWindow();

	if (dirty)
		BanConSist->ReloadCache();
}

void TBanConSistListWindow::EvLButtonDblClk(uint modKeys, TPoint& point)
{
	TListWindow::EvLButtonDblClk(modKeys, point);
	ParentNotify(IDEDIT);
}
