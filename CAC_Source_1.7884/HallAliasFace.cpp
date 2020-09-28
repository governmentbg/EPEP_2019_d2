//{{THallAliasFace Implementation}}

THallAliasFace::THallAliasFace(TWindow* parent, int resourceId, const char *name, TIntegerArray *halls)
:
	TLongAliasFace(parent, resourceId, name, Hall, NULL, halls ? FLAG_NULL : FLAG_ACTIVE | ALIAS_EMPTY)
{
	sortcol = 0;

	if (halls)
	{
		for (int i = 0; i < halls->Count(); i++)
		{
			THall *hall = new THall;

			hall->hall = (*halls)[i];

			if (hall->Get())
				array->Add(hall);
			else
				delete hall;
		}
	}
}
