TFloatSendersDialog::TFloatSendersDialog(TWindow* parent, TFloatSendersGroup *group, int resId)
	: TFloatDialog(parent, group, resId)
{
	types = new TCharAutoListFace(this, IDC_FLOAT_SENDERS_TYPES, "F_TYPES", College, Default->xColleges, SIZE_OF_TYPES);
	minNo = new TLongFace(this, IDC_FLOAT_SENDERS_MIN_NO, "F_MIN_NO", SIZE_OF_NO);
	maxNo = new TLongFace(this, IDC_FLOAT_SENDERS_MAX_NO, "F_MAX_NO", SIZE_OF_NO);
	minYear = new TYearFace(this, IDC_FLOAT_SENDERS_MIN_YEAR, "F_MIN_YEAR");
	maxYear = new TYearFace(this, IDC_FLOAT_SENDERS_MAX_YEAR, "F_MAX_YEAR");
	minDate = new TDateFace(this, IDC_FLOAT_SENDERS_MIN_DATE, "F_MIN_DATE", false);
	maxDate = new TDateFace(this, IDC_FLOAT_SENDERS_MAX_DATE, "F_MAX_DATE", false);
	kinds = new TCharListFace(this, IDC_FLOAT_SENDERS_KINDS, "F_KINDS", Kind, KIND_OUT_REG_DOCS, SIZE_OF_KINDS);
	senders = new TSenderListBox(this, IDC_FLOAT_SENDERS_CONNECTS, UCN_RECEIVER_SENDERS);
	add_postal_buttons(this);
	text = new TStringFace(this, IDC_FLOAT_SENDERS_TEXT, "F_TEXT", SIZE_OF_OUTREG_REMARK);
}

bool TFloatSendersDialog::IsValid()
{
	bool isValid = TFloatDialog::IsValid();

	if (isValid)
	{
		if (invalid_range(minNo, maxNo))
			isValid = error("Ќачални€т номер е по-гол€м от крайни€");
		else if (invalid_range(minYear, maxYear))
			isValid = error("Ќачалната година е по-гол€ма от крайната.");
		if (invalid_range(minDate, maxDate))
			isValid = error("Ќачалната дата е по-гол€ма от крайната.");
	}

	return isValid;
}

void TFloatSendersDialog::Action(bool execute)
{
	msql m;

	senders->GetMark(&FloatSendersGroup()->senders);
	FloatSendersGroup()->FormCriteria(m);

	if (execute)
	{
		query_senders(this, str(m), &FloatSendersGroup()->senders);
		CmOk();
	}
	else
		Message(m);
}
