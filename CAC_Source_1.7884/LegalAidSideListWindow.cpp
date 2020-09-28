//{{TLegalAidSideListWindow Implementation}}

TLegalAidSideListWindow::TLegalAidSideListWindow(TWindow* parent, int resourceId, TGArray<TLegalSide> *laids)
:
	TTryAddressSideListWindow(parent, resourceId, laids, 432)
{
	Add(new TGroupListColumn("", group_print, 420));
}

void TLegalAidSideListWindow::Lawer(bool set)
{
	char *specifiedLawer = ((TLegalSide *) Group())->specifiedLawer;

	if (set)
	{
		TTransGroup ucnGroup;

		if (get_ucn(this, &ucnGroup, UCN_CITIZENS, C2S[INVOLVEMENT_LAWER], true))
		{
			TCitizen citizen;
			citizen << ucnGroup;
			print_user(specifiedLawer, &citizen, false, false);
		}
	}
	else if (ask("Желаете ли да изтриете посочения от лицето адвокат?"))
		*specifiedLawer = '\0';

	DrawGroupAt(Group(), GetVisualIndex(), false);
}

void TLegalAidSideListWindow::DrawSecond(TListWindItem &lwi, TGroup *group)
{
	mstr m;
	char *specifiedLawer = ((TLegalSide *) group)->specifiedLawer;

	lwi.iSubItem = 1;
	if (any(specifiedLawer))
		m.printf("    Адвокат по чл.25, ал.5 ЗПП %s", specifiedLawer);
	lwi.SetText(str(m));
	SetItem(lwi);
}
