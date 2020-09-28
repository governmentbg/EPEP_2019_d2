#include "cac.h"

#include "EditCurrencyDialog.h"
#include "EditCurrencyDialog.cpp"

static const offield currency_fields[] =
{
	{ "F_CURRENCY",	NULL },
	{ "F_NAME",		NULL },
	{ NULL, NULL }
};

static void print_currences(TGroupArray *currences)
{
	TOffice office;

	if (office.Open("Currences.htm"))
	{
		constant tr1start = office.Find("tr1 start");
		constant tr1final = office.Find("tr1 final");

		office.Emit(NULL, tr1start, null_fields, NULL);

		for (int i = 0; i < currences->Count(); i++)
			office.Emit(tr1start, tr1final, currency_fields, (*currences)[i]);

		office.Emit(tr1final, NULL, null_fields, NULL);
		office.Show(NULL);
	}
}

#include "CurrencyListWindow.h"
#include "CurrencyListWindow.cpp"

#include "CurrencyListDialog.h"
#include "CurrencyListDialog.cpp"

void edit_currences(TWindow *parent)
{
	log(LOG_NOM, CM_ADMIN_COUNTRIES, "валути");
	TCurrencyListDialog(parent).Execute();
	Currency->ReloadCache();
}
