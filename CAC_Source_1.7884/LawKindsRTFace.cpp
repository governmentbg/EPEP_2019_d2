//{{TLawKindsRTFace Implementation}}

TLawKindsRTFace::TLawKindsRTFace(TWindow* parent, int resourceId, const char *name)
:
	TCharListFace(parent, resourceId, name, Kind, Default->xKinds, SIZE_OF_KINDS)
{
}

const char *TLawKindsRTFace::S()
{
	TCharListFace::S();

	if (!any(s))
	{
		*s = KIND_INVALID;
		strcpy(s + 1, Default->xKinds);
	}

	return s;
}

void TLawKindsRTFace::Refresh()
{
	TCharListFace::Refresh();

	TKind *kind = new TKind();

	kind->kind = KIND_INVALID;
	strcpy(kind->name, Type->Seek(TYPE_REQUEST));
	array->Insert(kind, 0);
	InsertVisual(0);
	SetCaretIndex(0, false);
}
