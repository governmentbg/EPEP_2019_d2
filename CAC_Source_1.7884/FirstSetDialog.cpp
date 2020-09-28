TFirstSetDialog::TFirstSetDialog(TWindow* parent, TFirstSetGroup *group)
	: TFloatSubjectDialog(parent, group,  IDD_FLOAT_FIRST_SET)
{
	build_ckinds(ExchangeKinds, KIND_EXCHANGE_LAWS, false);
	kinds = new TCharListFace(this, IDC_FLOAT_FIRST_SET_LAW_TYPE, "F_KINDS", Kind, ExchangeKinds, SIZE_OF_KINDS);
	minNo = new TLongFace(this, IDC_FLOAT_FIRST_SET_MIN_NO, "F_MIN_NO", SIZE_OF_NO);
	maxNo = new TLongFace(this, IDC_FLOAT_FIRST_SET_MAX_NO, "F_MAX_NO", SIZE_OF_NO);
	minYear = new TYearFace(this, IDC_FLOAT_FIRST_SET_MIN_YEAR, "F_MIN_YEAR");
	maxYear = new TYearFace(this, IDC_FLOAT_FIRST_SET_MAX_YEAR, "F_MAX_YEAR");
	request = new TStringListBox(this, IDC_FLOAT_FIRST_SET_INREG);
	minDate = new TDateFace(this, IDC_FLOAT_FIRST_SET_MIN_DATE, "F_MIN_DATE", false);
	maxDate = new TDateFace(this, IDC_FLOAT_FIRST_SET_MAX_DATE, "F_MAX_DATE", false);
	doublex = new TCheckFace(this, IDC_FLOAT_FIRST_SET_DOUBLEX, "F_DOUBLEX");
	firstSet = new TCheckFace(this, IDC_FLOAT_FIRST_SET_BASIC, "F_FIRST_SET");
	new TRadioFace(this, IDC_FLOAT_FIRST_SET_ALL, "F_SET_ALL");
	new TRadioFace(this, IDC_FLOAT_FIRST_SET_MONTH, "F_SET_MONTH");
	new TRadioFace(this, IDC_FLOAT_FIRST_SET_MONTH_2, "F_SET_MONTH_2");
	new TRadioFace(this, IDC_FLOAT_FIRST_SET_MONTH_3, "F_SET_MONTH_3");
	new TRadioFace(this, IDC_FLOAT_FIRST_SET_MONTHS, "F_SET_MONTHS");
	new TRadioFace(this, IDC_FLOAT_FIRST_SET_SLOW, "F_SLOW");
	new TRadioFace(this, IDC_FLOAT_FIRST_SET_QUICK, "F_QUICK");
	new TRadioFace(this, IDC_FLOAT_FIRST_SET_ANY_SPEED, "F_ANY_SPEED");
}

bool TFirstSetDialog::IsValid()
{
	bool isValid = TFloatSubjectDialog::IsValid();

	if (isValid)
	{
		if (!*kinds->S())
			isValid = error("Не е избран нито един вид дело.");
		else if (invalid_range(minDate, maxDate))
			isValid = error("Началната дата е по-голяма от крайната.");
		else if (invalid_range(minNo, maxNo))
			isValid = error("Началния номер дело е по-голям от крайния.");
		else if (invalid_range(minYear, maxYear))
			isValid = error("Началната година е по-голяма от крайната.");
	}

	return isValid;
}

void TFirstSetDialog::Action(bool execute)
{
	msql m;

	TFloatSubjectDialog::GetSubjects(FirstSetGroup()->newSubjects, FirstSetGroup()->oldSubjects);
	FirstSetGroup()->FormCriteria(m);

	if (execute)
		first_set(this, str(m), FirstSetGroup());
	else
		Message(m);
}

void TFirstSetDialog::SetupWindow()
{
	TFloatSubjectDialog::SetupWindow();

	request->AddString("Искова молба");
	// 2011:199 LRQ: see const1
	if (CourtType == COURT_AREAL)
	{
		request->AddString("Молба за обявяване в несъстоятелност");
		request->AddString("Установителен иск");
	}
}
