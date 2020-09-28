#include "cac.h"

DEFINE_RESPONSE_TABLE1(TDoubleListWindow, TColumnListWindow)
//{{TDoubleListWindowRSP_TBL_BEGIN}}
	EV_WM_CHAR,
	EV_WM_KEYDOWN,
	EV_WM_SETFOCUS,
	EV_WM_KILLFOCUS,
	EV_WM_LBUTTONDOWN,
	EV_WM_PAINT,
	EV_WM_SIZE,
//{{TDoubleListWindowRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TDoubleListWindow Implementation}}

TDoubleListWindow::TDoubleListWindow(TWindow* parent, int resourceId, TGroupArray *tArray, int tAdjustWidth)
:
	TColumnListWindow(parent, resourceId, tAdjustWidth), TQuickList(parent, QUICK_SEARCH_ABLE), doubleLock(false),
		virtualFocus(-1), columnsAdded(false), surroundIcon(false), allowDeselect(false), array(tArray)
{
}

void TDoubleListWindow::Add(TGroupListColumn *column1, TGroupListColumn *column2, ...)
{
	va_list ap;

	va_start(ap, column2);
	TColumnListWindow::Add(column1, column2, ap);
	va_end(ap);
}

void TDoubleListWindow::Add()
{
	TGroup *group = NewGroup();

	if (AddGroup(group))
		InsertGroup(group);
	else
		delete group;
}

void TDoubleListWindow::Edit()
{
	int index = GetVisualIndex();

	if (index >= 0)
	{
		TGroup *group = Group();
		TGroup *tGroup = NewGroup();

		*tGroup << group;

		if (EditGroup(group))
			DrawGroupAt(group, index, false);
		else
			*group << tGroup;

		delete tGroup;
	}
}

void TDoubleListWindow::Delete()
{
	TGroup *group = Group();

	if (group && AskDelete(group) && DeleteGroup(group))
		RemoveGroup(IDDELETE);
}

bool TDoubleListWindow::AddGroup(TGroup *)
{
	return false;
}

bool TDoubleListWindow::EditGroup(TGroup *)
{
	return false;
}

bool TDoubleListWindow::AskDelete(TGroup *)
{
	return ask("Желаете ли да изтриете избраният елемент?");
}

bool TDoubleListWindow::DeleteGroup(TGroup *group)
{
	return group->Delete(true);
}

TGroup *TDoubleListWindow::Group()
{
	int index = GetVisualIndex();
	return index >= 0 ? GroupAt(index >> 1) : NULL;
}

void TDoubleListWindow::InsertGroup(TGroup *group)
{
	int index = array->Add(group) << 1;

	if (columnsAdded)
	{
		InsertVisual(group, index);
		SetItemState(index, LVIS_DROPHILITED, LVIS_DROPHILITED);
	}
}

void TDoubleListWindow::RemoveGroup(int command)
{
	int index = GetVisualIndex();

#if RANGECHECK
	if (index < 0)
		fatal("TDoubleListWindow::RemoveGroup");
#endif  // RANGECHECK

	DeleteAnItem(index);
	DeleteAnItem(index);
	virtualFocus = index;

	index >>= 1;
	array->Remove(index);
	if (index >= array->Count())
		virtualFocus -= 1 + (array->Count() > 0);

	if (!UnblockFocus(command))
		RedrawFocus();
}

int TDoubleListWindow::GetFocusedIndex()
{
	int index = GetVisualIndex();
	return index >= 0 ? index : virtualFocus;
}

void TDoubleListWindow::FlushVisuals()
{
	TColumnListWindow::FlushVisuals();
	virtualFocus = -1;
}

void TDoubleListWindow::GetSearchText(mstr &m, int index)
{
	for (int column = 0; column < columns.Count(); column++)
	{
		if (columns[column]->search)
		{
			char buffer[0x100];

			GetItemText(index, column, buffer, sizeof buffer);
			m.sep(" ");
			m.cat(skiplead(buffer));
		}
	}
}

void TDoubleListWindow::SetVisualIndex(int index)
{
	SetItemState(index, LVIS_DROPHILITED, LVIS_DROPHILITED);
	EnsureVisible(index, false);
}

void TDoubleListWindow::DrawGroupAt(TGroup *group, int index, bool insert)
{
	TColumnListWindow::DrawGroupAt(group, index, insert);

	TListWindItem lwi;

	lwi.iItem = index + 1;
	lwi.iSubItem = 0;
	lwi.mask = 0;

	if (insert)
	{
		lwi.SetText("");
		InsertItem(lwi);
	}

	lwi.iSubItem++;
	DrawSecond(lwi, group);
}

bool TDoubleListWindow::Deselect()
{
	int index = GetVisualIndex();

	if (index >= 0)
	{
		doubleLock = true;
		SetItemState(index, 0, LVIS_DROPHILITED);
		SetItemState(index + 1, 0, LVIS_DROPHILITED);
		doubleLock = false;
		return true;
	}

	return false;
}

TGroup *TDoubleListWindow::NewGroup() const
{
	return NULL;
}

void TDoubleListWindow::Refresh(int size)
{
	if (!columnsAdded)
	{
		columnsAdded = true;
		AddColumns(size);

		for (int index = 0; index < array->Count(); index++)
			InsertVisual(GroupAt(index), index << 1);

		if (array->Count() > 0)
		{
			virtualFocus = 0;
			RedrawFocus();
		}
	}
}

