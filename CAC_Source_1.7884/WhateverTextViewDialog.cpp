#include "cac.h"

DEFINE_RESPONSE_TABLE1(TWhateverTextViewDialog, TWhateverTextBaseDialog)
//{{TWhateverTextViewDialogRSP_TBL_BEGIN}}
	EV_BN_CLICKED(IDVIEW, ViewBNClicked),
//{{TWhateverTextViewDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TWhateverTextViewDialog Implementation}}

TWhateverTextViewDialog::TWhateverTextViewDialog(TWindow* parent, TGroup *group, const char *tTitle, const char *what, int size, TDatedContainer *tElectric, int resId)
:
	TWhateverTextBaseDialog(parent, group, tTitle, what, size, resId), electric(tElectric)
{
}

TWhateverTextViewDialog::TWhateverTextViewDialog(TWindow* parent, TText *field, const char *tTitle, const char *what, int size, TDatedContainer *tElectric, int resId)
:
	TWhateverTextBaseDialog(parent, field, tTitle, what, size, resId), electric(tElectric)
{
}

void TWhateverTextViewDialog::ViewBNClicked()
{
	electric_action(electric, TYPE_TEXT, ACTION_VIEW);
}
