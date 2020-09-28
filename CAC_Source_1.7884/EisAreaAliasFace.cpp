//{{TEisAreaAliasFace Implementation}}

TEisAreaAliasFace::TEisAreaAliasFace(TWindow* parent, int resourceId, const char *name, TXMLObject *object)
:
	TEisAliasFace(parent, resourceId, name, nmk_oblast, object, ALIAS_ABCSORT)
{
	initialRefresh = true;
	flags &= ~ALIAS_EMPTY;
}
