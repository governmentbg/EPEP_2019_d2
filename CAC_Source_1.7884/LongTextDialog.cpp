#include "cac.h"

DEFINE_RESPONSE_TABLE1(TLongTextDialog, TScaleDialog)
//{{TLongTextDialogRSP_TBL_BEGIN}}
	EV_EN_SETFOCUS(IDC_LONG_TEXT_TEXT, TextSetFocus),
//{{TLongTextDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TLongTextDialog Implementation}}

TLongTextDialog::TLongTextDialog(TWindow* parent, const char *tText)
:
	TScaleDialog(parent, IDD_LONG_TEXT), text(tText)
{
	longText = new TEdit(this, IDC_LONG_TEXT_TEXT, 0);
}

void TLongTextDialog::SetupWindow()
{
	TScaleDialog::SetupWindow();

	mstr m;

	for (const char *s = text; *s; s++)
	{
		if (*s == '\n')
			m.cach('\r');
		m.cach(*s);
	}
	longText->SetText(str(m));
}

void TLongTextDialog::TextSetFocus()
{
	longText->SetSelection(0, 0);
}
