DEFINE_RESPONSE_TABLE1(TColorButton, TButton)
//{{TColorButtonRSP_TBL_BEGIN}}
	EV_MESSAGE(WM_DRAW_FACE, EvDrawFace),
	EV_NOTIFY_AT_CHILD(BN_SETFOCUS, EvSetFocus),
	EV_NOTIFY_AT_CHILD(BN_KILLFOCUS, EvSetFocus),
//{{TColorButtonRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TColorButton Implementation}}

TColorButton::TColorButton(TWindow* parent, int resourceId, COLORREF tColor)
:
	TButton(parent, resourceId), color(tColor), clicked(false)
{
}

LRESULT TColorButton::EvDrawFace(WPARAM, LPARAM lParam)
{
	if (Styled())
	{
	#if 20170236
		TDC dc((HDC) lParam);
		TRect rect(TPoint(0, 0), TSize(Attr.W, Attr.H));

		if (SendMessage(BM_GETSTATE) & 0x04)
			rect.Offset(1, 1);

		dc.SetBkColor(COLOR_3D_BACKGROUND);
		dc.SetTextColor(color);
		dc.DrawText(Title, -1, rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
	#else  // 20170236
		HDC hDC = (HDC) lParam;
		TRect rect(TPoint(0, 0), TSize(Attr.W, Attr.H));

		if (SendMessage(BM_GETSTATE) & 0x04)
			rect.Offset(1, 1);

		SetBkColor(hDC, COLOR_3D_BACKGROUND);
		SetTextColor(hDC, color);
		DrawText(hDC, Title, -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
	#endif  // 20170236
	}

	return MW_FACE_DRAWN;
}

void TColorButton::EvSetFocus()
{
	if (Styled())
		Invalidate();
}

void TColorButton::EvKillFocus()
{
	if (Styled())
		Invalidate();
}
