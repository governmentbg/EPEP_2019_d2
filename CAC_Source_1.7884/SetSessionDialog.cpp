DEFINE_RESPONSE_TABLE1(TSetSessionDialog, TSetSessionBaseDialog)
//{{TSetSessionDialogRSP_TBL_BEGIN}}
	EV_BN_CLICKED(IDOK, OKBNClicked),
	EV_BN_CLICKED(IDC_SET_SESSION_JURORS, JurorsBNClicked),
//{{TSetSessionDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TSetSessionDialog Implementation}}

TSetSessionDialog::TSetSessionDialog(TWindow* parent, TSession *session, TLawsuit *lawsuit, const char *kinds,
	TLoadGroup *tLoadGroup, TLoadGroup *tInvisible)
:
	TSetSessionBaseDialog(parent, session, lawsuit, kinds, false, IDD_SET_SESSION), loadGroup(tLoadGroup),
		invisible(tInvisible)
{
	time = new TTimeFace(this, IDC_SET_SESSION_TIME, "F_TIME", true);
	if (loadGroup)
		time->NoZeros();
	hall = new THallAliasFace(this, IDC_SET_SESSION_HALL, "F_HALL", loadGroup ? &loadGroup->halls : NULL);
	setReserve = new TCheckFace(this, IDC_SET_SESSION_SET_RESERVE, "F_SET_RESERVE");
	jurors = new TCheckFace(this, IDC_SET_SESSION_JURORS, "F_JURORS");
}

bool TSetSessionDialog::IsValid()
{
	bool isValid = TSetSessionBaseDialog::IsValid();

	if (isValid)
	{
		// 2007:220 LPR/LRQ: simple new 1-session check
		if (loadGroup && entime->Time().Empty())
			isValid = error("Крайния час е задължителен при насрочване по съдебни зали.");
		// 2008:060 FIX: was missing
		else if ((end->Date().Empty() || end->Date() == date->Date()) && !entime->Time().Empty() &&
			entime->Time() < time->Time())
		{
			isValid = error("Крайния час е преди началния.");
		}
		// 2006:339
		else if (loadGroup && !hall->L())
			isValid = error("Залата е задължителна при насрочване по съдебни зали.");
	}

	return isValid;
}

void TSetSessionDialog::SetupWindow()
{
	TSetSessionBaseDialog::SetupWindow();

	// 2005:108
	end->SetS("");
	entime->SetS("");

	if (!loadGroup)
		setReserve->EnableWindow(true);

	if (can_func(RIGHT_DISTRIB))
	{
		Subject->Seek(lawsuit->key.type, lawsuit->subject);

		if (strchr(Subject->Attrib(lawsuit->kind), POST_JUROR))
		{
			jurors->ShowWindow(SW_SHOW);
			jurors->EnableWindow(true);
		}
	}

	TPersonAliasFace *persons[] = { juror, reJuror, jurorX, NULL };

	for (int i = 0; persons[i]; i++)
	{
		TPersonAliasFace *person = persons[i];

		strcpy(jurorBackup[i], person->S());
		jurorEnabled[i] = person->IsWindowEnabled();
	}
}

void TSetSessionDialog::OKBNClicked()
{
	if (IsValid())
	{
		TSession *session = Session();
		// 2007:220 LPR/LRQ: simple new 1-session check/save
		*session << this;
		Kind->Seek(session->kind);

		if ((session->date.Work() || ask("%Dг. е почивен ден. %s", &session->date, Q_CONTINUE)) &&
			(session->date >= Today || ask("%Dг. е в минал период. %s", &session->date, Q_CONTINUE)) &&
			(session->date != Today || session->time >= Now() ||
				ask("%Tч. днес е в минало време. %s", &session->time, Q_CONTINUE)) &&
			(!strchr(KIND_FINISHED_SESSIONS, kind->C()) || TLawInterval::Finished(lawsuit, session->date) ||
				ask("Делото не е отбелязано като приключило. Желаете ли да го насрочите за %s?",
					Kind->Seek(session->kind))) &&  // 2017:159 re-seek Kind
			(!loadGroup || check_load_session(session, lawsuit, loadGroup, invisible)) &&
			check_set_session(session))
		{
			CloseDialog(setReserve && setReserve->GetCheck() == BF_CHECKED ? IDRESERVE : IDOK);
		}
	}
}

void TSetSessionDialog::JurorsBNClicked()
{
	TPersonAliasFace *persons[] = { juror, reJuror, jurorX, NULL };

	for (int i = 0; persons[i]; i++)
	{
		TPersonAliasFace *person = persons[i];

		if (jurors->GetCheck() == BF_CHECKED)
		{
			person->SetS(jurorBackup[i]);
			person->EnableWindow(false);
		}
		else
			person->EnableWindow(jurorEnabled[i]);
	}
}
