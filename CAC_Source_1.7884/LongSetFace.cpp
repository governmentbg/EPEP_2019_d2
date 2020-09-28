//{{TLongSetFace Implementation}}

TLongSetFace::TLongSetFace(TWindow* parent, int resourceId, const char *name, uint textLimit)
:
	TStringFace(parent, resourceId, name, textLimit)
{
	SetValidator(new TLongSetValidator());
}

TLongSetFace::~TLongSetFace()
{
}
