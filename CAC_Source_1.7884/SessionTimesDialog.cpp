DEFINE_RESPONSE_TABLE1(TSessionTimesDialog, TSessionCompositionDialog)
//{{TSessionTimesDialogRSP_TBL_BEGIN}}
#if COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
	EV_EN_UPDATE(IDC_SESSION_TIMES_STARTIME, StarTimeENUpdate),
	EV_EN_UPDATE(IDC_SESSION_TIMES_ENTIME, EnTimeENUpdate),
	EV_BN_CLICKED(IDC_SESSION_TIMES_TIMES, TimesBNClicked),
#endif  // AREAL || MILITARY || REGIONAL
	EV_EN_UPDATE(IDC_SESSION_TIMES_END, EndENUpdate),
#if COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
	EV_CBN_SELCHANGE(IDC_SESSION_COMPOSITION_JUROR, JurorCBNSelChange),
	EV_CBN_SELCHANGE(IDC_SESSION_COMPOSITION_REJUROR, JurorCBNSelChange),
#endif  // AREAL || MILITARY || REGIONAL
#if COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY
	EV_CBN_SELCHANGE(IDC_SESSION_COMPOSITION_JUROR_X, JurorCBNSelChange),
#endif  // AREAL || MILITARY
//{{TSessionTimesDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TSessionTimesDialog Implementation}}

TSessionTimesDialog::TSessionTimesDialog(TWindow* parent, TBaseSession *session, TLawsuit *lawsuit, long tEdit, int resId)
:
	TSessionCompositionDialog(parent, session, lawsuit, tEdit, resId), sesTimesList(session), times(NULL), singleDate(-1),
		sessionTimes(-1), initialized(false)
{
#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_ADMIN
	startime = NULL;
	entime = NULL;
	hoursLabel = NULL;
	hours = NULL;
#endif  // APPEAL || ADMIN
#if COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
	startime = new TTimeFace(this, IDC_SESSION_TIMES_STARTIME, "F_STARTIME", false);
	startime->NoZeros();
	entime = new TTimeFace(this, IDC_SESSION_TIMES_ENTIME, "F_ENTIME", false);
	entime->NoZeros();
	hoursLabel = new TStatic(this, IDC_SESSION_TIMES_HOURS_LABEL);
	hours = new TLongFace(this, IDC_SESSION_TIMES_HOURS, "F_HOURS", SIZE_OF_HOUR);
	if (edit)
	{
		sesTimesList.Load();
		sesTimesList.FillGaps();
	}
#endif  // AREAL || MILITARY || REGIONAL
	end = new TDateFace(this, IDC_SESSION_TIMES_END, "F_END", false);		// 2017:074 mandatory
}

bool TSessionTimesDialog::SingleDate() const
{
	return !end->IsValid(false) || end->Date() <= Session()->date;
}

void TSessionTimesDialog::SingleDateChanged(bool force)
{
#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_ADMIN
	(void) force;
#endif  // APPEAL || ADMIN
#if COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
	bool tSingleDate = SingleDate();

	if (force || tSingleDate != singleDate)
	{
		int showCmd = sessionTimes ? SW_SHOW : SW_HIDE;

		hours->SetL(0);

		if (tSingleDate)
		{
			if (times)
			{
				times->EnableWindow(false);
				times->ShowWindow(SW_HIDE);
			}

			if (sessionTimes)
			{
				if (sesTimesList.oldTimes.Count())
					hours->SetL(sesTimesList.oldTimes[0]->hours);
				else
					RecalcHours();
			}

			hoursLabel->ShowWindow(showCmd);
			hours->EnableWindow(sessionTimes);
			hours->ShowWindow(showCmd);
		}
		else
		{
			hoursLabel->ShowWindow(SW_HIDE);
			hours->ShowWindow(SW_HIDE);
			hours->EnableWindow(false);

			if (times)
			{
				times->EnableWindow(sessionTimes);
				times->ShowWindow(showCmd);
			}
		}

		singleDate = tSingleDate;
	}
#endif  // AREAL || MILITARY || REGIONAL
}

bool TSessionTimesDialog::SessionTimes() const
{
	if (!jurors || !can_session_times() || result->C() == RESULT_FIXED)
		return false;

	if (strchr(KIND_CLOSED_SESSIONS, Session()->kind))
	{
		if (DISTRIB_SESSION_TIMES == 1)
			return false;
	}
	else if (DISTRIB_SESSION_TIMES_FORCE)
		return true;

	return atob(juror) || atob(reJuror) || (CourtType != COURT_REGIONAL && atob(jurorX));
}

