//{{TLoadUnitListWindow Implementation}}

TLoadUnitListWindow::TLoadUnitListWindow(TWindow* parent, int resourceId, TLoadGroup *tLoadGroup, TLoadGroup *tInvisible)
:
	TGroupListWindow(parent, resourceId, &tLoadGroup->units, 344), loadGroup(tLoadGroup), invisible(tInvisible)
{
	Add(
		new TGroupListColumn("Дата", loadunit_date, 52, LVCFMT_RIGHT),
		new TGroupListColumn("Ден", loadunit_weekday, 24),
		new TGroupListColumn("Зала", loadunit_hall, 32),
		new TGroupListColumn("", loadunit_load, 224),
		NULL
	);

	loadData = dynamic = false;
}

bool TLoadUnitListWindow::AddGroup(TGroup *)
{
	TLoadUnit *loadUnit = (TLoadUnit *) Group();
	TLawsuit lawsuit;
	TSession session;

	session.time.hour = 8;

	if (loadUnit)
	{
		session.hall = loadUnit->hall;
		session.date = loadUnit->date;

		if (loadUnit->session)
		{
			lawsuit.key = loadUnit->session->key;
			// 2014:160 LPR: was loadUnit->session->time
			session.time.hour = 0;
		}
		else if (!loadUnit->bef)
			session.time.hour = 12;
	}

	if (get_group(this, &lawsuit, TYPE_LAWSUITS, NULL))
	{
		if (set_session(this, &session, &lawsuit, loadGroup, invisible))
		{
			loadGroup->AddSession(session);
			loadGroup->FixupUnits();
			Refresh();
		}
	}

	return false;
}
