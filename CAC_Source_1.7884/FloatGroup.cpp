#include "cac.h"

// ----- TFloatGroup ---------------------------------------------------------
TFloatGroup::TFloatGroup(const char *name, const char *tFields)
	: TGroup(name, NULL), fields(tFields)
{
	Add(
		new TRadio("F_BY_TKYN", &order, ORDER_BY_TKYN),
		new TRadio("F_BY_TYN", &order, ORDER_BY_TYN),
		new TRadio("F_BY_TSYN", &order, ORDER_BY_TSYN),
		new TRadio("F_BY_KYN", &order, ORDER_BY_KYN),
		new TRadio("F_BY_JTYN", &order, ORDER_BY_JTYN),
		new TRadio("F_BY_DTYN", &order, ORDER_BY_DTYN),
		NULL
	);

	order = ORDER_BY_TSYN;
}

void TFloatGroup::EndCriteria(msql &m, const char *prefix)
{
	static constant orders[ORDER_BY_COUNT] =
	{
		"",
		"F_TYPE, F_KIND, F_YEAR, F_NO",
		"F_TYPE, F_YEAR, F_NO",
		"F_TYPE, F_SUBJECT, F_YEAR, F_NO",
		"F_KIND, F_YEAR, F_NO",
		"F_JUDGE, F_TYPE, F_YEAR, F_NO",
		"F_DATE, F_TYPE, F_YEAR, F_NO"
	};

#if RANGECHECK
	if (order < 0 || order >= ORDER_BY_COUNT)
		fatal("%ld: order not supported", order);
#endif  // RANGECHECK

	if (order)
		m.AddOrder(orders[order], prefix);
}
