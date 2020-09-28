DEFINE_RESPONSE_TABLE1(TQueryBankDialog, TGroupDialog)
//{{TQueryBankDialogRSP_TBL_BEGIN}}
	EV_COMMAND_ENABLE(IDC_QUERY_BANK_INCOMPLETE_MIN_DATE, IncompleteDateEnable),
	EV_COMMAND_ENABLE(IDC_QUERY_BANK_INCOMPLETE_MAX_DATE, IncompleteDateEnable),
	EV_COMMAND_ENABLE(IDC_QUERY_BANK_COMPLETED_MIN_DATE, CompletedDateEnable),
	EV_COMMAND_ENABLE(IDC_QUERY_BANK_COMPLETED_MAX_DATE, CompletedDateEnable),
	EV_COMMAND_ENABLE(IDC_QUERY_BANK_YOUR_NO, YourNoEnable),
	EV_COMMAND_ENABLE(IDOK, QueryEnable),
	EV_BN_CLICKED(IDOK, OKBNClicked),
//{{TQueryBankDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TQueryBankDialog Implementation}}

TQueryBankDialog::TQueryBankDialog(TWindow* parent, TQueryBankGroup *group, int resId)
:
	TGroupDialog(parent, group, resId)
{
	incompleteQuery = new TCheckFace(this, IDC_QUERY_BANK_INCOMPLETE_QUERY, "F_INCOMPLETE_QUERY");
	incompleteMinDate = new TDateFace(this, IDC_QUERY_BANK_INCOMPLETE_MIN_DATE, "F_INCOMPLETE_MIN_DATE", false);
	incompleteMaxDate = new TDateFace(this, IDC_QUERY_BANK_INCOMPLETE_MAX_DATE, "F_INCOMPLETE_MAX_DATE", false);
	completedQuery = new TCheckFace(this, IDC_QUERY_BANK_COMPLETED_QUERY, "F_COMPLETED_QUERY");
	completedMinDate = new TDateFace(this, IDC_QUERY_BANK_COMPLETED_MIN_DATE, "F_COMPLETED_MIN_DATE", true);
	completedMaxDate = new TDateFace(this, IDC_QUERY_BANK_COMPLETED_MAX_DATE, "F_COMPLETED_MAX_DATE", true);
	header = new TCheckFace(this, IDC_QUERY_BANK_HEADER, "F_HEADER");
	yourNo = new TLongFace(this, IDC_QUERY_BANK_YOUR_NO, "F_YOUR_NO", SIZE_OF_NO);
}

bool TQueryBankDialog::IsValid()
{
	bool isValid = TGroupDialog::IsValid();

	if (isValid)
	{
		if (invalid_range(incompleteMinDate, incompleteMaxDate))
			isValid = error("Началната дата на образуване е по-голяма от крайната.");
		else if (invalid_range(completedMinDate, completedMaxDate))
			isValid = error("Началната дата на приключване е по-голяма от крайната.");
	}

	return isValid;
}

void TQueryBankDialog::IncompleteDateEnable(TCommandEnabler &tce)
{
	tce.Enable(incompleteQuery->GetCheck() == BF_CHECKED);
}

void TQueryBankDialog::CompletedDateEnable(TCommandEnabler &tce)
{
	tce.Enable(completedQuery->GetCheck() == BF_CHECKED);
}

void TQueryBankDialog::QueryEnable(TCommandEnabler &tce)
{
	tce.Enable(incompleteQuery->GetCheck() == BF_CHECKED || completedQuery->GetCheck() == BF_CHECKED);
}

void TQueryBankDialog::YourNoEnable(TCommandEnabler &tce)
{
	tce.Enable(header->IsWindowEnabled() && header->GetCheck() == BF_CHECKED);
}

void TQueryBankDialog::OKBNClicked()
{
	if (IsValid())
	{
		TQueryBankGroup *group = (TQueryBankGroup *) Group();
		*group << this;

		if (group->incomplete.maxDate.Empty() && group->incomplete.minDate < Today)
			group->incomplete.maxDate = Today;

		CmOk();
	}
}