void TSessionTimesDialog::SessionTimesChanged()
{
#if COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
	bool tSessionTimes = SessionTimes();

	if (tSessionTimes != sessionTimes)
	{
		if (tSessionTimes)
		{
			startime->SetTime(sesTimesList.oldTimes.Count() ? sesTimesList.oldTimes[0]->start : Session()->time);
			startime->EnableWindow(true);

			if (!entime->IsWindowEnabled())
			{
				entime->SetTime(Session()->entime);
				entime->EnableWindow(true);
			}
		}
		else
		{
			startime->EnableWindow(false);
			startime->SetS("");

			if (end->IsWindowEnabled())
				entime->EnableWindow(true);
			else
			{
				entime->EnableWindow(false);
				entime->SetS("");
			}
		}

		sessionTimes = tSessionTimes;
		SingleDateChanged(true);
	}
#endif  // AREAL || MILITARY || REGIONAL
}

bool TSessionTimesDialog::CheckSessionTimes() const
{
	return check_session_times(startime, entime, sesTimesList.oldTimes.Count());
}

void TSessionTimesDialog::RecalcHours()
{
	if (initialized && SessionTimes() && SingleDate())
		recalc_session_hours(startime, entime, hours);
}

void TSessionTimesDialog::CommitTimes()
{
#if COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
	if (!SessionTimes())
	{
		// 2017:074 non-times may be multi-date
		sesTimesList.newTimes.Flush();
	}
	else if (SingleDate())
	{
		// 2016:281
		TSessionTimes *sTimes = new TSessionTimes();

		sesTimesList.newTimes.Flush();
		*sTimes << Session();
		(CDate &) sTimes->start = Session()->date;
		(CTime &) sTimes->start = startime->Time();
		sTimes->hours = hours->L();
		sesTimesList.newTimes.Add(sTimes);
	}

	sesTimesList.RemoveEmpty();
	sesTimesList.Update();
#endif  // AREAL || MILITARY || REGIONAL
}

bool TSessionTimesDialog::IsValid()
{
	bool isValid = TSessionCompositionDialog::IsValid();

#if COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
	if (isValid && SessionTimes())
	{
		if (SingleDate())
		{
			const char *status = session_times_invalid(false, Session()->date, startime->Time(), entime->Time(),
				hours->L());

			if (status)
				isValid = error("%s.", status);
		}
		else
		{
			isValid = sesTimesList.IsValid(startime->Time(), end->Date(), entime->Time());

			if (isValid)
			{
				mstr m;

				if (!session_times_valid(&sesTimesList.newTimes, m))
					isValid = error("%s.", str(m));
			}
		}
	}
#endif  // AREAL || MILITARY || REGIONAL

	return isValid;
}

bool TSessionTimesDialog::IsClean()
{
	bool isClean = TSessionCompositionDialog::IsClean();

	if (isClean)
	{
		isClean = !edit || !SessionTimes() || SingleDate() || !sesTimesList.dirty || ask("Промените в продължителността "
			"по заседателни дни се записват само при потвърждение. Желаете ли да прекъснете редактирането?");
	}

	return isClean;
}

void TSessionTimesDialog::JurorCBNSelChange()
{
	SessionTimesChanged();
}

void TSessionTimesDialog::StarTimeENUpdate()
{
	RecalcHours();
}

void TSessionTimesDialog::EnTimeENUpdate()
{
	RecalcHours();
}

void TSessionTimesDialog::TimesBNClicked()
{
	if (startime->IsValid(false) && end->IsValid(true) && entime->IsValid(true))
	{
		const char *status = session_times_complect(false, startime->Time(), entime->Time());
		const CDate &endate = end->Date();

		if (status)
			error("%s.", status);
		else if (endate.Empty())
			error("Крайната дата е задължителна.");
		else
		{
			sesTimesList.ReInit(startime->Time(), endate, entime->Time());

			if (edit_session_times(this, sesTimesList.newTimes))
				sesTimesList.dirty = true;
		}
	}
}

void TSessionTimesDialog::EndENUpdate()
{
	if (initialized && end->IsValid(false))
		SingleDateChanged();
}
