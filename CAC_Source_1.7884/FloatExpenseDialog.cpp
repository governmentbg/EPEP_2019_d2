//{{TFloatExpenseDialog Implementation}}

TFloatExpenseDialog::TFloatExpenseDialog(TWindow* parent, TFloatExpenseGroup *group, int resId)
:
	TFloatDialog(parent, group, resId)
{
	types = new TTypeListFace(this, IDC_FLOAT_EXPENSE_TYPES, "F_TYPES", group->sumType == SUM_TYPE_TAX ? TYPE_MONEYABLES :
		TYPE_LAWSUITS, false);
	minNo = new TLongFace(this, IDC_FLOAT_EXPENSE_MIN_NO, "F_MIN_NO", SIZE_OF_NO);
	maxNo = new TLongFace(this, IDC_FLOAT_EXPENSE_MAX_NO, "F_MAX_NO", SIZE_OF_NO);
	minYear = new TYearFace(this, IDC_FLOAT_EXPENSE_MIN_YEAR, "F_MIN_YEAR");
	maxYear = new TYearFace(this, IDC_FLOAT_EXPENSE_MAX_YEAR, "F_MAX_YEAR");
	minDate = new TDateFace(this, IDC_FLOAT_EXPENSE_MIN_DATE, "F_MIN_DATE", false);
	maxDate = new TDateFace(this, IDC_FLOAT_EXPENSE_MAX_DATE, "F_MAX_DATE", false);

	if (group->sumType == SUM_TYPE_TAX)
	{
		kinds = NULL;
		accountTypes = NULL;
		reasonTypes = NULL;
		penalityTypes = NULL;
		sumStates = NULL;
		compositions = NULL;
		minPaidDate = new TDateFace(this, IDC_FLOAT_EXPENSE_MIN_PAID_DATE, "F_MIN_PAID_DATE", false);
		maxPaidDate = new TDateFace(this, IDC_FLOAT_EXPENSE_MAX_PAID_DATE, "F_MAX_PAID_DATE", false);
		new TRadioFace(this, IDC_FLOAT_EXPENSE_BY_YNTK, "F_BY_YNTK");
		new TRadioFace(this, IDC_FLOAT_EXPENSE_BY_PAID, "F_BY_PAID");
		paidNote = NULL;
	}
	else
	{
		kinds = new TCharListFace(this, IDC_FLOAT_EXPENSE_KINDS, "F_KINDS", Kind, KIND_VISIBLE_MONEYABLES, SIZE_OF_KINDS);

		if (group->sumType == SUM_TYPE_PENALITY)
		{
			penalityTypes = new TAliasListBox(this, IDC_FLOAT_EXPENSE_WHATEVER_TYPES, PenalityType, NULL, FLAG_NULL);
			accountTypes = NULL;
			reasonTypes = NULL;
			minPaidDate = new TDateFace(this, IDC_FLOAT_EXPENSE_MIN_PAID_DATE, "F_MIN_PAID_DATE", false);
			maxPaidDate = new TDateFace(this, IDC_FLOAT_EXPENSE_MAX_PAID_DATE, "F_MAX_PAID_DATE", false);
			paidNote = new TStringFace(this, IDC_FLOAT_EXPENSE_PAID_NOTE, "F_PAID_NOTE", SIZE_OF_EXPENSE_NOTE);
		}
		else
		{
			accountTypes = new TCharListFace(this, IDC_FLOAT_EXPENSE_WHATEVER_TYPES, "F_ACCOUNT_TYPES", AccountType, ACCOUNT_TYPE_ALLS, SIZE_OF_ACCOUNT_TYPES);
			new TStringFace(this, IDC_FLOAT_EXPENSE_TEXT, "F_TEXT", SIZE_OF_EXPENSE_TEXT);
			reasonTypes = new TAliasListBox(this, IDC_FLOAT_EXPENSE_REASON_TYPES, ReasonType, NULL, FLAG_NULL);
			penalityTypes = NULL;
			minPaidDate = NULL;
			maxPaidDate = NULL;
			paidNote = NULL;
		}

		sumStates = new TCharListFace(this, IDC_FLOAT_EXPENSE_SUM_STATES, "F_SUM_STATES", SumState,
			group->sumType == SUM_TYPE_PENALITY ? SUM_STATE_PENALITIES : SUM_STATE_REWARDS, SIZE_OF_SUM_STATES);
		compositions = new TCharAutoListFace(this, IDC_FLOAT_EXPENSE_COMPOSITIONS, "F_COMPOSITIONS", CharComposition, QUERY_COMPOSITIONS, SIZE_OF_COMPOSITIONS);
	}
}

bool TFloatExpenseDialog::IsValid()
{
	bool isValid = TFloatDialog::IsValid();

	if (isValid)
	{

		if (invalid_range(minNo, maxNo))
			isValid = error("Ќачални€ номер дело е по-гол€м от крайни€.");
		else if (invalid_range(minYear, maxYear))
			isValid = error("Ќачалната година е по-гол€ма от крайната.");
		else if (invalid_range(minDate, maxDate))
			isValid = error("Ќачалната дата е по-гол€ма от крайната.");
		else if (FloatExpenseGroup()->sumType == SUM_TYPE_TAX && invalid_range(minPaidDate, maxPaidDate))
			isValid = error("Ќачалната дата на внас€не е по-гол€ма от крайната.");
	}

	return isValid;
}

void TFloatExpenseDialog::Action(bool execute)
{
	msql m;

	if (penalityTypes)
		penalityTypes->GetMark(&FloatExpenseGroup()->penalityTypes);
	if (reasonTypes)
		reasonTypes->GetMark(&FloatExpenseGroup()->reasonTypes);

	FloatExpenseGroup()->FormCriteria(m);

	if (execute)
		query_expense(this, str(m), FloatExpenseGroup()->sumType, FloatExpenseGroup()->order);
	else
		Message(m);
}
