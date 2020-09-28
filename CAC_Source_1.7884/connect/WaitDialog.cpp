#include "cac.h"

//{{TWaitDialog Implementation}}

TWaitDialog::TWaitDialog(TWindow* parent, TResId resId, TModule* module)
:
	TDialog(parent, resId, module)
{
	text = new TStatic(this, IDC_WAIT_TEXT);
}

TWaitDialog::~TWaitDialog()
{
	Destroy(IDCANCEL);
}

void TWaitDialog::SetText(const char *tText)
{
	text->SetText(tText);
}

// ----- TWaitWindow ---------------------------------------------------------
TWaitWindow::TWaitWindow(TWindow* parent, TResId resId, TModule* module)
:
	wait(parent, resId, module)
{
	wait.Create();
	wait.ShowWindow(SW_RESTORE);
	::SetCursor(::LoadCursor(NULL, IDC_WAIT));
}
