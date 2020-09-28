DEFINE_RESPONSE_TABLE1(TEditSessionTimesDialog, TGroupDialog)
//{{TEditSessionTimesDialogRSP_TBL_BEGIN}}
	EV_EN_UPDATE(IDC_EDIT_SESSION_TIMES_STARTIME, StarTimeENUpdate),
	EV_EN_UPDATE(IDC_EDIT_SESSION_TIMES_ENTIME, EnTimeENUpdate),
	EV_BN_CLICKED(IDOK, OKBNClicked),
//{{TEditSessionTimesDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TEditSessionTimesDialog Implementation}}

TEditSessionTimesDialog::TEditSessionTimesDialog(TWindow* parent, TSessionTimes *sessionTimes, int tIndex, int tCount)
:
	TGroupDialog(parent, sessionTimes, IDD_EDIT_SESSION_TIMES), index(tIndex), count(tCount)
{
	stardate = new TDateFace(this, IDC_EDIT_SESSION_TIMES_DATE, "F_STARDATE", true);
	startime = new TTimeFace(this, IDC_EDIT_SESSION_TIMES_STARTIME, "F_STARTIME", false);
	startime->NoZeros();
	entime = new TTimeFace(this, IDC_EDIT_SESSION_TIMES_ENTIME, "F_ENTIME", false);
	entime->NoZeros();
	hours = new TLongFace(this, IDC_EDIT_SESSION_TIMES_HOURS, "F_HOURS", SIZE_OF_HOUR);
}

bool TEditSessionTimesDialog::IsValid()
{
	bool isValid = TGroupDialog::IsValid();

	if (isValid)
	{
		const char *status = session_times_invalid(index > 0 && index < count - 1, STimes()->start, startime->Time(),
			entime->Time(), hours->L());

		if (status)
			isValid = error("%s.", status);
	}

	return isValid;
}

void TEditSessionTimesDialog::SetupWindow()
{
	TGroupDialog::SetupWindow();

	stardate->SetDate((CDate &) STimes()->start);
	startime->SetTime((CTime &) STimes()->start);
	startime->EnableWindow(index > 0);
	entime->EnableWindow(index < count - 1);
}

void TEditSessionTimesDialog::RecalcHours()
{
	recalc_session_hours(startime, entime, hours);
}

void TEditSessionTimesDialog::OKBNClicked()
{
	if (IsValid() && check_equal_times(startime, entime))
	{
		(CTime &) STimes()->start = startime->Time();
		STimes()->entime = entime->Time();
		STimes()->hours = hours->L();
		CmOk();
	}
}

void TEditSessionTimesDialog::StarTimeENUpdate()
{
	RecalcHours();
}

void TEditSessionTimesDialog::EnTimeENUpdate()
{
	RecalcHours();
}
