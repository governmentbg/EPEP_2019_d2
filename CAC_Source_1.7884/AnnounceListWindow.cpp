//{{TAnnounceListWindow Implementation}}

TAnnounceListWindow::TAnnounceListWindow(TWindow* parent, int resourceId, TAnnounceGroup *tGroup, const char *types,
	TGroupArray *tDeliverers)
:
	TGroupListWindow(parent, resourceId, new TGSArray<TAnnounce>, 348), group(tGroup), deliverers(tDeliverers),
		assignable(0)
{
	Add(new TGroupListColumn("Призовка/съобщение", group_print, 336));

	*connectKinds = '\0';
	for (const char *s = types; *s; s++)
		strcach(connectKinds, type2ConnectKind(*s));

	loadData = initialRefresh = false;
}

void TAnnounceListWindow::ServeAnnounce()
{
	if (serve_announce(this, Announce()))
		RedrawVisual(GetVisualIndex());
}

void TAnnounceListWindow::Assign()
{
	TWaitWindow *wait = new TWaitWindow(this, WAIT);
	int assigned = 0;
	SetRedraw(false);

	for (int index = 0; index < array->Count(); index++)
	{
		TAnnounce *announce = (TAnnounce *) GroupAt(index);
		TSubpoena *subpoena = announce->subpoena;

		if (subpoena)
		{
			char backupUcn[SIZE_OF_UCN];
			bool first = true;

			strcpy(backupUcn, subpoena->delivererUcn);
			for (int i = 0; i < deliverers->Count(); i++)
			{
				TUserAliasGroup *user = (TUserAliasGroup *) (*deliverers)[i];

				if (user->Serves(subpoena->uclp, subpoena->street, subpoena->adrNo, subpoena->blockNo))
				{
					if (first)
					{
						strcpy(subpoena->delivererUcn, user->ucn);
						first = false;
					}
					else
					{
						*subpoena->delivererUcn = '\0';
						break;
					}
				}
			}

			if (strcmp(subpoena->delivererUcn, backupUcn))
			{
				if (subpoena->Update("F_DELIVERER_UCN"))
				{
					RedrawVisual(index);
					assigned++;
				}
				else
					strcpy(subpoena->ucn, backupUcn);
			}
		}
	}

	SetRedraw(true);
	delete wait;

	if (assigned == 1)
		message("1 разпределена призовка.");
	else
		message("%d разпределени призовки.", assigned);

	UnblockFocus(IDASSIGN);
}

void TAnnounceListWindow::AddRepeat(bool tCopy)
{
	TAnnounce *copy = Announce();

	do
	{
		TAnnounce *announce = new TAnnounce;

		announce->court = group->court;
		announce->subpoenaDate = group->date;

		if (tCopy)
		{
			announce->key = copy->key;
			// 2010:342 FIX: were missing
			announce->composition = copy->composition;
			announce->subpoenaKind = copy->subpoenaKind;
			announce->sessionDate = copy->sessionDate;
		}

		char kinds[SIZE_OF_CONNECT_KINDS];
		// 2010:013 LPR: compute kinds
		if (group->type)
			strcpy(kinds, C2S[type2ConnectKind(group->type)]);
		else
			strcpy(kinds, group->court == COURT_CODE ? connectKinds : CONNECT_KIND_ANNOUNCES);
		// 2018-01-19: always SRS_FOR_DELIVER
		announce->receivedStatus = SRS_FOR_DELIVER;

		if (!edit_announce(this, announce, false, deliverers, kinds, false))
		{
			delete announce;
			break;
		}

		SetVisualIndex(InsertGroup(announce));
		UpdateVisual();

	} while (REPEAT_ANNOUNCES);
}

void TAnnounceListWindow::FindText(const char *text, int step)
{
	QuickSearch(step, text);
}

TGroup *TAnnounceListWindow::NewGroup() const
{
	return new TAnnounce;
}

void TAnnounceListWindow::Refresh()
{
	msql m;

	array->Flush();

	if (group->type)
	{
		TWaitWindow wait(this, WAIT);

		// pass 1: internal
		{
			group->FormCriteria(m);
			TQuery q(str(m));

			assignable = 0;
			while (q.Read())
				if (add_announce_subpoena(array, q) != INT_MAX)		// 2013:107 LPR: was -1
					assignable++;
		}
		// pass 2: external
		{
			TExternalAnnounceGroup tGroup;

			tGroup << group;
			for (int i = 0; i < group->deliverers.Count(); i++)
				tGroup.deliverers.Add(group->deliverers[i]);

			tGroup.FormCriteria(m);
			TQuery q(str(m));

			while (q.Read())
				add_announce_announce(array, q);

			tGroup.deliverers.Clear();
		}
	}
	else
	{
		group->FormCriteria(m);
		LoadData(str(m));
	}

	TGroupListWindow::Refresh();
}

bool TAnnounceListWindow::EditGroup(TGroup *group)
{
	TAnnounce *tAnnounce = Announce();
	TAnnounce *announce = (TAnnounce *) group;

	// 2018-03-21: LPR: use subpoena copy
	if (tAnnounce->subpoena)
	{
		TSubpoena *tSubpoena = new TSubpoena;
		*tSubpoena << tAnnounce->subpoena;
		announce->SetSubpoena(tSubpoena);
	}

	return edit_announce(this, announce, true, deliverers, C2S[announce->key.kind], false);
}

bool TAnnounceListWindow::AskDelete(TGroup *group)
{
	TAnnounce *announce = (TAnnounce *) group;
	TSubpoena *subpoena = announce->subpoena;

	if (subpoena)
		return subpoena->AskDelete();

	if (!announce->subpoenaReceived.Empty())
		return error("Има попълнени данни за връчване.");

	return ask("Желаете ли да изтриете избраната призовка/съобщение?");
}

bool TAnnounceListWindow::DeleteGroup(TGroup *group)
{
	TAnnounce *announce = (TAnnounce *) group;
	TSubpoena *subpoena = announce->subpoena;

	if (subpoena)
	{
		TGArray<TUCNGroup> affecteds;
		return delete_subpoena(subpoena, &affecteds, NULL);
	}

	if (announce->Delete(true))
	{
		char s[SIZE_OF_LOG_TEXT];

		// 2010:011 FIX: connect kind name, subpoena kind name -> aliases
		ConnectKind->Seek(announce->key.kind);
		SubpoenaKind->Seek(announce->subpoenaKind);
		sprintf(s, "%-15s %3ld %15s", ConnectKind->alias, announce->court, SubpoenaKind->alias);
		log(LOG_DELETE, IDDELETE, announce->key.no, announce->key.year, TYPE_NULL, KIND_NULL, &announce->subpoenaDate, s);

		return true;
	}

	return false;
}
