DEFINE_RESPONSE_TABLE1(TElectricPushButton, TPushButton)
//{{TElectricPushButtonRSP_TBL_BEGIN}}
	EV_WM_KEYUP,
	EV_WM_KEYDOWN,
	EV_WM_SYSKEYUP,
	EV_WM_SYSKEYDOWN,
//{{TElectricPushButtonRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TElectricPushButton Implementation}}

TElectricPushButton::TElectricPushButton(TScaleDialog *parent, int resourceId)
:
	TPushButton(parent, resourceId)
{
}

void TElectricPushButton::EvKeyUp(uint key, uint repeatCount, uint flags)
{
	TPushButton::EvKeyUp(key, repeatCount, flags);

	if (key >= VK_F1 && key <= VK_F12)
		AppMainWindow->PostMessage(WM_KEYUP, key, repeatCount + (flags << 16));
}

void TElectricPushButton::EvKeyDown(uint key, uint repeatCount, uint flags)
{
	TPushButton::EvKeyDown(key, repeatCount, flags);

	if (key >= VK_F1 && key <= VK_F12)
		AppMainWindow->PostMessage(WM_KEYDOWN, key, repeatCount + (flags << 16));
}

void TElectricPushButton::EvSysKeyUp(uint key, uint repeatCount, uint flags)
{
	TPushButton::EvSysKeyUp(key, repeatCount, flags);

	if (key >= VK_F1 && key <= VK_F12)
		AppMainWindow->PostMessage(WM_SYSKEYUP, key, repeatCount + (flags << 16));
}

void TElectricPushButton::EvSysKeyDown(uint key, uint repeatCount, uint flags)
{
	TPushButton::EvSysKeyDown(key, repeatCount, flags);

	if (key >= VK_F1 && key <= VK_F12)
		AppMainWindow->PostMessage(WM_SYSKEYDOWN, key, repeatCount + (flags << 16));
}
