//{{TSelectSubpoenaListBox Implementation}}

TSelectSubpoenaListBox::TSelectSubpoenaListBox(TWindow* parent, int resourceId, TGArray<TSubpoena> *subpoenas)
:
	TCloneListBox(parent, resourceId, subpoenas, TSubpoena::Clone)
{
	loadData = dynamic = false;
}

void TSelectSubpoenaListBox::Select(const char *ucn, char ucnType)
{
	TCloneListBox::SetupWindow();

	for (int i = 0; i < array->Count(); i++)
	{
		TSubpoena *subpoena = static_cast<TSubpoena *> ((*array)[i]);

		if (!strcmp(subpoena->ucn, ucn) && subpoena->ucnType == ucnType)
		{
			SetSelIndex(i);
			break;
		}
	}
}

void TSelectSubpoenaListBox::GetGroupText(mstr &m, TGroup *group, int)
{
	TSubpoena *subpoena = static_cast<TSubpoena *> (group);
	print_name(m, subpoena->ucn, subpoena->ucnType);
}
