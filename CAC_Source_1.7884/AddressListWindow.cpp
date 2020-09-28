//{{TAddressListWindow Implementation}}

TAddressListWindow::TAddressListWindow(TWindow* parent, int resourceId, TUCNGroup *tUCNGroup, int tAddrMode)
:
	TDoubleListWindow(parent, resourceId, &addresses, 340), ucnGroup(tUCNGroup), addrMode(tAddrMode)
{
	Add(new TGroupListColumn("", generic_null, 0));
	Add(new TGroupListColumn("", address_text, 328));

	TAnyAddress::LoadAddresses(ucnGroup, &addresses);
}

TAddressListWindow::~TAddressListWindow()
{
	addresses.DeleteExtraText();
}

TAnyAddress *TAddressListWindow::OK()
{
	int index = GetVisualIndex();
	TAnyAddress *address = NULL;

	if (index >= 0)
	{
		TAnyAddress *tAddress = AddressAt(index >> 1);
		const char *status;

		if (tAddress->address > 0 && (status = ((TAddress *) tAddress)->Match(addrMode)) != NULL)
			error("%s.", status);
		else
		{
			address = tAddress;
			address->DeleteExtraText();
			addresses.Detach(index >> 1);
		}
	}

	return address;
}

void TAddressListWindow::Add(bool foreign)
{
	TAnyAddress *address = TAnyAddress::NewGroup(foreign);
	int result;

	*address << ucnGroup;

	// avoid destructor in conditional expression
	if (foreign)
		result = TEditForeignAddressDialog(this, (TForeignAddress *) address, false).Execute();
	else
		result = TEditAddressDialog(this, (TAddress *) address, false, addrMode).Execute();

	if (result == IDOK)
	{
		InsertGroup(address);
		SetFocus();
	}
	else
		delete address;
}

void TAddressListWindow::Edit()
{
	int index = GetVisualIndex();

	if (index >= 0)
	{
		TAnyAddress *address = AddressAt(index >> 1);
		bool foreign = address->address < 0;
		TAnyAddress *const tAddress = TAnyAddress::NewGroup(foreign);
		TVirtPtr VP(tAddress);
		int result;

		*tAddress << address;

		// avoid destructor in conditional expression
		if (foreign)
			result = TEditForeignAddressDialog(this, (TForeignAddress *) tAddress, true).Execute();
		else
			result = TEditAddressDialog(this, (TAddress *) tAddress, true, addrMode).Execute();

		if (result == IDOK)
		{
			*address << tAddress;
			DrawGroupAt(address, index, false);
		}
	}
}

void TAddressListWindow::Delete(int command)
{
	TAnyAddress *address = (TAddress *) Group();

	if (address && address->address != 1 &&
		ask("Желаете ли да изтриете избрания %sадрес?", address->address < 0 ? "чуждестранен " : "") &&
		address->Delete(true))
	{
		address->DeleteExtraText();
		RemoveGroup(command);
	}
}

int TAddressListWindow::Count(bool foreign)
{
	int count = 0;

	for (int index = 0; index < array->Count(); index++)
		if ((AddressAt(index)->address < 0) == foreign)
			count++;

	return count;
}

void TAddressListWindow::DrawSecond(TListWindItem &lwi, TGroup *group)
{
	mstr m;

	lwi.iSubItem = 1;
	m.cat(group->FindExtraText()->S());
	lwi.SetText(split_at(ncstr(m), 75));
	SetItem(lwi);
}

void TAddressListWindow::SetupWindow()
{
	TDoubleListWindow::SetupWindow();
	Refresh(GetClientRect().Width());
}
