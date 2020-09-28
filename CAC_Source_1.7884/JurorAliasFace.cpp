//{{TJurorAliasFace Implementation}}

TJurorAliasFace::TJurorAliasFace(TWindow* parent, int resourceId, const char *name, const char *ucn0, const char *ucn1, long flags)
:
	TKnownAliasFace(parent, resourceId, name, INVOLVEMENT_JURORS, flags, NULL)
{
	sortcol = -2;
	loadData = false;
	ucns[0] = ucn0 && atob(ucn0) ? ucn0 : NULL;
	ucns[1] = ucn1 && atob(ucn1) ? ucn1 : NULL;
	ucns[2] = NULL;
}

TJurorAliasFace::~TJurorAliasFace()
{
	delete[] include;
}

void TJurorAliasFace::SetUCN(const char *ucn0)
{
	ucns[0] = ucn0;
	ucns[1] = NULL;
	Refresh();
	SetVisualIndex(0);
}

void TJurorAliasFace::Include(const char *ucn)
{
	for (int i = 0; i < 3 && ucns[i]; i++)
		if (!strcmp(ucns[i], ucn))
			return;

	ucns[2] = ucns[1];
	ucns[1] = ucns[0];
	ucns[0] = include = strnew(ucn);
}

void TJurorAliasFace::Refresh()
{
#if RANGECHECK
	bool null = false;

	for (int i = 0; i < 3; i++)
	{
		const char *ucn = ucns[i];

		if (ucn)
		{
			if (null)
				fatal("%s: ucn '%s' after NULL", name, ucn);
			else if (!atob(ucn))
				fatal("%s: empty ucn '%s'", name, ucn);
		}
		else
			null = true;
	}
#endif  // RANGECHECK

	array->Flush();

	for (int i = 0; i < 3 && ucns[i]; i++)
	{
		// 2010:279 LPR: TKnownPerson -> new TKnownAliasGroup
		TKnownAliasGroup *known = (TKnownAliasGroup *) NewGroup();

		strcpy(known->ucn, ucns[i]);
		known->involvement = INVOLVEMENT_JUROR;

		if (known->Try())
			array->Add(known);
		else
			delete known;
	}

	if (InsertEmptyOption())
		array->Add(NewGroup());

	TGroupComboBox::Refresh();
}
