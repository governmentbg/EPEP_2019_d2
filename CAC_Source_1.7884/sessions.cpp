#include "cac.h"

static constant ASK_PRINT_SUBS = "Желаете ли да отпечатате призовки за заседанието?";

static void eportal_update(const TSession *session, const TLawsuit *lawsuit, char tResult)
{
	if (tResult)
	{
		// 2018-10-19: LPR: don't compare w/ old ISEPortSessEndoc()
		if (!session->IsEPortSessEndoc())
			eportal_send(NULL, session, lawsuit, WHAT_SESS_ENDOC, LOG_DELETE);

		// 2018-10-19: LPR: don't compare w/ old ISEPortSessEndoc()
		if (!session->IsEPortSession())
			eportal_send(NULL, session, lawsuit, WHAT_SESSION, LOG_DELETE);
	}

	if (session->IsEPortSession())
		eportal_send(NULL, session, lawsuit, WHAT_SESSION, LOG_WRITE);

	if (session->IsEPortSessEndoc())
		eportal_send(NULL, session, lawsuit, WHAT_SESS_ENDOC, LOG_WRITE);
}

// 2005:145 LRQ/LPR: made function: used by followiation, unset and reset
static bool revoke_session(TSession *session, const TRCDKeyContainer *resolution, const TLawsuit *lawsuit, char result)
{
	if (resolution)
	{
		session->text.clear();
		session->text.printf("Поради %s от %Dг. - %s", Kind->Seek(resolution->kind),
			&container_event_date1(resolution), Result->Seek(resolution->Status()));
	}

	char tResult = session->result;
	session->result = result;

	if (session->Update())	// 2018-10-09: LPR: F_RESULT, F_TEXT -> NULL to update F5
	{
		// 2010:011 LPR: always unschedule on revoke
		unschedule(SCHED_PPREPARE, session);
		// 2010:350 FIX: was in unset only
		unschedule(SCHED_FIXED, session);
		// 2016:261; 2018-10-05: was _send
		eportal_update(session, lawsuit, tResult);
		return true;
	}

	return false;
}

static void abort_after(const CDate &after, TLawsuit *lawsuit, TSession *killer)
{
	TGArray<TSession> unset;
	msql m(killer->Name);

	m.Add(killer, "F_NO, F_YEAR, F_TYPE");
	m.AddChars("F_KIND", KIND_OPEN_SESSIONS);
	m.Add("F_DATE");
	m.printf(">= %ld", after.Value());
	m.AddChar("F_RESULT", RESULT_FIXED);

	TQuery q(str(m));
	while (q.Read())
	{
		TSession *session = new TSession;
		*session << q;

		if (ask("По делото има насрочено заседание за %D.\nЖелаете ли да го отмените?", &session->date))
		{
			TSideWind sideWind;
			TLinkWind linkWind;
			char tResult = session->result;

			revoke_session(session, killer, lawsuit, RESULT_ABORTED);
			log(LOG_ABORT, 0, session);
			copy_winds(session, killer, sideWind);
			copy_winds(session, killer, linkWind);

			session->result = tResult;
			unset.Add(session);
		}
		else
			delete session;
	}

	xfer_bank_delete_sessions(lawsuit, &unset);
}

class TBackupIndex : private TEventIndex
{
public:
	TBackupIndex(const TIndexContainer *container);

	bool Update(TIndexContainer *container) const;	// 2018-04-19: +related
	long EventNo() { return eventNo; }

	bool SameIndex(const TIndexContainer *container) const;
	void CheckIndex(const TIndexContainer *container, TLongFace *eventNo) const;

private:
	virtual bool Update(const char *fields = NULL) { return false; }
};

TBackupIndex::TBackupIndex(const TIndexContainer *container)
{
	eventNo = container->eventNo;
	year = container->IndexYear();
	type = container->IndexType();
	kind = container->IndexKind();
}

bool TBackupIndex::Update(TIndexContainer *container) const
{
	if (SameIndex(container))
		return container->Update();

	if (container->IndexEvent())
	{
		if (container->Update())
		{
			if (eventNo)
			{
				*DecisionIndex << this;
				DecisionIndex->Delete(true);
			}

			return true;
		}
		else
		{
			container->DeleteIndex();
			container->eventNo = eventNo;
		}
	}

	return false;
}

bool TBackupIndex::SameIndex(const TIndexContainer *container) const
{
	return container->IndexYear() == year && container->IndexType() == type && container->IndexKind() == kind;
}

void TBackupIndex::CheckIndex(const TIndexContainer *container, TLongFace *tEventNo) const
{
	if (SameIndex(container))
	{
		tEventNo->EnableWindow(false);
		tEventNo->SetL(eventNo);
	}
	else
	{
		tEventNo->SetL(0);
		tEventNo->EnableWindow(container->IndexType());
	}
}

