DEFINE_RESPONSE_TABLE1(TKnownPersonListWindow, TGroupListWindow)
//{{TKnownPersonListWindowRSP_TBL_BEGIN}}
	EV_WM_KEYDOWN,
//{{TKnownPersonListWindowRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TKnownPersonListWindow Implementation}}

TKnownPersonListWindow::TKnownPersonListWindow(TWindow* parent, int resourceId)
:
	TGroupListWindow(parent, resourceId, NULL, 184), involvement('\0')
{
	Add(
		new TGroupListColumn("", group_inactive, 12, LVCFMT_CENTER | LVCFMT_NO_SEARCH),
		new TGroupListColumn("", person_pedagog, 12, LVCFMT_CENTER | LVCFMT_NO_SEARCH),
		new TGroupListColumn("Èìå", group_print, 148),
		NULL
	);

	initialRefresh = false;
	abcskip = ABC_SKIP_LOWER;
	qsMode = QUICK_SEARCH_AUTO;
	sortcol = 3;
}

void TKnownPersonListWindow::SetInvolvement(char tInvolvement)
{
	if (involvement != tInvolvement)
	{
		involvement = tInvolvement;

		if (CHROME_LOAD_KNOWNS)
		{
		#if SPEEDTESTING
			#pragma message speed testing only
			DWORD ticks = GetTickCount();
		#endif  // SPEEDTESTING
			Refresh();
		#if SPEEDTESTING
			if (involvement == INVOLVEMENT_LEGALESE)
				message("test version: %lu d-seconds\n", (GetTickCount() - ticks) / 100);
		#endif  // SPEEDTESTING
		}
		else
		{
			FlushVisuals();
			array->Flush();
		}
	}
}

void TKnownPersonListWindow::Registry()
{
	TKnownPerson *person = Person();

	// 2010:267 URQ: pedagog support
	if (involvement == INVOLVEMENT_JUROR)
	{
		person->flags ^= JUROR_PEDAGOG;
		if (person->Update("F_FLAGS"))
			RedrawGroup();
		else
			person->flags ^= JUROR_PEDAGOG;
	}
	else if (involvement == INVOLVEMENT_LEGALESE)
	{
		TKnownPerson tPerson;
		tPerson << person;

		if (edit_lawer_data(this, &tPerson, false))
			*person << tPerson;
	}
	else
	{
		TKnownPerson tPerson;
		tPerson << person;

		if (TEditCSJIDDialog(this, &tPerson).Execute() == IDOK)
			*person << tPerson;
	}
}

void TKnownPersonListWindow::List(long active)
{
	print_knowns(this, array, involvement, active);
}

bool TKnownPersonListWindow::Active()
{
	TKnownPerson *person = Person();
	person->flags ^= FLAG_ACTIVE;

	if (person->Update("F_FLAGS"))
	{
		RedrawGroup();
		return true;
	}

	person->flags ^= FLAG_ACTIVE;
	return false;
}

void TKnownPersonListWindow::Addresses()
{
	view_addresses(this, Person());
}

void TKnownPersonListWindow::Criteria(msql &m, TGroup *group)
{
	((TKnownNamesPerson *) group)->NamesCriteria(m);
}

TGroup *TKnownPersonListWindow::NewGroup() const
{
	TKnownPerson *person = new TKnownNamesPerson;
	person->involvement = involvement;
	return person;
}

bool TKnownPersonListWindow::AddGroup(TGroup *group)
{
	return add_known_person(this, (TKnownPerson *) group);
}

bool TKnownPersonListWindow::EditGroup(TGroup *group)
{
	TKnownNamesPerson *person = (TKnownNamesPerson *) Group();

	if (edit_known_person(this, (TKnownPerson *) group))
	{
		person->ClearCache();
		return true;
	}

	return false;
}

bool TKnownPersonListWindow::DeleteGroup(TGroup *group)
{
	if (TGroupListWindow::DeleteGroup(group))
	{
		TKnownPerson *person = (TKnownPerson *) group;

		if (strchr(INVOLVEMENT_CSJIDS, person->involvement))
			TSender(person).Delete(true);

		return true;
	}

	return false;
}

void TKnownPersonListWindow::EvKeyDown(uint key, uint repeatCount, uint flags)
{
	if (key == VK_F6 && (GetAsyncKeyState(VK_CONTROL) & GetAsyncKeyState(VK_MENU) & 0x01))
		count_lawer_cases(this);
	else
		TGroupListWindow::EvKeyDown(key, repeatCount, flags);
}
