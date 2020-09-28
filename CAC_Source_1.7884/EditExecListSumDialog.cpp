DEFINE_RESPONSE_TABLE1(TEditExecListSumDialog, TGroupDialog)
//{{TEditExecListSumDialogRSP_TBL_BEGIN}}
	EV_CBN_SELCHANGE(IDC_EDIT_EXEC_LIST_SUM_SENDER, SenderCBNSelChange),
	EV_COMMAND_ENABLE(IDADDRESS, AddressBNEnable),
	EV_BN_CLICKED(IDADDRESS, AddressBNClicked),
	EV_COMMAND_ENABLE(IDOK, OKBNEnable),
	EV_BN_CLICKED(IDOK, OKBNClicked),
	EV_CBN_SELCHANGE(IDC_EDIT_EXEC_LIST_SUM_SUM_TYPE, SumTypeCBNSelChange),
//{{TEditExecListSumDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TEditExecListSumDialog Implementation}}

TEditExecListSumDialog::TEditExecListSumDialog(TWindow* parent, TExecList *tExecList, TExecListSum *tSum, bool tEdit,
	int resId)
:
	TGroupDialog(parent, tSum, resId), execList(tExecList), edit(tEdit), lastSumType('\0')
{
	persons = new TExecListPersonListBox(this, IDC_EDIT_EXEC_LIST_SUM_PERSONS, &execList->allSumPersons,
		execList->execListKind == EXEC_LIST_PRIVATE);
	sumType = new TCharAliasFace(this, IDC_EDIT_EXEC_LIST_SUM_SUM_TYPE, "F_SUM_TYPE", SumType,
		execList->execListKind == EXEC_LIST_PRIVATE ? SUM_TYPE_EXEC_LIST_PRIVATES : SUM_TYPE_EXEC_LIST_COUNTRIES);
	sum = new TMoneyFace(this, IDC_EDIT_EXEC_LIST_SUM_SUM, "F_SUM", SIZE_OF_SUM);
	text = new TStringFace(this, IDC_EDIT_EXEC_LIST_SUM_TEXT, "F_SUM_TEXT", SIZE_OF_PAYMENT_TEXT);
	account = new TAccountAliasFace(this, IDC_EDIT_EXEC_LIST_SUM_SENDER, "F_SENDER", UCN_EXEC_LIST, ALIAS_OPTION);
	bankName = new TStringFace(this, IDC_EDIT_EXEC_LIST_SUM_BANK_NAME, "F_BANK_NAME", SIZE_OF_EXEC_TEXT);
	bankAccount = new TStringFace(this, IDC_EDIT_EXEC_LIST_SUM_BANK_ACCOUNT, "F_BANK_ACCOUNT", SIZE_OF_BANK_ACCOUNT);
	bic = new TStringFace(this, IDC_EDIT_EXEC_LIST_SUM_BIC, "F_BIC", SIZE_OF_BIC);
	currency = new TCurrencyAliasFace(this, IDC_EDIT_EXEC_LIST_SUM_CURRENCY, "F_CURRENCY", ALIAS_OPTION);
}

bool TEditExecListSumDialog::IsValid()
{
	bool isValid = TGroupDialog::IsValid();

	if (isValid)
	{
		char tSumType = sumType->C();

		if (sum->M() == 0.0 && strchr(SUM_TYPE_VALUES, tSumType))
			isValid = error("При \"%s\" сумата е задължителна.", SumType->Seek(tSumType));
		else if (strchr(SUM_TYPE_OTHERS, sumType->C()) && !any(text))
			isValid = error("При \"%s\" текстът е задължителен", SumType->Seek(tSumType));
	}

	return isValid;
}

void TEditExecListSumDialog::SetupWindow()
{
	TGroupDialog::SetupWindow();

	if (edit)
		persons->SetMark(&Sum()->persons);
	else if (execList->execListKind == EXEC_LIST_COUNTRY || execList->allSumPersons.Count() == 1)
		persons->SetMark(&execList->allSumPersons);

	SumTypeCBNSelChange();
}

void TEditExecListSumDialog::SumTypeCBNSelChange()
{
	char tSumType = sumType->C();
	bool values = strchr(SUM_TYPE_VALUES, tSumType);

	sum->EnableWindow(values);
	currency->SetXCrit(values ? NULL : "");
	SenderCBNSelChange();

	if (!edit)
	{
		if (tSumType == SUM_TYPE_EXEC_TAX && sum->M() == 0.0)
		{
			sum->SetM(SUM_EXEC_TAX_MONEY);
			currency->SetS(CURRENCY_BGN);
		}
		else if (lastSumType == SUM_TYPE_EXEC_TAX && sum->M() == SUM_EXEC_TAX_MONEY)
			sum->SetM(0.0);

		lastSumType = tSumType;
	}
}

void TEditExecListSumDialog::SenderCBNSelChange()
{
	account->SetXCrit(execList->execListKind == EXEC_LIST_COUNTRY && strchr(SUM_TYPE_VALUES, sumType->C()) ?
		C2S[UCN_EXEC_LIST] : "");

	TAccount *tAccount = account->Account();

	bankName->SetS(tAccount->bankName);
	bankAccount->SetS(tAccount->bankAccount);
	bic->SetS(tAccount->bic);
}

void TEditExecListSumDialog::AddressBNEnable(TCommandEnabler &tce)
{
	tce.Enable(execList->execListKind == EXEC_LIST_PRIVATE && persons->GetSelCount() == 1);
}

void TEditExecListSumDialog::AddressBNClicked()
{
	persons->Address();
}

void TEditExecListSumDialog::OKBNEnable(TCommandEnabler &tce)
{
	tce.Enable(persons->GetSelCount() >= 1);
}

void TEditExecListSumDialog::OKBNClicked()
{
	persons->GetMark(&Sum()->persons);
	TGroupDialog::OKBNClicked();
}
