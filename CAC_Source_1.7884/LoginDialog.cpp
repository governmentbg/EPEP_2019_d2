DEFINE_RESPONSE_TABLE1(TLoginDialog, TGroupDialog)
//{{TLoginDialogRSP_TBL_BEGIN}}
	EV_BN_CLICKED(IDOK, OKBNClicked),
	EV_WM_TIMER,
//{{TLoginDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TLoginDialog Implementation}}

TLoginDialog::TLoginDialog(TWindow* parent, TUser *user, int resId)
:
	TGroupDialog(parent, user, resId), caps(FALSE)
{
	new TStringFace(this, IDC_LOGIN_LOGIN, "F_LOGIN", SIZE_OF_NAME);
	new TStringFace(this, IDC_LOGIN_PASSWORD, "F_PASSWORD", SIZE_OF_ALIAS);
	capsLock = new TStatic(this, IDC_LOGIN_CAPS_LOCK);
}

TLoginDialog::~TLoginDialog()
{
}

void TLoginDialog::SetupWindow()
{
	TGroupDialog::SetupWindow();

	if (!Parent && MainShowWindow == SW_MAXIMIZE)
		ShowWindow(SW_RESTORE);

	timerId = SetTimer(0, 50, NULL);
}

void TLoginDialog::OKBNClicked()
{
	if (IsValid())
	{
		*Group() << this;
		CmOk();
	}
}

void TLoginDialog::EvTimer(uint timerId)
{
	BOOL tCaps = GetKeyState(VK_CAPITAL) & 0x01;

	if (tCaps != caps)
		capsLock->ShowWindow(caps = tCaps);

	TGroupDialog::EvTimer(timerId);
}
