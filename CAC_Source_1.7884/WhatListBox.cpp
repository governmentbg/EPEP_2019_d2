//{{TWhatListBox Implementation}}

TWhatListBox::TWhatListBox(TWindow* parent, int resourceId)
:
	TGroupListBox(parent, resourceId, NULL)
{
	initialRefresh = false;
}

void TWhatListBox::GetMark(TIntegerArray *selected)
{
	selected->Clear();

	for (int index = 0; index < array->Count(); index++)
		if (GetSel(index))
			selected->Add(TYPECHECK_PTR_CAST(TWhat, GroupAt(index))->what);
}

void TWhatListBox::SetLog(bool tLog)
{
	log = tLog;
	Refresh();
}

TGroup *TWhatListBox::NewGroup() const
{
	return new TWhat;
}

void TWhatListBox::Criteria(msql &m, TGroup *group)
{
	m.Begin(group->Name);

	if (log)
		m.AddLongs("F_WHAT", WHAT_LOGS);

	m.AddOrder("F_NAME");
}
