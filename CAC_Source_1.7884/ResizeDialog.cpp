#include "cac.h"

DEFINE_RESPONSE_TABLE1(TResizeDialog, TResourceDialog)
//{{TResizeDialogRSP_TBL_BEGIN}}
	EV_WM_SIZE,
	EV_WM_SIZING,
//{{TResizeDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TResizeDialog Implementation}}

TResizeDialog::TResizeDialog(TWindow* parent, int tShowMax, int resId)
:
	TResourceDialog(parent, showMax == SHOW_SCALED ? 1 : -1, resId), showMax(tShowMax), autoSize(NULL)
{
	memset(autoDown, '\0', sizeof autoDown);
}

void TResizeDialog::Resize(TWindow *window, int resize)
{
	TRect rect = window->GetWindowRect();
	TPoint point(rect.left, rect.top);
	TSize size(rect.Width(), rect.Height());

	ScreenToClient(point);

	if (resize & RESIZE_WIDTH)
		size.cx += dx;
	if (resize & RESIZE_RIGHT)
		point.x += dx;
	if (resize & RESIZE_HEIGHT)
		size.cy += dy;
	if (resize & RESIZE_DOWN)
		point.y += dy;

	// 2016:099 JRQ/FIX: | SWP_NOCOPYBITS to avoid artifacts
	window->SetWindowPos(NULL, point.x, point.y, size.cx, size.cy, SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOCOPYBITS);
}

void TResizeDialog::Resize()
{
	for (int i = 0; i < DOWN_CONTROLS_MAX; i++)
		if (autoDown[i])
			Resize(autoDown[i], RESIZE_DOWN);

	if (autoSize)
		Resize(autoSize, RESIZE_SIZE);
}

void TResizeDialog::SetupWindow()
{
	TResourceDialog::SetupWindow();

	TRect window = GetWindowRect();
	TRect client = GetClientRect();

	minWidth = window.Width();
	minHeight = window.Height();

	lastWidth = client.Width();
	lastHeight = client.Height();

	if (showMax == SHOW_ADJUSTED)
		MoveWindow(AdjustedRect);
	else if (showMax == SHOW_MAXIMIZED)
		ShowWindow(SW_MAXIMIZE);
	else
	{
		TRect rect = GetWindowRect();

		if (showMax == SHOW_LARGER)
		{
			rect.left /= 3;
			rect.right += rect.left * 2;
			rect.top /= 2;
			rect.bottom += rect.top;
		}

		MoveWindow(rect);
	}
}

void TResizeDialog::EvSize(uint sizeType, TSize& size)
{
	TResourceDialog::EvSize(sizeType, size);

	dx = size.cx - lastWidth;
	dy = size.cy - lastHeight;
	Resize();
	lastWidth = size.cx;
	lastHeight = size.cy;
}

bool TResizeDialog::EvSizing(uint side, TRect& rect)
{
	if (rect.Height() < minHeight)
	{
		if (side == WMSZ_TOP || side == WMSZ_TOPLEFT || side == WMSZ_TOPRIGHT)
			rect.top = rect.bottom - minHeight;
		else if (side == WMSZ_BOTTOM || side == WMSZ_BOTTOMLEFT || side == WMSZ_BOTTOMRIGHT)
			rect.bottom = rect.top + minHeight;
	}

	if (rect.Width() < minWidth)
	{
		if (side == WMSZ_LEFT || side == WMSZ_TOPLEFT || side == WMSZ_BOTTOMLEFT)
			rect.left = rect.right - minWidth;
		else if (side == WMSZ_RIGHT || side == WMSZ_TOPRIGHT || side == WMSZ_BOTTOMRIGHT)
			rect.right = rect.left + minWidth;
	}

	return true;
}
