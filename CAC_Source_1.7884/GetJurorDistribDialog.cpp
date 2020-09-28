DEFINE_RESPONSE_TABLE1(TGetJurorDistribDialog, TQuickDialog)
//{{TGetJurorDistribDialogRSP_TBL_BEGIN}}
	EV_CBN_SELCHANGE(IDC_GET_GROUP_TYPE, TypeCBNSelChange),
	EV_CBN_SELCHANGE(IDC_GET_GROUP_KIND, KindCBNSelChange),
	EV_BN_CLICKED(IDOK, OKBNClicked),
	EV_COMMAND_ENABLE(IDQUERY, QueryBNEnable),
	EV_COMMAND(IDQUERY, QueryBNClicked),
//{{TGetJurorDistribDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TGetJurorDistribDialog Implementation}}

TGetJurorDistribDialog::TGetJurorDistribDialog(TWindow* parent, TBaseSession **tSessptr, sign tEdit,
	const TExternSession *tSuggest, bool tForceExcess)
:
	TGetWhatExtern(tEdit, tForceExcess), TQuickDialog(parent, resWhat), sessptr(tSessptr), suggest(tSuggest)
{
	type = new TCharAliasFace(this, IDC_GET_GROUP_TYPE, "F_TYPE", Type, Default->colleges);
	no = new TLongFace(this, IDC_GET_GROUP_NO, "F_NO", SIZE_OF_NO);
	year = new TYearFace(this, IDC_GET_GROUP_YEAR, "F_YEAR");
	kind = new TCharAliasFace(this, IDC_GET_GROUP_KIND, "F_KIND", Kind, KIND_OPEN_SESSIONS);
	date = new TDateFace(this, IDC_GET_GROUP_DATE, "F_DATE", false);
	timeLabel = new TStatic(this, IDC_GET_GROUP_TIME_LABEL);
	time = new TTimeFace(this, IDC_GET_GROUP_TIME, "F_TIME", false);

	if (resWhat == IDD_GET_JUROR_DISTRIB)
		external = new TCheckFace(this, IDC_GET_JUROR_DISTRIB_EXTERNAL, "F_EXTERNAL");
	else
		external = NULL;
}

bool TGetJurorDistribDialog::IsValidBasic()
{
	return (no->L() && year->L()) || error("Номерът и годината са задължителни.");
}

bool TGetJurorDistribDialog::IsValid()
{
	bool isValid = IsValidBasic();

	if (isValid)
	{
		if (date->Date().Empty())
			isValid = error("Датата е задължителна.");
		else if (strchr(KIND_CLOSED_SESSIONS, kind->C()))
		{
			if (!edit && time->Time().Empty())
				isValid = error("Часът е задължителен при разпределение за закрито заседание.");
			else if (edit != -1 && date->Date() < Today)
				isValid = error("Датата на заседанието е по-малка от днешната дата.");
		}
	}

	if (isValid && !external)
	{
		const TRCDKey key = { no->L(), year->L(), type->C() };

		SetPrimaryGroup(can_access_basic(key, true, true, NULL));

		if (!_primary_group)
			isValid = false;
		else if (!edit)
		{
			const char *status = jurorable(PrimaryGroup(TLawsuit), kind->C());

			if (status)
				isValid = error("%s.", status);
		}
	}

	return isValid;
}

void TGetJurorDistribDialog::SetupWindow()
{
	TQuickDialog::SetupWindow();

	date->SetS("");
	type->SetC(kind2type(Default->kind));
	TypeCBNSelChange();

	if (forceExcess)
	{
		external->SetCheck(BF_CHECKED);
		external->EnableWindow(false);
	}

	if (suggest)
	{
		no->SetL(suggest->key.no);
		year->SetL(suggest->key.year);
		type->SetC(suggest->key.type);

		if (strchr(KIND_OPEN_SESSIONS, suggest->kind) && (!edit || suggest->result == RESULT_FIXED))
		{
			kind->SetC(suggest->kind);
			date->SetDate(suggest->date);
		}
	}
}

void TGetJurorDistribDialog::TypeCBNSelChange()
{
	kinds.cpy(type2ses_kinds(type->C()));
	kinds.cat(KIND_VISIBLE_CLOSEDS);
	kind->SetXCrit(str(kinds));
	KindCBNSelChange();
}

void TGetJurorDistribDialog::KindCBNSelChange()
{
	if (strchr(KIND_CLOSED_SESSIONS, kind->C()) && !edit)
	{
		timeLabel->ShowWindow(SW_SHOW);
		time->EnableWindow(true);
		time->ShowWindow(SW_SHOW);
	}
	else
	{
		time->ShowWindow(SW_HIDE);
		time->EnableWindow(false);
		time->SetS("");
		timeLabel->ShowWindow(SW_HIDE);
	}
}

void TGetJurorDistribDialog::OKBNClicked()
{
	if (IsValid())
	{
		TBaseSession *const session = TBaseSession::NewGroup(External());
		TVirtPtr VP(session);

		*session << this;
		bool closed = strchr(KIND_CLOSED_SESSIONS, session->kind);

		if (!closed)
		{
			if (!session->Get())
				return;

			const char *status;

			if (edit != -1 && (status = jurorable(session->result)) != NULL)
			{
				error("%s.", status);
				return;
			}
		}

		if (edit != false)
		{
			TJurorDistrib distrib;

			if (!distrib.Find(session, false))
			{
				mstr m("По делото няма разпределени съдебни заседатели");

				if (closed)
					m.printf(" за %s от %Dг", Kind->Seek(KIND_CLOSED_SESSION), &session->date);

				error("%s.", str(m));
				return;
			}
		}

		*sessptr = (TBaseSession *) session->clone();
		**sessptr << session;
		CmOk();
	}
}

void TGetJurorDistribDialog::QueryBNEnable(TCommandEnabler &tce)
{
	tce.Enable(!External());
}

void TGetJurorDistribDialog::QueryBNClicked()
{
	if (IsValidBasic())
	{
		const TRCDKey key = { no->L(), year->L(), type->C() };
		query_key(this, key);
	}
}
