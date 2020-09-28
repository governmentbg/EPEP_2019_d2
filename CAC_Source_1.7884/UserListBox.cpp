#include "cac.h"

//{{TUserListBox Implementation}}

TUserListBox::TUserListBox(TWindow* parent, int resourceId, const char *posts, long flags, const char *tExclude)
:
	TAliasListBox(parent, resourceId, UserAliasGroup, posts, flags), exclude(tExclude)
{
	abcskip = ABC_SKIP_LOWER;
}

void TUserListBox::GetMark(TGroupArray *tArray)
{
	if (GetSelCount() > 0 || ((flags & USER_VIMIT) && (!IsWindowVisible() || COURT_CODE == COURT_BRKFC)) ||
		(flags & USER_NOLIMIT))
	{
		TAliasListBox::GetMark(tArray);
	}
	else
	{
		tArray->Flush();

		// 2016:032 LPR/IRQ: POST_JUDGES -> VISIBLE
		if ((intersect(xCrit, POST_VISIBLE_JUDGES) && strchr(POST_JUDGES, Default->post) &&
			Default->post != POST_CHIEF) || (strchr(xCrit, POST_DELIVERER) && Default->post == POST_DELIVERER))
		{
			for (int index = 0; index < array->Count(); index++)
				tArray->AddCopy(GroupAt(index));
		}
	}
}

void TUserListBox::SetMark(TGArray<TUserAliasGroup> *users)
{
	for (int i = 0; i < users->Count(); i++)
		for (int j = 0; j < array->Count(); j++)
			if (!strcmp((*users)[i]->ucn, UserAt(j)->ucn))
				SetSel(j, true);
}

void TUserListBox::LoadData(const char *s)
{
	TAliasListBox::LoadData(s);

	// 2002:289 LRQ: INTER support; 2002:317 LRQ: For admins only
	if ((flags & USER_INTER) && Default->post == POST_ADMIN)
	{
		TUserAliasGroup *user = (TUserAliasGroup *) NewGroup();
		strcpy(user->ucn, INTER);

		if (user->Get())
			InsertGroup(user);
		else
			delete user;
	}

	for (int index = 0; index < array->Count(); index++)
	{
		if (!strcmp(UserAt(index)->ucn, exclude))
		{
			RemoveVisual(index);
			array->Remove(index);
			break;
		}
	}
}
