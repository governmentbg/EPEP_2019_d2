#include "cac.h"

DEFINE_RESPONSE_TABLE1(TWhateverTextBaseDialog, TGroupDialog)
//{{TWhateverTextBaseDialogRSP_TBL_BEGIN}}
	EV_BN_CLICKED(IDOK, OKBNClicked),
//{{TWhateverTextBaseDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TWhateverTextBaseDialog Implementation}}

TWhateverTextBaseDialog::TWhateverTextBaseDialog(TWindow* parent, TGroup *group, const char *tTitle, const char *what, int size, int resId)
:
	TGroupDialog(parent, group, resId), title(tTitle), field(NULL)
{
	text = new TStringFace(this, IDC_WHATEVER_TEXT_BASE_TEXT, what, size);
}

TWhateverTextBaseDialog::TWhateverTextBaseDialog(TWindow* parent, TText *tField, const char *tTitle, const char *what, int size, int resId)
:
	TGroupDialog(parent, NULL, resId), title(tTitle), field(tField)
{
	text = new TStringFace(this, IDC_WHATEVER_TEXT_BASE_TEXT, what, size);
}

void TWhateverTextBaseDialog::OKBNClicked()
{
	if (IsValid())
	{
		if (field)
			field->SetS(text->S());
		else
			*Group() << this;

		CmOk();
	}
}

void TWhateverTextBaseDialog::SetupWindow()
{
	TGroupDialog::SetupWindow();

	SetCaption(title);
}
