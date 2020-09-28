//{{TBulStatFace Implementation}}

TBulStatFace::TBulStatFace(TWindow* parent, int resourceId, const char *name)
:
	TStringFace(parent, resourceId, name, SIZE_OF_BULSTAT)
{
	SetValidator(new TBulStatValidator());
}
