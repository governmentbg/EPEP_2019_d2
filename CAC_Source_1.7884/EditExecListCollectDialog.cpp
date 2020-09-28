DEFINE_RESPONSE_TABLE1(TEditExecListCollectDialog, TEditDialog)
//{{TEditExecListCollectDialogRSP_TBL_BEGIN}}
	EV_COMMAND_ENABLE(IDOK, OKBNEnable),
	EV_BN_CLICKED(IDOK, OKBNClicked),
//{{TEditExecListCollectDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TEditExecListCollectDialog Implementation}}

TEditExecListCollectDialog::TEditExecListCollectDialog(TWindow* parent, TPrintExecList *tExecList, TExecListCollect *collect,
	bool edit)
:
	TEditDialog(parent, collect, edit, IDD_EDIT_EXEC_LIST_COLLECT), execList(tExecList)
{
	date = new TDateFace(this, IDC_EDIT_EXEC_LIST_COLLECT_DATE, "F_DATE", true),
	sum = new TMoneyFace(this, IDC_EDIT_EXEC_LIST_COLLECT_SUM, "F_SUM", SIZE_OF_SUM);
	currency = new TCurrencyAliasFace(this, IDC_EDIT_EXEC_LIST_COLLECT_CURRENCY, "F_CURRENCY", FLAG_NULL);
	voluntary = new TCheckFace(this, IDC_EDIT_EXEC_LIST_COLLECT_VOLUNTARY, "F_FLAGS_VOLUNTARY");
	debtor = new TSelectDebtorListBox(this, IDC_EDIT_EXEC_LIST_COLLECT_DEBTOR, &execList->debtors);
	text = new TStringFace(this, IDC_EDIT_EXEC_LIST_COLLECT_SUM_TEXT, "F_SUM_TEXT", SIZE_OF_PAYMENT_TEXT);
}

bool TEditExecListCollectDialog::IsValid()
{
	bool isValid = TEditDialog::IsValid();

	if (isValid)
	{
		if (date->Date() > Today)
			isValid = error("Датата е по-голяма от текущата дата.");
		else if (sum->M() == 0.0 && !any(text))
			isValid = error("Задължително е въвеждането на сума или текст.");
	}

	return isValid;
}

void TEditExecListCollectDialog::SetupWindow()
{
	TEditDialog::SetupWindow();

	if (edit)
		debtor->Select(Collect()->ucn, Collect()->ucnType);
	else if (debtor->GetCount() == 1)
		debtor->SetSelIndex(0);

	if (execList->tdepInNo)
		voluntary->EnableWindow(true);
	else
		voluntary->SetCheck(BF_CHECKED);
}

void TEditExecListCollectDialog::OKBNEnable(TCommandEnabler &tce)
{
	tce.Enable(debtor->Group());
}

void TEditExecListCollectDialog::OKBNClicked()
{
	strcpy(Collect()->ucn, debtor->ExecDebtor()->ucn);
	Collect()->ucnType = debtor->ExecDebtor()->ucnType;
	TEditDialog::OKBNClicked();
}
