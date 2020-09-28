//{{TEisppLawsListBox Implementation}}

TEisppLawsListBox::TEisppLawsListBox(TWindow* parent, int resourceId, TLawsuit *tLawsuit)
:
	TGroupListBox(parent, resourceId, NULL), lawsuit(tLawsuit)
{
}

void TEisppLawsListBox::Query()
{
	query_group(this, (TRCDKeyGroup *) Group());
}

TGroup *TEisppLawsListBox::NewGroup() const
{
	return new TLawsuit;
}

void TEisppLawsListBox::Criteria(msql &m, TGroup *group)
{
	m.Begin(group->Name);
	m.AddString("F_EISPP_NMR", lawsuit->eispp_nmr);
}
