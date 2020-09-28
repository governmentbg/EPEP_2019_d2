#include "cac.h"

DEFINE_RESPONSE_TABLE1(TGroupListBox, TListBox)
//{{TGroupListBoxRSP_TBL_BEGIN}}
	EV_WM_CHAR,
	EV_WM_KEYDOWN,
	EV_WM_SETFOCUS,
	EV_WM_KILLFOCUS,
	EV_WM_LBUTTONDOWN,
	EV_WM_LBUTTONUP,
	EV_WM_LBUTTONDBLCLK,
	EV_WM_RBUTTONDOWN,
	EV_WM_RBUTTONUP,
	EV_WM_RBUTTONDBLCLK,
	EV_MESSAGE(WM_FACE_VALID, EvFaceValid),
//{{TGroupListBoxRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TGroupListBox Implementation}}

TGroupListBox::TGroupListBox(TWindow* parent, int resourceId, TGroupArray *array)
:
	TListBox(parent, resourceId), TGroupList(parent, array)
{
}

TGroupListBox::TGroupListBox(TWindow* parent, int resourceId, TGroupArray &array)
:
	TListBox(parent, resourceId), TGroupList(parent, array)
{
}

#if TYPECHECK
TGroup *TGroupListBox::Group() const
{
	if (multiSelect)
		fatal("multi select listbox Group() called");

	return TGroupList::Group();
}
#endif  // TYPECHECK

void TGroupListBox::GetMark(TGroupArray *tArray)
{
	tArray->Flush();

	for (int index = 0; index < array->Count(); index++)
		if (GetSel(index))
			tArray->AddCopy(GroupAt(index));
}

void TGroupListBox::CutMark()
{
#if RANGECHECK
	if (dynamic)
		fatal("CutMark() called for dynamic array");
#endif  // RANGECHECK

	int point = 0;

	for (int index = 0; index < GetCount(); index++)
	{
		if (GetSel(index))
			point++;
		else
			array->Remove(point);
	}
}

void TGroupListBox::SelectAll(bool select)
{
#if TYPECHECK
	if (!multiSelect)
		fatal("single select listbox SelectAll() called");
#endif  // TYPECHECK

	for (int index = 0; index < array->Count(); index++)
		SetSel(index, select);
}

int TGroupListBox::GetVisualIndex()
{
	return multiSelect ? GetCaretIndex() : GetSelIndex();
}

void TGroupListBox::SetVisualIndex(int index)
{
	if (multiSelect)
		SetCaretIndex(index, false);
	else
		SetSelIndex(index);
}

char *TGroupListBox::GetVisualText(int index, int column)
{
	char *buffer = new char[GetStringLen(index) + 1];

	GetString(buffer, index);
	strmove(buffer, buffer + column);
	return buffer;
}

void TGroupListBox::FlushVisuals()
{
	ClearList();
	SetVisualIndex(-1);
}

void TGroupListBox::InsertVisual(int index)
{
	mstr m; 
	GetGroupText(m, GroupAt(index), 0);
	InsertString(str(m), index);
}

void TGroupListBox::RemoveVisual(int index)
{
	DeleteString(index);
}

void TGroupListBox::UpdateVisual()
{
	int width = get_list_box_maxc_width();

	if (width > 0)
	{
		SetHorizontalExtent(width);
		if (multiColumn)
			SetColumnWidth(width);
	}
}

bool TGroupListBox::IsValid()
{
	return GetSelCount() <= MARKEDS_MAX ? true : error("Избрани са повече от %d елемента.\n", MARKEDS_MAX);
}

bool TGroupListBox::Inside(TPoint &point)
{
	int index = SendMessage(LB_ITEMFROMPOINT, 0, MAKELPARAM(point.x, point.y));
	return index >= 0 && index < GetCount();
}

void TGroupListBox::LButton(TPoint &point)
{
	if (Inside(point))
		DefaultProcessing();
	else
		SetFocus();
}

