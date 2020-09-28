#include "cac.h"

static void address_text(mstr &m, TGroup *group)
{
	((TAnyAddress *) group)->PrintExtra();
	m.cat(group->FindExtraText()->S());
	split_at(ncstr(m), 75);
}

#include "EditForeignAddressDialog.h"
#include "EditForeignAddressDialog.cpp"

#include "AddressListWindow.h"
#include "AddressListWindow.cpp"

#include "AddressListDialog.h"
#include "AddressListDialog.cpp"

void view_addresses(TWindow *parent, TUCNGroup *ucnGroup)
{
	TAddressListDialog(parent, ucnGroup, ADDRESS_ALLOW_FOREIGN, IDD_VIEW_ADDRESSES).Execute();
}

#include "SelectAddressDialog.h"
#include "SelectAddressDialog.cpp"

void edit_addresses(TWindow *parent, TUCNGroup *ucnGroup, int addrMode)
{
	TSelectAddressDialog(parent, ucnGroup, NULL, addrMode, false, IDD_EDIT_ADDRESSES).Execute();
}

TAnyAddress *get_address(TWindow *parent, TUCNGroup *ucnGroup, int addrMode)
{
	TAnyAddress *address;
	int result = TSelectAddressDialog(parent, ucnGroup, &address, addrMode, true, IDD_SELECT_ADDRESS).Execute();
	return result == IDOK ? address : NULL;
}

TAnyAddress *get_address(TWindow *parent, const char *ucn, char ucnType, int addrMode)
{
	TUCNGroup *const ucnGroup = TNamedGroup::NewGroup(ucnType);
	TVirtPtr VP(ucnGroup);

	strcpy(ucnGroup->ucn, ucn);
	ucnGroup->Get();
	return get_address(parent, ucnGroup, addrMode);
}

bool get_address(TWindow *parent, TSubpoena &subpoena, TUCNGroup *ucnGroup, int addrMode)
{
	TAnyAddress *const address = get_address(parent, ucnGroup, addrMode);

	if (address)
	{
		TVirtPtr VP(address);

		address->Remove("F_UCN");
		address->Remove("F_UCN_TYPE");
		subpoena << address;
		return true;
	}

	return false;
}

bool get_address(TWindow *parent, const char *ucn, char ucnType, TAddress *address)
{
	TAnyAddress *tAddress;

	if ((tAddress = get_address(parent, ucn, ucnType, ADDRESS_BULGARIAN_ONLY)) != NULL)
	{
		*address << tAddress;
		delete tAddress;
	}

	return tAddress != NULL;	// delete does not nullify it
}

bool edit_address(TWindow *parent, TUCNGroup *ucnGroup)
{
	TAddress address;
	int addrMode = ucnGroup->ucnType == UCN_INFORMED ? ADDRESS_REQUIRE_EMAIL : ADDRESS_BULGARIAN_ONLY;

	address.ucnType = ucnGroup->ucnType;
	strcpy(address.ucn, ucnGroup->ucn);
	return TEditAddressDialog(parent, &address, address.Try(), addrMode).Execute() == IDOK;
}

static bool print_addresses(mstr &m, TUCNGroup *ucnGroup, long flags, bool foreign, const char *sep, bool printed)
{
	TAnyAddress *address = TAnyAddress::NewGroup(foreign);
	msql w;

	*address << ucnGroup;
	address->FloatCriteria(w);
	w.AddOrder(foreign ? "F_ADDRESS DESC" : "F_ADDRESS");

	TQuery q(str(w));
	while (q.Read())
	{
		if (printed || (flags & ADDRESS_PREFIXED))
			m.cat(sep);

		*address << q;
		address->Print(m, flags);
		printed = true;

		if (!sep)
			break;
	}

	delete address;
	return printed;
}

bool print_exact_address(mstr &m, TUCNGroup *ucnGroup, long address, bool foreign, long flags)
{
	TAnyAddress *const tAddress = TAnyAddress::NewGroup(foreign);
	TVirtPtr VP(tAddress);
	bool result;

	*tAddress << ucnGroup;
	tAddress->address = address;
	result = tAddress->Try();

	if (result)
		tAddress->Print(m, flags);

	return result;
}

void print_address(mstr &m, TUCNGroup *ucnGroup, long address, long flags)
{
	bool foreign = address < 0;

	if (!print_exact_address(m, ucnGroup, address, foreign, flags) &&
		!print_addresses(m, ucnGroup, flags, foreign, NULL, false))
	{
		print_addresses(m, ucnGroup, flags, !foreign, NULL, false);
	}
}

void print_addresses(mstr &m, TUCNGroup *ucnGroup, const char *sep, long flags)
{
	bool printed = print_addresses(m, ucnGroup, flags, false, sep, false);
	print_addresses(m, ucnGroup, flags, true, sep, printed);
	m.cut(1023);	// _ limitation
}
