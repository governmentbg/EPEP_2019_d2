//{{TNameFace Implementation}}

TNameFace::TNameFace(TWindow* parent, int resourceId, const char *name, uint textLimit)
:
	TStringFace(parent, resourceId, name, textLimit)
{
}

const char *TNameFace::S()
{
	TStringFace::S();
	compress(buffer);
	return buffer;
}
