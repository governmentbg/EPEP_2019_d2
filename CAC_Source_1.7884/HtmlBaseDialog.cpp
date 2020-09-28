DEFINE_RESPONSE_TABLE1(THtmlBaseDialog, TShowDialog)
//{{THtmlBaseDialogRSP_TBL_BEGIN}}
	EV_BN_CLICKED(IDPRINT, PrintBNClicked),
	EV_BN_CLICKED(IDC_HTML_BASE_PRINT_DIRECT, PrintDirectBNClicked),
	EV_BN_CLICKED(IDVIEW, ViewBNClicked),
	EV_BN_CLICKED(IDC_HTML_BASE_PAGE_SETUP, PageSetupBNClicked),
	EV_BN_CLICKED(IDEDIT, EditBNClicked),
	EV_BN_CLICKED(IDSAVE, SaveBNClicked),
	EV_BN_CLICKED(IDREFRESH, RefreshBNClicked),
	EV_MESSAGE(WM_FOCUS_FACE, EvFocusFace),
//{{THtmlBaseDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{THtmlBaseDialog Implementation}}

THtmlBaseDialog::THtmlBaseDialog(TWindow* parent, int showMax, const char *tName, bool tRemove, int resId)
:
	TShowDialog(parent, showMax, resId), iBrowser(NULL), hrOleInitialize(OLE_E_WRONGCOMPOBJ), text(NULL), remove(tRemove)
{
	strcpy(name, tName);
	autoSize = new TEdit(this, IDC_HTML_BASE_FAKE);
}

THtmlBaseDialog::THtmlBaseDialog(TWindow* parent, int showMax, char *tText, int resId)
:
	TShowDialog(parent, showMax, resId), iBrowser(NULL), hrOleInitialize(OLE_E_WRONGCOMPOBJ), text(tText), remove(false)
{
	*name = '\0';
	autoSize = new TEdit(this, IDC_HTML_BASE_FAKE);
}

THtmlBaseDialog::~THtmlBaseDialog()
{
	if (iBrowser)
		CloseEmbeddedWebControl(iBrowser);

	if (hrOleInitialize == S_OK)
		OleUninitialize();

	if (*name)
		remove_file(name, NULL, remove);

	delete[] text;
}

void THtmlBaseDialog::Resize()
{
	TShowDialog::Resize();
	ResizeBrowser();
}

void THtmlBaseDialog::PrintBNClicked()
{
	IWebBrowser2_Print(iBrowser, FALSE);
}

void THtmlBaseDialog::PrintDirectBNClicked()
{
	IWebBrowser2_Print(iBrowser, TRUE);
}

void THtmlBaseDialog::ViewBNClicked()
{
	IWebBrowser2_PrintPreview(iBrowser);
}

void THtmlBaseDialog::PageSetupBNClicked()
{
	IWebBrowser2_PageSetup(iBrowser);
}

void THtmlBaseDialog::EditBNClicked()
{
	if (!*name)
	{
		TUserFile uf("временен");
		const char *tName = uf.WriteTemp("htm", text, -1);

		if (!tName)
			return;

		strcpy(name, tName);
		Navigate(name);
		remove = true;
	}

	// 2014:177 LPR: rewrite for real Word sync and to avoid launch(sync)
	TWaitWindow wait(this, WAIT_MORE);

	filter_display(&wait, name, "Документът ще бъде отворен", "Документът е записан");
	wait.ResetForeground();
	RefreshBNClicked();
}

void THtmlBaseDialog::SaveBNClicked()
{
	char tName[SIZE_OF_PATH];

	if (GetSaveFileName(this, tName, "HTML файлове", NULL, "HTM", sizeof tName, NULL, false) && strcmpi(name, tName))
	{
		// 2008:036 LPR: IWebBrowser2_SaveAs() -> CopyFile() to avoid side FX
		if (*name)
			TUserFile::Copy(name, tName);
		else
			TUserFile("хипертекстов").WriteAll(tName, text);
	}
}

void THtmlBaseDialog::SetupWindow()
{
	TShowDialog::SetupWindow();

	bool isValid = false;

	if ((iBrowser = CreateEmbeddedWebControl(this)) == NULL)
		error("Грешка при създаване на WEB контрол.");
	else
	{
		isValid = Navigate(name);
		IWebBrowser2_SetFontSize(iBrowser, 2);
	}

	if (isValid)
		ResizeBrowser();
	else
	{
		for (int i = 0; i < DOWN_CONTROLS_MAX; i++)
			if (autoDown[i])
				autoDown[i]->EnableWindow(false);

		PostMessage(WM_CLOSE);
	}
}

void THtmlBaseDialog::ResizeBrowser()
{
	if (iBrowser)
	{
		TRect rect = autoSize->GetWindowRect();
		rect.MoveTo(autoSize->Attr.X, autoSize->Attr.Y);
		IWebBrowser2_Resize(iBrowser, &rect);
	}
}

void THtmlBaseDialog::FocusBrowser()
{
	if (iBrowser)
	{
		HWND hwndBrowser = IWebBrowser2_GetHWnd(iBrowser);

		if (hwndBrowser)
			::SetFocus(hwndBrowser);
	}
}

bool THtmlBaseDialog::Navigate(const char *tName)
{
	TBSTR bs(*tName ? tName : HTML_BLANK_PAGE, -1, false);

	if (bs.bstr())
	{
		IWebBrowser2_NavigateURL(iBrowser, bs.bstr());

		if (*tName)
			return true;

		TBSTR bs(text, -1, false);

		if (bs.bstr())
		{
			return WriteEmbeddedWebControl(iBrowser, bs.bstr()) ||
				error("Грешка при зареждане на текст в WEB контрол.");
		}
	}

	return false;
}

void THtmlBaseDialog::RefreshBNClicked()
{
	IWebBrowser2_Refresh(iBrowser);
}

void THtmlBaseDialog::BrowserBNKillFocus()
{
	if (GetFocus() == IWebBrowser2_GetHWnd(iBrowser))
		IWebBrowser2_Activate(iBrowser);
}

long THtmlBaseDialog::EvFocusFace(TParam1, TParam2)
{
	FocusBrowser();
	return MW_FACE_FOCUSED;
}
