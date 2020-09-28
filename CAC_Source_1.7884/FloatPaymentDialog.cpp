DEFINE_RESPONSE_TABLE1(TFloatPaymentDialog, TFloatDialog)
//{{TFloatPaymentDialogRSP_TBL_BEGIN}}
	EV_BN_CLICKED(IDSEARCH, SearchBNClicked),
	EV_BN_CLICKED(IDC_FLOAT_PAYMENT_PERSON_TYPE, PersonTypeBNClicked),
//{{TFloatPaymentDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TFloatPaymentDialog Implementation}}

TFloatPaymentDialog::TFloatPaymentDialog(TWindow* parent, TFloatPaymentGroup *group, int resId)
:
	TFloatDialog(parent, group, resId), personType(PERSON_TYPE_UCN_OBL)
{
	sumTypes = new TCharListFace(this, IDC_FLOAT_PAYMENT_SUM_TYPES, "F_SUM_TYPES", SumType, SUM_TYPE_PAYMENTS, SIZE_OF_SUM_TYPES);
	minTransId = new TStringFace(this, IDC_FLOAT_PAYMENT_MIN_TRANS_ID, "F_MIN_TRANS_ID", SIZE_OF_TRANS_ID);
	maxTransId = new TStringFace(this, IDC_FLOAT_PAYMENT_MAX_TRANS_ID, "F_MAX_TRANS_ID", SIZE_OF_TRANS_ID);
	minDate = new TDateFace(this, IDC_FLOAT_PAYMENT_MIN_DATE, "F_MIN_DATE", false);
	maxDate = new TDateFace(this, IDC_FLOAT_PAYMENT_MAX_DATE, "F_MAX_DATE", false);
	minTime = new TTimeFace(this, IDC_FLOAT_PAYMENT_MIN_TIME, "F_MIN_TIME", false);
	maxTime = new TTimeFace(this, IDC_FLOAT_PAYMENT_MAX_TIME, "F_MAX_TIME", false);
	accountTypes = new TCharAutoListFace(this, IDC_FLOAT_PAYMENT_ACCOUNT_TYPES, "F_ACCOUNT_TYPES", AccountType, NULL, SIZE_OF_ACCOUNT_TYPES);
	ecr = new TLongFace(this, IDC_FLOAT_PAYMENT_ECR, "F_ECR", SIZE_OF_ECR);
	connectKinds = new TCharListFace(this, IDC_FLOAT_PAYMENT_CONNECT_KINDS, "F_CONNECT_KINDS", ConnectKind, CONNECT_KIND_PAYMENTS, SIZE_OF_CONNECT_KINDS);
	minNo = new TLongFace(this, IDC_FLOAT_PAYMENT_MIN_NO, "F_MIN_NO", SIZE_OF_NO);
	maxNo = new TLongFace(this, IDC_FLOAT_PAYMENT_MAX_NO, "F_MAX_NO", SIZE_OF_NO);
	minYear = new TYearFace(this, IDC_FLOAT_PAYMENT_MIN_YEAR, "F_MIN_YEAR");
	maxYear = new TYearFace(this, IDC_FLOAT_PAYMENT_MAX_YEAR, "F_MAX_YEAR");
	courts = new TSenderListBox(this, IDC_FLOAT_PAYMENT_COURTS, UCN_INSTANCE_PAYMENTS);
	personTypeBN = new TPushButton(this, IDC_FLOAT_PAYMENT_PERSON_TYPE);
	persons = new TEditPaymentListBox(this, IDC_FLOAT_PAYMENT_PERSONS);
	text = new TStringFace(this, IDC_FLOAT_PAYMENT_TEXT, "F_TEXT", SIZE_OF_PAYMENT_TEXT);
}

bool TFloatPaymentDialog::IsValid()
{
	bool isValid = TFloatDialog::IsValid();

	if (isValid)
	{
		if (invalid_range(minTransId, maxTransId))
			isValid = error("Началния номер на транзакция е по-голям от крайния.");
		else if (invalid_range(minDate, maxDate))
			isValid = error("Началната дата на транзакция е по-голяма от крайната.");
		else if (invalid_range(minTime, maxTime))
			isValid = error("Началния час на транзакция е по-голям от крайния.");
		else if (invalid_range(minNo, maxNo))
			isValid = error("Началния номер дело е по-голям от крайния.");
		else if (invalid_range(minYear, maxYear))
			isValid = error("Началната година е по-голяма от крайната.");
	}

	return isValid;
}

void TFloatPaymentDialog::Action(bool execute)
{
	courts->GetMark(&FloatPaymentGroup()->courts);
	persons->GetMark(&FloatPaymentGroup()->persons);
	FloatPaymentGroup()->personType = personType;

	if (execute)
		query_payment(this, FloatPaymentGroup());
	else
	{
		msql m;

		FloatPaymentGroup()->FormCriteria(m);
		MessageEx(m);
	}
}

void TFloatPaymentDialog::SetupWindow()
{
	TFloatDialog::SetupWindow();

	if (FloatPaymentGroup()->minTime.Empty())
		minTime->SetS("");

	if (FloatPaymentGroup()->maxTime.Empty())
		maxTime->SetS("");
}

void TFloatPaymentDialog::SearchBNClicked()
{
	TFoundGroup ucnGroup;

	if (get_ucn(this, &ucnGroup, UCN_REAL_SIDES, true))
		persons->Select(ucnGroup.ucn, ucnGroup.ucnType);
}

void TFloatPaymentDialog::PersonTypeBNClicked()
{
	personType = (personType + 1) % PERSON_TYPE_COUNT;
	personTypeBN->SetCaption(person_titles[personType]);
}
