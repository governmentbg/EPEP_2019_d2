DEFINE_RESPONSE_TABLE1(TQueryBankWebDialog, TQueryBankDialog)
//{{TQueryBankWebDialogRSP_TBL_BEGIN}}
	EV_COMMAND_ENABLE(IDC_QUERY_BANK_INCOMPLETE_WEB_FILE, IncompleteWebFileEnable),
	EV_COMMAND_ENABLE(IDC_QUERY_BANK_INCOMPLETE_MIN_DATE, IncompleteDateEnable),
	EV_COMMAND_ENABLE(IDC_QUERY_BANK_INCOMPLETE_MAX_DATE, IncompleteDateEnable),
	EV_COMMAND_ENABLE(IDC_QUERY_BANK_COMPLETED_WEB_FILE, CompletedWebFileEnable),
	EV_COMMAND_ENABLE(IDC_QUERY_BANK_COMPLETED_MIN_DATE, CompletedDateEnable),
	EV_COMMAND_ENABLE(IDC_QUERY_BANK_COMPLETED_MAX_DATE, CompletedDateEnable),
	EV_COMMAND_ENABLE(IDC_QUERY_BANK_HEADER, QueryEnable),
	EV_COMMAND_ENABLE(IDOK, OKBNEnable),
//{{TQueryBankWebDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TQueryBankWebDialog Implementation}}

TQueryBankWebDialog::TQueryBankWebDialog(TWindow* parent, TQueryBankGroup *group)
:
	TQueryBankDialog(parent, group, IDD_QUERY_BANK_WEB)
{
	incompleteWebQuery = new TCheckFace(this, IDC_QUERY_BANK_INCOMPLETE_WEB_QUERY, "F_INCOMPLETE_WEB_QUERY");
	incompleteWebFile = new TStringFace(this, IDC_QUERY_BANK_INCOMPLETE_WEB_FILE, "F_INCOMPLETE_WEB_FILE", SIZE_OF_PATH);
	completedWebQuery = new TCheckFace(this, IDC_QUERY_BANK_COMPLETED_WEB_QUERY, "F_COMPLETED_WEB_QUERY");
	completedWebFile = new TStringFace(this, IDC_QUERY_BANK_COMPLETED_WEB_FILE, "F_COMPLETED_WEB_FILE", SIZE_OF_PATH);
}

bool TQueryBankWebDialog::Invalid(TCheckFace *webQuery, const char *name, const char *what)
{
	bool isValid = true;

	if (webQuery->GetCheck() == BF_CHECKED)
	{
		if (!any(name))
			isValid = error("Името на изходния файл за %s е задължително", what);
		else if (!TOffice::ValidTargetName(name))
			isValid = false;
		else if (exist(name))
			isValid = ask("Файлът\n%s\nсъществува. Да бъде ли презаписан?", name);
	}

	return !isValid;
}

bool TQueryBankWebDialog::IsValid()
{
	bool isValid = TQueryBankDialog::IsValid();

	if (isValid)
	{
		if (Invalid(incompleteWebQuery, incompleteWebFile->S(), "движението на делата"))
			isValid = false;
		else if (Invalid(completedWebQuery, completedWebFile->S(), "приключилите дела"))
			isValid = false;
	}

	return isValid;
}

void TQueryBankWebDialog::IncompleteWebFileEnable(TCommandEnabler &tce)
{
	tce.Enable(incompleteWebQuery->GetCheck() == BF_CHECKED);
}

void TQueryBankWebDialog::IncompleteDateEnable(TCommandEnabler &tce)
{
	if (incompleteWebQuery->GetCheck() == BF_CHECKED)
		tce.Enable(true);
	else
		TQueryBankDialog::IncompleteDateEnable(tce);
}

void TQueryBankWebDialog::CompletedWebFileEnable(TCommandEnabler &tce)
{
	tce.Enable(completedWebQuery->GetCheck() == BF_CHECKED);
}

void TQueryBankWebDialog::CompletedDateEnable(TCommandEnabler &tce)
{
	if (completedWebQuery->GetCheck() == BF_CHECKED)
		tce.Enable(true);
	else
		TQueryBankDialog::CompletedDateEnable(tce);
}

void TQueryBankWebDialog::OKBNEnable(TCommandEnabler &tce)
{
	if (incompleteWebQuery->GetCheck() == BF_CHECKED || completedWebQuery->GetCheck() == BF_CHECKED)
		tce.Enable(true);
	else
		QueryEnable(tce);
}