// 2008:287 LPR: bool -> void, result used by open_result only
static void finish_session(TWindow *parent, TSession *session, TLawsuit *tLawsuit, TLawsuit *lawsuit,
	TGArray<TSurround> *surrounds, bool enter, char tResult)
{
	// 2018-10-05: moved to _update
	eportal_update(session, lawsuit, tResult);

	if (session->flags & SESSION_EXPERT)
	{
		TWaitWindow wait(parent, WAIT_CC);
		xfer_request(session, lawsuit->kind);
		xfer_assign(session, lawsuit->kind);
	}

	if (!strchr(RESULT_ENTERABLES, session->result))
		eispp_accept(parent, lawsuit, session, NULL);
	else	// 2013:036
		eispp_delete(session);

	if (strchr(RESULT_UNDOS, session->result))
	{
		if (CourtType == COURT_APPEAL)
			lawsuit->decMade = session->returned.Empty() ? session->date : session->returned;
		// 2003:064
		unschedule(SCHED_DPREPARE, lawsuit);
	}

	// 2005:116 LRQ; 2005:136 BUFGIX: tSession -> tResult
	// 2017:326 LPR: ask() is included in delete_finalization()
	if (tResult && strchr(RESULT_FINALIZEDS, tResult) != NULL && strchr(RESULT_FINALIZEDS, session->result) == NULL)
		delete_finalization(session, lawsuit);

	// 2012:125 FIX: check for empty finalized
	// 2017:065 intervals: lawsuit->finished -> session->final
	if (strchr(RESULT_FINALIZEDS, session->result) && !session->final.Empty())
		abort_after(session->final, lawsuit, session);
	else if (strchr(RESULT_FOLLOWABLES, session->result))
		abort_after(session->date, lawsuit, session);

	// 2005:136 BUFGIX: tSession -> tResult
	// 2011:258 LPR: enteriation_cleanup() inlined
	// 2011:271 LPR/LRQ: delete exact unsupported acts, check inregs/surrounds
	// 2011:271 LPR: RESULT_ENTERABLES -> !FIXED, may have handing documents
	// 2011:290 LPR: exclude empty tResult; 2011:290 event only if unchanged
	if (!tResult || tResult == RESULT_FIXED || session->result == tResult)
		xfer_bank_event(lawsuit, session, surrounds, NULL, NULL, NULL, NULL, tResult ? XFER_EDIT : XFER_ADD);
	else
	{
		TGArray<TDecision> decisions;
		mstr m;
		bool failed = false;
		mstr decisionKinds;
		int index = 0;

		session->DecisionKinds(decisionKinds, lawsuit);
		try2decide(session, &decisions);

		while (index < decisions.Count())
		{
			TDecision *decision = decisions[index];
			const char *error = NULL;

			if (decisionKinds.chr(decision->kind))
			{
				index++;
				continue;
			}

			if (any(m))
				m.cat("\n");

			m.printf("Документът %s от %D ", Kind->Seek(decision->kind), &decision->fromDate);

			if (!inreg_clean(decision, NULL))
				error = "има документи от входящ регистър";
			else if (!surround_clean(decision, NULL))
				error = "има съпровождащи документи";
			else if (!delete_decision(decision, lawsuit))
				error = "грешка при изтриване";

			if (error)
			{
				m.printf("не е изтрит - %s.", error);
				failed = true;
				index++;
			}
			else
			{
				m.cat("е изтрит.");
				decisions.Remove(index);
			}
		}

		TGArray<TSurroundment> surrouendocs;

		if ((tResult && strchr(RESULT_SURROUENDOCS, tResult)) || strchr(RESULT_SURROUENDOCS, session->result))
		{
			try2surround(session, &surrouendocs, NULL, KIND_SURROUENDOCS, true);

			for (int i = 0; i < surrouendocs.Count(); i++)
			{
				TSurroundment *surrouendoc = surrouendocs[i];

				if (strchr(RESULT_SURROUENDOCS, session->result))
				{
					TDecision *decision = new TDecision;
					decision->Init(surrouendoc);
					decisions.Add(decision);
				}
				else
					delete_surroundment(surrouendoc, lawsuit, false);
			}
		}

		xfer_bank_session_result_change(lawsuit, session, surrounds, &decisions, tResult);

		if (any(m))
			message(failed ? MB_ERROR : MB_INFO, "%s", str(m));
	}

	bool finishChange = lawsuit->finished != tLawsuit->finished && !lawsuit->finished.Empty();

	if (!strcmp(lawsuit->judge, INTER) || (session->flags & SESSION_CHANJED) || finishChange ||
		// 2008:287 LRQ
		(lawsuit->finished.Empty() && session->result == RESULT_PUBLIC_DECISION))
	{
		finished_judge(lawsuit, session);
		// 2013:238
		if (finishChange)
		{
			unschedule(SCHED_RET_PROS_1, lawsuit);
			//unschedule(SCHED_RET_PROS_3, lawsuit);
		}
	}

	// 2014:163 cutDown flag -> value
	if (lawsuit->cutDown != tLawsuit->cutDown)
	{
		lawsuit->cutDown = tLawsuit->cutDown;
		lawsuit->Update("F_CUTDOWN");
	}

#if COURT_TYPE == COURT_AREAL
	// 2002:113; 2007:265 entire if goes to AREAL
	if (lawsuit->key.type == TYPE_FIRM_LAW)
	{
		// 2014:002 using set
		if (strchr(SCHED_UNHOLDS, session->result))
			unschedule(SCHED_HOLD, lawsuit);
	}
	else
#endif  // AREAL
	{
		if (session->result == RESULT_STOPPED)
			unschedule(SCHED_HOLD, lawsuit);
		// 2002:140 LRQ: RESULT_WITH_SETTLEMENT
		else if (session->result == RESULT_CANCELED || session->result == RESULT_WITH_SETTLEMENT)
		{
			unschedule(SCHED_HOLD, lawsuit);
			unschedule(SCHED_STOPPED_M, lawsuit);
		}
	}

	// 2006:180; 2010:281 sched allow 368 -> inline
#if COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
	if (session->result == RESULT_WITH_SETTLEMENT && lawsuit->kind == KIND_PRIVATE_PUNISHMENT_LAW &&
		lawsuit->sourceKind == KIND_REQUEST_368)
	{
		schedule(SCHED_SACTION, session, &session->date);
	}
#endif  // AREAL || MILITARY || REGIONAL

	// 2003:247
	if (enter && AUTORUN_MSRS && can_func(RIGHT_MESSAGES) && strchr(RESULT_SESSION_MESSAGES, session->result) &&
		ask("Желаете ли да изготвите съобщения?"))
	{
		edit_sides(parent, session, lawsuit);
	}

	if (enter && AUTORUN_ENDOCS && can_func(RIGHT_DECISION) && strchr(RESULT_SURROUENTERABLES, session->result))
		enter_act(parent, session, lawsuit);

	if (session->flagsX & SESSION_X_FIRSET)
	{
		TSession tSession;
		set_session(parent, &tSession, lawsuit, NULL, NULL);
	}
}

