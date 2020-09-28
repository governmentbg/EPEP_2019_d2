//{{TFindUCLPAliasFace Implementation}}

TFindUCLPAliasFace::TFindUCLPAliasFace(TWindow* parent, int resourceId, const char *name, TAreaAliasFace *tArea, TRegionAliasFace *tRegion)
:
	TFindAddrAliasFace(parent, resourceId, name, SIZE_OF_UCLP_NAME, UCLP), area(tArea), region(tRegion)
{
}

void TFindUCLPAliasFace::Add()
{
	TUCLP *uclp = new TUCLP;

	uclp->area = area->L();
	uclp->region = region->L();
	uclp->uclp = 0;

	if (add_uclp(this, uclp, false))
	{
		SetIndexNotify(InsertGroup(uclp));
		UpdateVisual();
	}
	else
		delete uclp;
}

bool TFindUCLPAliasFace::Search()
{
	GetText();
	strcpy(UCLP->name, text);
	return select_uclp(this, UCLP);
}

bool TFindUCLPAliasFace::EditGroup(TGroup *group)
{
	return edit_uclp(this, (TUCLP *) group, false);
}

void TFindUCLPAliasFace::Criteria(msql &m, TGroup *group)
{
#if RANGECHECK
	if (!area->L())
		fatal("%s: criteria: no area", name);
	if (!region->L())
		fatal("%s: criteria: no region", name);
#endif  // RANGECHECK
	UCLP->area = area->L();
	UCLP->region = region->L();
	TFindLongAliasFace::Criteria(m, group);
}
