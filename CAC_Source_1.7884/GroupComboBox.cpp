#include "cac.h"

DEFINE_RESPONSE_TABLE1(TGroupComboBox, TComboBox)
//{{TGroupComboBoxRSP_TBL_BEGIN}}
	EV_WM_CHAR,
	EV_WM_KEYDOWN,
	EV_WM_SETFOCUS,
	EV_WM_KILLFOCUS,
	EV_NOTIFY_AT_CHILD(CBN_DROPDOWN, CBNDropDown),
	EV_NOTIFY_AT_CHILD(CBN_CLOSEUP, CBNCloseUp),
//{{TGroupComboBoxRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TGroupComboBox Implementation}}

TGroupComboBox::TGroupComboBox(TWindow* parent, int resourceId, TGroupArray *array, int size)
:
	TComboBox(parent, resourceId, size), TGroupList(parent, array), autoEnable(false)
{
}

TGroupComboBox::TGroupComboBox(TWindow* parent, int resourceId, TGroupArray &array, int size)
:
	TComboBox(parent, resourceId, size), TGroupList(parent, array), autoEnable(false)
{
}

TGroupComboBox::~TGroupComboBox()
{
}

int TGroupComboBox::GetVisualIndex()
{
	return GetSelIndex();
}

void TGroupComboBox::SetVisualIndex(int index)
{
	SetSelIndex(index);
}

char *TGroupComboBox::GetVisualText(int index, int column)
{
	char *buffer = new char[GetStringLen(index) + 1];

	GetString(buffer, index);
	strmove(buffer, buffer + column);
	return buffer;
}

void TGroupComboBox::FlushVisuals()
{
	ClearList();
	SetVisualIndex(-1);
}

void TGroupComboBox::InsertVisual(int index)
{
	mstr m;
	GetGroupText(m, GroupAt(index), 0);
	InsertString(str(m), index);
}

void TGroupComboBox::RemoveVisual(int index)
{
	DeleteString(index);
}

void TGroupComboBox::QuickPosition()
{
	TRect list = GetWindowRect();
	TPoint origin(list.left, list.bottom);
	Parent->ScreenToClient(origin);

	for (int i = 0; i < QUICK_SEARCH_CONTROLS; i++)
	{
		TRect move = qs->controls[i]->GetWindowRect();
		qs->controls[i]->MoveWindow(origin.x, origin.y, move.Width(), move.Height());
		origin.x += move.Width();
	}
}

void TGroupComboBox::Refresh()
{
	TGroupList::Refresh();

	if (autoEnable)
		EnableWindow(array->Count() >= 2);
}

void TGroupComboBox::SetupWindow()
{
	TComboBox::SetupWindow();

	if (IsWindowEnabled())
		autoEnable = true;

	if (initialRefresh)
		Refresh();
}

void TGroupComboBox::EvChar(uint key, uint repeatCount, uint flags)
{
	if (QuickChar(key))
		Parent->PostMessage(WM_COMMAND, MAKEWPARAM(Attr.Id, CBN_SELCHANGE), (LPARAM) Handle);
	else
		TComboBox::EvChar(key, repeatCount, flags);
}

void TGroupComboBox::EvKeyDown(uint key, uint repeatCount, uint flags)
{
	if (QuickKey(key))
		Parent->PostMessage(WM_COMMAND, MAKEWPARAM(Attr.Id, CBN_SELCHANGE), (LPARAM) Handle);
	else
		TComboBox::EvKeyDown(key, repeatCount, flags);
}

void TGroupComboBox::EvSetFocus(THandle hWndLostFocus)
{
	TComboBox::EvSetFocus(hWndLostFocus);
	QuickSetFocus();
}

void TGroupComboBox::EvKillFocus(THandle hWndGetFocus)
{
	QuickKillFocus();
	TComboBox::EvKillFocus(hWndGetFocus);
}

void TGroupComboBox::CBNDropDown()
{
	if (qs)
		qs->Disable();
}

void TGroupComboBox::CBNCloseUp()
{
	if (qs)
		qs->ReShow();
}
