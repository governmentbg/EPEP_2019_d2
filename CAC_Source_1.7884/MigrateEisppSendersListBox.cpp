//{{TMigrateEisppSendersListBox Implementation}}

TMigrateEisppSendersListBox::TMigrateEisppSendersListBox(TWindow* parent, int resourceId, char tUCNType)
:
	TMigrateSendersListBox(parent, resourceId, ucnType), ucnType(tUCNType)
{
	sortcol = 1;
}

void TMigrateEisppSendersListBox::SetUCNType(char tUCNType)
{
	if (ucnType != tUCNType)
	{
		ucnType = tUCNType;
		Refresh();
	}
}

void TMigrateEisppSendersListBox::Criteria(msql &m, TGroup *group)
{
	m.Begin(group->Name);
	m.AddChar("F_UCN_TYPE", ucnType);
	m.AddIsNull("F_EISPP", true);
}
