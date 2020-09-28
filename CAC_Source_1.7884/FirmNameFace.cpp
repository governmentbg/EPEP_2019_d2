//{{TFirmNameFace Implementation}}

TFirmNameFace::TFirmNameFace(TWindow* parent, int resourceId, const char *name, uint textLimit)
:
	TNameFace(parent, resourceId, name, textLimit)
{
	SetValidator(new TFirmNameValidator);
}
