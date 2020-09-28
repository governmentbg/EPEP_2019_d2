#include "cac.h"

DEFINE_RESPONSE_TABLE1(TAlterStreetDialog, TGroupDialog)
//{{TAlterStreetDialogRSP_TBL_BEGIN}}
	EV_CBN_SELCHANGE(IDC_ALTER_STREET_AREA, AreaCBNSelChange),
	EV_CBN_SELCHANGE(IDC_ALTER_STREET_REGION, RegionCBNSelChange),
	EV_CBN_SELCHANGE(IDC_ALTER_STREET_UCLP, UCLPCBNSelChange),
//{{TAlterStreetDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TAlterStreetDialog Implementation}}

TAlterStreetDialog::TAlterStreetDialog(TWindow* parent, TStreet *tSource, TStreet *tTarget, int resId)
:
	TGroupDialog(parent, NULL, resId), source(tSource), target(tTarget)
{
	area = new TAreaAliasFace(this, IDC_ALTER_STREET_AREA, "F_AREA");
	region = new TRegionAliasFace(this, IDC_ALTER_STREET_REGION, "F_REGION", area);
	uclp = new TFindUCLPAliasFace(this, IDC_ALTER_STREET_UCLP, "F_UCLP", area, region);
	sourceStreet = new TFindStreetAliasFace(this, IDC_ALTER_STREET_SOURCE_STREET, "F_SOURCE_STREET", uclp, ADDR_ALTER);
}

bool TAlterStreetDialog::IsValid()
{
	bool isValid = TGroupDialog::IsValid();

	if (isValid)
	{
		source->uclp = uclp->L();
		source->street = sourceStreet->L();
		target->uclp = uclp->L();
		target->street = TargetValue();

		if (uclp->L() == 0 || sourceStreet->L() == 0 || TargetValue() == 0)
			isValid = error("Населеното място и улиците са задължителни.");
		else if (source->uclp == target->uclp && source->street == target->street)
			isValid = error("Избрана е една и съща улица.");
	}

	return isValid;
}

void TAlterStreetDialog::AreaCBNSelChange()
{
	region->Reset(0);
	RegionCBNSelChange();
}

void TAlterStreetDialog::RegionCBNSelChange()
{
	uclp->Reset(0);
	UCLPCBNSelChange();
}

void TAlterStreetDialog::UCLPCBNSelChange()
{
	sourceStreet->Reset(0);
	ClearTarget();
}

void TAlterStreetDialog::SetupWindow()
{
	TGroupDialog::SetupWindow();

	area->SetL(DEFAULT_AREA);
	region->Reset(DEFAULT_REGION);
	uclp->Reset(DEFAULT_UCLP);
	sourceStreet->Reset(0);
	ClearTarget();
}
