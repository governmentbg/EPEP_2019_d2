//{{TFloatSurroundDialog Implementation}}

TFloatSurroundDialog::TFloatSurroundDialog(TWindow* parent, TFloatSurroundGroup *group)
:
	TFloatSubjectDialog(parent, group, IDD_FLOAT_SURROUND)
{
	lawTypes = new TTypeListFace(this, IDC_FLOAT_SURROUND_LAW_TYPES, "F_LAWSUIT_TYPES", TYPE_SURROUNDABLES, false);
	minLawNo = new TLongFace(this, IDC_FLOAT_SURROUND_MIN_LAW_NO, "F_MIN_LAWSUIT_NO", SIZE_OF_NO);
	maxLawNo = new TLongFace(this, IDC_FLOAT_SURROUND_MAX_LAW_NO, "F_MAX_LAWSUIT_NO", SIZE_OF_NO);
	minLawYear = new TYearFace(this, IDC_FLOAT_SURROUND_MIN_LAW_YEAR, "F_MIN_LAWSUIT_YEAR");
	maxLawYear = new TYearFace(this, IDC_FLOAT_SURROUND_MAX_LAW_YEAR, "F_MAX_LAWSUIT_YEAR");
	judges = new TUserListBox(this, IDC_FLOAT_SURROUND_JUDGES, POST_VISIBLE_JUDGES, USER_INTER);
	kinds = new TCharListFace(this, IDC_FLOAT_SURROUND_KINDS, "F_KINDS", Kind, KIND_SURROUNDS, SIZE_OF_KINDS);
	minDate = new TDateFace(this, IDC_FLOAT_SURROUND_MIN_DATE, "F_MIN_DATE", false);
	maxDate = new TDateFace(this, IDC_FLOAT_SURROUND_MAX_DATE, "F_MAX_DATE", false);
	minSessionDate = new TDateFace(this, IDC_FLOAT_SURROUND_MIN_SESSION_DATE, "F_MIN_SESSION_DATE", false);
	maxSessionDate = new TDateFace(this, IDC_FLOAT_SURROUND_MAX_SESSION_DATE, "F_MAX_SESSION_DATE", false);
	new TRadioFace(this, IDC_FLOAT_SURROUND_BY_TYN, "F_BY_TYN");
	new TRadioFace(this, IDC_FLOAT_SURROUND_BY_KYN, "F_BY_KYN");
	firmData = new TCheckFace(this, IDC_FLOAT_SURROUND_FIRM_DATA, "F_FIRM_DATA");
	text = new TStringFace(this, IDC_FLOAT_SURROUND_TEXT, "F_TEXT", SIZE_OF_ORIGIN_TEXT);
}

bool TFloatSurroundDialog::IsValid()
{
	bool isValid = TFloatSubjectDialog::IsValid();

	if (isValid)
	{
		if (invalid_range(minLawNo, maxLawNo))
			isValid = error("Ќачални€ номер дело е по-гол€м от крайни€.");
		else if (invalid_range(minLawYear, maxLawYear))
			isValid = error("Ќачалната година е по-гол€ма от крайната.");
		else if (invalid_range(minDate, maxDate))
			isValid = error("Ќачалната дата на образуване е по-гол€ма от крайната.");
		else if (invalid_range(minSessionDate, maxSessionDate))
			isValid = error("Ќачалната дата на разглеждане в заседание е по-гол€ма от крайната.");
	}

	return isValid;
}

void TFloatSurroundDialog::Action(bool execute)
{
	msql m;

	TFloatSubjectDialog::GetSubjects(FloatSurroundGroup()->newSubjects, FloatSurroundGroup()->oldSubjects);
	judges->GetMark(&FloatSurroundGroup()->judges);
	FloatSurroundGroup()->FormCriteria(m);

	if (execute)
		query_surround(this, str(m), FloatSurroundGroup()->firmData, FloatSurroundGroup()->order);
	else
		Message(m);
}

void TFloatSurroundDialog::SetupWindow()
{
	TFloatSubjectDialog::SetupWindow();

#if COURT_TYPE == COURT_AREAL
	firmData->EnableWindow(true);
	firmData->ShowWindow(SW_SHOW);
#endif  // AREAL
}
