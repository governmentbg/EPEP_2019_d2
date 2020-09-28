#include "cac.h"

DEFINE_RESPONSE_TABLE1(TResizeListWindow, TListWindow)
//{{TResizeListWindowRSP_TBL_BEGIN}}
	EV_WM_SIZE,
//{{TResizeListWindowRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TResizeListWindow Implementation}}

TResizeListWindow::TResizeListWindow(TWindow* parent, int resourceId)
:
	TListWindow(parent, resourceId)
{
}

void TResizeListWindow::SetupWindow()
{
	TListWindow::SetupWindow();

	initialWidth = GetClientRect().Width();
	for (int i = 0; i < 10; i++)
		sizedAtWidth[i] = initialWidth;
}

void TResizeListWindow::EvSize(uint sizeType, TSize& size)
{
	int currentWidth = GetClientRect().Width();

	for (int i = 0; i < 10; i++)
	{
		int old_width = GetColumnWidth(i);

		if (old_width > 0)
		{
			int new_width = uround(old_width * (double) currentWidth / sizedAtWidth[i]);

			if (new_width > 0 && new_width != old_width)
			{
				// Do not exchange these! If you do, BCC will over-
				// optimize and skip the last sizedAtWidth assign!
				sizedAtWidth[i] = currentWidth;
				SetColumnWidth(i, new_width);
			}
		}
	}

	TListWindow::EvSize(sizeType, size);
}
