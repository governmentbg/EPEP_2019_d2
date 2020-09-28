//{{TKnownAliasFace Implementation}}

TKnownAliasFace::TKnownAliasFace(TWindow* parent, int resourceId, const char *name, const char *xCrit, long flags,
	const char *include)
:
	TPersonAliasFace(parent, resourceId, name, KnownAliasGroup, xCrit, flags, include)
{
}

void TKnownAliasFace::GetGroupText(mstr &m, TGroup *group, int)
{
	TYPECHECK_PTR_CAST(TKnownAliasGroup, group)->PrintPrefixed(m);
}
