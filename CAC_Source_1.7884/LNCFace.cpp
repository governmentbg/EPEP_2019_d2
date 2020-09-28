//{{TLNCFace Implementation}}

TLNCFace::TLNCFace(TWindow* parent, int resourceId, const char *name)
:
	TStringFace(parent, resourceId, name, SIZE_OF_UCN)
{
	SetValidator(new TLNCValidator());
}

TLNCFace::~TLNCFace()
{
}
