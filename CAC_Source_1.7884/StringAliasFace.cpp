//{{TStringAliasFace Implementation}}

TStringAliasFace::TStringAliasFace(TWindow* parent, int resourceId, const char *name, TStringAliasGroup *alias,
	const char *xCrit, long flags, TGroupArray *array)
:
	TAliasFace(parent, resourceId, name, alias, xCrit, flags, array)
{
}

const char *TStringAliasFace::S()
{
	int index = GetVisualIndex();
	return index >= 0 ? StringAt(GetVisualIndex()) : "";
}

void TStringAliasFace::SetS(const char *s)
{
	int index;

	for (index = 0; index < array->Count(); index++)
		if (!strcmp(StringAt(index), s))
			break;

	SetDefaultIndex(index);
}

void TStringAliasFace::Refresh()
{
	mstr m(S());
	TAliasFace::Refresh();
	SetS(str(m));
}
