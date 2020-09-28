#include "cac.h"

#include "EditAccountDialog.h"
#include "EditAccountDialog.cpp"

#define account OFFICE_GROUP_CAST(TAccount)

static void account_type(mstr &m, TGroup *group) { m.cat(AccountType->Seek(account->accountType)); }

#undef account

static const offield account_fields[] =
{
	{ "F_ACCOUNT_TYPE",	account_type },
	{ "F_NAME",			NULL },
	{ "F_BANK_ACCOUNT",	NULL },
	{ "F_BIC",			NULL },
	{ "F_BANK_NAME",		NULL },
	{ NULL, NULL }
};

static void print_accounts(TGroupArray *accounts)
{
	TOffice office;

	if (office.Open("Accounts.htm"))
	{
		constant tr1start = office.Find("tr1 start");
		constant tr1final = office.Find("tr1 final");

		office.Emit(NULL, tr1start, null_fields, NULL);

		for (int i = 0; i < accounts->Count(); i++)
			office.Emit(tr1start, tr1final, account_fields, (*accounts)[i]);

		office.Emit(tr1final, NULL, null_fields, NULL);
		office.Show(NULL);
	}
}

#include "AccountListWindow.h"
#include "AccountListWindow.cpp"

#include "AccountListDialog.h"
#include "AccountListDialog.cpp"

void edit_accounts(TWindow *parent)
{
	log(LOG_NOM, CM_ADMIN_BANK_ACCOUNTS, "банкови сметки");
	TAccountListDialog(parent).Execute();
}
