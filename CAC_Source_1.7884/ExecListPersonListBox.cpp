//{{TExecListPersonListBox Implementation}}

TExecListPersonListBox::TExecListPersonListBox(TWindow* parent, int resourceId, TGArray<TExecListPerson> *allPersons,
	bool tPrivList)
:
	TGroupListBox(parent, resourceId, allPersons), privList(tPrivList)
{
	loadData = dynamic = false;
	// copied from legal aids
	if (privList)
		for (int i = 0; i < allPersons->Count(); i++)
			(*allPersons)[i]->TryAddress();
}

void TExecListPersonListBox::SetMark(TGArray<TExecListPerson> *persons)
{
	for (int i = 0; i < persons->Count(); i++)
	{
		TExecListPerson *person = (*persons)[i];

		for (int n = 0; n < array->Count(); n++)
			if (!strcmp(person->ucn, PersonAt(n)->ucn) && person->ucnType == PersonAt(n)->ucnType)
				SetSel(n, true);
	}
}

void TExecListPersonListBox::GetMark(TGArray<TExecListPerson> *persons)
{
	persons->Flush();

	for (int i = 0; i < array->Count(); i++)
	{
		if (GetSel(i))
		{
			TExecListPerson *person = PersonAt(i);
			TExecListPerson *tPerson = new TExecListPerson;

			*tPerson << person;

			if (person->address)
				tPerson->SetAddress(person->address, person->address->address < 0);

			persons->Add(tPerson);
		}
	}
}

void TExecListPersonListBox::Address()
{
	int index;

	for (index = 0; index < array->Count(); index++)
	{
		if (GetSel(index))
		{
			TExecListPerson *person = PersonAt(index);
			TAnyAddress *address = get_address(this, person->ucn, person->ucnType, ADDRESS_ALLOW_FOREIGN);

			if (address)
			{
				for (int i = 0; i < array->Count(); i++)
				{
					TExecListPerson *tPerson = PersonAt(i);

					if (!strcmp(tPerson->ucn, person->ucn) && tPerson->ucnType == person->ucnType)
					{
						TAnyAddress *tAddress = TAnyAddress::NewGroup(address->address < 0);

						*tAddress << address;
						delete tPerson->address;
						tPerson->address = tAddress;
						RedrawVisual(i);
						SetSel(i, true);
					}
				}

				delete address;
			}
			// multisel, but only 1 selected
			break;
		}
	}
}

bool TExecListPersonListBox::IsValid()
{
	bool isValid = TGroupListBox::IsValid();

	if (isValid)
	{
		for (int i = 0; i < array->Count() - 1; i++)
		{
			if (!GetSel(i))
				continue;

			TExecListPerson *person = PersonAt(i);

			for (int n = i + 1; n < array->Count(); n++)
			{
				if (GetSel(n) && !strcmp(person->ucn, PersonAt(n)->ucn) &&
					person->ucnType == PersonAt(n)->ucnType)
				{
					mstr m;
					person->Print(m);
					isValid = error("Лицето %s не може да бъде длъжник два пъти за една и съща сума.", str(m));
					break;
				}
			}
		}
	}

	return isValid;
}

TGroup *TExecListPersonListBox::NewGroup() const
{
	return new TExecListPerson;
}

void TExecListPersonListBox::GetGroupText(mstr &m, TGroup *group, int)
{
	TSideWind sideWind;
	sideWind << group;
	sideWind.Print(m);

	TAnyAddress *address = TYPECHECK_PTR_CAST(TExecListPerson, group)->address;

	if (address)
	{
		size_t len = m.len();

		m.cat(" - ");
		address->Print(m, ADDRESS_DISTINCT);
		m.compress(len);
	}
}
