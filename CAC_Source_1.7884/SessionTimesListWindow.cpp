//{{TSessionTimesListWindow Implementation}}

TSessionTimesListWindow::TSessionTimesListWindow(TWindow* parent, int resourceId, TGSArray<TSessionTimes> *times,
	bool *tEdited)
:
	TGroupListWindow(parent, resourceId, times, 236), edited(tEdited)
{
	Add(
		new TGroupListColumn("Заседателен ден", start_date, 68),
		new TGroupListColumn("Начален час", start_time, 48),
		new TGroupListColumn("Краен час", "F_ENTIME", 48),
		new TGroupListColumn("Отработени часове", "F_HOURS", 12),
		NULL
	);
}

bool TSessionTimesListWindow::IsValid(mstr &m)
{
	return session_times_valid((TGSArray<TSessionTimes> *) array, m);
}

TGroup *TSessionTimesListWindow::NewGroup() const
{
	return new TSessionTimes;
}

bool TSessionTimesListWindow::EditGroup(TGroup *group)
{
	if (TEditSessionTimesDialog(this, (TSessionTimes *) group, GetVisualIndex(), GetVisualCount()).Execute() == IDCANCEL)
		return false;

	*edited = true;
	return true;
}
