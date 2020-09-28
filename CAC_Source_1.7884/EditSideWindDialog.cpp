#include "cac.h"

DEFINE_RESPONSE_TABLE1(TEditSideWindDialog, TGroupDialog)
//{{TEditSideWindDialogRSP_TBL_BEGIN}}
	EV_CBN_SELCHANGE(IDC_EDIT_SIDE_WIND_INVOLVEMENT, InvolvementCBNSelChange),
//{{TEditSideWindDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TEditSideWindDialog Implementation}}

TEditSideWindDialog::TEditSideWindDialog(TWindow* parent, TSideWind *sideWind, const char *involvements, bool tUnwanted, int resId)
:
	TGroupDialog(parent, sideWind, resId)
{
	involvement = new TCharAliasFace(this, IDC_EDIT_SIDE_WIND_INVOLVEMENT, "F_INVOLVEMENT", Involvement, involvements);
	fromSide = new TCharAliasFace(this, IDC_EDIT_SIDE_WIND_FROM_SIDE, "F_FROM_SIDE", Involvement, INVOLVEMENT_FROM_SIDES);
	unwanted = tUnwanted ? new TStringFace(this, IDC_EDIT_SIDE_WIND_UNWANTED, "F_UNWANTED", SIZE_OF_UNWANTED) : NULL;
}

void TEditSideWindDialog::InvolvementCBNSelChange()
{
	if (strchr(INVOLVEMENT_FROMABLE_SIDES, involvement->C()))
		fromSide->EnableWindow(true);
	else
	{
		fromSide->EnableWindow(false);
		fromSide->SetC(INVOLVEMENT_FROM_NULL);
	}
}

void TEditSideWindDialog::SetupWindow()
{
	TGroupDialog::SetupWindow();

	InvolvementCBNSelChange();
	if (unwanted)
		unwanted->EnableWindow(true);
}
