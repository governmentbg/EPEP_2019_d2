//{{THostNameFace Implementation}}

THostNameFace::THostNameFace(TWindow* parent, int resourceId, const char *name, uint textLimit)
:
	TStringFace(parent, resourceId, name, textLimit)
{
	SetValidator(new THostNameValidator);
}
