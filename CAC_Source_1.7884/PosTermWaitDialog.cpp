//{{TPosTermWaitDialog Implementation}}

TPosTermWaitDialog::TPosTermWaitDialog(TWindow* parent, int resId)
:
	TScaleDialog(parent, resId), allowCancel(false), canceled(false)
{
	text = new TStatic(this, IDC_POSTERM_WAIT_TEXT);
}

void TPosTermWaitDialog::SetText(const char *tText)
{
	text->SetText(tText);
	PumpWaitingMessages();
}

bool TPosTermWaitDialog::Canceled()
{
	PumpWaitingMessages();
	return canceled;
}

void TPosTermWaitDialog::PumpWaitingMessages()
{
	MSG msg;

	while (::PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
	{
		if (msg.message == WM_KEYDOWN)
		{
			if (allowCancel && (msg.wParam == VK_F6 && (GetAsyncKeyState(VK_CONTROL) &
				GetAsyncKeyState(VK_MENU) & 0x01)))
			{
				canceled = true;
			}
		}
		else
			GetApplicationObject()->ProcessMsg(msg);
	}
}
