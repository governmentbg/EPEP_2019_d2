//{{TJurorLoadDialog Implementation}}

TJurorLoadDialog::TJurorLoadDialog(TWindow* parent, TJurorLoadGroup *group, bool tRepeat)
:
	TFloatDialog(parent, group, IDD_JUROR_LOAD), repeat(tRepeat)
{
	minDate = new TDateFace(this, IDC_JUROR_LOAD_MIN_DATE, "F_MIN_DATE", true);
	maxDate = new TDateFace(this, IDC_JUROR_LOAD_MAX_DATE, "F_MAX_DATE", true);
	explain = new TCheckFace(this, IDC_JUROR_LOAD_EXPLAIN, "F_EXPLAIN");
	jurors = new TJurorLoadListBox(this, IDC_JUROR_LOAD_JURORS);
}

bool TJurorLoadDialog::IsValid()
{
	bool isValid = TFloatDialog::IsValid();

	if (isValid)
	{
		if (invalid_range(minDate, maxDate))
			isValid = error("Ќачалната дата е по-гол€ма от крайната.");
	}

	return isValid;
}

void TJurorLoadDialog::Action(bool execute)
{
	TJurorLoadGroup *loadGroup = TYPECHECK_PTR_CAST(TJurorLoadGroup, Group());

	jurors->GetMark(&loadGroup->jurors);
	loadGroup->jurorMark = loadGroup->jurors.Count();

	if (!loadGroup->jurorMark)
		jurors->GetCopy(&loadGroup->jurors);

	if (execute)
	{
		query_juror_load(this, loadGroup);

		if (!repeat)
			CmOk();
	}
	else
	{
		msql m;
		loadGroup->FormCriteria(m);
		MessageEx(m);
	}
}
