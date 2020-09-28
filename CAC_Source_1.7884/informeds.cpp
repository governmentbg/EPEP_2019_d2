#include "cac.h"

// ----- query --------------------------------------------------------------
#define sender OFFICE_GROUP_CAST(TSender)

static void sender_name(mstr &m, TGroup *group) { m.cpy(sender->name); }

static void sender_email(mstr &m, TGroup *group)
{
	TAddress address;

	address.ucnType = sender->ucnType;
	strcpy(address.ucn, sender->ucn);
	m.cpy(address.Try() ? address.email : "");
}

static const offield sender_fields[] =
{
	{ "F_NAME", sender_name },
	{ "F_EMAIL", sender_email },
	{ NULL, NULL }
};

#undef sender

static const offield informed_fields[] =
{
	{ "F_TYPE", keygroup_key_type },
	{ "F_NO", keygroup_key_no },
	{ "F_YEAR", keygroup_key_year },
	{ NULL, NULL }
};

static void print_informeds(TWindow *parent, TGroupArray *array, const char *informed)
{
	TOffice office;

	if (office.Open("LawMail.htm"))
	{
		TWaitWindow wait(parent, WAIT);
		constant tr1start = office.Find("tr1 start");
		constant tr1final = office.Find("tr1 final");
		TSender sender;

		sender.ucnType = UCN_INFORMED;
		strcpy(sender.ucn, informed);
		sender.Try();

		office.Emit(NULL, tr1start, sender_fields, &sender);

		for (int i = 0; i < array->Count(); i++)
			office.Emit(tr1start, tr1final, informed_fields, (*array)[i]);

		office.Emit(tr1final, NULL, null_fields, NULL);
		office.Show(&wait);
	}
}

// ----- informeds ----------------------------------------------------------
#include "EditInformedDialog.h"
#include "EditInformedDialog.cpp"

#include "InformedListWindow.h"
#include "InformedListWindow.cpp"

#include "InformedListDialog.h"
#include "InformedListDialog.cpp"

void edit_informeds(TWindow *parent)
{
	TInformedListDialog(parent).Execute();
}
