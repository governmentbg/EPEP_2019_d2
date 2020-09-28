#include "cac.h"

//{{TTryAddressSideListWindow Implementation}}

TTryAddressSideListWindow::TTryAddressSideListWindow(TWindow* parent, int resourceId, TGroupArray *array, int width)
:
	TDoubleListWindow(parent, resourceId, array, width)
{
	Add(new TGroupListColumn("", generic_null, 0));
}

void TTryAddressSideListWindow::Address()
{
	TTryAddress *tryAddress = dynamic_cast<TTryAddress *> (Group());
	TAnyAddress *address;

	if (tryAddress && (address = get_address(this, (TBaseWind *) Group(), ADDRESS_ALLOW_FOREIGN)) != NULL)
	{
		delete tryAddress->address;
		tryAddress->address = address;
		DrawGroupAt(Group(), GetVisualIndex(), false);
	}
}

bool TTryAddressSideListWindow::IsValid()
{
	for (int i = 0; i < array->Count(); i++)
	{
		TBaseWind *baseWind = (TBaseWind *) GroupAt(i);
		TTryAddress *tryAddress = dynamic_cast<TTryAddress *> (baseWind);

		if (!tryAddress->address)
		{
			mstr m;
			baseWind->PrintNames(m);
			return error("Няма избран адрес за %s.", str(m));
		}
	}

	return true;
}

void TTryAddressSideListWindow::SetupWindow()
{
	TDoubleListWindow::SetupWindow();
	Refresh(GetClientRect().Width());
}
