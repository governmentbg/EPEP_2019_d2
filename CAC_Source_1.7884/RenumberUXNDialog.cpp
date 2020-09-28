#include "cac.h"

DEFINE_RESPONSE_TABLE1(TRenumberUXNDialog, TAlterUXNDialog)
//{{TRenumberUXNDialogRSP_TBL_BEGIN}}
	EV_CBN_SELCHANGE(IDC_ALTER_UXN_SOURCE_TYPE, SourceTypeCBNSelChange),
//{{TRenumberUXNDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TRenumberUXNDialog Implementation}}

TRenumberUXNDialog::TRenumberUXNDialog(TWindow* parent, TUCNGroup *source, TUCNGroup *target, int resId)
:
	TAlterUXNDialog(parent, source, target, UCN_RENUMBERABLE_FROMS, UCN_RENUMBERABLE_TOS, resId)
{
}

void TRenumberUXNDialog::SetupWindow()
{
	TAlterUXNDialog::SetupWindow();

	sourceType->SetC(source->ucnType);
	sourceCode->SetS(source->ucn);
	SourceTypeCBNSelChange();
}

void TRenumberUXNDialog::SourceTypeCBNSelChange()
{
	targetType->SetC(ucn2uxn(sourceType->C()));
}
