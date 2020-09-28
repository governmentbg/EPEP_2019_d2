//{{TTransIdFace Implementation}}

TTransIdFace::TTransIdFace(TWindow* parent, int resourceId, const char *name, bool partial)
:
	TStringFace(parent, resourceId, name, SIZE_OF_TRANS_ID)
{
	SetValidator(new TTransIdValidator(partial));
}
