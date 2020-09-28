//{{TGetContainerAliasFace Implementation}}

TGetContainerAliasFace::TGetContainerAliasFace(TWindow* parent, int resourceId, const char *name, const char *tKindsMax)
:
	TCharAliasFace(parent, resourceId, name, ConnectKind, KIND_V_EMPTIES), kindsMax(tKindsMax)
{
}

void TGetContainerAliasFace::SetType(char type)
{
	compute_kinds(type, kindsMax, kinds);

	if (!any(kinds))
		strcach(kinds, KIND_V_EMPTY);

	SetXCrit(kinds);
}
