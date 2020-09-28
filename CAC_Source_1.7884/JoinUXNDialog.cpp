#include "cac.h"

DEFINE_RESPONSE_TABLE1(TJoinUXNDialog, TAlterUXNDialog)
//{{TJoinUXNDialogRSP_TBL_BEGIN}}
	EV_CBN_SELCHANGE(IDC_ALTER_UXN_SOURCE_TYPE, SourceTypeCBNSelChange),
//{{TJoinUXNDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TJoinUXNDialog Implementation}}

TJoinUXNDialog::TJoinUXNDialog(TWindow* parent, TUCNGroup *source, TUCNGroup *target, int resId)
:
	TAlterUXNDialog(parent, source, target, UCN_JOINABLE_FROMS, UCN_JOINABLE_TOS, resId)
{
}

void TJoinUXNDialog::SetupWindow()
{
	TAlterUXNDialog::SetupWindow();

	if (target->ucnType)
		sourceType->SetC(ucn2code(target->ucnType));

	SourceTypeCBNSelChange();
	targetType->SetC(target->ucnType);
	targetCode->SetS(target->ucn);
}

void TJoinUXNDialog::SourceTypeCBNSelChange()
{
	char ucnType = sourceType->C();

	*targetUCNTypes = '\0';

	if (strchr(UCN_JOINABLE_TOS, ucnType))
		strcach(targetUCNTypes, ucnType);

	if (is_code(ucnType) && strchr(UCN_JOINABLE_TOS, ucn2code(ucnType)))
		strcach(targetUCNTypes, ucn2uxn(ucnType));

	targetType->SetXCrit(targetUCNTypes);
}
