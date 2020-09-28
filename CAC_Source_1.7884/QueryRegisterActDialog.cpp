//{{TQueryRegisterActDialog Implementation}}

TQueryRegisterActDialog::TQueryRegisterActDialog(TWindow* parent, TQueryRegisterActGroup *group, int resId)
:
	TFloatDialog(parent, group, resId)
{
	types = new TTypeListFace(this, IDC_QUERY_REGISTER_ACT_TYPES, "F_TYPES", TYPE_ORIGINS, false);
	minNo = new TLongFace(this, IDC_QUERY_REGISTER_ACT_MIN_NO, "F_MIN_NO", SIZE_OF_NO);
	maxNo = new TLongFace(this, IDC_QUERY_REGISTER_ACT_MAX_NO, "F_MAX_NO", SIZE_OF_NO);
	minYear = new TYearFace(this, IDC_QUERY_REGISTER_ACT_MIN_YEAR, "F_MIN_YEAR");
	maxYear = new TYearFace(this, IDC_QUERY_REGISTER_ACT_MAX_YEAR, "F_MAX_YEAR");
	minDate = new TDateFace(this, IDC_QUERY_REGISTER_ACT_MIN_DATE, "F_MIN_DATE", true);
	maxDate = new TDateFace(this, IDC_QUERY_REGISTER_ACT_MAX_DATE, "F_MAX_DATE", true);
	judges = new TUserListBox(this, IDC_QUERY_REGISTER_ACT_JUDGES, POST_VISIBLE_JUDGES);
}

bool TQueryRegisterActDialog::IsValid()
{
	bool isValid = TFloatDialog::IsValid();

	if (isValid)
	{
		if (invalid_range(minNo, maxNo))
			isValid = error("Ќачални€ номер дело е по-гол€м от крайни€.");
		else if (invalid_range(minYear, maxYear))
			isValid = error("Ќачалната година е по-гол€ма от крайната.");
		else if (invalid_range(minDate, maxDate))
			isValid = error("Ќачалната дата на регистриране е по-гол€ма от крайната.");
	}

	return isValid;
}

void TQueryRegisterActDialog::Action(bool execute)
{
	msql m;

	judges->GetMark(&QueryRegisterActGroup()->judges);
	QueryRegisterActGroup()->FormCriteria(m);

	if (execute)
		query_register_act(this, str(m), QueryRegisterActGroup());
	else
		Message(m);
}
