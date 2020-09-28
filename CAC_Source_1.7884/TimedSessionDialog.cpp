DEFINE_RESPONSE_TABLE1(TTimedSessionDialog, TFloatSubjectDialog)
//{{TTimedSessionDialogRSP_TBL_BEGIN}}
	EV_BN_CLICKED(IDC_TIMED_SESSION_SESSIONS, SessionsBNClicked),
	EV_BN_CLICKED(IDC_TIMED_SESSION_DECISIONS, SessionsBNClicked),
//{{TTimedSessionDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TTimedSessionDialog Implementation}}

TTimedSessionDialog::TTimedSessionDialog(TWindow* parent, TTimedSessionGroup *group, int resId)
:
	TFloatSubjectDialog(parent, group, resId)
{
	lawKinds = new TCharAutoListFace(this, IDC_TIMED_SESSION_LAW_KINDS, "F_LAW_KINDS", Kind, Default->xKinds,
		SIZE_OF_KINDS);
	minNo = new TLongFace(this, IDC_TIMED_SESSION_MIN_NO, "F_MIN_NO", SIZE_OF_NO);
	maxNo = new TLongFace(this, IDC_TIMED_SESSION_MAX_NO, "F_MAX_NO", SIZE_OF_NO);
	minYear = new TYearFace(this, IDC_TIMED_SESSION_MIN_YEAR, "F_MIN_YEAR");
	maxYear = new TYearFace(this, IDC_TIMED_SESSION_MAX_YEAR, "F_MAX_YEAR");
	judges = new TUserListBox(this, IDC_TIMED_SESSION_JUDGES, POST_VISIBLE_JUDGES);
	compositions = new TCharAutoListFace(this, IDC_TIMED_SESSION_COMPOSITIONS, "F_COMPOSITIONS", CharComposition,
		QUERY_COMPOSITIONS, SIZE_OF_COMPOSITIONS);
	sesKinds = new TCharListFace(this, IDC_TIMED_SESSION_SES_KINDS, "F_SES_KINDS", Kind, KIND_VISIBLE_SESSIONS,
		SIZE_OF_KINDS);
	// 2008:199
	new TRadioFace(this, IDC_TIMED_SESSION_AGE_NULL, "F_AGE_NULL");
	new TRadioFace(this, IDC_TIMED_SESSION_AGE_ONE_OR_LESS, "F_AGE_ONE_OR_LESS");
	new TRadioFace(this, IDC_TIMED_SESSION_AGE_TWO_OR_LESS, "F_AGE_TWO_OR_LESS");
	new TRadioFace(this, IDC_TIMED_SESSION_AGE_MORE_THAN_TWO, "F_AGE_MORE_THAN_TWO");
	new TRadioFace(this, IDC_TIMED_SESSION_AGE_ONE_TO_THREE, "F_AGE_ONE_TO_THREE");
	new TRadioFace(this, IDC_TIMED_SESSION_AGE_MORE_THAN_THREE, "F_AGE_MORE_THAN_THREE");
	// 2008:203
	ruleNull = new TRadioFace(this, IDC_TIMED_SESSION_RULE_NULL, "F_RULE_NULL");
	ruleAct = new TRadioFace(this, IDC_TIMED_SESSION_RULE_ACT, "F_RULE_ACT");
	ruleUndo = new TRadioFace(this, IDC_TIMED_SESSION_RULE_UNDO, "F_RULE_UNDO");
	ruleNone = new TRadioFace(this, IDC_TIMED_SESSION_RULE_NONE, "F_RULE_NONE");
	// 2008:205
	new TRadioFace(this, IDC_TIMED_SESSION_BY_TYND, "F_BY_TYND");
	orderDTYN = new TRadioFace(this, IDC_TIMED_SESSION_BY_DTYN, "F_BY_DTYN");
	new TRadioFace(this, IDC_TIMED_SESSION_BY_JDTYN, "F_BY_JDTYN");
	new TCheckFace(this, IDC_TIMED_SESSION_PUBLIC_DECISION, "F_PUBLIC_DECISION");	// 2008:212
	// 2010:328; sesion dates were the previous only dates
	sessions = new TRadioFace(this, IDC_TIMED_SESSION_SESSIONS, "F_SESSIONS");
	minSesDate = new TDateFace(this, IDC_TIMED_SESSION_MIN_SES_DATE, "F_MIN_SES_DATE", false);
	maxSesDate = new TDateFace(this, IDC_TIMED_SESSION_MAX_SES_DATE, "F_MAX_SES_DATE", false);
	decisions = new TRadioFace(this, IDC_TIMED_SESSION_DECISIONS, "F_DECISIONS");
	minDecDate = new TDateFace(this, IDC_TIMED_SESSION_MIN_DEC_DATE, "F_MIN_DEC_DATE", false);
	maxDecDate = new TDateFace(this, IDC_TIMED_SESSION_MAX_DEC_DATE, "F_MAX_DEC_DATE", false);
}

bool TTimedSessionDialog::IsValid()
{
	bool isValid = TFloatSubjectDialog::IsValid();

	if (isValid)
	{
		if (invalid_range(minNo, maxNo))
			isValid = error("Началния номер дело е по-голям от крайния.");
		else if (invalid_range(minYear, maxYear))
			isValid = error("Началната година е по-голяма от крайната.");
		else if (sessions->GetCheck() == BF_CHECKED && invalid_range(minSesDate, maxSesDate))
			isValid = error("Началната дата на заседание е по-голяма от крайната.");
		else if (sessions->GetCheck() == BF_UNCHECKED && invalid_range(minDecDate, maxDecDate))
			isValid = error("Началната дата на съдебен акт е по-голяма от крайната.");
	}

	return isValid;
}

void TTimedSessionDialog::Action(bool execute)
{
	msql criteria;
	msql undocrit;

	TFloatSubjectDialog::GetSubjects(TimedGroup()->newSubjects, TimedGroup()->oldSubjects);
	judges->GetMark(&TimedGroup()->judges);

	if (TimedGroup()->sessions)
		TimedGroup()->FormCriteria(criteria);
	else
	{
		if (TimedGroup()->rule != RULE_UNDO)
			TimedGroup()->FormCriteria(criteria);

		TTimedSessionGroup undog;

		undog << this;
		TFloatSubjectDialog::GetSubjects(undog.newSubjects, undog.oldSubjects);
		judges->GetMark(&undog.judges);

		undog.sessions = TRUE;
		undog.undo = true;
		undog.FormCriteria(undocrit);
	}

	if (execute)
	{
		TTimedSessionQuery(this, criteria.len() ? str(criteria) : NULL, undocrit.len() ? str(undocrit) : NULL,
			TimedGroup()->age, TimedGroup()->rule, TimedGroup()->pdec).Execute();
	}
	else
	{
		mstr m;

		if (any(criteria) && any(undocrit))
			m.printf("%s;\n\n%s", str(criteria), str(undocrit));
		else
			m.cpy(any(criteria) ? str(criteria) : str(undocrit));

		MessageEx(m);
	}
}

void TTimedSessionDialog::SessionsBNClicked()
{
	bool tSessions = sessions->GetCheck() == BF_CHECKED;

	minSesDate->EnableWindow(tSessions);
	maxSesDate->EnableWindow(tSessions);
	minDecDate->EnableWindow(!tSessions);
	maxDecDate->EnableWindow(!tSessions);
	// 2010:337 TRQ: can't support "session" date
	orderDTYN->SetCaption(tSessions ? "По дата на заседание" : "По дата на съдебен акт");
}
