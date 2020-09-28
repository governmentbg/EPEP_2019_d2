//{{TCitizenNameFace Implementation}}

TCitizenNameFace::TCitizenNameFace(TWindow* parent, int resourceId, const char *name, uint textLimit)
:
	TNameFace(parent, resourceId, name, textLimit)
{
	SetValidator(new TCitizenNameValidator);
}

TCitizenNameFace::~TCitizenNameFace()
{
}
