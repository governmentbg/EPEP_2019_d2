//{{TSLCReasonAliasFace Implementation}}

TSLCReasonAliasFace::TSLCReasonAliasFace(TWindow* parent, int resourceId, const char *name, char tLawKind, long flags)
:
	TLongAliasFace(parent, resourceId, name, SLCReason, "", flags | ALIAS_EMPTY), lawKind(tLawKind)
{
}

TGroup *TSLCReasonAliasFace::NewGroup() const
{
	TSLCReason *reason = new TSLCReason;
	reason->slcType = *xCrit;
	return reason;
}

void TSLCReasonAliasFace::Criteria(msql &m, TGroup *group)
{
	TGroupList::Criteria(m, group);
	m.AddLike("F_LAW_KINDS", C2S[lawKind], true);
}

void TSLCReasonAliasFace::GetGroupText(mstr &m, TGroup *group, int)
{
	TYPECHECK_PTR_CAST(TSLCReason, group)->PrintPrefixed(m, xCrit);
}

void TSLCReasonAliasFace::SetupWindow()
{
	delete array;
	array = new TCharAliasArray;	// sort by order, not value

	TAliasFace::SetupWindow();
}
