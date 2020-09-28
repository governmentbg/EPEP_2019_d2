DEFINE_RESPONSE_TABLE1(TOpenResultDialog, TSessionResultDialog)
//{{TOpenResultDialogRSP_TBL_BEGIN}}
	EV_BN_CLICKED(IDOK, OKBNClicked),
	EV_CBN_SELCHANGE(IDC_SESSION_RESULT_RESULT, ResultCBNSelChange),
	EV_BN_CLICKED(IDC_OPEN_RESULT_CUTDOWN, CutDownBNClicked),
	EV_BN_CLICKED(IDRIGHT, RightBNClicked),
	EV_CBN_SELCHANGE(IDC_SESSION_COMPOSITION_JUDGE, JudgeCBNSelChange),
//{{TOpenResultDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TOpenResultDialog Implementation}}

TOpenResultDialog::TOpenResultDialog(TWindow* parent, TSession *session, TLawsuit *tLawsuit)
:
	TSessionResultDialog(parent, session, tLawsuit, session->result != RESULT_FIXED, IDD_OPEN_RESULT)
{
#if COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
	flagsProceed = new TCheckFace(this, IDC_OPEN_RESULT_PROCEED, "F_FLAGS_PROCEED");
	cutDown = new TCheckFace(this, IDC_OPEN_RESULT_CUTDOWN, "F_CUTDOWN");
	cutDownP1 = new TRadioFace(this, IDC_OPEN_RESULT_CUTDOWN_P1, "F_CUTDOWN_P1");
	cutDownP2 = new TRadioFace(this, IDC_OPEN_RESULT_CUTDOWN_P2, "F_CUTDOWN_P2");
#endif  // AREAL || MILITARY || REGIONAL
	header1 = new TStatic(this, IDC_OPEN_RESULT_HEADER_1);
	header2 = new TStatic(this, IDC_OPEN_RESULT_HEADER_2);
	flagsClosedD = new TCheckFace(this, IDC_OPEN_RESULT_CLOSEDD, "F_FLAGS_CLOSEDD");
#if COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
	times = new TPushButton(this, IDC_SESSION_TIMES_TIMES);
#endif  // AREAL || MILITARY || REGIONAL
	ReadInterval(session->date, true);
}

void TOpenResultDialog::SetupWindow()
{
	TSessionResultDialog::SetupWindow();

	// 2017:068 moved here from SessionResultDialog
	AdjustPresident();
	// 2008:060 LPR: copied from SetSessionDialog
	end->EnableWindow(lawsuit->key.type == TYPE_PUNISHMENT_LAW);

	initialized = true;
	SessionTimesChanged();

#if COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
	cutDown->EnableWindow(lawsuit->kind == KIND_COMMON_PUNISHMENT_LAW);
	cutDown->SetCheck(lawsuit->cutDown ? BF_CHECKED : BF_UNCHECKED);
#endif  // AREAL || MILITARY || REGIONAL
	CutDownBNClicked();
	ResultCBNSelChange();

#if COURT_TYPE == COURT_ADMIN || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY
	if (strchr(KIND_1ST_PUNISHMENT_LAWS, lawsuit->kind))
	{
		// Seek to empty 2nd juror for 1st instance punishment laws
		if (jurorX && !atob(Session()->jurorX))
			jurorX->SetS(EMPTY);
	}
#endif  // ADMIN || AREAL || MILITARY

#if COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
	flagsProceed->EnableWindow(lawsuit->key.type == TYPE_PUNISHMENT_LAW);
#endif  // AREAL || MILITARY || REGIONAL

	msql m;
	// 2006:075
	m.printf("%s %ld/%ldг., %s от %Dг.", Type->Seek(Session()->key.type), Session()->key.no, Session()->key.year,
		Kind->Seek(Session()->kind), &Session()->date);
	if (!Session()->time.Empty())
		m.printf(", %T часа", &Session()->time);
	header1->SetCaption(str(m));
	header2->SetCaption(Subject->Seek(lawsuit->key.type, lawsuit->subject));
}

void TOpenResultDialog::OKBNClicked()
{
	if (IsValid() && CheckSurrouendocs() && CheckSessionTimes() &&
		present_ses_person(lawsuit->kind, judge, SPERS_FIRST, SPERS_JUDGE) &&
		present_ses_person(lawsuit->kind, judgeX, SPERS_SECOND, SPERS_JUDGE) &&
		present_ses_person(lawsuit->kind, reJudgeX, SPERS_THIRD, SPERS_JUDGE) &&
		(!prosecutor->IsWindowEnabled() || atob(prosecutor) ||
			ask("В заседанието липсва %s. %s", SPERS_PROSECUTOR, Q_CONTINUE)))
	{
		if (!text->IsWindowEnabled())
			text->SetS("");

		TSession *session = Session();
		*session << this;

		if (!PreMove())
			return;
			
		// 2008:246 LPR: copied from closed res
		// 2008:317 FIX: returned -> end for open!
		// 2008:330 LPR: don't clear final on public w/ decision(s)
		if (session->result != RESULT_PUBLIC_DECISION || !try2decide(session, NULL))
			adjust_ses_final(session, RESULT_FINALIZEDS, session->end);

		if (!president)
			strcpy(session->president, Session()->judge);

	#if COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
		if (cutDownP1->GetCheck() == BF_CHECKED)
			lawsuit->cutDown = LAWSUIT_CUTDOWN_P1;
		else if (cutDownP2->GetCheck() == BF_CHECKED)
			lawsuit->cutDown = LAWSUIT_CUTDOWN_P2;
		else	// 2015:127 IRQ: FIX: clear on uncheck
			lawsuit->cutDown = 0;
	#endif  // AREAL || MILITARY || REGIONAL

		if (session->Update())
		{
			UpdateInterval();
			CommitGeneric();
		}
	}
}

