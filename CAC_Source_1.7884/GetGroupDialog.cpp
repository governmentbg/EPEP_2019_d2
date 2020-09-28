DEFINE_RESPONSE_TABLE1(TGetGroupDialog, TGroupDialog)
//{{TGetGroupDialogRSP_TBL_BEGIN}}
	EV_BN_CLICKED(IDOK, OKBNClicked),
	EV_BN_CLICKED(IDQUERY, QueryBNClicked),
	EV_CBN_SELCHANGE(IDC_GET_GROUP_TYPE, TypeCBNSelChange),
	EV_CBN_SELCHANGE(IDC_GET_GROUP_KIND, KindCBNSelChange),
//{{TGetGroupDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TGetGroupDialog Implementation}}

TGetGroupDialog::TGetGroupDialog(TWindow* parent, TRCDKeyGroup *group, TRCDKeyGroup **tGrouptr, const char *tTypes, const char *tKinds, const char *tStatuses, match_func tMatch, int getWhat, int resId)
:
	TGetTypesKinds(tTypes, tKinds, getWhat), TGroupDialog(parent, group, resId ? resId : resWhat), grouptr(tGrouptr), statuses(tStatuses), match(tMatch)
{
	typeLabel = new TStatic(this, IDC_GET_GROUP_TYPE_LABEL);
	type = new TCharAliasFace(this, IDC_GET_GROUP_TYPE, "F_TYPE", Type, types);
	no = new TLongFace(this, IDC_GET_GROUP_NO, "F_NO", SIZE_OF_NO);
	year = new TYearFace(this, IDC_GET_GROUP_YEAR, "F_YEAR");

	if (resWhat == IDD_GET_GROUP)
	{
		kindLabel = new TStatic(this, IDC_GET_GROUP_KIND_LABEL);
		kind = new TGetContainerAliasFace(this, IDC_GET_GROUP_KIND, "F_KIND", kinds);
		dateLabel = new TStatic(this, IDC_GET_GROUP_DATE_LABEL);
		date = new TDateFace(this, IDC_GET_GROUP_DATE, "F_DATE", false);
		timeLabel = new TStatic(this, IDC_GET_GROUP_TIME_LABEL);
		time = new TTimeFace(this, IDC_GET_GROUP_TIME, "F_TIME", false);
	}
	else
	{
		kind = NULL;
		kindLabel = NULL;
		dateLabel = NULL;
		date = NULL;
		timeLabel = NULL;
		time = NULL;
	}

	// 2009:190 NOTE: query used to en/disable if [not] empty type only
	query = getWhat == GET_GROUP_QUERY ? NULL : new TPushButton(this, IDQUERY);
}

TRCDKey TGetGroupDialog::last_key = { 0, 0, 0 };
int TGetGroupDialog::last_func = 0;

void TGetGroupDialog::SetupWindow()
{
	TGroupDialog::SetupWindow();

	if (kind)
	{
		kind->EnableWindow(true);
		date->EnableWindow(true);
		date->SetS("");
		time->EnableWindow(true);
		time->SetS("");
	}

	int tTypes = 0;

	for (const char *tType = types; *tType; tType++)
		tTypes |= strchr(TYPE_LAWSUITS, *tType) ? 1 : 2;

	if (tTypes != 3)
		typeLabel->SetCaption(tTypes == 1 ? "&Вид дело" : "&Вид документ");

	if (CHROME_AUTO_REMEMBER && last_key.type && strchr(types, last_key.type))
	{
		type->SetC(last_key.type);

		if (CHROME_AUTO_REMEMBER > 1)
		{
			year->SetL(last_key.year);

			if (CHROME_AUTO_REMEMBER > 2 && (!repeat_func || repeat_func != last_func))
				no->SetL(last_key.no);
		}
	}

	TypeCBNSelChange();

	if (any(PrimaryTitle))
		SetCaption(PrimaryTitle);

	// 2009:212
	if (kindLabel)
	{
		int tKinds = 0;

		for (const char *tKind = kinds; *tKind; tKind++)
		{
			if (strchr(KIND_SESSIONS, *tKind))
				tKinds |= 0x01;
			else if (strchr(KIND_DECISIONS, *tKind))
				tKinds |= 0x02;
			else if (*tKind == KIND_REQUEST_RESOLUTION || strchr(KIND_INREG_EVENTS, *tKind))
				tKinds |= 0x04;
		}

		constant names[0x08] = { NULL, "Вид &заседание", "Вид съдебен &акт", NULL,
			"Вид ра&зпореждане", "Вид &заседание/разпореждане", "Вид ра&зпореждане/съдебен акт", NULL };

		if (names[tKinds])
			kindLabel->SetCaption(names[tKinds]);
	}
}

