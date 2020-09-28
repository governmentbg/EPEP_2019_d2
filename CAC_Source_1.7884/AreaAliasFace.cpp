//{{TAreaAliasFace Implementation}}

TAreaAliasFace::TAreaAliasFace(TWindow* parent, int resourceId, const char *name, long flags)
:
	TLongAliasFace(parent, resourceId, name, Area, NULL, flags | ALIAS_ABCSORT)
{
}
