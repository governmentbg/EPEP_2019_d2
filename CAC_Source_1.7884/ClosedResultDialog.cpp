DEFINE_RESPONSE_TABLE1(TClosedResultDialog, TSessionResultDialog)
//{{TClosedResultDialogRSP_TBL_BEGIN}}
	EV_BN_CLICKED(IDOK, OKBNClicked),
	EV_BN_CLICKED(IDPRINT, PrintBNClicked),
	EV_EN_KILLFOCUS(IDC_SESSION_RESULT_RETURNED, ReturnedKillFocus),
#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_REGIONAL
	EV_BN_CLICKED(IDLAUNCH, LaunchBNClicked),
#endif  // APPEAL || AREAL || REGIONAL
	EV_CBN_SELCHANGE(IDC_SESSION_RESULT_RESULT, ResultCBNSelChange),
	EV_BN_CLICKED(IDRIGHT, RightBNClicked),
	EV_BN_CLICKED(IDC_SESSION_RESULT_SURROUNDS, SurroundsBNClicked),
	EV_BN_CLICKED(IDC_SESSION_RESULT_CHANJED, ChanjedBNClicked),
	EV_BN_CLICKED(IDC_CLOSED_RESULT_RESTART, RestartBNClicked),
//{{TClosedResultDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TClosedResultDialog Implementation}}

TClosedResultDialog::TClosedResultDialog(TWindow* parent, TSession *session, TLawsuit *lawsuit, bool edit, bool tLockDate)
:
	TSessionResultDialog(parent, session, lawsuit, edit, IDD_CLOSED_RESULT), forceDate(session->forceDate),
		oldFirstSet(session->flagsX & SESSION_X_FIRSET), oldRestart(session->flagsY & SESSION_Y_RESTART),
		lockDate(tLockDate), backupIndex(session)
{
	header = new TStatic(this, IDC_CLOSED_RESULT_HEADER);
	date = new TDateFace(this, IDC_CLOSED_RESULT_DATE, "F_DATE");
	index = new TIndexFace(this, IDC_CLOSED_RESULT_INDEX, "F_INDEX", KIND_CLOSED_SESSIONS, SIZE_OF_SESSION_INDEX);
	eventNo = new TLongFace(this, IDC_CLOSED_RESULT_EVENT_NO, "F_EVENT_NO", SIZE_OF_EVENT_NO);
	print = new TPushButton(this, IDPRINT);

	firset = new TCheckFace(this, IDC_CLOSED_RESULT_FIRST_SET, "F_FLAGS_FIRSET");
#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_REGIONAL
	launch = new TButton(this, IDLAUNCH);
#endif  // APPEAL || AREAL || REGIONAL
	publInet = new TCheckFace(this, IDC_TROUBLE_PUBLINET, "F_FLAGS_PUBLIC");
#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_AREAL
	if (strchr(KIND_INCONSISTENCE_REQUESTS, lawsuit->sourceKind))
		publTreg = new TCheckFace(this, IDC_TROUBLE_PUBLTREG, "F_FLAGS_PUBLTREG");
#endif  // APPEAL || AREAL

	restart = new TCheckFace(this, IDC_CLOSED_RESULT_RESTART, "F_FLAGS_RESTART");
	ReadInterval(session->date, edit);
}

void TClosedResultDialog::SetupWindow()
{
	TSessionResultDialog::SetupWindow();

	mstr m;

	m.printf("%s %ld/%ldг.", Type->Seek(lawsuit->key.type), lawsuit->key.no, lawsuit->key.year);
	header->SetCaption(str(m));

	if (edit)
		publInet->SetCheck(!(Session()->flags & SESSION_PRIVATE));
	else
	{
		if (lockDate)
			returned->SetDate(Today);
		else
		{
			returned->SetDate(date->Date());
			date->SetS("");
			date->EnableWindow(true);
		}
	}

	// 2009:150 FIX: lock president if !extended
	if (president && !posts.chr(POST_EXTENDED))
		president->SetXCrit("");

	// 2006:018 Lock returned if requested; 2006:184 defun; 2006
	returned->EnableWindow(!return_date_locked(lawsuit));

	SetupPublic(true, Session()->flagsY & SESSION_Y_PUBLTREG, lawsuit);
	ResultCBNSelChange();

	if (edit)
		index->SetL(kind2index(Session()->kind, KIND_CLOSED_SESSIONS));
	else
		adjust_xxx_index(Session()->kind, index, KIND_CLOSED_SESSIONS);
	// 2017:039 FIX: also check killDate
	firset->EnableWindow(strchr(KIND_FIRST_SET_LAWS, lawsuit->kind));
#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_REGIONAL
	launch->ShowWindow(SW_SHOW);
	launch->EnableWindow(XFER_ACTIVE & XFER_EXPERTS);
#endif  // APPEAL || AREAL || REGIONAL

	initialized = true;
	SessionTimesChanged();
	restart->EnableWindow(flagsChanjed->GetCheck() == BF_CHECKED);
}

