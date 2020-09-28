//{{TFindLongAliasFace Implementation}}

TFindLongAliasFace::TFindLongAliasFace(TWindow* parent, int resourceId, const char *name, uint textLimit, TLongAliasGroup *alias, const char *xCrit, long flags)
:
	TLongAliasFace(parent, resourceId, name, alias, xCrit, flags | ALIAS_ABCSORT | ALIAS_DROPDOWN)
{
	SetTextLimit(textLimit);
}

int TFindLongAliasFace::GetVisualIndex()
{
	return FindVisualIndex();
}

long TFindLongAliasFace::LNow()
{
	int index = GetSelIndex();
	return index >= 0 ? LongAt(index) : 0;
}
