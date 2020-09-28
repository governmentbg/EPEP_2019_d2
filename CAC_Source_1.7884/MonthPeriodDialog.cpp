#include "cac.h"

//{{TMonthPeriodDialog Implementation}}

TMonthPeriodDialog::TMonthPeriodDialog(TWindow* parent, TGroup *group, int resId)
:
	TGroupDialog(parent, group, resId)
{
	minMonth = new TLongAliasFace(this, IDC_MONTH_PERIOD_MIN_MONTH, "F_MIN_MONTH", Month, NULL, FLAG_NULL);
	minYear = new TYearFace(this, IDC_MONTH_PERIOD_MIN_YEAR, "F_MIN_YEAR");
	maxMonth = new TLongAliasFace(this, IDC_MONTH_PERIOD_MAX_MONTH, "F_MAX_MONTH", Month, NULL, FLAG_NULL);
	maxYear = new TYearFace(this, IDC_MONTH_PERIOD_MAX_YEAR, "F_MAX_YEAR");
}

bool TMonthPeriodDialog::IsValid()
{
	bool isValid = TGroupDialog::IsValid();

	if (isValid)
	{
		long tMinYear = minYear->L();
		long tMaxYear = minYear->L();

		if (!tMinYear || !tMaxYear)
			isValid = error("Годините са задължителни.");
		else if (CDate(1, minMonth->L(), tMinYear) > CDate(1, maxMonth->L(), tMaxYear))
			isValid = error("Началните месец и година са по-големи от крайните.");
	}

	return isValid;
}
