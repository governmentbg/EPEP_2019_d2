//{{TXferDeleteListWindow Implementation}}

TXferDeleteListWindow::TXferDeleteListWindow(TWindow* parent, int resourceId)
:
	TGroupListWindow(parent, resourceId, NULL, 260), court(0)
{
	Add(
		new TGroupListColumn("Трансфер", xfer_transfer_type, 76),
		new TGroupListColumn("Вид", xfer_connect_kind, 36),
		new TGroupListColumn("Номер/Година", xfer_no_year, 60, LVCFMT_RIGHT),
		//new TGroupListColumn("Съд", xfer_court, 168),
		new TGroupListColumn("Изх. писмо №/година", xfer_outreg, 76, LVCFMT_RIGHT),
		NULL
	);

	initialRefresh = false;
}

void TXferDeleteListWindow::SetCourt(long tCourt)
{
	if (court != tCourt)
	{
		court = tCourt;
		Refresh();
	}
}

TGroup *TXferDeleteListWindow::NewGroup() const
{
	return new TXferDocument;
}

void TXferDeleteListWindow::Criteria(msql &m, TGroup *group)
{
	m.Begin(group->Name);
	m.AddLong("F_COURT_SENDER", court);
	m.AddOrder("F_YEAR, F_NO, F_XFER_KIND");
}

bool TXferDeleteListWindow::AskDelete(TGroup *)
{
	return ask("Желаете ли да изтриете избраният документ?");
}

bool TXferDeleteListWindow::DeleteGroup(TGroup *group)
{
	TXferDocument *document = (TXferDocument *) group;

	if (xfer_delete(document, "Избраните"))
	{
		// 2010:020 TRQ: log user delete
		document->LogAny("на документ от електронен обмен", "", LOG_DELETE);
		return true;
	}

	return false;
}
