//{{TFloatLogDialog Implementation}}

TFloatLogDialog::TFloatLogDialog(TWindow* parent, TFloatLogGroup *group, int resId)
:
	TFloatDialog(parent, group, resId)
{
	// 2013:065 LRQ: limit types to user-accessible
	build_ctypes(extypes, TYPE_LOGS, false);
	// 2013:112 FIX: support > SIZE_OF_TYPES; 2013:138: AutoList
	types = new TCharAutoListFace(this, IDC_FLOAT_LOG_TYPES, "F_TYPES", Type, extypes, SIZE_OF_EXT_TYPES);
	minNo = new TLongFace(this, IDC_FLOAT_LOG_MIN_NO, "F_MIN_NO", SIZE_OF_NO);
	maxNo = new TLongFace(this, IDC_FLOAT_LOG_MAX_NO, "F_MAX_NO", SIZE_OF_NO);
	minYear = new TYearFace(this, IDC_FLOAT_LOG_MIN_YEAR, "F_MIN_YEAR");
	maxYear = new TYearFace(this, IDC_FLOAT_LOG_MAX_YEAR, "F_MAX_YEAR");
	minDate = new TDateFace(this, IDC_PERIOD_MIN_DATE, "F_MIN_DATE");
	new TDateFaceUpDown(this, IDC_PERIOD_MIN_UPDN, minDate);
	maxDate = new TDateFace(this, IDC_PERIOD_MAX_DATE, "F_MAX_DATE");
	new TDateFaceUpDown(this, IDC_PERIOD_MAX_UPDN, maxDate);
	minTime = new TTimeFace(this, IDC_FLOAT_LOG_MIN_TIME, "F_MIN_TIME");
	maxTime = new TTimeFace(this, IDC_FLOAT_LOG_MAX_TIME, "F_MAX_TIME");
	users = new TUserListBox(this, IDC_FLOAT_LOG_USERS, POST_USERS);
	gops = new TCharListFace(this, IDC_FLOAT_LOG_GOPS, "F_GOPS", GOP, NULL, SIZE_OF_GOPS);
	new TRadioFace(this, IDC_FLOAT_LOG_TEXT, "F_TEXT");
	new TRadioFace(this, IDC_FLOAT_LOG_HYPER, "F_HYPER");
	new TRadioFace(this, IDC_FLOAT_LOG_COUNT, "F_COUNT");
}

bool TFloatLogDialog::IsValid()
{
	bool isValid = TFloatDialog::IsValid();

	if (isValid)
	{
		if (invalid_range(minNo, maxNo))
			isValid = error("Ќачални€ номер документ е по-гол€м от крайни€.");
		else if (invalid_range(minYear, maxYear))
			isValid = error("Ќачалната година е по-гол€ма от крайната.");
		else if (invalid_range(minDate, maxDate))
			isValid = error("Ќачалната дата е по-гол€ма от крайната.");
		else if (minDate->Date() == maxDate->Date() && invalid_range(minTime, maxTime))
			isValid = error("Ќачални€ час е по-гол€м от крайни€.");
	}

	return isValid;
}

void TFloatLogDialog::Action(bool execute)
{
	TFloatLogGroup *group = (TFloatLogGroup *) Group();
	msql m;

	if (group->maxStamp.Time().Empty())
	{
		group->maxStamp.hour = 23;
		group->maxStamp.minute = 59;
	}

	users->GetMark(&group->users);
	group->FormCriteria(m);

	if (execute)
		query_log(this, str(m), group->variant);
	else
		Message(m);
}