void TClosedResultDialog::CommitInterval()
{
	bool newFirstSet = Session()->flagsX & SESSION_X_FIRSET;

	if (!handle_restart(lawsuit->key, oldRestart, Session()->flagsY & SESSION_Y_RESTART, Session()->date, newFirstSet))
	{
		if (newFirstSet != oldFirstSet)
			interval.firstSet = newFirstSet ? Session()->date : CDate();

		TSessionResultDialog::UpdateInterval();
	}

	TSessionResultDialog::CommitGeneric();
}

void TClosedResultDialog::OKBNClicked()
{
	if (IsValid() && CheckSurrouendocs() && CheckSessionTimes() &&
		(!strchr(RESULT_FINALIZEDS, result->C()) ||
			(present_ses_person(lawsuit->kind, judge, SPERS_FIRST, SPERS_JUDGE) &&
			present_ses_person(lawsuit->kind, judgeX, SPERS_SECOND, SPERS_JUDGE) &&
			present_ses_person(lawsuit->kind, reJudgeX, SPERS_THIRD, SPERS_JUDGE))))
	{
		TSession *session = Session();
		*session << this;

		if ((restart->GetCheck() == BF_CHECKED) < oldRestart)
		{
			if (oldFirstSet)
				session->flagsX |= SESSION_X_FIRSET;
			else
				session->flagsX &= ~SESSION_X_FIRSET;
		}

		// 2015:296 must be before pre-move
		if (!edit)
			adjust_xxx_kind(session, index, KIND_CLOSED_SESSIONS);

		if (!PreMove())
			return;

		// for compatibility; 2016:286 not any more
		//session->entime.Clear();

		// 2011:341 SGS: for bankrupts
		if (publInet->GetCheck() == BF_UNCHECKED)
			session->flags |= SESSION_PRIVATE;
		else
			session->flags &= ~SESSION_PRIVATE;
		// 2011:343 LPR/LRQ: force date support
		if (assumes_decision(session->result))
			session->forceDate.Clear();
		else
			session->forceDate = forceDate;
		// 2007:016 LRQ: don't save president
		//strcpy(Session()->president, Session()->judge);
		// 2008:246 LPR: common (was missing)
		adjust_ses_final(session, RESULT_FINALIZEDS, session->returned);

		if (edit ? backupIndex.Update(session) : session->Insert())
			CommitInterval();
	}
}

bool TClosedResultDialog::IsValid()
{
	// 2010:216 FIX: session results reads interval based on date
	// 2016:131 LPR: also required for surround selection checks
	Session()->date = date->Date();

	bool isValid = TSessionResultDialog::IsValid();

	if (isValid)
	{
		bool newRestart = restart->GetCheck() == BF_CHECKED;

		if (returned->Date() < date->Date())
			isValid = error("Датата на връщане е по-малка от тази на заседанието.");
		else if (strchr(RESULT_FINALIZEDS, result->C()) && firset->GetCheck() == BF_CHECKED)
			isValid = error("Не може да насрочите I-во заседание с финализиращ делото резултат.");
		else if (strchr(RESULT_FINALIZEDS, result->C()) && restart->GetCheck() == BF_CHECKED)
			isValid = error("Не можете да продължите делото под същия номер с финализиращ делото резултат.");
		else if (firset->GetCheck() == BF_CHECKED && !oldFirstSet && !interval.firstSet.Empty())
		{
			isValid = error("Делото вече е насрочено за I-во заседание със закрито/разпоредително заседание от %Dг.",
				&interval.firstSet);
		}
		else if (oldRestart == newRestart)
		{
			// 2008:195 LPR: generic finished valid check
			if (!strchr(RESULT_FINALIZEDS, result->C()) || finished->Date() != returned->Date())
				isValid = lawsuit_finished_valid(interval, finished);
		}
		else if (already_restarted(lawsuit->key, oldRestart, newRestart, Session()->date))
			isValid = error("Делото вече продължава под същия номер на %Dг.", &Session()->date);
	}

	return isValid;
}

