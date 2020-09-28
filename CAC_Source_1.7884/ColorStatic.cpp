DEFINE_RESPONSE_TABLE1(TColorStatic, TStatic)
//{{TColorStaticFaceRSP_TBL_BEGIN}}
	EV_WM_PAINT,
//{{TColorStaticFaceRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TColorStatic Implementation}}

TColorStatic::TColorStatic(TWindow* parent, int resourceId, const char *tText, COLORREF tColor)
:
	TStatic(parent, resourceId, 0), text(tText), color(tColor)
{
}

bool TColorStatic::ShowWindow(int cmdShow)
{
	Invalidate();
	return TStatic::ShowWindow(cmdShow);
}

void TColorStatic::EvPaint()
{
	TStatic::EvPaint();

	TRect rect(TPoint(0, 0), TSize(Attr.W, Attr.H));
	InvalidateRect(rect, false);
	TPaintDC dc(HWindow);

	dc.FillRect(rect, BRUSH_3D_BACKGROUND);
	dc.SetBkColor(COLOR_3D_BACKGROUND);
	dc.SetTextColor(color);

	HFONT hFont = (HFONT) SendMessage(WM_GETFONT);

	if (hFont)
		dc.SelectObject(hFont);

	dc.DrawText(text, -1, rect, (uint16) (GetStyle() & (SS_LEFT | SS_RIGHT | SS_CENTER)));
}
