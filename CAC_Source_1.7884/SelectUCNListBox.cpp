DEFINE_RESPONSE_TABLE1(TSelectUCNListBox, TGroupListBox)
//{{TSelectUCNListBoxRSP_TBL_BEGIN}}
	EV_WM_KEYDOWN,
	EV_WM_LBUTTONDOWN,
	EV_WM_LBUTTONDBLCLK,
//{{TSelectUCNListBoxRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TSelectUCNListBox Implementation}}

TSelectUCNListBox::TSelectUCNListBox(TWindow* parent, int resourceId, const char *tUCNTypes, bool tActive, bool tStat)
:
	TGroupListBox(parent, resourceId, new TGArray<TUCNGroup>), ucnTypes(tUCNTypes), active(tActive), stat(tStat)
{
	initialRefresh = false;
}

void TSelectUCNListBox::FlushList()
{
	FlushVisuals();
	array->Flush();
	UpdateVisual();
}

void TSelectUCNListBox::Print(char uxnType)
{
	const char *title = uxnType == UXN_SENDER ? "адресантите" : uxnType == UXN_DAMN ? "фирмите" : "лицата";
	print_ucn_groups(array, NULL, "ucnlist.htm", title);
}

void TSelectUCNListBox::Address()
{
	view_addresses(this, (TUCNGroup *) Group());
}

void TSelectUCNListBox::AddUCNGroup(TNamedGroup *ucnGroup, bool extra, char involvement)
{
	msql m;

	ucnGroup->NamedCriteria(m, NULL, extra ? "%" : "", involvement, active);

	if (stat && !strcmp(ucnTypes, C2S[UCN_INSTANCE_EQUAL]))
	{
		m.CutOrder();
		m.AddLong("NOT F_COURT", COURT_CODE);
		m.AddOrder("F_NAME");
	}

	TQuery q(str(m));

	while (q.Read())
	{
		*ucnGroup << q;

		if (strchr(ucnTypes, ucnGroup->ucnType))
		{
			TUCNGroup *tUCNGroup = TNamedGroup::NewGroup(ucnGroup->ucnType);
			*tUCNGroup << q;
			InsertGroup(tUCNGroup);
		}
	}

	UpdateVisual();
}

void TSelectUCNListBox::SetUCNGroup(TNamedGroup *ucnGroup, bool extra, char involvement)
{
	FlushList();
	AddUCNGroup(ucnGroup, extra, involvement);
}

void TSelectUCNListBox::GetGroupText(mstr &m, TGroup *group, int column)
{
	TUCNGroup *ucnGroup = (TUCNGroup *) group;

	if (strchr(UCN_LIST_NAMES_ONLY, ucnGroup->ucnType))
		ucnGroup->Find("F_NAME")->Print(m);
	else
	{
		TGroupListBox::GetGroupText(m, group, column);
		ucngroup_extra(m, ucnGroup);
	}
}

TGroup *TSelectUCNListBox::NewGroup() const
{
	return new TFoundGroup;
}

void TSelectUCNListBox::EvKeyDown(uint key, uint repeatCount, uint flags)
{
	// 2007:215 LPR: (un)mark with space
	// 2008:071 LPR: handle unmark only as KeyDown not Char
	// 2009:022 LPR: moved here from select ucn list box
	if (!multiSelect && key == VK_SPACE && GetVisualIndex() >= 0)
	{
		SetVisualIndex(-1);
		Parent->PostMessage(WM_COMMAND, MAKEWPARAM(Attr.Id, LBN_SELCHANGE), (LPARAM) Handle);
	}
	else
		TGroupListBox::EvKeyDown(key, repeatCount, flags);
}

void TSelectUCNListBox::EvLButtonDown(uint modKeys, TPoint &point)
{
	int index = GetVisualIndex();
	TRect rect;

	if (!multiSelect && index >= 0 && GetItemRect(index, rect) != LB_ERR && rect.Contains(point))
	{
		SetSelIndex(-1);
		Parent->PostMessage(WM_COMMAND, MAKEWPARAM(Attr.Id, LBN_SELCHANGE), (LPARAM) Handle);
	}
	else
		TGroupListBox::EvLButtonDown(modKeys, point);
}

void TSelectUCNListBox::EvLButtonDblClk(uint modKeys, TPoint &point)
{
	if (!multiSelect)
	{
		SendMessage(WM_LBUTTONDOWN, modKeys, MAKELONG(point.x, point.y));
		SendMessage(WM_LBUTTONUP, modKeys, MAKELONG(point.x, point.y));
	}
	else
		TGroupListBox::EvLButtonDblClk(modKeys, point);
}
