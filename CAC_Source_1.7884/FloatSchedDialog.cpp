//{{TFloatSchedDialog Implementation}}

TFloatSchedDialog::TFloatSchedDialog(TWindow* parent, TFloatSchedGroup *group, int resId)
:
	TFloatDialog(parent, group, resId)
{
	types = new TTypeListFace(this, IDC_FLOAT_SCHED_TYPES, "F_TYPES", TYPE_LAWSUITS, false);
	scheds = new TCharListFace(this, IDC_FLOAT_SCHED_SCHEDS, "F_SCHEDS", CalInterval, NULL, SIZE_OF_SCHEDS);
	minStart = new TDateFace(this, IDC_FLOAT_SCHED_MIN_START, "F_MIN_START", false);
	maxStart = new TDateFace(this, IDC_FLOAT_SCHED_MAX_START, "F_MAX_START", false);
	minFinal = new TDateFace(this, IDC_FLOAT_SCHED_MIN_FINAL, "F_MIN_FINAL", false);
	maxFinal = new TDateFace(this, IDC_FLOAT_SCHED_MAX_FINAL, "F_MAX_FINAL", false);
	judges = new TUserListBox(this, IDC_FLOAT_SCHED_JUDGES, POST_VISIBLE_JUDGES, USER_INTER);
	minNo = new TLongFace(this, IDC_FLOAT_SCHED_MIN_NO, "F_MIN_NO", SIZE_OF_NO);
	maxNo = new TLongFace(this, IDC_FLOAT_SCHED_MAX_NO, "F_MAX_NO", SIZE_OF_NO);
	minYear = new TYearFace(this, IDC_FLOAT_SCHED_MIN_YEAR, "F_MIN_YEAR");
	maxYear = new TYearFace(this, IDC_FLOAT_SCHED_MAX_YEAR, "F_MAX_YEAR");
}

bool TFloatSchedDialog::IsValid()
{
	bool isValid = TFloatDialog::IsValid();

	if (isValid)
	{
		if (invalid_range(minStart, maxStart))
			isValid = error("Ќачалните дати формират невалиден период.");
		else if (invalid_range(minFinal, maxFinal))
			isValid = error(" райните дати формират невалиден период.");
		else if (invalid_range(minNo, maxNo))
			isValid = error("Ќачални€ номер дело е по-гол€м от крайни€.");
		else if (invalid_range(minYear, maxYear))
			isValid = error("Ќачалната година е по-гол€ма от крайната.");
	}

	return isValid;
}

void TFloatSchedDialog::Action(bool execute)
{
	msql m;

	if (judges->GetSelCount() > 0)
		judges->GetMark(&FloatSchedGroup()->judges);

	FloatSchedGroup()->FormCriteria(m);

	if (execute)
	{
		if (!FloatSchedGroup()->judges.Count())
			query_sched_sched(this, str(m));
		else
			query_sched_judge(this, str(m));
	}
	else
		Message(m);
}

void TFloatSchedDialog::SetupWindow()
{
	TFloatDialog::SetupWindow();
	judges->SetMark(&FloatSchedGroup()->judges);
}
