DEFINE_RESPONSE_TABLE1(TJurorDistribDialog, TGroupDialog)
//{{TJurorDistribDialogRSP_TBL_BEGIN}}
	EV_CBN_SELCHANGE(IDC_JUROR_DISTRIB_JUROR_X, JurorXCBNSelChange),
	EV_CBN_SELCHANGE(IDC_JUROR_DISTRIB_RESERVE, ReserveCBNSelChange),
	EV_BN_CLICKED(IDC_JUROR_DISTRIB_JUROR_CHANGE, JurorChangeBNClicked),
	EV_BN_CLICKED(IDC_JUROR_DISTRIB_JUROR_MANUAL, JurorManualBNClicked),
	EV_BN_CLICKED(IDC_JUROR_DISTRIB_REJUROR_CHANGE, ReJurorChangeBNClicked),
	EV_BN_CLICKED(IDC_JUROR_DISTRIB_REJUROR_MANUAL, ReJurorManualBNClicked),
	EV_BN_CLICKED(IDC_JUROR_DISTRIB_JUROR_X_CHANGE, JurorXChangeBNClicked),
	EV_BN_CLICKED(IDC_JUROR_DISTRIB_JUROR_X_MANUAL, JurorXManualBNClicked),
	EV_BN_CLICKED(IDC_JUROR_DISTRIB_RESERVE_CHANGE, ReserveChangeBNClicked),
	EV_BN_CLICKED(IDC_JUROR_DISTRIB_RESERVE_MANUAL, ReserveManualBNClicked),
	EV_BN_CLICKED(IDQUERY, QueryBNClicked),
	EV_BN_CLICKED(IDPRINT, PrintBNClicked),
	EV_BN_CLICKED(IDC_JUROR_DISTRIB_JUROR_PEDAGOG, JurorPedagogBNClicked),
	EV_BN_CLICKED(IDC_JUROR_DISTRIB_REJUROR_PEDAGOG, ReJurorPedagogBNClicked),
	EV_BN_CLICKED(IDC_JUROR_DISTRIB_JUROR_X_PEDAGOG, JurorXPedagogBNClicked),
	EV_BN_CLICKED(IDC_JUROR_DISTRIB_RESERVE_PEDAGOG, ReservePedagogBNClicked),
	EV_BN_CLICKED(IDDELETE, DeleteBNClicked),
	EV_BN_CLICKED(IDCANCEL, CancelBNClicked),
	EV_BN_CLICKED(IDOK, OKBNClicked),
	EV_WM_CLOSE,
//{{TJurorDistribDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TJurorDistribDialog Implementation}}

TJurorDistribDialog::TJurorDistribDialog(TWindow* parent, TJurorDistribution *tDistribution, TBaseSession *tSession,
	const TLawsuit *tLawsuit)
:
	TGroupDialog(parent, tDistribution->distrib, IDD_JUROR_DISTRIB), distribution(tDistribution), session(tSession),
		lawsuit(tLawsuit), locked(false)
{
	header = new TStatic(this, IDC_JUROR_DISTRIB_HEADER);
	labels[JUROR_DISTRIB_JUROR] = new TStatic(this, IDC_JUROR_DISTRIB_JUROR_LABEL);
	jurors[JUROR_DISTRIB_JUROR] = new TJurorAliasFace(this, IDC_JUROR_DISTRIB_JUROR, "F_JUROR",
		distribution->distrib->juror, NULL, ALIAS_OPTION);
	pedagogs[JUROR_DISTRIB_JUROR] = new TCheckFace(this, IDC_JUROR_DISTRIB_JUROR_PEDAGOG, "F_PEDAGOG");
	buttons[JUROR_DISTRIB_JUROR] = new TTwinButton(this, IDC_JUROR_DISTRIB_JUROR_CHANGE, IDC_JUROR_DISTRIB_JUROR_MANUAL);
	labels[JUROR_DISTRIB_REJUROR] = new TStatic(this, IDC_JUROR_DISTRIB_REJUROR_LABEL);
	jurors[JUROR_DISTRIB_REJUROR] = new TJurorAliasFace(this, IDC_JUROR_DISTRIB_REJUROR, "F_REJUROR",
		distribution->distrib->reJuror, NULL, ALIAS_OPTION);
	pedagogs[JUROR_DISTRIB_REJUROR] = new TCheckFace(this, IDC_JUROR_DISTRIB_REJUROR_PEDAGOG, "F_PEDAGOG");
	buttons[JUROR_DISTRIB_REJUROR] = new TTwinButton(this, IDC_JUROR_DISTRIB_REJUROR_CHANGE,
		IDC_JUROR_DISTRIB_REJUROR_MANUAL);
	labels[JUROR_DISTRIB_JUROR_X] = new TStatic(this, IDC_JUROR_DISTRIB_JUROR_X_LABEL);
	jurors[JUROR_DISTRIB_JUROR_X] = new TJurorAliasFace(this, IDC_JUROR_DISTRIB_JUROR_X, "F_JUROR_X",
		distribution->distrib->jurorX, NULL, ALIAS_EMPTY);
	pedagogs[JUROR_DISTRIB_JUROR_X] = new TCheckFace(this, IDC_JUROR_DISTRIB_JUROR_X_PEDAGOG, "F_PEDAGOG");
	buttons[JUROR_DISTRIB_JUROR_X] = new TTwinButton(this, IDC_JUROR_DISTRIB_JUROR_X_CHANGE,
		IDC_JUROR_DISTRIB_JUROR_X_MANUAL);
	labels[JUROR_DISTRIB_RESERVE] = new TStatic(this, IDC_JUROR_DISTRIB_RESERVE_LABEL);
	jurors[JUROR_DISTRIB_RESERVE] = new TJurorAliasFace(this, IDC_JUROR_DISTRIB_RESERVE, "F_RESERVE",
		distribution->distrib->reserve, NULL, ALIAS_EMPTY);
	pedagogs[JUROR_DISTRIB_RESERVE] = new TCheckFace(this, IDC_JUROR_DISTRIB_RESERVE_PEDAGOG, "F_PEDAGOG");
	buttons[JUROR_DISTRIB_RESERVE] = new TTwinButton(this, IDC_JUROR_DISTRIB_RESERVE_CHANGE,
		IDC_JUROR_DISTRIB_RESERVE_MANUAL);
	cancel = new TPushButton(this, IDCANCEL);
	erase = new TPushButton(this, IDDELETE);
}