void TGetGroupDialog::QueryBNClicked()
{
	if (IsValidBasic())
	{
		*Group() << this;

		TRCDKeyGroup *const tGroup = can_access_basic(Group(), false, false, NULL);
		TVirtPtr VP(tGroup);

		if (tGroup)
			query_show_hide(this, tGroup);
	}
}

TRCDKeyGroup *TGetGroupDialog::OKLowLevel(const char tKind, TRCDKeyGroup *tGroup)
{
	if (tKind != KIND_V_EMPTY)
	{
		SetPrimaryGroup(tGroup);

		if (tKind == KIND_V_ARCHIVE)
		{
			tGroup = new TArchive;
			tGroup->key = Group()->key;
			if (!tGroup->Get())
				return tGroup;
		}
		else if ((tGroup = select_event(this, Group(), tKind, &date->Date(), &time->Time(), statuses, match)) == NULL)
			return NULL;
	}
	// 2009:176 FIX: was missing
	else if (any(kinds) && resWhat != IDD_GET_GROUP)
	{
		TCharAlias *tKind = (TCharAlias *) tGroup->Find("F_KIND");

		if (!tKind)
			fatal("Unable to apply kinds %s to %s", kinds, tGroup->Name);

		if (!strmatch(tKind->C(), kinds))
		{
			error("%s %ld/%ld - %s - Точният вид не съответства.", Type->Seek(tGroup->key.type), tGroup->key.no,
				tGroup->key.year, Kind->Seek(tKind->C()));
			return tGroup;
		}
	}

	char status = tGroup->Status();
	const char *ts = wrong_status;
	// 2015:124
	mstr m;
	m.printf("%s %ld/%ld - ", Type->Seek(tGroup->key.type), tGroup->key.no, tGroup->key.year);

	if ((tKind != KIND_V_EMPTY && tKind != KIND_V_ARCHIVE) ||
		(strmatch(status, statuses) && (ts = match(tGroup, status)) == NULL))
	{
		if (last_key != tGroup->key)
		{
			last_key = tGroup->key;
			last_func = repeat_func;
		}

		if (grouptr)
		{
			*grouptr = tGroup;
			tGroup = NULL;		// prevent high level delete
		}
		else
			*Group() << tGroup;

		CmOk();
	}
	else
		error("%s%s - %s.", str(m), rrseek(tGroup, status), ts);

	return tGroup;
}

void TGetGroupDialog::OKBNClicked()
{
	if (IsValid())
	{
		*Group() << this;

		// 2009:237 LPR: don't create/access/remember empty group
		if (type->C() == TYPE_EMPTY)
		{
			if (grouptr)
			{
				*grouptr = new TDummyContainer;
				(*grouptr)->key.type = TYPE_EMPTY;
			}
			CmOk();
		}
		else
		{
			char tKind;

			if (kind && !strchr(KIND_V_BASICS, kind->C()))
				tKind = kind->C();
			else if (!strcmp(kinds, KIND_V_ARCHIVES))
				tKind = KIND_V_ARCHIVE;
			else
				tKind = KIND_V_EMPTY;

			if (fix_incoming_type(Group(), types))
			{
				TRCDKeyGroup *tGroup = can_access_basic(Group(), write, tKind != KIND_V_ARCHIVE ? archeck : false, NULL);

				if (tGroup)
					delete OKLowLevel(tKind, tGroup);
			}
		}
	}
}

bool TGetGroupDialog::IsValidBasic()
{
	return type->C() == TYPE_EMPTY || (no->L() && year->L()) || error("Номерът и годината са задължителни.");
}

bool TGetGroupDialog::IsValid()
{
	bool isValid = TGroupDialog::IsValid();

	if (isValid)
	{
		if (!IsValidBasic())
			isValid = false;
		else if (date && date->Date().Empty() && !time->Time().Empty())
			isValid = error("При попълнен час, датата е задължителна.");
	}

	return isValid;
}

void TGetGroupDialog::TypeCBNSelChange()
{
	// 2009:212 can_query()
	if (type->C() == TYPE_EMPTY)
	{
		no->EnableWindow(false);
		no->SetL(0);
		year->EnableWindow(false);
		year->SetL(0);
		if (query)
			query->EnableWindow(false);
	}
	else
	{
		no->EnableWindow(true);
		year->EnableWindow(true);
		if (!year->L())
			year->SetL(Today.year);
		if (query)
			query->EnableWindow(can_query());
	}

	if (kind)
	{
		kind->SetType(type->C());
		KindCBNSelChange();
	}
}

void TGetGroupDialog::KindCBNSelChange()
{
	dateLabel->SetCaption(kind->C() && strchr(KIND_ENDOCS, kind->C()) ? "&Дата на засед." : "&Дата");

	if (strchr(KIND_V_NODATES, kind->C()))
	{
		date->EnableWindow(false);
		date->SetS("");
		time->EnableWindow(false);
		time->SetS("");
	}
	else
	{
		date->EnableWindow(true);
		time->EnableWindow(true);
	}
}
