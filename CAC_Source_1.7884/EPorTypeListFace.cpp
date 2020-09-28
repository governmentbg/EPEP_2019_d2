TEPorTypeListFace::TEPorTypeListFace(TWindow *parent, int resourceId, const char *name)
	: TTypeListFace(parent, resourceId, name, TYPE_ORIGINS, false)
{
	strcach(types, TYPE_EMPTY);
}

void TEPorTypeListFace::GetGroupText(mstr &m, TGroup *group, int column)
{
	if (TYPECHECK_PTR_CAST(TType, group)->type == TYPE_EMPTY)
		m.cat("Други обекти");
	else
		TTypeListFace::GetGroupText(m, group, column);
}
