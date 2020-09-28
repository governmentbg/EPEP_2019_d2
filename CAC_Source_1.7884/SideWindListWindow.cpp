#include "cac.h"

//{{TSideWindListWindow Implementation}}

TSideWindListWindow::TSideWindListWindow(TWindow* parent, long tSideMode, int resourceId, TRCDKeyContainer *tContainer,
	TLawsuit *tLawsuit)
:
	TGroupListWindow(parent, resourceId, new TSideWindArray, 356), container(tContainer), lawsuit(tLawsuit),
		edit(tSideMode & SIDE_MODE_EDIT), links(NULL), stat(false)
{
	int nonNameSearch = CHROME_SIDE_SEARCH < SIDE_SEARCH_QUICK ? LVCFMT_NO_SEARCH : LVCFMT_LEFT;

	Add(
		new TGroupListColumn("Участие", sidewind_involvement, 84, nonNameSearch),
		new TGroupListColumn("Флагове", sidewind_state, 16, LVCFMT_NO_SEARCH),
	#if TESTVER
		new TGroupListColumn(EPORTAL_NAME, generic_nothing, 16, LVCFMT_NO_SEARCH),
	#endif  // TESTVER
		new TGroupListColumn("Вид", basewind_ucn_type, 16, LVCFMT_NO_SEARCH),
		new TGroupListColumn("Код", "F_UCN", 44, nonNameSearch),
	#if TESTVER
		new TGroupListColumn("Име", basewind_names, 168),
	#else  // TESTVER
		new TGroupListColumn("Име", basewind_names, 184),
	#endif  // TESTVER
		NULL
	);

	container->Involvements(involvements);
	status = container->Status();
	unwanted = strchr(TYPE_UNWANTED_LAWS, find_college(container));

	if (CHROME_SIDE_SEARCH > SIDE_SEARCH_ALPHA)
		qsMode = QUICK_SEARCH_ABLE;
}

TSideWindListWindow::~TSideWindListWindow()
{
	delete links;
}

void TSideWindListWindow::Links()
{
	LoadLinkWinds();
	process_links(this, links);
}

void TSideWindListWindow::Messages()
{
	// 2003:333 BRW: let's hope this is universal
	// 2004:008 BUGFIX: should be called with selection only
	int sel = GetVisualIndex();
	TSideWind *sideWind = SideWind();
	TGArray<TUCNGroup> affecteds;

	LoadLinkWinds();
	process_subpoenas(this, container, status, sideWind, sel, links, Sides(), &affecteds, lawsuit);
	// 2013:340 redraw all notified sides
	for (int i = 0; i < Sides()->Count(); i++)
	{
		TSideWind *sideWind = (*Sides())[i];

		for (int n = 0; n < affecteds.Count(); n++)
		{
			// we don't check the involvement, it makes almost no difference
			if (sideWind->ucnType == affecteds[n]->ucnType && !strcmp(sideWind->ucn, affecteds[n]->ucn))
			{
				RedrawVisual(i);
				break;
			}
		}
	}
}

void TSideWindListWindow::List(bool right)
{
	LoadLinkWinds();
	print_side_list(this, container, *Sides(), *links, right);
}

void TSideWindListWindow::Roll()
{
	print_subpoena_list(this, container, Sides());
}

void TSideWindListWindow::Child(int command)
{
	TSideWind *sideWind = SideWind();

	if (sideWind && command)
	{
		if (command == SIDE_WIND_ADMNU)
			command = SIDE_WIND_ADULT;

		if (child_sidewind(sideWind, command))
			RedrawGroup();
	}
}

void TSideWindListWindow::Expend(TSession *session, TLawsuit *lawsuit)
{
	expend_sidewind(this, container, session, lawsuit, SideWind());
	RedrawGroup();
}

void TSideWindListWindow::Attach()
{
	electric_items_interact(this, container, true, lawsuit);
}

