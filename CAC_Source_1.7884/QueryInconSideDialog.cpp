//{{TQueryInconSideDialog Implementation}}

TQueryInconSideDialog::TQueryInconSideDialog(TWindow* parent, TInconSideGroup *group, int resId)
:
	TFloatDialog(parent, group, resId)
{
	creditor = new TRadioFace(this, IDC_QUERY_INCONSIDE_CREDITOR, "F_CREDITOR");
	deptor = new TRadioFace(this, IDC_QUERY_INCONSIDE_DEBTOR, "F_DEBTOR");
	minDate = new TDateFace(this, IDC_QUERY_INCONSIDE_MIN_DATE, "F_MIN_DATE", false);
	maxDate = new TDateFace(this, IDC_QUERY_INCONSIDE_MAX_DATE, "F_MAX_DATE", false);
}

bool TQueryInconSideDialog::IsValid()
{
	bool isValid = TFloatDialog::IsValid();

	if (isValid)
	{
		if (invalid_range(minDate, maxDate))
			isValid = error("Началната дата на архивиране е по-голяма от крайната.");
	}

	return isValid;
}
