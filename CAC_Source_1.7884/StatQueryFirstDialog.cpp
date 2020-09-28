TStatQueryFirstDialog::TStatQueryFirstDialog(TWindow* parent, TStatQueryFirstGroup *group)
	: TFloatDialog(parent, group, IDD_STAT_QUERY_FIRST)
{
	senders = new TSenderListBox(this, IDC_STAT_QUERY_FIRST_COURT, C2S[UCN_INSTANCE_LOWER]);
	minDate = new TDateFace(this, IDC_STAT_QUERY_FIRST_MIN_DATE, "F_MIN_DATE");
	new TDateFaceUpDown(this, IDC_STAT_QUERY_FIRST_MIN_DATE_UPDN, minDate);
	maxDate = new TDateFace(this, IDC_STAT_QUERY_FIRST_MAX_DATE, "F_MAX_DATE");
	new TDateFaceUpDown(this, IDC_STAT_QUERY_FIRST_MAX_DATE_UPDN, maxDate);
}

bool TStatQueryFirstDialog::IsValid()
{
	bool isValid = TFloatDialog::IsValid();

	if (isValid)
	{
		if (invalid_range(minDate, maxDate))
			isValid = error("��������� ���� � ��-������ �� ��������.");
		else if (!senders->Group())
			isValid = error("���� ������ ���.");
	}

	return isValid;
}

void TStatQueryFirstDialog::Action(bool execute)
{
	senders->GetMark(&StatQueryFirstGroup()->senders);
	TFloatDialog::Action(execute);
}
