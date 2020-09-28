#include "cac.h"

DEFINE_RESPONSE_TABLE1(TSelectDialog, TGroupDialog)
//{{TSelectDialogRSP_TBL_BEGIN}}
	EV_BN_CLICKED(IDOK, OKBNClicked),
	EV_COMMAND_ENABLE(IDOK, OKBNEnable),
//{{TSelectDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TSelectDialog Implementation}}

TSelectDialog::TSelectDialog(TWindow* parent, TGroup *group, int resId)
:
	TGroupDialog(parent, group, resId), glist(NULL)
{
}

void TSelectDialog::SetupWindow()
{
	TGroupDialog::SetupWindow();

#if RANGECHECK
	if (!glist)
		fatal("select dialog: no list");
#endif  // RANGECHECK
}

void TSelectDialog::OKBNClicked()
{
	if (IsValid())
	{
		*Group() << glist->Group();
		CmOk();
	}
}

void TSelectDialog::OKBNEnable(TCommandEnabler &tce)
{
	tce.Enable(glist->Group());
}
