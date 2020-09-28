DEFINE_RESPONSE_TABLE1(TFloatUCNsPeriodDialog, TFloatDialog)
	EV_BN_CLICKED(IDC_FLOAT_UCNS_PERIOD_NON_FIN_LAW, OneDate),
	EV_BN_CLICKED(IDC_FLOAT_UCNS_PERIOD_CREATED_LAW, TwoDates),
	EV_BN_CLICKED(IDC_FLOAT_UCNS_PERIOD_FIN_LAW_PERIOD, TwoDates),
	EV_BN_CLICKED(IDC_FLOAT_UCNS_PERIOD_SESSION_PERIOD, TwoDates),
END_RESPONSE_TABLE;

TFloatUCNsPeriodDialog::TFloatUCNsPeriodDialog(TWindow *parent, TFloatUCNsGroup *group)
	: TFloatDialog(parent, group, IDD_FLOAT_UCNS_PERIOD)
{
	nonFinLaw = new TRadioFace(this, IDC_FLOAT_UCNS_PERIOD_NON_FIN_LAW, "F_NON_FIN_LAW");
	createdLaw = new TRadioFace(this, IDC_FLOAT_UCNS_PERIOD_CREATED_LAW, "F_CREATED_LAW");
	finLawPeriod = new TRadioFace(this, IDC_FLOAT_UCNS_PERIOD_FIN_LAW_PERIOD, "F_FIN_LAW_PERIOD");
	sessPeriod = new TRadioFace(this, IDC_FLOAT_UCNS_PERIOD_SESSION_PERIOD, "F_SES_PERIOD");
	date = new TDateFace(this, IDC_FLOAT_UCNS_PERIOD_DATE, "F_DATE", false);
	new TDateFaceUpDown(this, IDC_FLOAT_UCNS_PERIOD_DATE_UPDN, date);
	minDate = new TDateFace(this, IDC_FLOAT_UCNS_PERIOD_MIN_DATE, "F_MIN_DATE", false);
	new TDateFaceUpDown(this, IDC_FLOAT_UCNS_PERIOD_MIN_DATE_UPDN, minDate);
	maxDate = new TDateFace(this, IDC_FLOAT_UCNS_PERIOD_MAX_DATE, "F_MAX_DATE", false);
	new TDateFaceUpDown(this, IDC_FLOAT_UCNS_PERIOD_MAX_DATE_UPDN, maxDate);

}

bool TFloatUCNsPeriodDialog::IsValid()
{
	bool isValid = TFloatDialog::IsValid();

	if (finLawPeriod->GetCheck() == BF_CHECKED || sessPeriod->GetCheck() == BF_CHECKED)
	{
		if (invalid_range(minDate, maxDate))
			isValid = error("Ќачалната дата е по-гол€ма от крайната.");
	}

	return isValid;
}

void TFloatUCNsPeriodDialog::SetupWindow()
{
	TFloatDialog::SetupWindow();

	nonFinLaw->SetCheck(BF_CHECKED);
	OneDate();
}

void TFloatUCNsPeriodDialog::OneDate()
{
	date->EnableWindow(true);
	minDate->EnableWindow(false);
	maxDate->EnableWindow(false);
}

void TFloatUCNsPeriodDialog::TwoDates()
{
	date->EnableWindow(false);
	minDate->EnableWindow(true);
	maxDate->EnableWindow(true);
}