#include "cac.h"

DEFINE_RESPONSE_TABLE1(TScaleDialog, TResourceDialog)
//{{TScaleDialogRSP_TBL_BEGIN}}
	EV_BN_CLICKED(IDOK, OKBNClicked),
	EV_BN_CLICKED(IDCANCEL, CancelBNClicked),
	EV_WM_CLOSE,
//{{TScaleDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TScaleDialog Implementation}}

TScaleDialog::TScaleDialog(TWindow* parent, int resId)
:
	TResourceDialog(parent, true, resId), timerId(0)
{
}

TScaleDialog::~TScaleDialog()
{
	if (timerId)
		KillTimer(timerId);
}

static bool PushImage(HWND hWnd, unsigned long dlgId)
{
	return (dlgId >= IDD_FIRST_SORTED && dlgId < IDD_FIRST_UNSORTED) || (::GetWindowLong(hWnd, GWL_STYLE) & WS_GROUP);
}

static bool PushImage(int id, HWND hWnd, int min, int max, unsigned long dlgId)
{
	return ((max == IDFINAL && (id == IDOK || id == IDCANCEL)) || (id >= min && id <= max)) && PushImage(hWnd, dlgId);
}

bool TScaleDialog::MatchesId(TWindow *window, void *id)
{
	return window->Attr.Id == (int) id;
}

#define dialog ((TScaleDialog *) lParam)

BOOL CALLBACK TScaleDialog::CreatePush(HWND hWnd, LPARAM lParam)
{
	int id = ::GetDlgCtrlID(hWnd);

	if (PushImage(id, hWnd, IDSTART, IDFINAL, dialog->dlgId) && !dialog->FirstThat(MatchesId, (void *) id))
	{
		if (id == IDCANCEL)
			new TPushButton(dialog, IDCANCEL, dialog->FirstThat(MatchesId, (void *) IDOK) ? IDI_CANCEL : IDEND);
		else
			new TPushButton(dialog, id);
	}

	return TRUE;
}

#undef dialog

static bool WindowInvalid(TWindow *window, void *)
{
	return window->SendMessage(WM_FACE_VALID) == MW_INVALID_FACE;
}

bool TScaleDialog::IsValid()
{
	return !FirstThat(WindowInvalid, NULL);
}

bool TScaleDialog::IsClean()
{
	return true;
}

void TScaleDialog::SetupWindow()
{
	if (CHROME_PUSH_IMAGE)
		EnumChildWindows(Handle, CreatePush, (LPARAM) this);

	TResourceDialog::SetupWindow();

	if (CHROME_PUSH_IMAGE)
	{
		TWindow *button = FirstThat(MatchesId, (void *) IDOK);

		if (button && PushImage(button->HWindow, dlgId))
		{
			if (dlgMsbmp)
				button->SetCaption(dlgScale.fsOrig <= 24 ? "    Потвърждение" : "   Потвърждение");
			else
				button->SetCaption(" Потвърждение");
		}

		button = FirstThat(MatchesId, (void *) IDEDIT);

		if (button && PushImage(button->HWindow, dlgId))
			button->SetCaption(" &Редактиране");
	}
}

TResult TScaleDialog::EvCommand(uint id, THandle hWndCtl, uint notifyCode)
{
	if (notifyCode == BN_CLICKED && PushImage(id, hWndCtl, IDRIGHT, IDRIGHTEST, dlgId))
		GetApplicationObject()->PumpWaitingMessages();

	return TResourceDialog::EvCommand(id, hWndCtl, notifyCode);
}

TResult TScaleDialog::EvNotify(uint id, TNotify far& info)
{
	if (CHROME_PUSH_IMAGE && info.code == NM_CUSTOMDRAW && PushImage(info.idFrom, info.hwndFrom, IDRIGHT, IDFINAL, dlgId))
	{
		NMCUSTOMDRAW &draw = (NMCUSTOMDRAW &) info;

		if (draw.dwDrawStage == CDDS_PREPAINT)
			return CDRF_NOTIFYPOSTPAINT;

		if (draw.dwDrawStage == CDDS_POSTPAINT)
			::SendMessage(info.hwndFrom, WM_DRAW_FACE, 0, (LPARAM) draw.hdc);
	}

	return TResourceDialog::EvNotify(id, info);
}

void TScaleDialog::OKBNClicked()
{
	if (IsValid())
		CmOk();
}

void TScaleDialog::CancelBNClicked()
{
	EvClose();
}

void TScaleDialog::EvClose()
{
	if (IsClean())
		TResourceDialog::EvClose();
}