int TGroupListBox::get_list_box_maxc_width()
{
	int extent = 0;
	HDC dC = GetDC(HWindow);

	if (dC)
	{
		TDC DC(dC);
		HFONT hFont;	// 2005:116 LPR

		if ((hFont = (HFONT) SendMessage(WM_GETFONT)) != NULL)
		{
			// 2011:083 LPR: former CharPtr
			int size = 0x100;
			char *s = new char[size];

			DC.SelectObject(TFont(hFont));

			for (int i = 0; i < GetCount(); i++)
			{
				int len = GetStringLen(i);

				if (len > 0)
				{
					int tExtent;

					if (len >= size)
					{
						size = len + 1;
						delete[] s;
						s = new char[size];
					}

					if (GetString(s, i) >= 0 && (tExtent = DC.GetTextExtent(s, len).X()) > extent)
						extent = tExtent;
				}
			}

			// 2005:116 LPR
			if (extent > 0)
				extent += GetWindowRect().Width() - GetClientRect().Width();

			delete[] s;
			// 2005:116 LPR
			DC.RestoreFont();
		}
	}

	return extent;
}

void TGroupListBox::SetupWindow()
{
	TListBox::SetupWindow();

	multiSelect = (GetStyle() & (LBS_MULTIPLESEL | LBS_EXTENDEDSEL)) == LBS_MULTIPLESEL;
	multiColumn = GetStyle() & LBS_MULTICOLUMN;

	if (initialRefresh)
		Refresh();
}

void TGroupListBox::EvChar(uint key, uint repeatCount, uint flags)
{
	if (QuickChar(key))
		Parent->PostMessage(WM_COMMAND, MAKEWPARAM(Attr.Id, LBN_SELCHANGE), (LPARAM) Handle);
	else
		TListBox::EvChar(key, repeatCount, flags);
}

void TGroupListBox::EvKeyDown(uint key, uint repeatCount, uint flags)
{
	if (!QuickKey(key))
		TListBox::EvKeyDown(key, repeatCount, flags);
}

void TGroupListBox::EvSetFocus(THandle hWndLostFocus)
{
	TListBox::EvSetFocus(hWndLostFocus);
	QuickSetFocus();
}

void TGroupListBox::EvKillFocus(THandle hWndGetFocus)
{
	QuickKillFocus();
	TListBox::EvKillFocus(hWndGetFocus);
}

void TGroupListBox::EvLButtonDown(uint, TPoint &point)
{
	LButton(point);
}

void TGroupListBox::EvLButtonUp(uint, TPoint &point)
{
	LButton(point);
}

void TGroupListBox::EvLButtonDblClk(uint, TPoint &point)
{
	LButton(point);
}

void TGroupListBox::EvRButtonDown(uint modKeys, TPoint &point)
{
	int start = GetCaretIndex();

	if (start < 0)
		start = 0;

	if (multiSelect)
	{
		SendMessage(WM_LBUTTONDOWN, modKeys, MAKELONG(point.x, point.y));
		int final = GetCaretIndex();

		if (final >= 0)
		{
			int top = GetTopIndex();

			// 2010:336 LPR: separate < > to fix scroll
			// 2011:087 LPR: rejected SetSelItemRange()
			if (start < final)
			{
				for (int index = start + 1; index < final; index++)
					SetSel(index, !GetSel(index));
			}
			else
			{
				for (int index = start - 1; index > final; index--)
					SetSel(index, !GetSel(index));
			}
			SetCaretIndex(final, false);

			// 2010:336 LPR: to fix scroll
			if (top >= 0)
				SetTopIndex(top);
		}
	}
}

void TGroupListBox::EvRButtonUp(uint modKeys, TPoint &point)
{
	if (multiSelect)
		SendMessage(WM_LBUTTONUP, modKeys, MAKELONG(point.x, point.y));
}

void TGroupListBox::EvRButtonDblClk(uint modKeys, TPoint &point)
{
	if (multiSelect)
		SendMessage(WM_LBUTTONDBLCLK, modKeys, MAKELONG(point.x, point.y));
}

long TGroupListBox::EvFaceValid(TParam1, TParam2)
{
	return IsValid() ? 0 : MW_INVALID_FACE;
}
