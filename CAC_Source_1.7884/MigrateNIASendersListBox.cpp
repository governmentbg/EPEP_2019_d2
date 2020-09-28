//{{TMigrateNIASendersListBox Implementation}}

TMigrateNIASendersListBox::TMigrateNIASendersListBox(TWindow* parent, int resourceId, bool tSource)
:
	TMigrateSendersListBox(parent, resourceId, UCN_NIA_TDEP), source(tSource)
{
}

void TMigrateNIASendersListBox::Criteria(msql &m, TGroup *group)
{
	m.Begin(group->Name);
	m.AddChars("F_UCN_TYPE", ucnTypes);
	m.Add("F_UCN");
	m.printf("%s '0000000100'", source ? "<" : ">=");
}
