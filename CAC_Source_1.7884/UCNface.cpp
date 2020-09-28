//{{TUCNFace Implementation}}

TUCNFace::TUCNFace(TWindow* parent, int resourceId, const char *name, char type)
:
	TStringFace(parent, resourceId, name, SIZE_OF_UCN)
{
	SetValidator(new TUCNValidator(type));
}

TUCNFace::TUCNFace(TWindow* parent, int resourceId, const char *name, TCharAliasFace *face)
:
	TStringFace(parent, resourceId, name, SIZE_OF_UCN)
{
	SetValidator(new TUCNValidator(face));
}
