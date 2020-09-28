//{{TIndexFace Implementation}}

TIndexFace::TIndexFace(TWindow* parent, int resourceId, const char *name, const char *tKinds, uint textLimit)
:
	TLongFace(parent, resourceId, name, textLimit), kinds(tKinds)
{
}

TIndexFace::~TIndexFace()
{
}

void TIndexFace::SetupWindow()
{
	TStringFace::SetupWindow();

	Transfer("1", tdSetData);
	SetValidator(new TIndexValidator(kinds));
}
