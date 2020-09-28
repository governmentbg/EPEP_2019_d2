#include "cac.h"

DEFINE_RESPONSE_TABLE1(TQuickDialog, TScaleDialog)
//{{TQuickDialogRSP_TBL_BEGIN}}
	EV_MESSAGE(WM_QUICK_SEARCH, EvQuickSearch),
	EV_BN_CLICKED(IDC_QUICK_SEARCH_PREV, QuickSearchPrevBNClicked),
	EV_COMMAND_ENABLE(IDC_QUICK_SEARCH_PREV, QuickSearchPrevBNEnable),
	EV_BN_CLICKED(IDC_QUICK_SEARCH_EDIT, QuickSearchEditBNClicked),
	EV_BN_CLICKED(IDC_QUICK_SEARCH_NEXT, QuickSearchNextBNClicked),
	EV_COMMAND_ENABLE(IDC_QUICK_SEARCH_NEXT, QuickSearchNextBNEnable),
	EV_WM_TIMER,
//{{TQuickDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TQuickDialog Implementation}}

TQuickDialog::TQuickDialog(TWindow* parent, int resId)
	: TScaleDialog(parent, resId), qs_list(NULL), flashIndex(-1), flashDelay(0)
{
	if (dlgTempl)
	{
		qs.controls[0] = new TQuickSearchEdit(this, IDC_QUICK_SEARCH_PREV, true);
		qs.controls[1] = qs.edit = new TQuickSearchEdit(this, IDC_QUICK_SEARCH_EDIT, false);
		qs.controls[2] = new TQuickSearchEdit(this, IDC_QUICK_SEARCH_NEXT, true);
	}
}

void TQuickDialog::QuickSearch(int index)
{
	if (!timerId)
		timerId = SetTimer(0, 25, NULL);

	if (timerId)
	{
		if (flashIndex >= 0)
			qs.controls[flashIndex]->SetCaption(flashIndex ? "»" : "«");

		qs.controls[index]->SetCaption("=");
		flashIndex = index;
		flashDelay = 2;
	}

	qs_list->QuickSearch(index - 1);	// index 0 step -1, index 2 step 1
}

long TQuickDialog::EvQuickSearch(TParam1, TParam2 lParam)
{
	if (dlgTempl)
	{
		if (lParam)
		{
			qs_list = (TQuickList *) lParam;
			qs_list->SetQuickSearch(&qs);
			qs.Enable();
		}
		else
		{
			qs.Disable();
			qs_list->SetQuickSearch(NULL);
			qs_list = NULL;
		}
	}

	return MW_SEARCH_QUICK;
}

void TQuickDialog::QuickSearchPrevBNClicked()
{
	QuickSearch(QUICK_SEARCH_PREV);
}

void TQuickDialog::QuickSearchPrevBNEnable(TCommandEnabler &tce)
{
	tce.Enable(qs_list && any(qs.text));
}

void TQuickDialog::QuickSearchEditBNClicked()
{
	if (qs_list)
		qs_list->QuickSearchEdit();
}

void TQuickDialog::QuickSearchNextBNClicked()
{
	QuickSearch(QUICK_SEARCH_NEXT);
}

void TQuickDialog::QuickSearchNextBNEnable(TCommandEnabler &tce)
{
	tce.Enable(qs_list && any(qs.text));
}

void TQuickDialog::EvTimer(uint)
{
	if (flashIndex >= 0 && --flashDelay <= 0)
	{
		if (qs.controls[flashIndex]->IsWindow())
			qs.controls[flashIndex]->SetCaption(flashIndex ? "»" : "«");

		flashIndex = -1;
	}
}