#include "SelectSurroundsListBox.h"
#include "SelectSurroundsListBox.cpp"

#include "SelectSurroundsDialog.h"
#include "SelectSurroundsDialog.cpp"

// ----- TSurroundSelection --------------------------------------------------
struct TSurroundSelection
{
	TSurroundSelection(const TSession *tSession, bool tEdit);

	bool Contains(const char *kinds);
	bool InFuture() const;
	bool Changed() const;
	void Interact(TWindow *parent);
	void Update(TLawsuit *lawsuit);

protected:
	const TSession *session;
	bool edit;
	CDate loadedDate;
	TGArray<TSurround> possibles;
	TGArray<TSurround> oldSelecteds;
	TGArray<TSurround> newSelecteds;

	bool Reload();
};

TSurroundSelection::TSurroundSelection(const TSession *tSession, bool tEdit)
	: session(tSession), edit(tEdit)
{
}

bool TSurroundSelection::Contains(const char *kinds)
{
	Reload();

	for (int i = 0; i < newSelecteds.Count(); i++)
		if (strchr(kinds, newSelecteds[i]->kind))
			return true;

	return false;
}

bool TSurroundSelection::InFuture() const
{
	for (int i = 0; i < newSelecteds.Count(); i++)
		if (newSelecteds[i]->date > session->date)
			return true;

	return false;
}

bool TSurroundSelection::Changed() const
{
	if (oldSelecteds.Count() != newSelecteds.Count())
		return true;

	for (int i = 0; i < oldSelecteds.Count(); i++)
		if (oldSelecteds[i]->key != newSelecteds[i]->key)
			return true;

	return false;
}

void TSurroundSelection::Interact(TWindow *parent)
{
	bool reloaded = Reload();
	TSelectSurroundsDialog(parent, &possibles, &newSelecteds, reloaded).Execute();
}

void TSurroundSelection::Update(TLawsuit *lawsuit)
{
	int i = 0;
	TSurroundment surroundment;

	while (i < oldSelecteds.Count())
	{
		TSurround *oldSelected = oldSelecteds[i];
		int n;

		for (n = 0; n < newSelecteds.Count(); n++)
			if (newSelecteds[n]->key == oldSelected->key)
				break;

		if (n < newSelecteds.Count())
		{
			oldSelecteds.Remove(i);
			newSelecteds.Remove(n);
		}
		else
		{
			surroundment.Init(oldSelected, session);
			if (!strchr(RESULT_SURROUENDOCS, session->result) || surroundment.Try())
				delete_surroundment(&surroundment, lawsuit, true);
			i++;
		}
	}

	for (int n = 0; n < newSelecteds.Count(); n++)
	{
		surroundment.Init(newSelecteds[n], session);
		surroundment.Insert();
	}
}

bool TSurroundSelection::Reload()
{
	bool reloaded = false;

	if (loadedDate != session->date)
	{
		TSurround surround;
		msql m(surround.Name);
		TSurroundment surroundment;

		possibles.Flush();
		oldSelecteds.Flush();

		for (int i = 0; i < newSelecteds.Count(); )
		{
			if (newSelecteds[i]->date > session->date)
				newSelecteds.Remove(i);
			else
				i++;
		}

		surround.lawsuit = session->key;
		m.Add(&surround, "F_LAWSUIT_NO, F_LAWSUIT_YEAR, F_LAWSUIT_TYPE");
		m.Add("F_DATE");
		m.printf("<= %ld", session->date.Value());
		m.AddOrder("F_YEAR, F_NO");

		TQuery q(str(m));
		while (q.Read())
		{
			surround << q;
			surroundment.Init(&surround, session);

			if (edit && surroundment.Try("F_NO"))
			{
				possibles.AddCopy(&surround);
				oldSelecteds.AddCopy(&surround);
				newSelecteds.AddCopy(&surround);
			}
			else if (strmatch(surround.Status(), RESULT_SURROUNDABLES))
				possibles.AddCopy(&surround);
			//else
			//	add to mstr w/ "wrong status"
		}

		reloaded = !loadedDate.Empty();
		loadedDate = session->date;
	}

	return reloaded;
}

