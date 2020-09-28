//{{TUserLoadListBox Implementation}}

TUserLoadListBox::TUserLoadListBox(TWindow* parent, int resourceId, TGArray<TUser> *tArray, bool tMulti)
:
	TGroupListBox(parent, resourceId, tArray), multi(tMulti)
{
	dynamic = loadData = false;
}

void TUserLoadListBox::Change(const char *oldJudge, const char *newJudge)
{
	for (int index = 0; index < array->Count(); index++)
	{
		TUser *user = UserAt(index);

		if (!strcmp(user->ucn, oldJudge))
		{
			user->rawload--;
			RedrawVisual(index);
		}

		if (!strcmp(user->ucn, newJudge))
		{
			user->rawload++;
			RedrawVisual(index);
		}
	}
}

void TUserLoadListBox::GetMark(TGArray<TUser> *tUsers)
{
	for (int i = 0; i < GetCount(); i++)
		if (GetSel(i))
			tUsers->Add(UserAt(i));
}

TGroup *TUserLoadListBox::NewGroup() const
{
	return new TUser;
}

void TUserLoadListBox::GetGroupText(mstr &m, TGroup *group, int)
{
	if (multi)
		m.cat(" ");

	((TUser *) group)->PrintLoad(m, false);
}
