//{{TEisDepartListBox Implementation}}

TEisDepartListBox::TEisDepartListBox(TWindow* parent, int resourceId, char tUCNType, const char *initialName)
:
	TFindAliasListBox(parent, resourceId, EisDepart, &ucnType, initialName), ucnType(tUCNType)
{
	sortcol = 0;
}

void TEisDepartListBox::SetUCNType(char tUCNType)
{
	if (ucnType != tUCNType)
	{
		ucnType = tUCNType;
		Refresh();
	}
}
