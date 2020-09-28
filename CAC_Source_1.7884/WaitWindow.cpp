#include "cac.h"

// ----- TWaitDialog ---------------------------------------------------------
class TWaitDialog : public TScaleDialog
{
public:
	TWaitDialog(TWindow* parent, int resId);

	void SetText(const char *tText);
	void PumpWaitingMessages();
	void AllowCancel(bool show);
	bool Canceled();

protected:
	TStatic *text;
	TPushButton *cancel;
	sign allowCancel;

	void CancelBNClicked();
	void EvClose();
DECLARE_RESPONSE_TABLE(TWaitDialog);
};

DEFINE_RESPONSE_TABLE1(TWaitDialog, TScaleDialog)
	EV_BN_CLICKED(IDCANCEL, CancelBNClicked),
	EV_WM_CLOSE,
END_RESPONSE_TABLE;

//{{TWaitDialog Implementation}}

TWaitDialog::TWaitDialog(TWindow* parent, int resId)
:
	TScaleDialog(parent, resId), allowCancel(false)
{
	exceptTerminate = true;
	text = new TStatic(this, IDC_WAIT_TEXT);
	cancel = new TPushButton(this, IDCANCEL, IDI_CANCEL);
}

void TWaitDialog::SetText(const char *tText)
{
	text->SetText(tText);
}

void TWaitDialog::AllowCancel(bool show)
{
	if (show)
	{
		TRect window = GetWindowRect();
		TRect client = GetClientRect();

		window.bottom += (client.Height() * 46 / 22 - client.Height());
		MoveWindow(window, true);
		cancel->ShowWindow(SW_SHOW);
	}

	allowCancel = show ? true : -1;
}

bool TWaitDialog::Canceled()
{
	return allowCancel && !cancel->IsWindowEnabled();
}

