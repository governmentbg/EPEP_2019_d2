//{{TKeyWordsFace Implementation}}

TKeyWordsFace::TKeyWordsFace(TWindow* parent, int resourceId, const char *name)
:
	TStringFace(parent, resourceId, name, SIZE_OF_EXPANDED_KEYWORDS)
{
	SetValidator(new TKeyWordsValidator());
}
