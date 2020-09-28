DEFINE_RESPONSE_TABLE1(TTimedLawsuitDialog, TFloatSubjectDialog)
//{{TTimedLawsuitDialogRSP_TBL_BEGIN}}
	EV_BN_CLICKED(IDC_TIMED_LAWSUIT_REQUEST_DATE, RequestDateBNClicked),
	EV_BN_CLICKED(IDC_TIMED_LAWSUIT_LAWSUIT_DATE, RequestDateBNClicked),
//{{TTimedLawsuitDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TTimedLawsuitDialog Implementation}}

TTimedLawsuitDialog::TTimedLawsuitDialog(TWindow* parent, TTimedLawsuitGroup *group)
:
	TFloatSubjectDialog(parent, group, IDD_TIMED_LAWSUIT)
{
	lawKinds = new TCharAutoListFace(this, IDC_TIMED_LAWSUIT_LAW_KINDS, "F_LAW_KINDS", Kind, Default->xKinds,
		SIZE_OF_KINDS);
	minNo = new TLongFace(this, IDC_TIMED_LAWSUIT_MIN_NO, "F_MIN_NO", SIZE_OF_NO);
	maxNo = new TLongFace(this, IDC_TIMED_LAWSUIT_MAX_NO, "F_MAX_NO", SIZE_OF_NO);
	minYear = new TYearFace(this, IDC_TIMED_LAWSUIT_MIN_YEAR, "F_MIN_YEAR");
	maxYear = new TYearFace(this, IDC_TIMED_LAWSUIT_MAX_YEAR, "F_MAX_YEAR");
	judges = new TUserListBox(this, IDC_TIMED_LAWSUIT_JUDGES, POST_VISIBLE_JUDGES);
	compositions = new TCharAutoListFace(this, IDC_TIMED_LAWSUIT_COMPOSITIONS, "F_COMPOSITIONS", CharComposition,
		QUERY_COMPOSITIONS, SIZE_OF_COMPOSITIONS);
	requestDate = new TRadioFace(this, IDC_TIMED_LAWSUIT_REQUEST_DATE, "F_REQUEST_DATE");
	minRequestDate = new TDateFace(this, IDC_TIMED_LAWSUIT_MIN_REQUEST_DATE, "F_MIN_REQUEST_DATE", false);
	maxRequestDate = new TDateFace(this, IDC_TIMED_LAWSUIT_MAX_REQUEST_DATE, "F_MAX_REQUEST_DATE", false);
	lawsuitDate = new TRadioFace(this, IDC_TIMED_LAWSUIT_LAWSUIT_DATE, "F_LAWSUIT_DATE");
	minLawsuitDate = new TDateFace(this, IDC_TIMED_LAWSUIT_MIN_LAWSUIT_DATE, "F_MIN_LAWSUIT_DATE", false);
	maxLawsuitDate = new TDateFace(this, IDC_TIMED_LAWSUIT_MAX_LAWSUIT_DATE, "F_MAX_LAWSUIT_DATE", false);
	new TRadioFace(this, IDC_TIMED_LAWSUIT_AGE_ALL_SET, "F_AGE_ALL_SET");
	new TRadioFace(this, IDC_TIMED_LAWSUIT_AGE_TO_3_DAYS, "F_AGE_TO_3_DAYS");
	new TRadioFace(this, IDC_TIMED_LAWSUIT_AGE_TO_7_DAYS, "F_AGE_TO_7_DAYS");
	new TRadioFace(this, IDC_TIMED_LAWSUIT_AGE_ONE_OR_LESS, "F_AGE_ONE_OR_LESS");
	new TRadioFace(this, IDC_TIMED_LAWSUIT_AGE_ONE_TO_TWO, "F_AGE_ONE_TO_TWO");
	new TRadioFace(this, IDC_TIMED_LAWSUIT_AGE_ONE_TO_THREE, "F_AGE_ONE_TO_THREE");
	new TRadioFace(this, IDC_TIMED_LAWSUIT_AGE_MORE_THAN_THREE, "F_AGE_MORE_THAN_THREE");
	new TRadioFace(this, IDC_TIMED_LAWSUIT_AGE_NOT_SET, "F_AGE_NOT_SET");
	new TRadioFace(this, IDC_TIMED_LAWSUIT_BY_TYND, "F_BY_TYND");
	new TRadioFace(this, IDC_TIMED_LAWSUIT_BY_JDTYN, "F_BY_JDTYN");
}

bool TTimedLawsuitDialog::IsValid()
{
	bool isValid = TFloatSubjectDialog::IsValid();

	if (isValid)
	{
		if (invalid_range(minNo, maxNo))
			isValid = error("Началния номер дело е по-голям от крайния.");
		else if (invalid_range(minYear, maxYear))
			isValid = error("Началната година е по-голяма от крайната.");
		else if (requestDate->GetCheck() == BF_CHECKED && invalid_range(minRequestDate, maxRequestDate))
			isValid = error("Началната дата на входящ документ е по-голяма от крайната.");
		else if (lawsuitDate->GetCheck() == BF_CHECKED && invalid_range(minLawsuitDate, maxLawsuitDate))
			isValid = error("Началната дата на дело е по-голяма от крайната.");
	}

	return isValid;
}

void TTimedLawsuitDialog::Action(bool execute)
{
	msql m;

	TFloatSubjectDialog::GetSubjects(TimedGroup()->newSubjects, TimedGroup()->oldSubjects);
	judges->GetMark(&TimedGroup()->judges);
	TimedGroup()->FormCriteria(m);

	if (execute)
		TTimedLawsuitQuery(this, str(m), TimedGroup()->requestDate, TimedGroup()->age).Execute();
	else
		MessageEx(m);
}

void TTimedLawsuitDialog::RequestDateBNClicked()
{
	bool tRequestDate = requestDate->GetCheck() == BF_CHECKED;

	minRequestDate->EnableWindow(tRequestDate);
	maxRequestDate->EnableWindow(tRequestDate);
	minLawsuitDate->EnableWindow(!tRequestDate);
	maxLawsuitDate->EnableWindow(!tRequestDate);
}
