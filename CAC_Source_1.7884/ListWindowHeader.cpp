DEFINE_RESPONSE_TABLE1(TListWindowHeader, TWindow)
//{{TListWindowHeaderRSP_TBL_BEGIN}}
	EV_WM_LBUTTONDOWN,
	EV_WM_LBUTTONDBLCLK,
	EV_WM_MOUSEMOVE,
	EV_WM_SETCURSOR,
//{{TListWindowHeaderRSP_TBL_END}}
END_RESPONSE_TABLE;


TListWindowHeader::TListWindowHeader(THandle handle)
	: TWindow(handle)
{
}

// XP and later: HDS_NOSIZING 
void TListWindowHeader::EvLButtonDown(uint, TPoint &)
{
}

void TListWindowHeader::EvLButtonDblClk(uint, TPoint &)
{
}

void TListWindowHeader::EvMouseMove(uint, TPoint &)
{
}

bool TListWindowHeader::EvSetCursor(THandle, uint, uint)
{
	return true;
}
