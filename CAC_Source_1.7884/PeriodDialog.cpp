#include "cac.h"

//{{TPeriodDialog Implementation}}

TPeriodDialog::TPeriodDialog(TWindow* parent, TGroup *group, int resId)
:
	TGroupDialog(parent, group, resId)
{
	minDate = new TDateFace(this, IDC_PERIOD_MIN_DATE, "F_MIN_DATE");
	new TDateFaceUpDown(this, IDC_PERIOD_MIN_UPDN, minDate);
	maxDate = new TDateFace(this, IDC_PERIOD_MAX_DATE, "F_MAX_DATE");
	new TDateFaceUpDown(this, IDC_PERIOD_MAX_UPDN, maxDate);
}

bool TPeriodDialog::IsValid()
{
	bool isValid = TGroupDialog::IsValid();

	if (isValid)
	{
		if (invalid_range(minDate, maxDate))
			isValid = error("Ќачалната дата е по-гол€ма от крайната.");
	}

	return isValid;
}