// 2016:116 from T634Book::PreviousSurround, modified
static void try2surround(const TSession *session, TGArray<TSurround> *surrounds)
{
	msql m("T_SURROUNDMENT M JOIN T_SURROUND S ON S.F_NO = M.F_NO AND S.F_YEAR = M.F_YEAR AND S.F_TYPE = M.F_TYPE "
		"AND S.F_KIND = M.F_KIND", "S.*");
	TSurround surround;

	m.AddLong("M.F_LAWSUIT_NO", session->key.no);
	m.AddLong("M.F_LAWSUIT_YEAR", session->key.year);
	m.AddChar("M.F_LAWSUIT_TYPE", session->key.type);
	m.AddChar("M.F_SESSION_KIND", session->kind);
	m.AddDate("M.F_DATE", session->date);
	surround.LoadArray(surrounds, str(m));
}

#include "SessionCompositionDialog.h"
#include "SessionCompositionDialog.cpp"

// ----- TSesTimesList -------------------------------------------------------
struct TSesTimesList
{
	TSesTimesList(const TBaseSession *tSession);

	void Load();
	void FillGaps();  // 2017:243
	void ReInit(const CTime &startime, const CDate &endate, const CTime &entime);
	bool IsValid(const CTime &startime, const CDate &endate, const CTime &entime) const;
	void RemoveEmpty();  // 2017:243
	void Update();

	TGSArray<TSessionTimes> oldTimes;
	TGSArray<TSessionTimes> newTimes;
	bool dirty;

protected:
	const TBaseSession *session;

	int NDays(const CDate &endate) const;
	void AddNewTime(const CDate &date);
};

TSesTimesList::TSesTimesList(const TBaseSession *tSession)
	: session(tSession), dirty(false)
{
}

void TSesTimesList::Load()
{
	TSessionTimes sTimes;

	sTimes << session;
	sTimes.LoadFloat(oldTimes);

	for (int i = 0; i < oldTimes.Count(); i++)
		newTimes.AddCopy(oldTimes[i]);
}

int TSesTimesList::NDays(const CDate &endate) const
{
	int nDays = 0;

	for (CDate date = session->date; date <= endate; date += 1)
		nDays++;

	return nDays;
}

void TSesTimesList::AddNewTime(const CDate &date)
{
	TSessionTimes *sTimes = new TSessionTimes();

	*sTimes << session;
	(CDate &) sTimes->start = date;
	sTimes->entime.Clear();
	newTimes.Add(sTimes);
}

void TSesTimesList::FillGaps()
{
	if (newTimes.Count())
	{
		CDate final = session->end.Empty() ? session->date : session->end;
		TSessionTimes sTimes;

		for (CDate date = session->date; date < final; date += 1)
		{
			(CDate &) sTimes.start = date;

			if (newTimes.Find(&sTimes) == INT_MAX)
				AddNewTime(date);
		}
	}
}

void TSesTimesList::ReInit(const CTime &startime, const CDate &endate, const CTime &entime)
{
	if (NDays(endate) != newTimes.Count())
	{
		newTimes.Flush();

		for (CDate date = session->date; date <= endate; date += 1)
			AddNewTime(date);

		dirty = false;
	}
	else
	{
		mstr m;

		if ((const CTime &) newTimes[0]->start != startime)
			m.printf("Началния час за %Dг. е променен на на %Tч.", &session->date, &startime);

		if (newTimes[newTimes.Count() - 1]->entime != entime)
		{
			m.sep("\n");
			m.printf("Крайния час за %Dг. е променен на %Tч.", &endate, &entime);
		}

		if (any(m))
			message("%s\n\nПроверете отработените часове.", str(m));
	}

	(CTime &) newTimes[0]->start = startime;
	newTimes[newTimes.Count() - 1]->entime = entime;
}

bool TSesTimesList::IsValid(const CTime &startime, const CDate &endate, const CTime &entime) const
{
	const char *status = session_times_complect(false, startime, entime);

	if (status)
		return error("%s.", status);

	if (endate.Empty())
		return error("Крайната дата е задължителна.");

	if (NDays(endate) != newTimes.Count())
		return error("Крайната дата е променена.\nВъведете продължителността по заседателни дни.");

	if ((const CTime &) newTimes[0]->start != startime)
	{
		info("Началния час е променен.\nПрегледайте продължителността по заседателни дни.");
		return false;
	}

	if (newTimes[newTimes.Count() - 1]->entime != entime)
	{
		info("Крайния час е променен.\nПрегледайте продължителността по заседателни дни.");
		return false;
	}

	return true;
}

void TSesTimesList::RemoveEmpty()
{
	for (int i = 0; i < newTimes.Count(); )
	{
		TSessionTimes *newTime = newTimes[i];

		if (newTime->entime.Empty() && ((CTime &) newTime->start).Empty())
			newTimes.Remove(i);
		else
			i++;
	}
}

void TSesTimesList::Update()
{
	for (int i = 0; i < newTimes.Count(); i++)
	{
		TSessionTimes *newTime = newTimes[i];
		int n;

		for (n = 0; n < oldTimes.Count(); n++)
			if (newTime->start == oldTimes[n]->start)
				break;

		if (n == oldTimes.Count())
			newTime->Insert();
		else
		{
			newTime->Update();
			oldTimes.Remove(n);
		}
	}

	for (int i = 0; i < oldTimes.Count(); i++)
		oldTimes[i]->Delete(true);
}

