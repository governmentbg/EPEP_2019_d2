#include "cac.h"

// ----- TCounterGroup -------------------------------------------------------
TCounterGroup::TCounterGroup(const char *tNone, const char *tOne, const char *tMany)
	: maximum(INT_MAX), none(tNone), one(tOne), many(tMany)
{
	Add(new TLong("F_COUNT", &count));
}

void TCounterGroup::SetNames(const char *tNone, const char *tOne, const char *tMany)
{
	none = tNone;
	one = tOne;
	many = tMany;
}

TCounterGroup::TCounterGroup()
	: maximum(INT_MAX)
{
	Add(new TLong("F_COUNT", &count));
}
