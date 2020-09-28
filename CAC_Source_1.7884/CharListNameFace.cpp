//{{TCharListNameFace Implementation}}

TCharListNameFace::TCharListNameFace(TWindow* parent, int resourceId, const char *name, TCharAliasGroup *group, const char *xData, int size)
:
	TCharListFace(parent, resourceId, name, group, xData, size)
{
	sortcol = 0;
}
