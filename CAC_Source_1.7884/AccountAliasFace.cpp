//{{TAccountAliasFace Implementation}}

TAccountAliasFace::TAccountAliasFace(TWindow* parent, int resourceId, const char *name, char ucnType, long flags)
:
	TStringAliasFace(parent, resourceId, name, ::Account, C2S[ucnType], flags)
{
}