void TWaitDialog::PumpWaitingMessages()
{
	MSG msg;

#if 20180914
	while (::PeekMessage(&msg, HWindow, 0, 0, PM_REMOVE))
#else  // 20180914
	while (::PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
#endif  // 20180914
	{
		if (msg.message == WM_KEYDOWN || msg.message == WM_KEYUP)
		{
			if (allowCancel == -1)
			{
				if (msg.wParam == VK_F6 && (GetAsyncKeyState(VK_CONTROL) & GetAsyncKeyState(VK_MENU) & 0x01))
					cancel->EnableWindow(false);
			}
			else if (!allowCancel)
				return;
		}

		GetApplicationObject()->ProcessMsg(msg);
	}
}

void TWaitDialog::CancelBNClicked()
{
	if (allowCancel == true)
		cancel->EnableWindow(false);
}

void TWaitDialog::EvClose()
{
	CancelBNClicked();
}

// ----- TWaitWindow ---------------------------------------------------------
TWaitWindow::TWaitWindow(TWindow* parent, const char *text)
{
	Init(parent, text, ComputeResId(text));
}

TWaitWindow::TWaitWindow(TWindow* parent, int resId)
{
	Init(parent, WAIT, resId);
}

#if 20180914
void TWaitWindow::Init(TWindow *parent, const char *text, int resId)
{
	active = GetActiveWindow();
	wait = new TWaitDialog(parent, resId);
	wait->Create();

	if (strcmp(text, WAIT))
		wait->SetText(text);

	if (!wait->Parent && MainShowWindow == SW_MAXIMIZE)
		wait->ShowWindow(SW_RESTORE);

	last = fresh = GetTickCount();
	SetWaitCursor(true);
}

TWaitWindow::~TWaitWindow()
{
	if (wait)
	{
		DestroyWait();

		if (active && IsWindow(active) && !GetActiveWindow())
			// 2017:290 FIX: // to allow main window activation
			//if (electric_foreach(ELECTRIC_HANDLE, active) == -1 || TShowDialog::FindWindow(active))
				SetActiveWindow(active);
	}
}

void TWaitWindow::DestroyWait()
{
	delete wait;
	wait = NULL;
}
#else  // 20180914
void TWaitWindow::Init(TWindow *parent, const char *text, int resId)
{
	prevActive = GetActiveWindow();
	prevLocked = false;
	wait = new TWaitDialog(parent, resId);

	if (prevActive && IsWindowEnabled(prevActive))
	{
		EnableWindow(prevActive, FALSE);
		prevLocked = true;
	}

	wait->Create();
	if (strcmp(text, WAIT))
		wait->SetText(text);

	if (!wait->Parent && MainShowWindow == SW_MAXIMIZE)
		wait->ShowWindow(SW_RESTORE);

	last = fresh = GetTickCount();
	SetWaitCursor(true);
}

TWaitWindow::~TWaitWindow()
{
	if (wait)
	{
		DestroyWait();

		if (prevActive && IsWindow(prevActive) && !GetActiveWindow())
			// 2017:290 FIX: allow activating the main window
			//if (electric_foreach(ELECTRIC_HANDLE, prevActive) == -1 || TShowDialog::FindWindow(prevActive))
				SetActiveWindow(prevActive);
	}
}

void TWaitWindow::DestroyWait()
{
	delete wait;
	wait = NULL;

	if (prevLocked && IsWindow(prevActive))
	{
		EnableWindow(prevActive, TRUE);
		prevLocked = false;
	}
}
#endif  // 20180914

int TWaitWindow::ComputeResId(const char *text)
{
	return strlen(text) <= 30 ? IDD_WAIT : strlen(text) <= 45 ? IDD_WAIT_MORE : IDD_WAIT_MUCH_MORE;
}

void TWaitWindow::SetText(const char *text)
{
	wait->SetText(text);
	wait->PumpWaitingMessages();
}

void TWaitWindow::TickText(const char *text)
{
	DWORD now = GetTickCount();

	if (now - fresh >= WAITICK_FRESH)
	{
		SetText(text);
		last = fresh = now;
	}
	else if (now - last >= WATTICK_TICK)
	{
		wait->SetText(text);
		last = now;
	}
}

void TWaitWindow::TickCount(int count, const char *suffix)
{
	if (count >= WAITICK_COUNT_MIN || GetTickCount() - fresh >= WAITICK_FRESH)
	{
		mstr m;
		m.printf("%d %s...", count, suffix);
		TickText(str(m));
	}
}

void TWaitWindow::ShowWindow(int nCmdShow)
{
	wait->ShowWindow(nCmdShow);
	wait->PumpWaitingMessages();
}

void TWaitWindow::AllowCancel(bool show)
{
	wait->AllowCancel(show);
	wait->PumpWaitingMessages();
}

bool TWaitWindow::Canceled()
{
	wait->PumpWaitingMessages();
	return wait->Canceled();
}

void TWaitWindow::ResetForeground()
{
	if (wait)
	{
		TWindow *parent = GetParent();
		DestroyWait();
		SetForegroundWindow(parent->HWindow);
	}
}

TWindow *TWaitWindow::GetParent()
{
	return wait->Parent;
}

// ----- generic -------------------------------------------------------------
double scale_wait_compute(const char *text)
{
	return (double) (strlen(text) >= 45 ? 224 : strlen(text) >= 30 ? 184 : 136) / 136;
}

void scale_wait_dialog(TNTDialog *dialog, TStatic *text, double factor)
{
	TRect rect = text->GetWindowRect();
	TPoint origin(rect.left, rect.top);
	int new_text_width = floor(rect.Width() * factor);
	int delta_width = new_text_width - rect.Width();

	dialog->ScreenToClient(origin);
	text->MoveWindow(origin.x, origin.y, new_text_width, rect.Height(), FALSE);

	rect = dialog->GetWindowRect();
	rect.left -= delta_width / 2;
	rect.right += delta_width - (delta_width / 2);
	dialog->MoveWindow(rect);
}
