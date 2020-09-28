//{{TDamnNameFace Implementation}}

TDamnNameFace::TDamnNameFace(TWindow* parent, int resourceId, const char *name, uint textLimit)
:
	TNameFace(parent, resourceId, name, textLimit)
{
	SetValidator(new TDamnNameValidator);
}

TDamnNameFace::~TDamnNameFace()
{
}
