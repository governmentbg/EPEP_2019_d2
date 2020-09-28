//{{TPersonAliasFace Implementation}}

TPersonAliasFace::TPersonAliasFace(TWindow* parent, int resourceId, const char *name, TPersonAliasGroup *alias,
	const char *xCrit, long flags, const char *tInclude)
:
	TStringAliasFace(parent, resourceId, name, alias, xCrit, flags), include(tInclude)
{
	abcskip = ABC_SKIP_LOWER;
}

const char *TPersonAliasFace::S()
{
	return etz(TStringAliasFace::S());
}

void TPersonAliasFace::SetS(const char *s)
{
	TStringAliasFace::SetS(etz(s));
}

void TPersonAliasFace::LoadData(const char *s)
{
	TStringAliasFace::LoadData(s);

	if (include)
	{
		TPersonAliasGroup *person = (TPersonAliasGroup *) NewGroup();
		strcpy(person->ucn, include);

		if (person->Try())
			InsertGroup(person);
		else
			delete person;
	}
}
