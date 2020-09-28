#include "cac.h"

//{{TRenumberStreetDialog Implementation}}

TRenumberStreetDialog::TRenumberStreetDialog(TWindow* parent, TStreet *source, TStreet *target, int resId)
:
	TAlterStreetDialog(parent, source, target, resId)
{
	targetStreet = new TLongFace(this, IDC_ALTER_STREET_TARGET_STREET, "F_TARGET_STREET", SIZE_OF_STREET);
}

bool TRenumberStreetDialog::IsValid()
{
	bool isValid = TAlterStreetDialog::IsValid();

	if (isValid)
	{
		if (!source->Get())
			isValid = false;
		else
			isValid = ask("Желаете ли да преномерирате %s (%ld) на %ld?", source->name, source->street, target->street);
	}

	return isValid;
}

void TRenumberStreetDialog::ClearTarget()
{
	targetStreet->SetS("");
}

long TRenumberStreetDialog::TargetValue()
{
	return targetStreet->L();
}
