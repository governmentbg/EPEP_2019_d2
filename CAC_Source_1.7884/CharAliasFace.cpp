//{{TCharAliasFace Implementation}}

TCharAliasFace::TCharAliasFace(TWindow* parent, int resourceId, const char *name, TCharAliasGroup *alias, const char *xCrit,
	long flags)
:
	TAliasFace(parent, resourceId, name, alias, xCrit, flags)
{
}

char TCharAliasFace::C()
{
	int index = GetVisualIndex();
	return index >= 0 ? CharAt(index) : '\0';
}

void TCharAliasFace::SetC(char c)
{
	int index;

	for (index = 0; index < array->Count(); index++)
		if (CharAt(index) == c)
			break;

	SetDefaultIndex(index);
}

void TCharAliasFace::Refresh()
{
	char c = C();
	TAliasFace::Refresh();
	SetC(c);
}
