DEFINE_RESPONSE_TABLE1(TSessionResultDialog, TSessionTimesDialog)
//{{TSessionResultDialogRSP_TBL_BEGIN}}
	EV_CBN_SELCHANGE(IDC_SESSION_COMPOSITION_JUDGE, JudgeCBNSelChange),
	EV_BN_CLICKED(IDC_SESSION_RESULT_CHANJED, ChanjedBNClicked),
	EV_BN_CLICKED(IDC_SESSION_RESULT_SURROUNDS, SurroundsBNClicked),
//{{TSessionResultDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TSessionResultDialog Implementation}}

TSessionResultDialog::TSessionResultDialog(TWindow* parent, TSession *session, TLawsuit *lawsuit, bool edit, int resId)
:
	TSessionTimesDialog(parent, session, lawsuit, edit | ATTACH_RIGHT, resId), oldFinal(session->final),
		oldResult(session->result), chanjint(false), surroundSelection(session, edit), intervalRead(false)
{
	returned = new TDateFace(this, IDC_SESSION_RESULT_RETURNED, "F_RETURNED",
		strchr(KIND_CLOSED_SESSIONS, session->kind) != NULL);

	flagsChanjed = new TCheckFace(this, IDC_SESSION_RESULT_CHANJED, "F_FLAGS_CHANJED");

	strcpy(results, strchr(KIND_OPEN_SESSIONS, session->kind) ? Subject->Results(lawsuit->kind) :
		Subject->Closeds(lawsuit->kind));
#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_AREAL
	if (strchr(KIND_INCONSISTENCE_INDOCS, lawsuit->sourceKind))
		strcat(results, RESULT_SURROUENDOCS);
#endif  // APPEAL || AREAL
#if COURT_TYPE == COURT_REGIONAL
	if (lawsuit->sourceKind == KIND_HERITAGE_REQ_51 && strchr(KIND_OPEN_SESSIONS, session->kind))
		strcach_nx(results, RESULT_DETACHED_SETT);
#endif  // REGIONAL

	result = new TCharAliasFace(this, IDC_SESSION_RESULT_RESULT, "F_RESULT", Result, results);
	slcReason = new TSLCReasonAliasFace(this, IDC_SESSION_RESULT_SLC_REASON, "F_SLC_REASON", lawsuit->kind,
		edit ? FLAG_NULL : FLAG_ACTIVE);
	slcReasonLabel = new TStatic(this, IDC_SESSION_RESULT_SLC_REASON_LABEL);
	flagsRecujed = new TCheckFace(this, IDC_SESSION_RESULT_RECUJED, "F_FLAGS_RECUJED");
	// 2005:076 LPR: AGE_ALLS; 2005:088 AGE_AGES/AGE_ALLS; 2010:166 LPR/LRQ: AGE_AGES
	age = new TCharAliasFace(this, IDC_SESSION_RESULT_AGE, "F_AGE", Age, AGE_AGES);
	finished = new TDateFace(this, IDC_SESSION_RESULT_FINISHED, "F_FINISHED", false);
	text = new TStringFace(this, IDC_SESSION_RESULT_TEXT, "F_TEXT", SIZE_OF_SESSION_TEXT);
	expert = new TCheckFace(this, IDC_SESSION_RESULT_EXPERT, "F_FLAGS_EXPERT");
	appeal = new TCheckFace(this, IDC_SESSION_RESULT_APPEAL, "F_FLAGS_APPEAL");
	// 2016:090 TRQ/LPR: official
	surrounds = new TPushButton(this, IDC_SESSION_RESULT_SURROUNDS);
}

void TSessionResultDialog::CommitGeneric()
{
	TSessionTimesDialog::CommitTimes();
	surroundSelection.Update(lawsuit);	// 2016:085
	PostMove();
	CmOk();
}

void TSessionResultDialog::UpdateInterval()
{
	if (interval.age != age->C() || interval.finished != finished->Date() || interval.firstSet != oldFirstSet)
	{
		interval.age = age->C();
		interval.finished = finished->Date();
		interval.Update("F_AGE, F_FINISHED, F_FIRST_SET");
	}
}

void TSessionResultDialog::UpdateSLCReason()
{
	if (result->C() == RESULT_STOPPED)
	{
		slcReason->SetXCrit(C2S[SLC_TYPE_STOPPED]);
		slcReason->SetL(Session()->slcReason);
		slcReasonLabel->SetCaption("Основание за спиране");
	}
	else if (strchr(RESULT_SLC_LEFTS, result->C()))
	{
		slcReason->SetXCrit(C2S[SLC_TYPE_LEFT]);
		slcReason->SetL(Session()->slcReason);
		slcReasonLabel->SetCaption("Основание за отлагане");
	}
	else
		slcReason->SetXCrit("");
}

void TSessionResultDialog::UpdateRecujed()
{
	if (strchr(RESULT_RECUJEDS, result->C()))
	{
		flagsRecujed->EnableWindow(true);
		flagsRecujed->SetCheck(!edit || (Session()->flagsX & SESSION_X_RECUJED) ? BF_CHECKED : BF_UNCHECKED);
	}
	else
	{
		flagsRecujed->EnableWindow(false);
		flagsRecujed->SetCheck(BF_UNCHECKED);
	}
}

