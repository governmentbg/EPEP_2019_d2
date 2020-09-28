#include "cac.h"

#include "EditUserDialog.h"
#include "EditUserDialog.cpp"

// 2010:285 LRQ: copy support
static bool add_user(TWindow *parent, TUser *user, TUser *copy)
{
	TCitizen citizen;

	if (get_ucn(parent, &citizen, C2S[UCN_CITIZEN_UCN], true))
	{
		strcpy(user->ucn, citizen.ucn);
		strcpy(user->login, citizen.ucn);

		if (copy)
		{
			user->kind = copy->kind;
			strcpy(user->menus, copy->menus);
			strcpy(user->functions, copy->functions);
			strcpy(user->colleges, copy->colleges);
			strcpy(user->compositions, copy->compositions);
		}
		else
			user->kind = *KIND_LAWSUITS;

		// 2006:080 LPR: default password is ucn
		calc_pass(user, user->ucn);

		return user->Insert();
	}

	return false;
}

#define user OFFICE_GROUP_CAST(TUser)

static void user_name(mstr &m, TGroup *group) { print_user(m, user->ucn); }

static void user_ucn(mstr &m, TGroup *group)
{
	if (is_real_ucn(UCN_CITIZEN_UCN, user->ucn))
		m.cat(user->ucn);
}

static void user_address(mstr &m, TGroup *group)
{
	TFoundGroup ucnGroup;
	ucnGroup << user;
	print_address(m, &ucnGroup, 1, ADDRESS_DISTINCT);
}

static void user_title(mstr &m, TGroup *group)
{
	m.printf("%s - %s", Post->Seek(user->post), user->flags & FLAG_ACTIVE ? "активни" : "всички");
}

#undef user

static const offield user_fields[] =
{
	{ "F_NAME",		user_name },
	{ "F_UCN",		user_ucn },
	{ "F_ADDRESS",	user_address },
	{ "F_TITLE",	user_title },
	{ NULL,		NULL}
};

static void print_users(TWindow *parent, TGroupArray *array, char post, long active)
{
	TOffice office;

	if (office.Open("UserList.htm"))
	{
		TWaitWindow wait(parent, WAIT);
		constant tr1start = office.Find("tr1 start");
		constant tr1final = office.Find("tr1 final");
		TUser user;

		user.post = post;
		user.flags = active;
		office.Emit(NULL, tr1start, user_fields, &user);

		for (int i = 0; i < array->Count(); i++)
		{
			user << (*array)[i];

			if ((user.flags & FLAG_ACTIVE) >= active)
			{
				office.Emit(tr1start, tr1final, user_fields, &user);
				wait.TickCount(i + 1, "обработени лица");
			}
		}

		office.Emit(tr1final, NULL, null_fields, NULL);
		office.Show(&wait);
	}
}

#include "UserListWindow.h"
#include "UserListWindow.cpp"

#include "UserListDialog.h"
#include "UserListDialog.cpp"

void edit_users(TWindow *parent)
{
	log(LOG_NOM, CM_ADMIN_USERS, "потребители");
	TUserListDialog(parent).Execute();
	UserAliasGroup->ReloadCache();
}