bool TJurorDistribDialog::IsValid()
{
	bool isValid = TGroupDialog::IsValid();

	if (isValid)
	{
		if (!atob(jurors[JUROR_DISTRIB_JUROR]) || !atob(jurors[JUROR_DISTRIB_REJUROR]))
			isValid = error("Първите двама заседатели са задължителни.");
	}

	return isValid;
}

void TJurorDistribDialog::UpdateButton(int index)
{
#if COURT_TYPE == COURT_REGIONAL
	if (index == JUROR_DISTRIB_JUROR_X)
		return;
#endif  // REGIONAL

	bool pedagog = pedagogs[index]->GetCheck() == BF_CHECKED;
	TPushButton *button = buttons[index];

	if (distribution->FindFirst(pedagog))
	{
		button->SetDrawIcon(true);
		button->SetCaption(atob(jurors[index]) ? "Преразпределение" : "Разпределение");
		button->EnableWindow(true);
	}
	else
	{
		button->SetDrawIcon(false);
		button->SetCaption("Ръчно разпределение");
		button->EnableWindow(distribution->Filter(NULL, pedagog));
	}
}

void TJurorDistribDialog::Update()
{
	*Distrib() << this;

	for (int index = 0; index < JUROR_COUNT; index++)
		UpdateButton(index);
}

void TJurorDistribDialog::MaybeLock()
{
	if (distribution->edit && !locked)
	{
		cancel->EnableWindow(false);
		cancel->ShowWindow(SW_HIDE);
		erase->EnableWindow(true);
		erase->ShowWindow(SW_SHOW);
		locked = true;
	}
}

void TJurorDistribDialog::Select(char oper, const char *ucn, int index, const char *text)
{
	log_juror(session, oper, ucn, index, text);
	distribution->Exclude(ucn);
	jurors[index]->SetUCN(ucn);
	MaybeLock();
}

void TJurorDistribDialog::InitJuror(int index)
{
	if (!atob(jurors[index]))
	{
		const char *ucn = distribution->FindFirst(false);

		if (ucn)
			Select(JUROR_DISTRIB_AUTO, ucn, index, "");
	}
}

void TJurorDistribDialog::MaybeClear(int index)
{
	TJurorAliasFace *juror = jurors[index];
	bool clear = !atob(juror->S());

	if (clear)
	{
		const char *ucn = Distrib()->jurors[index];
		char motives[SIZE_OF_JUROR_MOTIVES];

		if (TJurorMotivesDialog(this, "Мотиви за изключване", motives).Execute() == IDOK)
		{
			log_juror(session, JUROR_DISTRIB_CLEAR, "", index, motives);
			juror->SetUCN(NULL);
			MaybeLock();
		}
		else	// 2010:182 LPR: canceled -> revert
			juror->SetS(ucn);

		Update();
	}
}

void TJurorDistribDialog::ChangeJuror(int index)
{
	TJurorAliasFace *juror = jurors[index];
	bool pedagog = pedagogs[index]->GetCheck() == BF_CHECKED;
	const char *ucn = distribution->FindFirst(pedagog);

	if (ucn)
	{
		bool initial = !atob(juror->S());
		char motives[SIZE_OF_JUROR_MOTIVES];

		*motives = '\0';

		if (initial || TJurorMotivesDialog(this, "Мотиви за преразпределение", motives).Execute() == IDOK)
			Select(initial ? JUROR_DISTRIB_DISTRIB : JUROR_DISTRIB_REDISTRIB, ucn, index, motives);

		Update();
	}
	else
		ManualJuror(index);
}

