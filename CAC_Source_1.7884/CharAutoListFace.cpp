#include "cac.h"

//{{TCharAutoListFace Implementation}}

TCharAutoListFace::TCharAutoListFace(TWindow* parent, int resourceId, const char *name, TCharAliasGroup *group, const char *xCrit, int size)
:
	TCharListFace(parent, resourceId, name, group, xCrit, size)
{
}

const char *TCharAutoListFace::S()
{
	TCharListFace::S();

	if (!any(s))
		for (int index = 0; index < GetCount(); index++)
			strcach(s, CharAt(index));

	return s;
}
