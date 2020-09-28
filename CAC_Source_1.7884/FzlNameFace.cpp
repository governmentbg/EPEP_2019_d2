//{{TFzlNameFace Implementation}}

TFzlNameFace::TFzlNameFace(TWindow* parent, int resourceId, const char *name, sign cyrillic, const char *errorMsg)
:
	TNameFace(parent, resourceId, name, SIZE_OF_EISPP_STRING)
{
	SetValidator(new TFzlNameValidator(cyrillic, errorMsg));
}