#if COURT_TYPE == COURT_REGIONAL
void TSideWindListWindow::Heritage(int command)
{
	TSideWind *sideWind = SideWind();

	if (sideWind && command)
	{
		char oldState = sideWind->state;

		switch (command)
		{
			case RESULT_HERITAGE_ACCEPT : sideWind->state = STATE_HERITAGE_ACCEPT; break;
			case RESULT_HERITAGE_REJECT : sideWind->state = STATE_HERITAGE_REJECT; break;
			case RESULT_HERITAGE_LOST :  sideWind->state = STATE_HERITAGE_LOST; break;
			default : sideWind->state = STATE_NONE;
		}

		if (sideWind->Update("F_STATE"))
			RedrawGroup();
		else
			sideWind->state = oldState;
	}
}
#endif  // REGIONAL

void TSideWindListWindow::LoadSideWinds(TGArray<TSideWind> *sideWinds)
{
	DrawLockCount();

	for (int index = 0; index < sideWinds->Count(); index++)
	{
		TSideWind *sideWind = (*sideWinds)[index];
		InitGroup(sideWind);

		if (sideWind->Insert())
			InsertGroup(sideWind);
		else
			delete sideWind;
	}

	UpdateVisual();
}

void TSideWindListWindow::LoadLinkWinds()
{
	if (!links)
	{
		TLinkWind linkWind;
		msql m;

		links = new TLinkWindArray(Sides());
		linkWind << container;
		linkWind.FloatCriteria(m);

		TQuery q(str(m));
		while (q.Read())
		{
			TLinkWind *tLinkWind = new TLinkWind;
			*tLinkWind << q;
			links->Add(tLinkWind);
		}

		sweep_invalid_link_winds(*Sides(), *links);
	}
}

void TSideWindListWindow::EPortalSend(TSideWind *sideWind, char oper)
{
	if (strchr(KIND_LAWSUITS, container->kind) || strchr(KIND_SESSIONS_ENDOCS, container->kind))
		eportal_send(this, sideWind, lawsuit, oper);
}

void TSideWindListWindow::GetGroupText(mstr &m, TGroup *group, int column)
{
	if (column != COLUMN_EPORTAL)
		TGroupListWindow::GetGroupText(m, group, column);
	else
	{
		TRCDKey origin;
		const TSideWind *sideWind = (const TSideWind *) group;

		if (lawsuit)
			origin = lawsuit->key;
		else if (strchr(TYPE_IOREGS, container->key.type) || container->key.type == TYPE_SURROUND ||
			container->key.type == TYPE_PROVE)
		{
			origin = ((const TPrimeContainer *) container)->key;
		}
		else
			origin = container->key;

		if (origin.type && strchr(TYPE_LAWSUITS, origin.type) &&
			eportal_side_assigned(origin, sideWind->ucnType, sideWind->ucn, sideWind->involvement))
		{
			m.cat("ЕД");
		}
	}
}

bool TSideWindListWindow::GetUCNGroup(TTransGroup &ucnGroup)
{
	return get_ucn(this, &ucnGroup, strchr(KIND_SESSIONS, container->kind) ? UCN_DATED_SIDES : UCN_SIDES,
		str(involvements), !edit, stat);
}

bool TSideWindListWindow::CheckInvolvement(const TSideWind *)
{
	return true;
}

bool TSideWindListWindow::EditableInvolvement(const TSideWind *)
{
	return true;
}

void TSideWindListWindow::GetSearchText(mstr &m, int index)
{
	TGroupListWindow::GetSearchText(m, index);

	if (CHROME_SIDE_SEARCH == SIDE_SEARCH_1NAME)
	{
		char *start = ncstr(m);
		char *s = start;

		while (*s && !isspace(*s))
			s++;

		if (s > start)
		{
			TSideWind *sideWind = SideWindAt(index);

			if (strchr(UCN_FIRMS, sideWind->ucnType) && CompanyType->DamnName(start))
			{
				while (isspace(*s))
					s++;
				while (*s && !isspace(*s))
					s++;
			}

			*s = '\0';
		}
	}
}

