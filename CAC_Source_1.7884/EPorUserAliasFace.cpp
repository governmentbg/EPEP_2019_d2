//{{TEPorUserAliasFace Implementation}}

TEPorUserAliasFace::TEPorUserAliasFace(TWindow* parent, int resourceId, const char *name, long flags)
	: TUserAliasFace(parent, resourceId, name, NULL, flags | ALIAS_EMPTY | USER_INSECT)
{
}

void TEPorUserAliasFace::LoadData(const char *s)
{
	TUserAliasFace::LoadData(s);

	for (int index = 0; index < array->Count(); )
	{
		TUserAliasGroup *user = TYPECHECK_PTR_CAST(TUserAliasGroup, GroupAt(index));

		if (atob(user->ucn) && !strchr(user->functions, RIGHT_EPORT_RES))
		{
			RemoveVisual(index);
			array->Remove(index);
		}
		else
			index++;
	}
}
