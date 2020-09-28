//{{TRegionAliasFace Implementation}}

TRegionAliasFace::TRegionAliasFace(TWindow* parent, int resourceId, const char *name, TAreaAliasFace *tArea, long flags)
:
	TLongAliasFace(parent, resourceId, name, Region, NULL, flags | ALIAS_ABCSORT), area(tArea)
{
	initialRefresh = false;
}

void TRegionAliasFace::Reset(long region)
{
	TAliasFace::Refresh();
	SetL(region);
}

void TRegionAliasFace::Criteria(msql &m, TGroup *group)
{
#if RANGECHECK
	if (!area->L())
		fatal("%s: criteria: no area", name);
#endif  // RANGECHECK
	Region->area = area->L();
	TLongAliasFace::Criteria(m, group);
}
