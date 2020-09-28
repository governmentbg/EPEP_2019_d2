#include "cac.h"

DEFINE_RESPONSE_TABLE1(TGroupDialog, TQuickDialog)
//{{TGroupDialogRSP_TBL_BEGIN}}
	EV_BN_CLICKED(IDOK, OKBNClicked),
//{{TGroupDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TGroupDialog Implementation}}

TGroupDialog::TGroupDialog(TWindow* parent, TGroup *tGroup, int resId)
:
	TQuickDialog(parent, resId), group(tGroup)
{
}

TGroupDialog::~TGroupDialog()
{
}

void TGroupDialog::OKBNClicked()
{
	if (IsValid())
	{
		if (Group())
			*Group() << this;
		CmOk();
	}
}

void TGroupDialog::SetupWindow()
{
	TQuickDialog::SetupWindow();

	if (Group())
		*Group() >> this;
}
