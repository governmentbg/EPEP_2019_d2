#include "cac.h"

DEFINE_RESPONSE_TABLE1(TGroupListWindow, TColumnListWindow)
//{{TGroupListWindowRSP_TBL_BEGIN}}
	EV_WM_PAINT,
	EV_WM_CHAR,
	EV_WM_KEYDOWN,
	EV_WM_SETFOCUS,
	EV_WM_KILLFOCUS,
	EV_WM_LBUTTONDBLCLK,
//{{TGroupListWindowRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TGroupListWindow Implementation}}

TGroupListWindow::TGroupListWindow(TWindow* parent, int resourceId, TGroupArray *array, int adjustWidth)
:
	TColumnListWindow(parent, resourceId, adjustWidth), TGroupList(parent, array), allowDeselect(false)
{
	Init();
}

TGroupListWindow::TGroupListWindow(TWindow* parent, int resourceId, TGroupArray &array, int adjustWidth)
:
	TColumnListWindow(parent, resourceId, adjustWidth), TGroupList(parent, array), allowDeselect(false)
{
	Init();
}

void TGroupListWindow::Init()
{
	Add(new TGroupListColumn("", generic_null, 0, LVCFMT_NO_SEARCH));
}

void TGroupListWindow::Add(TGroupListColumn *column1, TGroupListColumn *column2, ...)
{
	va_list ap;

	va_start(ap, column2);
	TColumnListWindow::Add(column1, column2, ap);
	va_end(ap);
}

void TGroupListWindow::BackEdit()
{
	TGroup *group = Group();

	if (group)
	{
		TGroup *tGroup = NewGroup();
		*tGroup << group;

		if (EditGroup(group))
		{
			ResortGroup();
			UpdateVisual();
		}
		else
			*group << tGroup;

		delete tGroup;
	}
}

bool TGroupListWindow::ItemChanging(TLwNotify &lwn)
{
	if (!allowDeselect)
	{
		bool focused = lwn.uNewState & LVIS_FOCUSED;

		if ((lwn.uOldState & LVIS_FOCUSED) != focused)
		{
			lwn.uChanged |= LVIS_DROPHILITED;

			if (focused)
				lwn.uNewState |= LVIS_DROPHILITED;
			else
				lwn.uNewState &= LVIS_DROPHILITED;

			SetItemState(lwn.iItem, lwn.uNewState, lwn.uChanged);
		}
		else if (lwn.uNewState & LVIS_SELECTED)
		{
			lwn.uNewState |= LVIS_DROPHILITED;
			SetItemState(lwn.iItem, lwn.uNewState, lwn.uChanged);
		}
	}

	return false;
}

void TGroupListWindow::Add()
{
	TGroup *group = NewGroup();

	if (AddGroup(group))
	{
		SetVisualIndex(InsertGroup(group));
		UpdateVisual();
	}
	else
		delete group;
}

void TGroupListWindow::Edit()
{
	TGroup *group = Group();

	if (group)
	{
		TGroup *tGroup = NewGroup();
		*tGroup << group;

		if (EditGroup(tGroup))
		{
			*group << tGroup;
			ResortGroup();
			UpdateVisual();
		}

		delete tGroup;
	}
}

void TGroupListWindow::Delete()
{
	TGroup *group = Group();

	if (group && AskDelete(group) && DeleteGroup(group))
		RemoveGroup(IDDELETE);
}

int TGroupListWindow::GetVisualIndex()
{
	return GetSelIndex(LVIS_DROPHILITED);
}

