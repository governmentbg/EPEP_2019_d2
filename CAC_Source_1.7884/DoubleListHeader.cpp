#include "cac.h"

DEFINE_RESPONSE_TABLE1(TDoubleListHeader, TWindow)
//{{TDoubleListHeaderRSP_TBL_BEGIN}}
	EV_WM_SETCURSOR,
//{{TDoubleListHeaderRSP_TBL_END}}
END_RESPONSE_TABLE;

TDoubleListHeader::TDoubleListHeader(THandle handle)
	: TWindow(handle)
{
	cursor = ::LoadCursor(NULL, IDC_ARROW);
}

#include <owl/colmnhdr.h>

bool TDoubleListHeader::EvSetCursor(THandle hWndCursor, uint hitTest, uint mouseMsg)
{
	TPoint point;

	GetCursorPos(point);
	ScreenToClient(point);

	if (point.x < 16 && GetCursor() == cursor)
	{
		THdrItem hdrItem = { HDI_WIDTH };

		if (SendMessage(HDM_GETITEM, 1, (LPARAM) &hdrItem) && abs(hdrItem.cxy - point.x) >= 8)
			return true;
	}

	return TWindow::EvSetCursor(hWndCursor, hitTest, mouseMsg);
}