#include "SessionTimesDialog.h"
#include "SessionTimesDialog.cpp"

#include "SessionResultDialog.h"
#include "SessionResultDialog.cpp"

#include "ClosedResultDialog.h"
#include "ClosedResultDialog.cpp"

#if TESTVER
static constant LAWSUIT_TO_SESSION = "F_NO, F_YEAR, F_TYPE, F_COMPOSITION, F_JUDGE";
#endif  // TESTVER

void closed_result(TWindow *parent, TLawsuit *lawsuit, CDate *sessionDate)
{
	TLawsuit tLawsuit;
	TSession session;

	tLawsuit << lawsuit;
#if TESTVER
	// 2018-07-02: only copy the required fields
	session.ImportFields(lawsuit, LAWSUIT_TO_SESSION);
	session.kind = KIND_CLOSED_SESSION;
#else  // TESTVER
	session << lawsuit;
	session.kind = KIND_CLOSED_SESSION;
	session.date = sessionDate ? *sessionDate : Today;
	session.text.cpy("");
	session.flags = 0;		// 2004:111 FIX: flags must not be copied
	session.forceDate.Clear();	// 2012:170 FIX: forceDate must not be copied
	session.flagsX = 0;		// 2016:174 FIX: flagsX must not be copied
	session.flagsY = 0;		// 2018-06-12: FIX: flagsY must not be copied
#endif  // TESTVER

	// 2016:009 kill date -> pre-wanted
	if ((!TLawInterval::Finished(lawsuit, Today) ||
		warn("Делото е отбелязано като приключило. Желаете ли да впишете резултат от ЗЗ/РЗ за него?")) &&
		TClosedResultDialog(parent, &session, lawsuit, false, sessionDate != NULL).Execute() == IDOK)
	{
		log_event(LOG_ENTER, CM_CLERK_CLOSED_RESULT, &session, !(session.flags & SESSION_PRIVATE));
		// 2014:293
		{
			if (strchr(RESULT_HOLDS, session.result))
				send_sms(lawsuit, "Обявено без движение на", SMS_HOLD, SMS_NEW, &session);
			else if (session.result == RESULT_STOPPED)
				send_sms(lawsuit, "Спряно на", SMS_STOP, SMS_NEW, &session);
		}

		xfer_email(&session);
		TGArray<TSurround> surrounds;
		try2surround(&session, &surrounds);

	#if COURT_TYPE == COURT_AREAL
		// 2003:066; 2003:070 BUGFIX: surrounds may be NULL
		// 2003:075 BUGFIX: always unschedule SCHED_FREGISTER
		if (lawsuit->key.type == TYPE_FIRM_LAW)
		{
			unschedule(SCHED_FREGISTER, (TRCDKeyContainer *) lawsuit);
			// 2003:080 LRQ: clear all FCHANGE-s if and only if no surrounds
			// 2011:290 LPR: always passing surrounds, check count
			if (!surrounds.Count())
			{
				TSurround tSurround;
				msql m(tSurround.Name);

				m.AddLong("F_LAWSUIT_NO", lawsuit->key.no);
				m.AddLong("F_LAWSUIT_YEAR", lawsuit->key.year);
				m.AddChar("F_LAWSUIT_TYPE", lawsuit->key.type);

				TQuery q(str(m));
				while (q.Read())
				{
					tSurround << q;
					unschedule(SCHED_FCHANGE, &tSurround);
				}
			}
			else
			{
				for (int i = 0; i < surrounds.Count(); i++)
					unschedule(SCHED_FCHANGE, surrounds[i]);
			}
		}
	#endif  // COURT_AREAL

		finish_session(parent, &session, lawsuit, &tLawsuit, &surrounds, true, RESULT_NULL);
	}
}

bool edit_closed(TWindow *parent, TSession *session, TLawsuit *lawsuit)
{
	char result = session->result;

	TLawsuit tLawsuit;
	tLawsuit << lawsuit;

	if (TClosedResultDialog(parent, session, lawsuit, true).Execute() == IDOK)
	{
		log_event(LOG_EDIT, CM_CLERK_EDIT_CLOSED, session, !(session->flags & SESSION_PRIVATE));
		unschedule(Container2Sched(session, result), session);
		if (result == RESULT_STOPPED)
			unschedule(SCHED_STOPPED_M, session);

		TGArray<TSurround> surrounds;
		try2surround(session, &surrounds);
		finish_session(parent, session, lawsuit, &tLawsuit, &surrounds, false, result);
		return true;
	}

	return false;
}

#include "OpenResultDialog.h"
#include "OpenResultDialog.cpp"

