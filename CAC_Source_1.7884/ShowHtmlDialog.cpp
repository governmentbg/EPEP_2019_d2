DEFINE_RESPONSE_TABLE1(TShowHtmlDialog, THtmlBaseDialog)
//{{TShowHtmlDialogRSP_TBL_BEGIN}}
	EV_BN_CLICKED(IDPRINT, PrintBNClicked),
	EV_BN_CLICKED(IDC_HTML_BASE_PRINT_DIRECT, PrintDirectBNClicked),
	EV_BN_CLICKED(IDCANCEL, CancelBNClicked),
	EV_BN_KILLFOCUS(IDPRINT, BrowserBNKillFocus),
	EV_BN_KILLFOCUS(IDCANCEL, BrowserBNKillFocus),
	EV_WM_CLOSE,
//{{TShowHtmlDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TShowHtmlDialog Implementation}}

TShowHtmlDialog::TShowHtmlDialog(TWindow* parent, const char *name, bool remove, int resId)
:
	THtmlBaseDialog(parent, CHROME_HTML_MAXIMIZED, name, remove, resId), printed(false)
{
	InitControls();
}

TShowHtmlDialog::TShowHtmlDialog(TWindow* parent, char *text, int resId)
	: THtmlBaseDialog(parent, CHROME_HTML_MAXIMIZED, text, resId), printed(false)
{
	InitControls();
}

void TShowHtmlDialog::InitControls()
{
	autoDown[0] = new TButton(this, IDPRINT);
	autoDown[1] = new TButton(this, IDC_HTML_BASE_PRINT_DIRECT);
	autoDown[2] = new TButton(this, IDVIEW);
	autoDown[3] = new TButton(this, IDC_HTML_BASE_PAGE_SETUP);
	autoDown[4] = new TButton(this, IDEDIT);
	autoDown[5] = new TButton(this, IDSAVE);
	autoDown[6] = new TButton(this, IDCANCEL);
}

void TShowHtmlDialog::PrintBNClicked()
{
	THtmlBaseDialog::PrintBNClicked();
	printed = true;
}

void TShowHtmlDialog::PrintDirectBNClicked()
{
	THtmlBaseDialog::PrintBNClicked();
	printed = true;
}

void TShowHtmlDialog::CancelBNClicked()
{
	EvClose();
}

void TShowHtmlDialog::EvClose()
{
	CloseDialog(printed ? IDOK : IDCANCEL);
}
