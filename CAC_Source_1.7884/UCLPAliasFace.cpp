//{{TUCLPAliasFace Implementation}}

TUCLPAliasFace::TUCLPAliasFace(TWindow* parent, int resourceId, const char *name, long tUCLP)
:
	TLongAliasFace(parent, resourceId, name, UCLP, NULL, ALIAS_ABCSORT), uclp(tUCLP)
{
#if RANGECHECK
	if (!uclp)
		fatal("zero initial UCLP value");
#endif  // RANGECHECL
}

void TUCLPAliasFace::Criteria(msql &m, TGroup *group)
{
	if (IsWindowEnabled())
	{
		UCLP->Seek(uclp);
		flags |= ADDR_NAMED;
		*UCLP->name = '\0';
	}
	else
		UCLP->uclp = uclp;

	TLongAliasFace::Criteria(m, group);
}

void TUCLPAliasFace::Refresh()
{
	TLongAliasFace::Refresh();
	SetL(uclp);
}
