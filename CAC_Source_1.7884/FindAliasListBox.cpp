//{{TFindAliasListBox Implementation}}

TFindAliasListBox::TFindAliasListBox(TWindow* parent, int resourceId, TAliasGroup *alias, const char *xCrit, const char *tInitialName)
:
	TAliasListBox(parent, resourceId, alias, xCrit, EISA_NAMED), initialName(tInitialName)
{
	initialRefresh = initialName != NULL;
}

void TFindAliasListBox::Search(const char *name, bool extra)
{
	if (strlen(name) + isdigit(*name) >= 3)
	{
		alias->NameField->SetS(name);

		if (extra)
			flags |= DEPART_EXTRA;
		else
			flags &= ~DEPART_EXTRA;

		TAliasListBox::Refresh();
	}
	else
		error("Критерия за търсене е невалиден или прекалено общ. Коригирайте го.");
}

void TFindAliasListBox::SetupWindow()
{
	if (initialRefresh)
		alias->NameField->SetS(initialName);

	TGroupListBox::SetupWindow();		// no default sorting
}
