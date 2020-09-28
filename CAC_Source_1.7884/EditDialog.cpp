#include "cac.h"

DEFINE_RESPONSE_TABLE1(TEditDialog, TGroupDialog)
//{{TEditDialogRSP_TBL_BEGIN}}
	EV_BN_CLICKED(IDOK, OKBNClicked),
//{{TEditDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TEditDialog Implementation}}

TEditDialog::TEditDialog(TWindow* parent, TGroup *group, bool tEdit, int resId)
:
	TGroupDialog(parent, group, resId), edit(tEdit)
{
}

void TEditDialog::OKBNClicked()
{
	if (IsValid())
	{
		*Group() << this;

		if (Group()->Save(edit))
			CmOk();
	}
}
