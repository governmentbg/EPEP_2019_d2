#include "cac.h"

bool invalid_range(TLongFace *min, TLongFace *max)
{
	return min->L() && max->L() && min->L() > max->L();
}

bool invalid_range(TDateFace *min, TDateFace *max)
{
	return !min->Date().Empty() && !max->Date().Empty() && min->Date() > max->Date();
}

bool invalid_range(TTimeFace *min, TTimeFace *max)
{
	return !min->Time().Empty() && !max->Time().Empty() && min->Time() > max->Time();
}

bool invalid_range(TStringFace *min, TStringFace *max)
{
	return any(min->S()) && any(max->S()) && strcmp(min->S(), max->S()) > 0;
}
