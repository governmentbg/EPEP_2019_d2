//{{TSLCReasonListWindow Implementation}}

TSLCReasonListWindow::TSLCReasonListWindow(TWindow* parent, int resourceId)
:
	TGroupListWindow(parent, resourceId, NULL, 304), slcType('\0')
{
	Add(
		new TGroupListColumn("", group_inactive, 12, LVCFMT_CENTER | LVCFMT_NO_SEARCH),
		new TGroupListColumn("", slc_reason_book, 12, LVCFMT_CENTER | LVCFMT_NO_SEARCH),
		new TGroupListColumn("Име", "F_NAME", 280),
		NULL
	);

	initialRefresh = false;
}

void TSLCReasonListWindow::SetSLCType(char tSLCType)
{
	if (slcType != tSLCType)
	{
		slcType = tSLCType;
		Refresh();
	}
}

void TSLCReasonListWindow::Print()
{
	TSLCReason reason;

	reason.slcType = slcType;
	print_slc_reasons(&reason, array);
}

TGroup *TSLCReasonListWindow::NewGroup() const
{
	TSLCReason *reason = new TSLCReason;
	reason->slcType = slcType;
	return reason;
}

void TSLCReasonListWindow::Criteria(msql &m, TGroup *group)
{
	group->FloatCriteria(m);
	m.AddOrder("F_ORDER");
}

bool TSLCReasonListWindow::AddGroup(TGroup *group)
{
	return TEditSLCReasonDialog(this, TYPECHECK_PTR_CAST(TSLCReason, group), false).Execute() == IDOK;
}

bool TSLCReasonListWindow::EditGroup(TGroup *group)
{
	return TEditSLCReasonDialog(this, TYPECHECK_PTR_CAST(TSLCReason, group), true).Execute() == IDOK;
}

bool TSLCReasonListWindow::AskDelete(TGroup *group)
{
	static constant tables[] = { "T_SESSION", "T_DECISION", NULL };
	const char *table = check_exists(tables, group, "F_SLC_REASON");

	if (table)
		return error("Основанието се използва в %s.", table);

	return TGroupListWindow::AskDelete(group);
}