bool open_result(TWindow *parent, TSession *session, TLawsuit *lawsuit)
{
	char result = session->result;

	if (result != RESULT_FIXED && !ask("Заседанието е с резултат %s. Ще го коригирате ли?", Result->Seek(result)))
		return false;

	bool enter = strchr(RESULT_ENTERABLES, session->result) == NULL;
	TLawsuit tLawsuit;
	tLawsuit << lawsuit;

	if (lawsuit->key.type == TYPE_PUNISHMENT_LAW && session->end < session->date)
		session->end = session->date;

	if (TOpenResultDialog(parent, session, lawsuit).Execute() != IDOK)
		return false;

	log(LOG_ENTER, CM_SECR_OPEN_RESULT, session);
	xfer_email(session);
	// 2003:070 BUGFIX: was missing
	unschedule(SCHED_PPREPARE, session);
	// 2002:094; 2002:098 Unsched the adequate sched
	// 2014:002 FIX: only on result change
	if (result != session->result)
		unschedule(Container2Sched(session, result), session);
	if (result == RESULT_STOPPED)
		unschedule(SCHED_STOPPED_M, session);
	// 2003:064; 2003:070 BUGFIX: test result -> session->result
	// 2003:075 BUGFIX: use end date for pinushment laws only
	// 2003:157 LRQ: not if already public decision - use "enter"?
	// 2014:136 LRQ/FIX: don't start if decision(s) exist
	else if (result != RESULT_PUBLIC_DECISION && session->key.type != TYPE_FIRM_LAW &&
		session->result == RESULT_PUBLIC_DECISION && !try2decide(session, NULL, KIND_DECISIONS, "F_NO"))
	{
		schedule(SCHED_DPREPARE, session, lawsuit->key.type == TYPE_PUNISHMENT_LAW ? &session->end :
			&session->date, NULL, session->judge);
	}

	TGArray<TSurround> surrounds;
	try2surround(session, &surrounds);
	finish_session(parent, session, lawsuit, &tLawsuit, &surrounds, enter, result);

	if (!strchr(RESULT_COMPLETIZEDS, session->result) && !strchr(RESULT_FOLLOWABLES, session->result) &&
		AUTORUN_FIXES &&
	#if COURT_TYPE == COURT_AREAL
		// 2007:131 LPR: never TYPE_FIRM_LAW for non-AREAL
		(lawsuit->key.type != TYPE_FIRM_LAW || strchr(RESULT_FIRM_LEFTS, session->result)) &&
	#endif  // AREAL
		can_func(RIGHT_FIX_LAW) && ask("Желаете ли да насрочите делото?"))
	{
		TSession tSession;
		set_session(parent, &tSession, &tLawsuit, NULL, NULL);
	}

	return true;
}

#include "SetSessionBaseDialog.h"
#include "SetSessionBaseDialog.cpp"

#include "ResetSessionDialog.h"
#include "ResetSessionDialog.cpp"

static const char *lawsuit2ses_kinds(TLawsuit *lawsuit)
{
#if COURT_TYPE == COURT_REGIONAL
	if (lawsuit->sourceKind == KIND_REQUEST_126G)
		return C2S[KIND_126_LISTEN_SESSION];

	if (!lawsuit_settable_ll(lawsuit))
		return C2S[KIND_NO1STSET_SESSION];
#endif  // REGIONAL
	return kind2ses_kinds(lawsuit->kind);
}

// 2014:293 +related
static void reset_sms(const TLawsuit *lawsuit, const TSession *source, const TSession *target)
{
	if (source->date != target->date || source->time != target->time)
	{
		mstr m;
		m.printf("Пренасрочено от %0Dг., %0Tч. за", &source->date, &source->time);
		send_sms(lawsuit, str(m), SMS_SET, SMS_EDIT, target);
	}
}

static void reset_session_common(const TLawsuit *lawsuit, const TSession *source, const TSession *target)
{
	log(LOG_RESET, 0, source);
	reset_sms(lawsuit, source, target);
	eportal_send(NULL, target, lawsuit, WHAT_SESSION, LOG_WRITE);
}

bool reset_session(TWindow *parent, TSession *source, TRCDKeyContainer *resolution, TLawsuit *lawsuit)
{
	TSession target;

	target << source;
	target.result = RESULT_FIXED;
	target.text.cpy("");	// 2005:087 LRQ: for aborted sessions

	if (TResetSessionDialog(parent, &target, lawsuit, lawsuit2ses_kinds(lawsuit)).Execute() == IDOK)
	{
		if (source->kind != target.kind || source->date != target.date)
		{
			// 2003:332 BRW: Insert() -> Combine()
			if (check_subpoenas(source) && target.Combine(source))
			{
				// 2003:063 PPREPARE support (+other locs)
				schedule(SCHED_FIXED, &target, &Today, &target.date);
				schedule(SCHED_PPREPARE, &target, &target.date);
				reset_session_common(lawsuit, source, &target);
				xfer_bank_session_reset(lawsuit, &target, source);
				// sides are xfered automatically - that is, at the group level...

				// 2005:145 LRQ: don't delete if resolution
				if (resolution)
					revoke_session(source, resolution, lawsuit, RESULT_ABORTED);
				else
				{
					eportal_send(NULL, source, lawsuit, WHAT_SESSION, LOG_DELETE);
					delete_winds(source);
					unschedule(RESULT_FIXED, source);
					// 2003:075 BUGFIX: was missing
					unschedule(SCHED_PPREPARE, source);

					if (!source->Delete(true))
						error("Старото насрочване НЕ е изтрито.");
				}

				if (AUTORUN_WALNS && can_func(RIGHT_MESSAGES) && ask(ASK_PRINT_SUBS))
					edit_sides(parent, &target, lawsuit);

				return true;
			}
		}
		// 2013:211 FIX: +F_TIME; 2014:175 FIX: ALL
		// 2013:211 FIX: +F_TIME; 2014:175 FIX: ALL
		//if (target.Update("F_TIME, F_RESULT, F_TEXT"))
		else if (target.Update())
		{
			reset_session_common(lawsuit, source, &target);
			xfer_bank_event(lawsuit, &target, NULL, NULL, NULL, NULL, NULL, XFER_EDIT);

			if (AUTORUN_WALNS && can_func(RIGHT_MESSAGES) && ask(ASK_PRINT_SUBS))
				edit_sides(parent, &target, lawsuit);

			return true;
		}
	}

	return false;
}