void TJurorDistribDialog::ManualJuror(int index)
{
	bool pedagog = pedagogs[index]->GetCheck() == BF_CHECKED;
	TGArray<TDistribJuror> jurors;

	if (distribution->Filter(&jurors, pedagog))
	{
		char ucn[SIZE_OF_UCN];
		char motives[SIZE_OF_JUROR_MOTIVES];

		if (TJurorSelectDialog(this, motives, labels[index]->Title, ucn, &jurors).Execute() == IDOK)
		{
			Select(JUROR_DISTRIB_MANUAL, ucn, index, motives);
			Update();
		}
	}
	else	// can't happen (tm), pre-checked
		error("Няма свободни заседатели.");

	jurors.Clear();
}

void TJurorDistribDialog::SetupWindow()
{
	TGroupDialog::SetupWindow();

	mstr m;

	m.printf("%s %s%ld/%ldг. %s %Dг.", Type->Seek(session->key.type), session->ExcessText(), session->key.no,
		session->key.year, Kind->Seek(session->kind), &session->date);
	header->SetCaption(str(m));

	if (!distribution->edit)
	{
		InitJuror(JUROR_DISTRIB_JUROR);
		InitJuror(JUROR_DISTRIB_REJUROR);
	}

#if COURT_TYPE == COURT_REGIONAL
#if TESTVER
	jurors[JUROR_DISTRIB_JUROR_X]->EnableWindow(atob(jurors[JUROR_DISTRIB_JUROR_X]));
#else  // TESTVER
	jurors[JUROR_DISTRIB_JUROR_X]->EnableWindow(false);
#endif  // TESTVER
	pedagogs[JUROR_DISTRIB_JUROR_X]->EnableWindow(false);
	buttons[JUROR_DISTRIB_JUROR_X]->EnableWindow(false);
#endif  // REGIONAL

	Update();
}

void TJurorDistribDialog::JurorXCBNSelChange()
{
	MaybeClear(JUROR_DISTRIB_JUROR_X);
}

void TJurorDistribDialog::ReserveCBNSelChange()
{
	MaybeClear(JUROR_DISTRIB_RESERVE);
}

void TJurorDistribDialog::JurorChangeBNClicked()
{
	ChangeJuror(JUROR_DISTRIB_JUROR);
}

void TJurorDistribDialog::ReJurorChangeBNClicked()
{
	ChangeJuror(JUROR_DISTRIB_REJUROR);
}

void TJurorDistribDialog::JurorXChangeBNClicked()
{
	ChangeJuror(JUROR_DISTRIB_JUROR_X);
}

void TJurorDistribDialog::ReserveChangeBNClicked()
{
	ChangeJuror(JUROR_DISTRIB_RESERVE);
}

void TJurorDistribDialog::QueryBNClicked()
{
	distribution->Display();
}

void TJurorDistribDialog::PrintBNClicked()
{
	query_juror_distrib(this, lawsuit, Distrib());
}

void TJurorDistribDialog::JurorManualBNClicked()
{
	ManualJuror(JUROR_DISTRIB_JUROR);
}

void TJurorDistribDialog::ReJurorManualBNClicked()
{
	ManualJuror(JUROR_DISTRIB_REJUROR);
}

void TJurorDistribDialog::JurorXManualBNClicked()
{
	ManualJuror(JUROR_DISTRIB_JUROR_X);
}

void TJurorDistribDialog::ReserveManualBNClicked()
{
	ManualJuror(JUROR_DISTRIB_RESERVE);
}

void TJurorDistribDialog::JurorPedagogBNClicked()
{
	UpdateButton(JUROR_DISTRIB_JUROR);
}

void TJurorDistribDialog::ReJurorPedagogBNClicked()
{
	UpdateButton(JUROR_DISTRIB_REJUROR);
}

void TJurorDistribDialog::JurorXPedagogBNClicked()
{
	UpdateButton(JUROR_DISTRIB_JUROR_X);
}

void TJurorDistribDialog::ReservePedagogBNClicked()
{
	UpdateButton(JUROR_DISTRIB_RESERVE);
}

void TJurorDistribDialog::DeleteBNClicked()
{
	if (warn("Внимание! След изтриване разпределението %s", Q_RESTORE_CONTINUE))
		Destroy(IDDELETE);
}

bool TJurorDistribDialog::CanClose()
{
	return !locked || error("Изберете Потвърждение или Изтриване");
}

void TJurorDistribDialog::CancelBNClicked()
{
	EvClose();
}

void TJurorDistribDialog::EvClose()
{
	if (CanClose())
		TGroupDialog::EvClose();
}

void TJurorDistribDialog::OKBNClicked()
{
	if (IsValid())
	{
		*Group() << this;
		bool saved;

		if (distribution->prevDate == distribution->distrib->date)
			saved = Group()->Update();
		else
		{
			saved = Group()->Insert();

			if (saved && !distribution->distrib->closed)
			{
				TJurorDistrib distrib;

				distrib << Group();
				distrib.date = distribution->prevDate;
				distrib.Delete(true);
			}
		}

		if (saved)
			Destroy(IDOK);
	}
}
