//{{TBankruptListBox Implementation}}

TBankruptListBox::TBankruptListBox(TWindow* parent, int resourceId, TGArray<TBankrupt> *bankrupts, int tSelect)
:
	TGroupListBox(parent, resourceId, bankrupts), select(tSelect)
{
	sortcol = 0;
}

TGroup *TBankruptListBox::NewGroup() const
{
	return new TBankrupt;
}

void TBankruptListBox::SetupWindow()
{
	TGroupListBox::SetupWindow();

	if (select == BANKRUPT_SELECT_ALL)
		SetSelItemRange(true, 0, array->Count() - 1);
	else if (select >= 0)
		SetSel(select, true);
}
