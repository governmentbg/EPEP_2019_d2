#include "cac.h"

#include "EditCountryDialog.h"
#include "EditCountryDialog.cpp"

#include "CountryListWindow.h"
#include "CountryListWindow.cpp"

#include "CountryListDialog.h"
#include "CountryListDialog.cpp"

void edit_countries(TWindow *parent)
{
	log(LOG_NOM, CM_ADMIN_COUNTRIES, "държави");
	TCountryListDialog(parent).Execute();
	Country->ReloadCache();
}
