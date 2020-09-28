//{{TUserAliasFace Implementation}}

TUserAliasFace::TUserAliasFace(TWindow* parent, int resourceId, const char *name, const char *xCrit, long flags,
	const char *tExclude)
:
	TPersonAliasFace(parent, resourceId, name, UserAliasGroup, xCrit, flags | USER_NOLIMIT,
		flags & USER_INTER ? INTER : NULL), exclude(tExclude)
{
}

void TUserAliasFace::SetInter(bool inter)
{
	if (inter)
	{
		flags |= USER_INTER;
		include = INTER;
	}
	else
	{
		flags &= ~USER_INTER;
		include = NULL;
	}
}

void TUserAliasFace::LoadData(const char *s)
{
	TPersonAliasFace::LoadData(s);

	int index = find_person(array, exclude);

	if (index != INT_MAX)
	{
		RemoveVisual(index);
		array->Remove(index);

		if (!array->Count() && (flags & ALIAS_OPTION))
			InsertEmpty();
	}
}
