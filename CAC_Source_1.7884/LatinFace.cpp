//{{TLatinFace Implementation}}

TLatinFace::TLatinFace(TWindow* parent, int resourceId, const char *name, uint textLimit)
:
	TStringFace(parent, resourceId, name, textLimit)
{
	SetValidator(new TNameValidator("A-Z"));
}