bool reset_session(TWindow *parent, TSession *source, TLawsuit *lawsuit)
{
	TRCDKeyContainer *const resolution = get_session_resolution(parent, source, RESULT_RESET_RESOLUTIONS, "пренасрочване",
		lawsuit);

	if (resolution)
	{
		TVirtPtr VP(resolution);

		if (!lawsuit)
			lawsuit = PrimaryGroup(TLawsuit);

		return reset_session(parent, source, resolution, lawsuit);
	}

	return false;
}

static bool check_load_session(TSession *session, TLawsuit *lawsuit, TLoadGroup *loadGroup, TLoadGroup *invisible)
{
	// 2014:169 LPR: check the full diration; 2014:184 invisible +related
	mstr m;
	bool outside;

	loadGroup->CheckPeriod(m, session);
	outside = any(m);
	loadGroup->CheckUsage(m, session, lawsuit);
	invisible->CheckUsage(m, session, lawsuit);
	return !any(m) || loadGroup->CheckAsk(m, session, lawsuit, outside);
}

#include "HallAliasFace.h"
#include "HallAliasFace.cpp"

#include "SetSessionDialog.h"
#include "SetSessionDialog.cpp"

// 2007:220 LPR: internal, so we can call it twice with different reserve
// 2011:318 LRQ/LPR: removed primary, any number secondary fixes +related
static int set_session_ll(TWindow *parent, TSession *session, TLawsuit *lawsuit, TLoadGroup *loadGroup,
	TLoadGroup *invisible)
{
	bool finished = TLawInterval::Finished(lawsuit, Today);
	int result = IDCANCEL;

	if (!finished || warn("Делото е отбелязано като приключило. Желаете ли да насрочите заседание по него?"))
	{
		const char *kinds = lawsuit2ses_kinds(lawsuit);
		// 2007:017 FIX: preserve possible load unit date
		CDate tDate = session->date;

	#if TESTVER
		// 2018-07-02: only copy the required fields
		session->ImportFields(lawsuit, LAWSUIT_TO_SESSION);
		session->date = tDate;
	#else  // TESTVER
		*session << lawsuit;
		session->date = tDate;
		session->text.cpy("");
		session->flags = 0;		// 2004:111 FIX: flags must be 0 not copied from lawsuit
		session->flagsX = 0;		// 2016:174 FIX: and flagsX must be 0
		session->forceDate.Clear();	// 2013:138 FIX: don't copy force date
		session->flagsY = 0;		// 2018-06-12: FIX: flagsY must not be copied
	#endif  // TESTVER

	#if TESTVER
		extern char suggest_ses_kind_1(TLawsuit *lawsuit, const char *kinds);
		session->kind = finished ? KIND_OTHER_OPEN_SESSION : suggest_ses_kind_1(lawsuit, kinds);
	#else  // TESTVER
		session->kind = finished ? KIND_OTHER_OPEN_SESSION : suggest_ses_kind(lawsuit, kinds);
	#endif  // TESTVER

		if (!loadGroup)
		{
			session->date = Today;
			session->time.hour = SESSION_DEFAULT_TIME.hour;
		}

		if (session->time.hour)
			session->time.minute = SESSION_DEFAULT_TIME.minute;

		session->text.cpy("");
		session->result = RESULT_FIXED;

		result = TSetSessionDialog(parent, session, lawsuit, kinds, loadGroup, invisible).Execute();

		if (result != IDCANCEL)
		{
			log(LOG_FIXED, loadGroup ? CM_JUDGE_SET_LOAD : CM_CLERK_SET, session);
			eportal_send(parent, session, lawsuit, WHAT_SESSION, LOG_WRITE);
			send_sms(lawsuit, "Насрочено за", SMS_SET, SMS_NEW, session);
			xfer_email(session);
			xfer_bank_event(lawsuit, session, NULL, NULL, NULL, NULL, NULL, XFER_ADD);

			schedule(SCHED_FIXED, session, &Today, &session->date);
			schedule(SCHED_PPREPARE, session, &session->date);

			if (lawsuit->key.type != TYPE_FIRM_LAW)
			{
				unschedule(SCHED_HOLD, lawsuit);
				unschedule(SCHED_STOPPED_M, lawsuit);
				unschedule(SCHED_REP_INDOC, lawsuit);
			}

			if (AUTORUN_WALNS && can_func(RIGHT_MESSAGES) && ask(ASK_PRINT_SUBS))
				edit_sides(parent, session, lawsuit);
		}
	}

	return result;
}

bool set_session(TWindow *parent, TSession *session, TLawsuit *lawsuit, TLoadGroup *loadGroup, TLoadGroup *invisible)
{
	int result = set_session_ll(parent, session, lawsuit, loadGroup, invisible);

	if (result == IDCANCEL)
		return false;

#if COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
	if (session->jurors)
		new_distrib_jurors(parent, session, lawsuit);
#endif  // AREAL || MILITARY || REGIONAL

	while (result == IDRESERVE)
	{
		// 2007:239 LRQ: FIX: use new session to clear 1st set data
		TSession tSession;
		result = set_session_ll(parent, &tSession, lawsuit, NULL, NULL);	// loadGroup disables RESERVE
	}

	return true;
}