void TSessionResultDialog::AttachExtra(const CDate &date)
{
	TGArray<TUser> judges;
	TPersonAliasFace *fields[4] = { judge, president, judgeX, reJudgeX };

	for (int i = 0; i < 4; i++)
	{
		TPersonAliasFace *field = fields[i];

		if (field && atob(field->S()))
		{
			TUser *user = new TUser;
			strcpy(user->ucn, field->S());
			judges.Add(user);
		}
	}

	if (judges.Count())
	{
		Session()->result = result->C();
		attach_right(this, Session()->key, trouble, Session()->IndexKind(), date, judges);
	}
	else
		error("Няма избран нито един съдия.");
}

bool TSessionResultDialog::PreMove()
{
	if (!trouble.act)
		return true;

	TPersonAliasFace *fields[4] = { judge, president, judgeX, reJudgeX };

	for (int i = 0; i < 4; i++)
	{
		TPersonAliasFace *field = fields[i];

		if (field && !strcmp(field->S(), trouble.act->judge))
			return true;
	}

	return error("Регистрирания съдебен акт не съответства на нито един от избраните съдии.");
}

bool TSessionResultDialog::CheckSurrouendocs()
{
	TNTResult tResult;

	return !oldResult || !strchr(RESULT_SURROUENDOCS, oldResult) || strchr(RESULT_SURROUENDOCS, result->C()) ||
		ask("Поради промяната на резултата от \"%s\" на \"%s\", всички съдебни актове, свързани с разглеждане на "
			"възражения, ще бъдат изтрити. %s", tResult.Seek(oldResult), Result->Seek(result->C()), Q_CONTINUE);
}

bool TSessionResultDialog::ReadInterval(const CDate &date, bool mandatory)
{
	if (!intervalRead)
	{
		interval.key = lawsuit->key;
		interval.date = date;

		if (interval.Read(NULL, !mandatory))
		{
			if (mandatory)
				intervalRead = true;

			oldFirstSet = interval.firstSet;
			return true;
		}
		else if (mandatory)
			failure();
	}

	return false;
}

bool TSessionResultDialog::IsValid()
{
	bool isValid = TSessionTimesDialog::IsValid();

	if (isValid)
	{
		const TSession *session = Session();
		bool tReturned = !returned->Date().Empty();
		bool tAge = age->C() != AGE_NONE_YET;
		bool tFinished = !finished->Date().Empty();

		interval.key = lawsuit->key;
		interval.date = session->date;

		if (tReturned && returned->Date() > Today)
			isValid = error("Датата на връщане е по-голяма от текущата дата.");
		else if (tAge != tFinished)
			isValid = error("Продължителност и край на делото не си съответстват.");
		else if (!interval.Read(NULL, true))
			isValid = error("Датата на заседание е преди образуването на делото.");
		else if (surroundSelection.InFuture())
			isValid = error("Избрани са съпровождащи документи, постъпили след датата на заседанието.");
		else if (strchr(RESULT_SURROUENDOCS, result->C()) && !surroundSelection.Contains(KIND_SURROUENDOCS))
			isValid = error("Не са избрани съпровождащи документи, съответстващи на резултата.");
	}

	return isValid;
}

bool TSessionResultDialog::IsClean()
{
	bool isClean = TSessionTimesDialog::IsClean();

	if (isClean)
	{
		isClean = !edit || !surroundSelection.Changed() || ask("Промените в съпровождащите документи се записват "
			"само при потвърждение. Желаете ли да прекъснете редактирането?");
	}

	return isClean;
}

void TSessionResultDialog::SetupWindow()
{
	if (edit)
		Session()->flags &= ~SESSION_EXPERT;

	TSessionTimesDialog::SetupWindow();

	if (!can_func(RIGHT_RECOMPOS))
		composition->EnableWindow(false);

	// 2010:336 now derived ResultCBNSelChange() -> JudgeCBNSelChange()
	//JudgeCBNSelChange();
#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_REGIONAL
	expert->ShowWindow(SW_SHOW);
	expert->EnableWindow(XFER_ACTIVE & XFER_EXPERTS);
#endif  // APPEAL || AREAL || REGIONAL

	if (!COURT_LOCK)
	{
		appeal->ShowWindow(SW_SHOW);
		appeal->EnableWindow(true);
	}

	if (surrounds)
	{
		surrounds->EnableWindow(true);
		surrounds->ShowWindow(SW_SHOW);
	}

	// 2017:038 common
	age->SetC(interval.age);
	finished->SetDate(interval.finished);
}

void TSessionResultDialog::JudgeCBNSelChange()
{
	// 2010:335 LPR: aeria gloris
	bool force = strchr(RESULT_COMPLETIZEDS, result->C()) && atob(judge->S()) && strcmp(judge->S(), lawsuit->judge);

	if (force && flagsChanjed->GetCheck() == BF_UNCHECKED)
	{
		flagsChanjed->SetCheck(BF_CHECKED);
		chanjint = true;
	}
	else if (PROPAGANDATE_JUDGE)
	{
		// 2002:220 LRQ: Extended again... That's for explicit (re)change
		// 2007:198 N.B. This always clears the flag on edit, which is right
		flagsChanjed->SetCheck(atob(judge) && strcmp(judge->S(), Session()->judge) ? BF_CHECKED : BF_UNCHECKED);
		chanjint = false;
	}
	else if (!force && chanjint)
	{
		flagsChanjed->SetCheck(BF_UNCHECKED);
		chanjint = false;
	}
}

void TSessionResultDialog::ChanjedBNClicked()
{
	chanjint = false;
}

void TSessionResultDialog::SurroundsBNClicked()
{
	surroundSelection.Interact(this);
}