void TSideWindListWindow::InitGroup(TSideWind *sideWind) const
{
	*sideWind << container;
	// 2004:111 BUGFIX: clear flags copied from container
	sideWind->flags = 0;
	sideWind->eventStatus = status;
	// 2011:025 BUGFIX: clear text copied from container
	TText *text = (TText *) sideWind->Find("F_TEXT");
	if (text)
		text->SetS("");
}

// 2009:183 LPR: from former TDocSideWindListWindow
TGroup *TSideWindListWindow::NewGroup() const
{
	TSideWind *sideWind = new TSideWind;
	InitGroup(sideWind);
	return sideWind;
}

void TSideWindListWindow::Refresh()
{
	TGroupListWindow::Refresh();

	if (container == lawsuit)
		eportal_send(this, Sides(), lawsuit, LOG_WRITE);
}

bool TSideWindListWindow::AddGroup(TGroup *group)
{
	TSideWind *sideWind = (TSideWind *) group;
	// 2004:166 LPR: ucn search changes, used as a model/standard/example
	TTransGroup ucnGroup;

	if (GetUCNGroup(ucnGroup))
	{
		// 2008:024 LPR: address for new ucn groups is handled in ucns
		*sideWind << ucnGroup;

		if (CheckInvolvement(sideWind))
		{
			// 2016:350
			if (sideWind->ucnType == UCN_CITIZEN_UCN && strchr(TYPE_ORIGINS, container->key.type))
			{
				CDate birth(sideWind->ucn);
				birth += -1;
				int months = interval2Months(birth, sideWind->date);

				if (months <= 168)
					sideWind->flags |= SIDE_WIND_INFANT;
				else if (months <= 216)
					sideWind->flags |= SIDE_WIND_MINOR;
			}

			if (sideWind->Insert())
			{
				// 2008:049 LPR: all logs: separate person/side log entries
				log(LOG_ADD, IDADD, container, sideWind);
				EPortalSend(sideWind, LOG_WRITE);
				return true;
			}
		}
	}

	return false;
}

bool TSideWindListWindow::EditGroup(TGroup *group)
{
	TSideWind *source = SideWind();
	// 2010:078 LPR: use GroupList side copy
	TSideWind *target = (TSideWind *) group;
	const char *tInvolvements = EditableInvolvement(source) ? str(involvements) : C2S[source->involvement];

	LoadLinkWinds();

	if (edit_sidewind(this, container, source, target, tInvolvements, unwanted, links, status, stat))
	{
		// 2008:288 LRQ: xfer back to request and request resolutions
		if (strchr(KIND_LAWSUITS, container->kind))
		{
			if (source->involvement != target->involvement)
			{
				update_prelawsuit_sides(source, (TLawsuit *) container, target->involvement);
				EPortalSend(source, LOG_DELETE);
			}
		}

		EPortalSend(target, LOG_WRITE);
		return true;
	}

	return false;
}

bool TSideWindListWindow::AskDelete(TGroup *group)
{
	// 2010:341 rewritten
	TSideWind *sideWind = (TSideWind *) group;
	const char *reason = blocked_sidewind(sideWind, status, false);

	if (!reason)
	{
		LoadLinkWinds();
		reason = blocked_sidewind(sideWind, links);
	}

	return reason ? error("Страната не може да бъде изтрита, защото %s.", reason) :
		ask("Желаете ли да изтриете избраната страна?");
}

bool TSideWindListWindow::DeleteGroup(TGroup *group)
{
	TSideWind *sideWind = (TSideWind *) group;

	if (TGroupListWindow::DeleteGroup(group))
	{
		log(LOG_DELETE, IDDELETE, container, sideWind);

		if (status == RESULT_NEW)
			EPortalSend(sideWind, LOG_DELETE);

		// 2006:039 LRQ/URQ: BLOAT
		if (KILL_FORWARD_SIDES)
			kill_forward_sides(container, sideWind);

		return true;
	}

	return false;
}
