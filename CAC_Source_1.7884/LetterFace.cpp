//{{TLetterFace Implementation}}

TLetterFace::TLetterFace(TWindow* parent, int resourceId, const char *name)
:
	TNameFace(parent, resourceId, name, 2)
{
	SetValidator(new TNameValidator);
}
