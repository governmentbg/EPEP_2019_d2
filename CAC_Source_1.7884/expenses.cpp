#include "cac.h"

#include "EditPenalityDialog.h"	// 2003:146
#include "EditPenalityDialog.cpp"

#include "EditRewardDialog.h"		// 2003:147 ~simpler edit penality
#include "EditRewardDialog.cpp"

#include "EditTaxDialog.h"		// 2006:191
#include "EditTaxDialog.cpp"

#include "ExpenseListWindow.h"
#include "ExpenseListWindow.cpp"
#include "ExpenseListDialog.h"
#include "ExpenseListDialog.cpp"

void expend_sidewind(TWindow *parent, TRCDKeyContainer *container, TSession *session, TLawsuit *lawsuit, TSideWind *sideWind)
{
	// 2014:211 LPR: rewritten +related, p.9 of 1.6181+
	TExpenseListDialog(parent, sideWind, container, session, lawsuit, session ? IDD_EXPENSE_LIST : IDD_TAX_LIST).Execute();
}
