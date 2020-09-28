//{{TGrjChjAliasFace Implementation}}

TGrjChjAliasFace::TGrjChjAliasFace(TWindow* parent, int resourceId, const char *name, TIntegerArray *tExclude,
	TXMLObject *object)
	: TEisAliasFace(parent, resourceId, name, nmk_grj, object, ALIAS_ABCSORT), exclude(tExclude)
{
}

void TGrjChjAliasFace::LoadData(const char *s)
{
	TEisAliasFace::LoadData(s);

	for (int index = 0; index < array->Count(); )
	{
		TEisVal *country = (TEisVal *) (*array)[index];

		if (exclude->Contains(country->code))
		{
			RemoveVisual(index);
			array->Remove(index);
		}
		else
			index++;
	}
}