void TGroupListWindow::SetVisualIndex(int index)
{
	if (index >= 0)
	{
		SetItemState(index, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
		EnsureVisible(index, false);
	}
	else if ((index == GetVisualIndex()) >= 0)
		SetItemState(index, 0, LVIS_SELECTED | LVIS_FOCUSED);
}

void TGroupListWindow::FlushVisuals()
{
	TColumnListWindow::FlushVisuals();
}

void TGroupListWindow::InsertVisual(int index)
{
	DrawItemAt(index, true);
}

void TGroupListWindow::RedrawVisual(int index)
{
	DrawItemAt(index, false);
}

void TGroupListWindow::RemoveVisual(int index)
{
	allowDeselect = true;
	DeleteAnItem(index);
	allowDeselect = false;
}

void TGroupListWindow::UpdateVisual()
{
	TColumnListWindow::UpdateVisual();

	// 2010:054
	if (EmptyRect())
		InvalidateRect(emptyRect, true);
}

void TGroupListWindow::GetGroupText(mstr &m, TGroup *group, int column)
{
	TColumnListWindow::GetGroupText(m, group, column);
}

void TGroupListWindow::GetSearchText(mstr &m, int index)
{
	for (int column = 0; column < columns.Count(); column++)
	{
		if (columns[column]->search)
		{
			char *s = GetVisualText(index, column);
			TCharPtr CP(s);

			m.sep(" ");
			m.cat(ABCSkip(s, true));
		}
	}
}

char *TGroupListWindow::GetVisualText(int index, int column)
{
	// TODO? unlimited; len = GetItemText() does not work
	char *buffer = new char[0x100];

	GetItemText(index, column, buffer, 0x100);
	return buffer;
}

bool TGroupListWindow::AddGroup(TGroup *)
{
	return false;
}

bool TGroupListWindow::EditGroup(TGroup *)
{
	return false;
}

bool TGroupListWindow::AskDelete(TGroup *)
{
	return ask("Желаете ли да изтриете избраният елемент?");
}

bool TGroupListWindow::DeleteGroup(TGroup *group)
{
	return group->Delete(true);
}

void TGroupListWindow::Refresh()
{
	DrawLockCount();
	TGroupList::Refresh();
}

void TGroupListWindow::RedrawGroup()
{
	int index = GetVisualIndex();

#if RANGECHECK
	if (index < 0)
		fatal("TGroupList::RedrawGroup");
#endif  // RANGECHECK

	RedrawVisual(index);
}

void TGroupListWindow::RemoveGroup(int command)
{
	TGroupList::RemoveGroup();
	UnblockFocus(command);
	UpdateVisual();
}

void TGroupListWindow::DrawItemAt(int index, bool insert)
{
	DrawGroupAt(GroupAt(index), index, insert);

	if (drawLockCount > 0 && !--drawLockCount)
	{
		UpdateWindow();
		SetWaitCursor(true);
		SetRedraw(false);
	}
}

bool TGroupListWindow::EmptyRect()
{
	if (emptyRect.flag && !array->Count())
	{
		emptyRect.right = emptyRect.left + initialWidth - 1;
		return true;
	}

	return false;
}

#include "ListWindowHeader.h"
#include "ListWindowHeader.cpp"

void TGroupListWindow::SetupWindow()
{
	TColumnListWindow::SetupWindow();

	HWND hWnd = (HWND) SendMessage(LVM_GETHEADER);

	if (hWnd)
	{
		SetWaitCursor(false);
		new TListWindowHeader(hWnd);
	}

#if RANGECHECK
	if (columns.Count() < 1)
		fatal("TGLW: no columns");
#endif  // RANGECHECK

	emptyRect = GetStyle() & LVS_NOCOLUMNHEADER ? LISTW_NOHEADS : LISTW_HEADERS;
	AddColumns(initialWidth);

	if (initialRefresh)
		Refresh();
}

void TGroupListWindow::EvPaint()
{
	bool fakeRect = false;
	PAINTSTRUCT ps;

	if (EmptyRect() && GetFocus() == HWindow)
	{
		fakeRect = true;
		ps = TPaintDC(*this).Ps;
		InvalidateRect(ps.rcPaint, ps.fErase);
	}

	TColumnListWindow::EvPaint();

	if (fakeRect)
	{
		InvalidateRect(ps.rcPaint, ps.fErase);
		TPaintDC dc(*this);
		dc.DrawFocusRect(emptyRect.left, emptyRect.top, emptyRect.right, emptyRect.bottom);
	}
}

void TGroupListWindow::EvChar(uint key, uint repeatCount, uint flags)
{
	if (!QuickChar(key))
		TColumnListWindow::EvChar(key, repeatCount, flags);
}

void TGroupListWindow::EvKeyDown(uint key, uint repeatCount, uint flags)
{
	if (key == VK_INSERT)
		ParentNotify(IDADD);
	else if (!QuickKey(key))
		TColumnListWindow::EvKeyDown(key, repeatCount, flags);
}

void TGroupListWindow::EvSetFocus(THandle hWndLostFocus)
{
	if (array->Count() && GetSelIndex(LVIS_FOCUSED) < 0)
	{
		allowDeselect = true;
		SetItemState(0, LVIS_FOCUSED, LVIS_FOCUSED);
		allowDeselect = false;
	}
	else if (EmptyRect())
		InvalidateRect(emptyRect, true);

	TColumnListWindow::EvSetFocus(hWndLostFocus);
	Parent->SendMessage(WM_FOCUS_FACE, TRUE, Attr.Id);
	QuickSetFocus();
}

void TGroupListWindow::EvKillFocus(THandle hWndGetFocus)
{
	QuickKillFocus();

	if (EmptyRect())
		InvalidateRect(emptyRect, true);

	TColumnListWindow::EvKillFocus(hWndGetFocus);
	Parent->SendMessage(WM_FOCUS_FACE, FALSE, Attr.Id);
}

void TGroupListWindow::EvLButtonDblClk(uint modKeys, TPoint& point)
{
	TColumnListWindow::EvLButtonDblClk(modKeys, point);

	uint defId = LOWORD(Parent->SendMessage(DM_GETDEFID));

	if (defId && HitTest(TLwHitTestInfo(point)) >= 0)
		ParentNotify(defId);
}