bool unset_session(TWindow *parent, TSession *session, TLawsuit *lawsuit)
{
	TRCDKeyContainer *const resolution = get_session_resolution(parent, session, RESULT_UNSET_RESOLUTIONS, "отсрочване",
		lawsuit);

	if (resolution)
	{
		TVirtPtr VP(resolution);
		TSession tSession;	// 2011:276 for xfer

		tSession << session;

		if (!lawsuit)
			lawsuit = PrimaryGroup(TLawsuit);

		// 2005:145 LRQ: resolution text
		if (revoke_session(session, resolution, lawsuit, RESULT_UNFIXED))
		{
			log(LOG_UNSET, CM_CLERK_UNSET, session);
			xfer_bank_event(lawsuit, &tSession, NULL, NULL, NULL, NULL, NULL, XFER_DELETE);
			abort_after(session->date, lawsuit, session);
			return true;
		}
	}

	return false;
}

#include "EditSetSessionDialog.h"
#include "EditSetSessionDialog.cpp"

bool edit_set_session(TWindow *parent, TSession *session, TLawsuit *lawsuit)
{
	if (TEditSetSessionDialog(parent, session, lawsuit, C2S[session->kind]).Execute() == IDOK)
	{
		log(LOG_EDIT, CM_CLERK_EDIT_SET, session, "на насрочване");
		eportal_send(parent, session, lawsuit, WHAT_SESSION, LOG_WRITE);
		return true;
	}

	return false;
}

static void surrouendoc_kind(mstr &m, TGroup *group)
{
	surrouendoc_text(m, (const TSurroundment *) group);
}

static void surrouendoc_act_kind(mstr &m, TGroup *group)
{
	char decisionKind = TYPECHECK_PTR_CAST(TSurroundment, group)->decisionKind;
	m.cat(decisionKind ? Kind->Seek(decisionKind) : "");
}

#include "EditSurrouendocDialog.h"
#include "EditSurrouendocDialog.cpp"

static bool edit_surrouendoc(TWindow *parent, TSurroundment *surrouendoc, TSession *session, TLawsuit *lawsuit)
{
	bool edit = surrouendoc->decisionKind;

	if (TEditSurrouendocDialog(parent, surrouendoc, session, lawsuit).Execute() == IDOK)
	{
		log_surrouendoc(edit ? LOG_EDIT : LOG_ADD, CM_CLERK_NEW_JUDICIAL_ACT, surrouendoc);

		TDecision decision;
		decision.Init(surrouendoc);

		if (decision.flags & DECISION_PUBLIC)
			xfer_web(&decision, session, lawsuit);

		xfer_bank_event(lawsuit, session, NULL, &decision, NULL, NULL, NULL, edit ? XFER_EDIT : XFER_ADD);
		return true;
	}

	return false;
}

#include "SurrouendocListWindow.h"
#include "SurrouendocListWindow.cpp"

#include "SurrouendocListDialog.h"
#include "SurrouendocListDialog.cpp"

void edit_surrouendocs(TWindow *parent, TSession *session, TLawsuit *lawsuit)
{
	TGArray<TSurroundment> surrouendocs;

	if (try2surround(session, &surrouendocs, NULL, KIND_SURROUENDOCS))
		TSurrouendocListDialog(parent, session, &surrouendocs, lawsuit, false).Execute();
	else
		error("В заседанието не са разгледани съпровождащи документи, съответстващи на резултата.");
}

bool appeal_surrouendocs(TWindow *parent, TSession *session, TLawsuit *lawsuit)
{
	TGArray<TSurroundment> surrouendocs;

	if (!try2surround(session, &surrouendocs, NULL, KIND_SURROUENDOCS, true))
		return false;

	TSurrouendocListDialog(parent, session, &surrouendocs, lawsuit, true).Execute();
	return true;
}

#if COURT_TYPE == COURT_AREAL

#include "EditExternSessionDialog.h"
#include "EditExternSessionDialog.cpp"

#include "EditExternOpenSessionDialog.h"
#include "EditExternOpenSessionDialog.cpp"

#include "EditExternClosedSessionDialog.h"
#include "EditExternClosedSessionDialog.cpp"

bool edit_extern_open_session(TWindow *parent, TExternSession *session, bool edit)
{
	if (!edit)
		session->time = SESSION_DEFAULT_TIME;

	if (TEditExternOpenSessionDialog(parent, session, edit).Execute() != IDOK)
		return false;

	log(edit ? LOG_EDIT : LOG_ADD, CM_CLERK_EXTERN_SESSIONS, session);
	return true;
}

bool edit_extern_closed_session(TWindow *parent, TExternSession *session, bool edit)
{
	if (!edit)
		session->kind = KIND_CLOSED_SESSION;

	if (TEditExternClosedSessionDialog(parent, session, edit).Execute() != IDOK)
		return false;

	log(edit ? LOG_EDIT : LOG_ADD, CM_CLERK_EXTERN_SESSIONS, session);
	return true;
}
#endif  // AREAL
