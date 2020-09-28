#include "cac.h"

static void window_show_functionals(TWindow *parent, int cmdShow)
{
	while (parent && parent != AppMainWindow && electric_foreach(ELECTRIC_VERIFY, parent) != -1)
	{
		parent->ShowWindow(cmdShow);
		parent = parent->Parent;
	}
}

void window_show_functionals(TWindow *parent)
{
	window_show_functionals(parent, SW_SHOW);
}

void window_hide_functionals(TWindow *parent)
{
	window_show_functionals(parent, SW_HIDE);
}

void window_show_permanents(UINT mainSW, HWND active)
{
	AppMainWindow->ShowWindow(mainSW);
	electric_foreach(ELECTRIC_SHOW, VOID_TRUE);
	::SetActiveWindow(active);
	::SetForegroundWindow(active);	// 2017:199
}

UINT window_hide_permanents()
{
	WINDOWPLACEMENT place;
	UINT mainSW = AppMainWindow->GetWindowPlacement(&place) ? place.showCmd : SW_RESTORE;

	AppMainWindow->ShowWindow(SW_HIDE);
	electric_foreach(ELECTRIC_SHOW, VOID_FALSE);
	return mainSW;
}

void window_move_upright_adjusted(TWindow *window)
{
	TRect rect = window->GetWindowRect();
	// 2017:198 FIX: AdjustedRect.left
	//rect.MoveTo(AdjustedRect.right - rect.Width(), AdjustedRect.top);
	rect.MoveTo(AdjustedRect.right - rect.Width() + AdjustedRect.left, AdjustedRect.top);
	window->MoveWindow(rect);
}

bool window_handle_color(TNotify far& info, uint start, uint final)
{
	if (info.code == NM_CUSTOMDRAW && info.idFrom >= start && info.idFrom <= final)
	{
		NMCUSTOMDRAW &draw = (NMCUSTOMDRAW &) info;

		if (draw.dwDrawStage == CDDS_PREPAINT)
			return true;

		if (draw.dwDrawStage == CDDS_POSTPAINT)
			::SendMessage(info.hwndFrom, WM_DRAW_FACE, 0, (LPARAM) draw.hdc);
	}

	return false;
}

// ----- TWindowHideParent ---------------------------------------------------
TWindowHideParent::TWindowHideParent(TWindow *tParent)
	: parent(tParent)
{
	if (parent != AppMainWindow)
		parent->ShowWindow(SW_HIDE);
}

TWindowHideParent::~TWindowHideParent()
{
	if (parent != AppMainWindow)
		parent->ShowWindow(SW_SHOW);
}

// ----- TWindowHideAlls -----------------------------------------------------
TWindowHideAlls::TWindowHideAlls(TWindow *tParent)
	: parent(tParent)
{
	window_hide_functionals(parent);
	mainSW = window_hide_permanents();
}

TWindowHideAlls::~TWindowHideAlls()
{
	window_show_functionals(parent);
	window_show_permanents(mainSW, parent->HWindow);
}
