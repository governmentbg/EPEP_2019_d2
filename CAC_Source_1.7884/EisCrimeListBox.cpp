//{{TEisCrimeListBox Implementation}}

TEisCrimeListBox::TEisCrimeListBox(TWindow* parent, int resourceId, long tCode)
:
	TFindAliasListBox(parent, resourceId, EisCrime, NULL, NULL), code(tCode)
{
	sortcol = 0;	// 2012:353 IRQ: alphabetical order
	initialRefresh = false;
}

void TEisCrimeListBox::SetupWindow()
{
	TFindAliasListBox::SetupWindow();

	if (code)
	{
		TGroup *crime = NewGroup();

		EisCrime->Seek(code);
		*crime << EisCrime;
		SetVisualIndex(InsertGroup(crime));
	}
}