void TDoubleListWindow::SetupWindow()
{
	TColumnListWindow::SetupWindow();

	HWND hWnd = (HWND) SendMessage(LVM_GETHEADER);

	if (hWnd)
	{
		SetWaitCursor(false);
		new TDoubleListHeader(hWnd);
	}
}

void TDoubleListWindow::EvChar(uint key, uint repeatCount, uint flags)
{
	if (!QuickChar(key))
		TColumnListWindow::EvChar(key, repeatCount, flags);
}

void TDoubleListWindow::EvKeyDown(uint key, uint repeatCount, uint flags)
{
	int index = GetFocusedIndex();
	int page = (GetCountPerPage() & ~1) + 2;
	int count = GetItemCount();

	// repeatCount is not intuitive for interactive stuff, so ignore it
	switch (key)
	{
		case VK_UP :
		{
			// win~1 does it that way...
			if ((index -= 2) < 0 && count == 2)
				index = 0;
			break;
		}
		case VK_DOWN : index += 2; break;
		case VK_PRIOR :
		{
			if ((index -= page) < 0)
				index = 0;
			break;
		}
		case VK_NEXT :
		{
			if ((index += page) >= count)
				index = count - 2;
			break;
		}
		case VK_HOME : index = 0; break;
		case VK_END : index = count - 2; break;
		case VK_SPACE : index = (index + 1) & ~1; break;
		case VK_INSERT :
		{
			if (allowDeselect)
				Deselect();
			// falldown
		}
		default :
		{
			if (!QuickKey(key))
				TColumnListWindow::EvKeyDown(key, repeatCount, flags);
			return;
		}
	}

	SetVisualIndex(index);
}

void TDoubleListWindow::EvSetFocus(THandle hWndLostFocus)
{
	TColumnListWindow::EvSetFocus(hWndLostFocus);
	Parent->SendMessage(WM_FOCUS_FACE, TRUE, Attr.Id);
	QuickSetFocus();
	RedrawFocus();
}

void TDoubleListWindow::EvKillFocus(THandle hWndGetFocus)
{
	QuickKillFocus();
	TColumnListWindow::EvKillFocus(hWndGetFocus);
	Parent->SendMessage(WM_FOCUS_FACE, FALSE, Attr.Id);
	RedrawFocus();
}

void TDoubleListWindow::EvSize(uint sizeType, TSize& size)
{
	TColumnListWindow::EvSize(sizeType, size);
	Refresh(size.cx);
}

bool TDoubleListWindow::ItemChanging(TLwNotify &lwn)
{
	if (lwn.uNewState & LVIS_FOCUSED)
	{
		if (lwn.uNewState != LVIS_FOCUSED)
			SetItemState(lwn.iItem, lwn.uNewState & ~LVIS_FOCUSED, lwn.uNewState);
		// block focus
		return true;
	}

	if (!doubleLock)
	{
		unsigned oldSelected = lwn.uOldState & LVIS_DROPHILITED;
		unsigned newSelected = lwn.uNewState & LVIS_DROPHILITED;

		if (newSelected > oldSelected)
		{
			if (!Deselect() && virtualFocus >= 0)
			{
				int index = virtualFocus;
				virtualFocus = -1;
				RedrawItems(index, index + 1);
			}

			doubleLock = true;
			SetItemState(lwn.iItem ^ 1, LVIS_DROPHILITED, LVIS_DROPHILITED);
			EnsureVisible(lwn.iItem ^ 1, false);
			virtualFocus = lwn.iItem & ~1;
			doubleLock = false;
		}
		// block any but our deselects
		else if (oldSelected > newSelected)
			return true;
	}

	return false;
}

void TDoubleListWindow::EvLButtonDown(uint, TPoint& point)
{
	TLwHitTestInfo info(point);
	int index = HitTest(info);

	if (GetFocus() != HWindow)
		SetFocus();

	if (index >= 0)
		SetVisualIndex(index);
	else if (allowDeselect)
		Deselect();
}

void TDoubleListWindow::EvPaint()
{
	int index = GetFocusedIndex();
	bool drawFrame = index >= 0 && GetFocus() == HWindow;
	PAINTSTRUCT ps;

	if (drawFrame)
	{
		// remember invalidated rect
		ps = TPaintDC(*this).Ps;
		// TPaintDC creared invalidated rect
		InvalidateRect(ps.rcPaint, ps.fErase);
	}

	TColumnListWindow::EvPaint();

	if (drawFrame)
	{
		RECT r0, r1;

		GetItemRect(index, &r0, surroundIcon ? Label : Bounds);
		GetItemRect(index + 1, &r1, Bounds);

		// default paint cleared invalidated rect
		InvalidateRect(ps.rcPaint, ps.fErase);
		// use paint DC to prevent a partial-redraw-de-inverse-focus
		TPaintDC dc(*this);

		dc.DrawFocusRect(r0.left, r0.top, r1.right, r1.bottom);
	}
}

void TDoubleListWindow::RedrawFocus()
{
	int index = GetFocusedIndex();

	if (index >= 0)
		RedrawItems(index, index + 1);
}
