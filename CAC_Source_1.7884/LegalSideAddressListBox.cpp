//{{TLegalSideAddressListBox Implementation}}

TLegalSideAddressListBox::TLegalSideAddressListBox(TWindow* parent, int resourceId, TGArray<TLegalSide> &legalSides)
:
	TGroupListBox(parent, resourceId, legalSides)
{
	for (int i = 0; i < legalSides.Count(); i++)
		legalSides[i]->TryAddress();
}

bool TLegalSideAddressListBox::IsValid()
{
	bool isValid = TGroupListBox::IsValid();

	if (isValid)
	{
		for (int i = 0; i < array->Count(); i++)
		{
			TLegalSide *legalSide = (TLegalSide *) (*array)[i];

			if (!legalSide->address)
			{
				mstr m;

				legalSide->PrintNames(m);
				isValid = error("Не е избран адрес за %s.", str(m));
				break;
			}
		}
	}

	return isValid;
}

void TLegalSideAddressListBox::Address()
{
	TLegalSide *legalSide = (TLegalSide *) Group();
	TAnyAddress *address;

	if (legalSide && (address = get_address(this, legalSide, ADDRESS_BULGARIAN_ONLY)) != NULL)
	{
		delete legalSide->address;
		legalSide->address = address;
		RedrawVisual(GetVisualIndex());
	}
}
