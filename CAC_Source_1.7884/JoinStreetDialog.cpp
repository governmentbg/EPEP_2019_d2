#include "cac.h"

//{{TJoinStreetDialog Implementation}}

TJoinStreetDialog::TJoinStreetDialog(TWindow* parent, TStreet *source, TStreet *target, int resId)
:
	TAlterStreetDialog(parent, source, target, resId)
{
	targetStreet = new TFindStreetAliasFace(this, IDC_ALTER_STREET_TARGET_STREET, "F_TARGET_STREET", uclp, ADDR_ALTER);
}

bool TJoinStreetDialog::IsValid()
{
	bool isValid = TAlterStreetDialog::IsValid();

	if (isValid)
	{
		if (!source->Get() || !target->Get())
			isValid = false;
		else
		{
			isValid = ask("Желаете ли да присъедините %s (%ld) към %s (%ld)", source->name, source->street,
				target->name, target->street);
		}
	}

	return isValid;
}

void TJoinStreetDialog::ClearTarget()
{
	targetStreet->Reset(0);
}

long TJoinStreetDialog::TargetValue()
{
	return targetStreet->L();
}
