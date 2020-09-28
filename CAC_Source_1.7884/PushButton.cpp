#include "cac.h"

DEFINE_RESPONSE_TABLE1(TPushButton, TButton)
//{{TPushButtonRSP_TBL_BEGIN}}
	EV_MESSAGE(WM_DRAW_FACE, EvDrawFace),
	EV_NOTIFY_AT_CHILD(BN_SETFOCUS, EvSetFocus),
	EV_NOTIFY_AT_CHILD(BN_KILLFOCUS, EvSetFocus),
//{{TPushButtonRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TPushButton Implementation}}

TPushButton::TPushButton(TScaleDialog *parent, int resourceId, int tIconId)
:
	TButton(parent, resourceId), iconId(tIconId), icon(NULL), icon1(NULL), lines(1), drawIcon(true)
{
}

TPushButton::~TPushButton()
{
	delete icon;
	delete icon1;
}

static bool PushImage(HWND hWnd, int dlgId)
{
	return (dlgId >= IDD_FIRST_SORTED && dlgId < IDD_FIRST_UNSORTED) || (::GetWindowLong(hWnd, GWL_STYLE) & WS_GROUP);
}

void TPushButton::SetupWindow()
{
	TButton::SetupWindow();

	if (iconId == -1)
	{
		if (Attr.Id == IDOK)
			iconId = IDI_OK;
		else if (Attr.Id == IDCANCEL)
			iconId = IDI_CANCEL;	// see ScaleDialog too
		else if (Attr.Id >= IDSTART && Attr.Id <= IDFINAL)
			iconId = Attr.Id;
		else
			iconId = 0;
	}

	int dlgId = (dynamic_cast<TScaleDialog *> (Parent))->dlgId;

	if (CHROME_PUSH_IMAGE && iconId && PushImage(HWindow, dlgId))
	{
		icon = new TIcon(*GetModule(), TResId(iconId));
		icon1 = new TIcon(*GetModule(), TResId(iconId + 100));
	}
}

void TPushButton::SetLines(int tLines)
{
	if (tLines >= 1 && tLines <= 10)
		lines = tLines;
}

LRESULT TPushButton::EvDrawFace(WPARAM, LPARAM lParam)
{
	if (icon && drawIcon)
	{
		TRect rect(TPoint(FONT_SCALE ? FONT_SCALE - 6 : 2, ((Attr.H - 12) / lines - 16) / 2 + 6), TSize(16, 16));

		if (SendMessage(BM_GETSTATE) & 0x04)
			rect.Offset(1, 1);

		DrawIconEx((HDC) lParam, rect.left, rect.top, IsWindowEnabled() ? *icon : *icon1, 16, 16, 0, NULL, DI_NORMAL);
	}

	return MW_FACE_DRAWN;
}

void TPushButton::EvSetFocus()
{
	Invalidate();
}

void TPushButton::EvKillFocus()
{
	Invalidate();
}
