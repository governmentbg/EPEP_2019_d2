#include "cac.h"

DEFINE_RESPONSE_TABLE1(TTwinButton, TPushButton)
//{{TTwinButtonRSP_TBL_BEGIN}}
	EV_WM_KEYDOWN,
	EV_WM_KEYUP,
	EV_WM_LBUTTONDOWN,
	EV_WM_LBUTTONUP,
	EV_WM_RBUTTONDOWN,
	EV_WM_RBUTTONUP,
	EV_WM_MOUSEMOVE,
//{{TTwinButtonRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TTwinButton Implementation}}

TTwinButton::TTwinButton(TScaleDialog *parent, int resourceId, int tSecondId, int iconId)
:
	TPushButton(parent, resourceId, iconId), secondId(tSecondId), right(false), grab(GRAB_NONE)
{
}

void TTwinButton::SetState(BOOL pushed)
{
	if (pushed != ((SendMessage(BM_GETSTATE) >> 2) & 0x01))
		SendMessage(BM_SETSTATE, pushed, 0);
}

void TTwinButton::SetCaption(const char *text)
{
	mstr m;
	m.printf(CHROME_PUSH_IMAGE || strchr(text, '"') ? "%s" : "(\") %s   ", text);
	TPushButton::SetCaption(str(m));
}

void TTwinButton::SetupWindow()
{
	TPushButton::SetupWindow();
	SetCaption(Title);
}

void TTwinButton::EvKeyDown(uint key, uint repeatCount, uint flags)
{
	TPushButton::EvKeyDown(key, repeatCount, flags);

	grab = (GRAB) (key == VK_INSERT);

	if (grab)
	{
		SetState(true);
		grab = GRAB_KEY;
	}
}

void TTwinButton::EvKeyUp(uint key, uint repeatCount, uint flags)
{
	TPushButton::EvKeyUp(key, repeatCount, flags);

	if (key == VK_INSERT)
	{
		SetState(false);
		if (grab == GRAB_KEY)
			Parent->PostMessage(WM_COMMAND, MAKEWPARAM(secondId, BN_CLICKED), (LPARAM) Handle);
		grab = GRAB_NONE;
	}
}

void TTwinButton::EvLButtonDown(uint modKeys, TPoint& point)
{
	if ((modKeys & (MK_LBUTTON | MK_RBUTTON)) == MK_LBUTTON)
	{
		TPushButton::EvLButtonDown(modKeys, point);
		right = false;
	}
}

void TTwinButton::EvLButtonUp(uint modKeys, TPoint& point)
{
	if (!right)
		TPushButton::EvLButtonUp(modKeys, point);
}

void TTwinButton::EvRButtonDown(uint modKeys, TPoint& point)
{
	if ((modKeys & (MK_LBUTTON | MK_RBUTTON)) == MK_RBUTTON)
	{
		TPushButton::EvRButtonDown(modKeys, point);

		right = true;
		SetState(true);
		SetCapture();
		grab = GRAB_MOUSE;
	}
}

void TTwinButton::EvRButtonUp(uint modKeys, TPoint& point)
{
	if (right)
	{
		TPushButton::EvRButtonUp(modKeys, point);

		SetState(false);
		ReleaseCapture();
		if (grab == GRAB_MOUSE && Inside(point))
			Parent->PostMessage(WM_COMMAND, MAKEWPARAM(secondId, BN_CLICKED), (LPARAM) Handle);
		grab = GRAB_NONE;
	}
}

void TTwinButton::EvMouseMove(uint modKeys, TPoint& point)
{
	TPushButton::EvMouseMove(modKeys, point);

	if (GetCapture() == Handle)
		SetState(Inside(point));
}
