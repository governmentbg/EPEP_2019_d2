//{{TExternSessionListWindow Implementation}}

TExternSessionListWindow::TExternSessionListWindow(TWindow* parent, int resourceId, TExternSessionListGroup &tGroup)
:
	TGroupListWindow(parent, resourceId, NULL, 348), group(tGroup)
{
	Add(
		new TGroupListColumn("Вид дело", keygroup_key_type, 60),
		new TGroupListColumn("Номер/Година", extern_key_no_year, 76, LVCFMT_RIGHT),
		new TGroupListColumn("Вид заседание", extern_kind, 84),
		new TGroupListColumn("Статус", keygroup_result_status, 60),
		new TGroupListColumn("От дата", container_date, 60, LVCFMT_RIGHT),
		NULL
	);
}

void TExternSessionListWindow::Right()
{
	TExternSession *session = (TExternSession *) NewGroup();
	TExternContainer *const lawsuit = get_closed_lawsuit(this, &session->date, Session(), true);
	TVirtPtr VP(lawsuit);

	if (lawsuit)
	{
		session->key = lawsuit->key;

		if (edit_extern_closed_session(this, session, false))
		{
			SetVisualIndex(InsertGroup(session));
			UpdateVisual();
			session = NULL;
		}
	}

	delete session;
}

void TExternSessionListWindow::Assign()
{
	TExternSession *const session = TYPECHECK_PTR_CAST(TExternSession, get_juror_distrib(this, false, Session(), true));
	TVirtPtr VP(session);

	if (session)
		new_distrib_jurors(this, session, NULL);
}

void TExternSessionListWindow::ReAssign()
{
	TExternSession *const session = TYPECHECK_PTR_CAST(TExternSession, get_juror_distrib(this, true, Session(), true));
	TVirtPtr VP(session);

	if (session)
		edit_distrib_jurors(this, session, NULL);
}

void TExternSessionListWindow::Query()
{
	query_juror_distrib(this, Session(), true);
}

void TExternSessionListWindow::Criteria(msql &m, TGroup *)
{
	group.FormCriteria(m);
}

TGroup *TExternSessionListWindow::NewGroup() const
{
	TExternSession *session = new TExternSession;
	const TExternSession *suggest = ((TExternSessionListWindow *) this)->Session();

	if (suggest)
		session->key = suggest->key;

	return session;
}

bool TExternSessionListWindow::AddGroup(TGroup *group)
{
	return edit_extern_open_session(this, (TExternSession *) group, false);
}

bool TExternSessionListWindow::EditGroup(TGroup *group)
{
	TExternSession *session = (TExternSession *) group;

	return strchr(KIND_OPEN_SESSIONS, session->kind) ?
		edit_extern_open_session(this, session, true) :
		edit_extern_closed_session(this, session, true);
}

bool TExternSessionListWindow::DeleteGroup(TGroup *group)
{
	if (!TGroupListWindow::DeleteGroup(group))
		return false;

	log(LOG_DELETE, CM_CLERK_EXTERN_SESSIONS, (const TExternSession *) group);
	return true;
}
