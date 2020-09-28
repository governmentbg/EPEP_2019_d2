#include "cac.h"

// ----- TQuickSearchEdit ----------------------------------------------------
DEFINE_RESPONSE_TABLE1(TQuickSearchEdit, TEdit)
	EV_WM_LBUTTONDOWN,
	EV_WM_LBUTTONDBLCLK,
	EV_WM_RBUTTONDOWN,
	EV_WM_MOUSEMOVE,
	EV_WM_SETCURSOR,
END_RESPONSE_TABLE;

TQuickSearchEdit::TQuickSearchEdit(TWindow *parent, int resourceId, bool tLeftButton)
	: TEdit(parent, resourceId, SIZE_OF_QUICK_SEARCH_TEXT), leftButton(tLeftButton)
{
}

void TQuickSearchEdit::PostClicked()
{
	Parent->PostMessage(WM_COMMAND, MAKEWPARAM(Attr.Id, BN_CLICKED), (LPARAM) Handle);
}

void TQuickSearchEdit::EvLButtonDown(uint, TPoint&)
{
	if (leftButton)
		PostClicked();
}

void TQuickSearchEdit::EvLButtonDblClk(uint, TPoint&)
{
	if (leftButton)
		PostClicked();
}

void TQuickSearchEdit::EvRButtonDown(uint, TPoint&)
{
	if (!leftButton)
		PostClicked();
}

void TQuickSearchEdit::EvMouseMove(uint, TPoint &)
{
}

bool TQuickSearchEdit::EvSetCursor(THandle, uint, uint)
{
	return true;
}
