//{{TEisPartFace Implementation}}

TEisPartFace::TEisPartFace(TWindow* parent, int resourceId, const char *name)
:
	TStringFace(parent, resourceId, name, SIZE_OF_EISPP_NMR)
{
	SetValidator(new TEisPartValidator);
}