bool TOpenResultDialog::IsValid()
{
	bool isValid = TSessionResultDialog::IsValid();

	if (isValid)
	{
		const TSession *session = Session();
		bool tReturned = !returned->Date().Empty();
		bool tEnd = !end->Date().Empty();
		CDate endate = tEnd ? end->Date() : session->date;

		if (tReturned && returned->Date() < session->date)
			isValid = error("Датата на връщане е преди датата на насрочване.");
		else if (tReturned && tEnd && returned->Date() < end->Date())
			isValid = error("Датата на връщане е преди крайната дата.");
		else if (tReturned && returned->Date() < session->date)
			isValid = error("Датата на връщане е преди датата на заседание.");
		else if (tEnd != (lawsuit->key.type == TYPE_PUNISHMENT_LAW))
			isValid = error("Крайната дата е невалидна за този вид дело.");
		// 2006:075 date->Date() -> Session()->date +others
		else if (tEnd && end->Date() < session->date)
			isValid = error("Крайната дата е преди датата на насрочване.");
		else if (secretar->IsWindowEnabled() && !atob(secretar))
			isValid = error("Секретарят е задължителен.");
		else if (startime && !startime->Time().Empty() && startime->Time() < session->time)
			isValid = error("Началния час е преди часа на насрочване.");
		else if (endate != session->date && entime && entime->Time().Empty())
			isValid = error("Крайния час е задължителен.");
	#if COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
		else if (cutDown->GetCheck() == BF_CHECKED && cutDownP1->GetCheck() == BF_UNCHECKED &&
			cutDownP2->GetCheck() == BF_UNCHECKED)
		{
			isValid = error("Не е уточнен вида на съкратеното производство.");
		}
	#endif  // AREAL || MILITARY || REGIONAL
		// 2008:195 LPR: generic finished valid check
		else if (!strchr(RESULT_FINALIZEDS, result->C()) || finished->Date() != endate)
			isValid = lawsuit_finished_valid(interval, finished);
	}

	return isValid;
}

void TOpenResultDialog::ResultCBNSelChange()
{
	char tResult = result->C();

	text->EnableWindow(!assumes_decision(tResult) ||
	#if COURT_TYPE == COURT_AREAL
		tResult == RESULT_LENDERS_MEETING ||	// 2010:111; 2012:037 FIX: was &&
	#endif  // AREAL
		tResult == RESULT_PUBLIC_DECISION);		// 2007:277

	if (strchr(RESULT_FINALIZEDS, tResult))
	{
		if (interval.finished.Empty())
			finished->SetDate(end->Date().Empty() ? Session()->date : end->Date());

		if (interval.age == AGE_NONE_YET)
		{
			char tAge = interval2LawAge(lawsuit->kind, interval.date, interval.firstSet, finished->Date());
			age->SetC(tAge);
		}
	}
	else
	{
		age->SetC(interval.age);
		finished->SetDate(interval.finished);
	}

	JudgeCBNSelChange();	// 2010:336
	UpdateSLCReason();
	UpdateRecujed();

#if COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
	if (strchr(RESULT_RECUJEDS, tResult))
	{
		flagsRecujed->ShowWindow(SW_SHOW);
		flagsProceed->ShowWindow(SW_HIDE);
		flagsProceed->EnableWindow(false);
		flagsProceed->SetCheck(BF_UNCHECKED);
	}
	else
	{
		flagsRecujed->ShowWindow(SW_HIDE);
		flagsProceed->ShowWindow(SW_SHOW);
		flagsProceed->EnableWindow(true);
		flagsProceed->SetCheck(edit && (Session()->flags & SESSION_PROCEED));
	}
#endif  // AREAL || MILITARY || REGIONAL

	Session()->result = tResult;
	trouble.IndexChanged(Session()->IndexKind());
}


void TOpenResultDialog::CutDownBNClicked()
{
#if COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
	bool cut = cutDown->GetCheck() == BF_CHECKED;

	cutDownP1->EnableWindow(cut);
	cutDownP1->SetCheck(cut && lawsuit->cutDown == LAWSUIT_CUTDOWN_P1 ? BF_CHECKED : BF_UNCHECKED);
	cutDownP2->EnableWindow(cut);
	cutDownP2->SetCheck(cut && lawsuit->cutDown == LAWSUIT_CUTDOWN_P2 ? BF_CHECKED : BF_UNCHECKED);
#endif  // AREAL || MILITARY || REGIONAL
}

void TOpenResultDialog::RightBNClicked()
{
	AttachExtra(Session()->date);
}

void TOpenResultDialog::JudgeCBNSelChange()
{
	TSessionResultDialog::JudgeCBNSelChange();
	// 2017:068 moved here from SessionResultDialog
	AdjustPresident();
}
