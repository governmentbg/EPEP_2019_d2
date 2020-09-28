//{{TSelectJurorListBox Implementation}}

TSelectJurorListBox::TSelectJurorListBox(TWindow* parent, int resourceId, TGArray<TDistribJuror> *jurors)
:
	TCloneListBox(parent, resourceId, jurors, TDistribJuror::Clone)
{
	abcskip = ABC_SKIP_LOWER;
	sortcol = 0;
}

void TSelectJurorListBox::GetGroupText(mstr &m, TGroup *group, int)
{
	TYPECHECK_PTR_CAST(TDistribJuror, group)->PrintPrefixed(m);
}
