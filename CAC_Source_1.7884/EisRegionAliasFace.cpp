//{{TEisRegionAliasFace Implementation}}

TEisRegionAliasFace::TEisRegionAliasFace(TWindow* parent, int resourceId, const char *name, TEisAreaAliasFace *tArea,
	TXMLObject *object)
:
	TEisAliasFace(parent, resourceId, name, 0, object, FLAG_NULL), area(tArea)
{
	initialRefresh = true;
	flags &= ~ALIAS_EMPTY;
}

void TEisRegionAliasFace::Reset(long region)
{
	TEisAliasFace::Refresh();
	SetL(region);
}

void TEisRegionAliasFace::Criteria(msql &m, TGroup *group)
{
#if RANGECHECK
	if (!area->L())
		fatal("%s: criteria: no area", name);
#endif  // RANGECHECK
	nom = area->L();
	TEisAliasFace::Criteria(m, group);
}

void TEisRegionAliasFace::SetupWindow()
{
	TAliasFace::SetupWindow();

	if (!initialRefresh)
		autoEnable = false;
}