void TClosedResultDialog::SetFinished()
{
	if (interval.finished.Empty())
	{
		// 2001:207 date -> returned; 2009:105 comout finished empty check
		//if (finished->Date().Empty())
			finished->SetDate(returned->Date());
	}

	if (interval.age == AGE_NONE_YET)
	{
		char tAge = interval2LawAge(lawsuit->kind, interval.date, interval.firstSet, finished->Date());
		age->SetC(tAge);
	}
}

void TClosedResultDialog::ResultCBNSelChange()
{
	char tResult = result->C();

	// 2005:089 LPR/LRQ: eventNo support
	if (assumes_decision(tResult))
	{
		text->EnableWindow(false);
		eventNo->EnableWindow(false);
		eventNo->SetL(0);
		appeal->EnableWindow(false);
		appeal->SetCheck(BF_UNCHECKED);
	}
	else
	{
		text->EnableWindow(true);
		eventNo->EnableWindow(true);
		appeal->EnableWindow(!COURT_LOCK);
	}

	if (strchr(RESULT_FINALIZEDS, tResult))
	{
		SetFinished();
		trouble.ClearAct();
		trouble.Enable(false);
		restart->SetCheck(BF_UNCHECKED);
	}
	else
	{
		age->SetC(interval.age);
		finished->SetDate(interval.finished);
		Session()->result = tResult;

		if (trouble.act)
			trouble.IndexChanged(Session()->IndexKind());
		else
			trouble.Enable(true);
	}

	print->EnableWindow(lawsuit->key.type != TYPE_PUNISHMENT_LAW && strchr(RESULT_UNDOS, tResult));

	if (edit)
	{
		Session()->result = tResult;
		backupIndex.CheckIndex(Session(), eventNo);
	}
	else	// 2015:296 FIX: no enable on edit
		date->EnableWindow(!trouble.act && !lockDate);

	JudgeCBNSelChange();	// 2010:336
	UpdateSLCReason();
	UpdateRecujed();
	PublInetBNClicked();

	if (!TVirtualDecision::DecisionKind(tResult))
	{
		if (publTreg)
		{
			publTreg->EnableWindow(false);
			publTreg->SetCheck(Session()->flagsY & SESSION_Y_PUBLTREG);
		}

		PublTregBNClicked();
	}
}

void TClosedResultDialog::PrintBNClicked()
{
	if (IsValid())
	{
		*Session() << this;
		print_undo_result(this, Session(), lawsuit);
	}
}

// 2009:105 LPR: +related
void TClosedResultDialog::ReturnedKillFocus()
{
	if (returned->Date().Valid() && strchr(RESULT_FINALIZEDS, result->C()))
		SetFinished();
}

void TClosedResultDialog::LaunchBNClicked()
{
	xfer_launch(Session(), lawsuit->kind);
}

void TClosedResultDialog::RightBNClicked()
{
	AttachExtra(date->Date());
	date->EnableWindow(!trouble.act);
}

void TClosedResultDialog::SurroundsBNClicked()
{
	Session()->date = date->Date();
	TSessionResultDialog::SurroundsBNClicked();
}

void TClosedResultDialog::ChanjedBNClicked()
{
	if (flagsChanjed->GetCheck() == BF_CHECKED)
		restart->EnableWindow(true);
	else
	{
		restart->EnableWindow(false);
		restart->SetCheck(BF_UNCHECKED);
	}

	TSessionResultDialog::ChanjedBNClicked();
}

void TClosedResultDialog::RestartBNClicked()
{
	static constant messages[4] =
	{
		"Няма да има продължаване на делото под същия номер",

		"При продължаване на делото под същия номер, данните за "
		"продължителност и край не се записват, тъй като се отнасят към "
		"досегашното движение на делото, а не към новото",

		"При отмяна на продължаване на делото под същия номер, данните за "
		"продължителност, край и първо насрочване не се записват, тъй като "
		"се отнасят към отмененото продължаване",

		"Продължаването на делото под същия номер остава в сила"
	};

	info("%s.", messages[(restart->GetCheck() == BF_CHECKED) + (oldRestart << 1)]);
}
