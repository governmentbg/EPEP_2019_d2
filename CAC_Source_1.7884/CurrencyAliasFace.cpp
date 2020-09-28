//{{TCurrencyAliasFace Implementation}}

TCurrencyAliasFace::TCurrencyAliasFace(TWindow* parent, int resourceId, const char *name, long flags)
:
	TStringAliasFace(parent, resourceId, name, Currency, NULL, flags)
{
	sortcol = -2;
}
