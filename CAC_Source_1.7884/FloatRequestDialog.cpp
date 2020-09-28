//{{TFloatRequestDialog Implementation}}

TFloatRequestDialog::TFloatRequestDialog(TWindow* parent, TFloatRequestGroup *group)
:
	TFloatSubjectDialog(parent, group, IDD_FLOAT_REQUEST)
{
	generalKinds = new TCharListFace(this, IDC_FLOAT_REQUEST_GENERAL_KINDS, "F_GENERAL_KINDS", Kind, KIND_INDOC_GENERALS, SIZE_OF_KINDS);
	minNo = new TLongFace(this, IDC_FLOAT_REQUEST_MIN_NO, "F_MIN_NO", SIZE_OF_NO);
	maxNo = new TLongFace(this, IDC_FLOAT_REQUEST_MAX_NO, "F_MAX_NO", SIZE_OF_NO);
	minYear = new TYearFace(this, IDC_FLOAT_REQUEST_MIN_YEAR, "F_MIN_YEAR");
	maxYear = new TYearFace(this, IDC_FLOAT_REQUEST_MAX_YEAR, "F_MAX_YEAR");
	colleges = new TCharAutoListFace(this, IDC_FLOAT_REQUEST_COLLEGES, "F_COLLEGES", College, QUERY_COLLEGES, SIZE_OF_COLLEGES);
	minDate = new TDateFace(this, IDC_FLOAT_REQUEST_MIN_DATE, "F_MIN_DATE", false);
	maxDate = new TDateFace(this, IDC_FLOAT_REQUEST_MAX_DATE, "F_MAX_DATE", false);
	text = new TStringFace(this, IDC_FLOAT_REQUEST_TEXT, "F_TEXT", SIZE_OF_ORIGIN_TEXT);
	slow = new TRadioFace(this, IDC_FLOAT_REQUEST_SLOW, "F_SLOW");
	quick = new TRadioFace(this, IDC_FLOAT_REQUEST_QUICK, "F_QUICK");
#if COURT_TYPE == COURT_APPEAL
	immediate = NULL;
	eisppNmr = new TEisPartFace(this, IDC_FLOAT_REQUEST_EISPP_NMR, "F_EISPP_NMR");
#endif  // APPEAL
#if COURT_TYPE == COURT_ADMIN
	immediate = new TRadioFace(this, IDC_FLOAT_REQUEST_IMMEDIATE, "F_IMMEDIATE");
	eisppNmr = NULL;
#endif  // ADMIN
#if COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
	immediate = new TRadioFace(this, IDC_FLOAT_REQUEST_IMMEDIATE, "F_IMMEDIATE");
	eisppNmr = new TEisPartFace(this, IDC_FLOAT_REQUEST_EISPP_NMR, "F_EISPP_NMR");
#endif  // AREAL || MILITARY || REGIONAL
	anySpeed = new TRadioFace(this, IDC_FLOAT_REQUEST_ANY_SPEED, "F_ANY_SPEED");
	new TRadioFace(this, IDC_FLOAT_REQUEST_BY_TYN, "F_BY_TYN");
	new TRadioFace(this, IDC_FLOAT_REQUEST_BY_CKYN, "F_BY_CKYN");
}

bool TFloatRequestDialog::IsValid()
{
	bool isValid = TFloatSubjectDialog::IsValid();

	if (isValid)
	{
		if (invalid_range(minNo, maxNo))
			isValid = error("Ќачални€ номер на документ е по-гол€м от крайни€.");
		else if (invalid_range(minYear, maxYear))
			isValid = error("Ќачалната година е по-гол€ма от крайната.");
		else if (invalid_range(minDate, maxDate))
			isValid = error("Ќачалната дата на образуване е по-гол€ма от крайната.");
	}

	return isValid;
}

void TFloatRequestDialog::Action(bool execute)
{
	msql m;

	TFloatSubjectDialog::GetSubjects(FloatRequestGroup()->newSubjects, FloatRequestGroup()->oldSubjects);
	FloatRequestGroup()->FormCriteria(m);

	if (execute)
		query_request(this, str(m));
	else
		Message(m);
}
