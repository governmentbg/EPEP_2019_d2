DEFINE_RESPONSE_TABLE1(TEditPaymentDialog, TEditDialog)
//{{TEditPaymentDialogRSP_TBL_BEGIN}}
	EV_CBN_SELCHANGE(IDC_EDIT_PAYMENT_SUM_TYPE, SumTypeCBNSelChange),
	EV_CBN_SELCHANGE(IDC_EDIT_PAYMENT_RECEIVER, ReceiverCBNSelChange),
	EV_CBN_SELCHANGE(IDC_EDIT_PAYMENT_TYPE, TypeCBNSelChange),
	EV_CBN_SELCHANGE(IDC_EDIT_PAYMENT_KIND, TypeCBNSelChange),
	EV_BN_CLICKED(IDOK, OKBNClicked),
	EV_COMMAND_ENABLE(IDOK, OKBNEnable),
	EV_EN_KILLFOCUS(IDC_EDIT_PAYMENT_NO, NoENKillFocus),
	EV_EN_KILLFOCUS(IDC_EDIT_PAYMENT_YEAR, YearENKillFocus),
	EV_EN_UPDATE(IDC_EDIT_PAYMENT_YEAR, YearENUpdate),
	EV_BN_CLICKED(IDC_EDIT_PAYMENT_SEARCH_OBL, SearchOblBNClicked),
	EV_BN_CLICKED(IDC_EDIT_PAYMENT_SEARCH_UCN, SearchUCNBNClicked),
//{{TEditPaymentDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TEditPaymentDialog Implementation}}

TEditPaymentDialog::TEditPaymentDialog(TWindow* parent, TPayment *payment, bool edit, int resId)
:
	TEditDialog(parent, payment, edit, resId)
{
	sumType = new TCharAliasFace(this, IDC_EDIT_PAYMENT_SUM_TYPE, "F_SUM_TYPE", SumType,
		TPosTerm::Active(POSTERM_BUDGET) || (edit && payment->accountType == ACCOUNT_TYPE_BUDGET) ?
		TPosTerm::Active(POSTERM_DEPOSITS) || (edit && payment->accountType == ACCOUNT_TYPE_DEPOSITS) ?
		SUM_TYPE_PAYMENTS : SUM_TYPE_BUDGETS : SUM_TYPE_DEPOSITS);
	accountType = new TCharAliasFace(this, IDC_EDIT_PAYMENT_ACCOUNT_TYPE, "F_ACCOUNT_TYPE", AccountType, NULL);
	sum = new TMoneyFace(this, IDC_EDIT_PAYMENT_SUM, "F_SUM", SIZE_OF_TRANS_SUM);
	new TDateFace(this, IDC_EDIT_PAYMENT_DATE, "F_DATE", false);
	new TTimeFace(this, IDC_EDIT_PAYMENT_TIME, "F_TIME", false);
	receiver = new TStringAliasFace(this, IDC_EDIT_PAYMENT_RECEIVER, "F_RECEIVER", Receiver, UCN_INSTANCE_PAYMENTS);
	type = new TCharAliasFace(this, IDC_EDIT_PAYMENT_TYPE, "F_TYPE", Type, TYPE_ORIGINS, ALIAS_EMPTY);
	kind = new TCharAliasFace(this, IDC_EDIT_PAYMENT_KIND, "F_KIND", ConnectKind, CONNECT_KIND_PAYMENTS, ALIAS_EMPTY);
	no = new TLongFace(this, IDC_EDIT_PAYMENT_NO, "F_NO", SIZE_OF_NO);
	year = new TYearFace(this, IDC_EDIT_PAYMENT_YEAR, "F_YEAR");
	obls = new TEditPaymentListBox(this, IDC_EDIT_PAYMENT_OBLS);
	ucns = new TEditPaymentListBox(this, IDC_EDIT_PAYMENT_UCNS);
	text = new TStringFace(this, IDC_EDIT_PAYMENT_TEXT, "F_TEXT", SIZE_OF_PAYMENT_TEXT);
	new TPushButton(this, IDC_EDIT_PAYMENT_SEARCH_OBL, IDSEARCH);
	new TPushButton(this, IDC_EDIT_PAYMENT_SEARCH_UCN, IDSEARCH);
}

bool TEditPaymentDialog::Self()
{
	return !strcmp(receiver->S(), INSTANCE_SELF) || atoi(receiver->S()) == COURT_CODE;
}

char TEditPaymentDialog::type_C()
{
	return Self() ? type->C() : connectKind2Type(kind->C());
}

int TEditPaymentDialog::LawCount()
{
	return (type_C() != '\0') + (no->L() != 0) + (year->L() != 0);
}

void TEditPaymentDialog::ProcessKey()
{
	if (Self() && LawCount() == 3)
	{
		TRCDKey key = { no->L(), year->L(), type->C() };

		SetWaitCursor(true);

		if (obls->Search(key))
			ucns->Search(obls);

		SetWaitCursor(false);
	}
	else if (!Self() && LawCount() == 0)
	{
		obls->Flush();
		ucns->Flush();
	}
}

bool TEditPaymentDialog::IsValid()
{
	bool isValid = TEditDialog::IsValid();

	if (isValid)
	{
		if (sum->M() == 0.0)
			isValid = error("Сумата е задължителна.");
		else if (!atob(receiver->S()))
			isValid = error("Съдът е задължителен.");
		else if (LawCount() % 3)
			isValid = error("Полетата вид дело/документ, номер и година не са комплектни.");
		else if (strchr(SUM_TYPE_OTHERS, sumType->C()) && !*text->S())
			isValid = error("Текстът е задължителен при Други суми.");
		//else if (!strchr(UCN_CITIZENS, obls->UCNGroup()->ucnType) && !ucns->UCNGroup())
		//	isValid = error("Задълженото лице е юридическо. Изберете физическо лице, което да извърши плащането.");
		else if (Self() && LawCount() == 3)
		{
			TRCDKeyContainer *container = type2RCDKeyContainer(type->C());

			container->key.no = no->L();
			container->key.year = year->L();
			isValid = container->Get("F_NO");
			delete container;
		}
	}

	return isValid;
}

void TEditPaymentDialog::SetupWindow()
{
	TEditDialog::SetupWindow();

	if (Payment()->court == COURT_CODE)
		receiver->SetS(INSTANCE_SELF);
	else
	{
		char tReceiver[SIZE_OF_UCN];
		sprintf(tReceiver, "%010ld", Payment()->court);
		receiver->SetS(tReceiver);
	}

	if (Payment()->court == COURT_CODE)
	{
		if (strchr(CONNECT_KIND_SELFS, Payment()->key.kind))
			type->SetC(connectKind2Type(Payment()->key.kind));
	}
	else
		kind->SetC(Payment()->key.kind);

	SumTypeCBNSelChange();
	ReceiverCBNSelChange();

	obls->Select(Payment()->obl, Payment()->oblType);
	ucns->Select(Payment()->ucn, Payment()->ucnType);
}

void TEditPaymentDialog::SumTypeCBNSelChange()
{
	SumType->Seek(sumType->C());
	accountType->SetC(SumType->accountType);
}

void TEditPaymentDialog::ReceiverCBNSelChange()
{
	if (Self())
	{
		type->EnableWindow(true);
		type->ShowWindow(SW_SHOW);
		kind->ShowWindow(SW_HIDE);
		kind->EnableWindow(false);
	}
	else
	{
		kind->EnableWindow(true);
		kind->ShowWindow(SW_SHOW);
		type->ShowWindow(SW_HIDE);
		type->EnableWindow(false);
	}

	TypeCBNSelChange();
}

void TEditPaymentDialog::TypeCBNSelChange()
{
	if (type_C())
	{
		no->EnableWindow(true);
		year->EnableWindow(true);
	}
	else
	{
		no->SetL(0);
		no->EnableWindow(false);
		year->SetL(0);
		year->EnableWindow(false);
	}

	ProcessKey();
}

void TEditPaymentDialog::OKBNClicked()
{
	TPayment *payment = Payment();

	strcpy(payment->obl, obls->UCNGroup()->ucn);
	payment->oblType = obls->UCNGroup()->ucnType;
	strcpy(payment->ucn, ucns->UCNGroup() ? ucns->UCNGroup()->ucn : obls->UCNGroup()->ucn);
	payment->ucnType = ucns->UCNGroup() ? ucns->UCNGroup()->ucnType : obls->UCNGroup()->ucnType;
	Payment()->key.kind = Self() ? type2ConnectKind(type->C()) : kind->C();
	Payment()->court = Self() ? COURT_CODE : atoi(receiver->S());

	TEditDialog::OKBNClicked();
}

void TEditPaymentDialog::OKBNEnable(TCommandEnabler &tce)
{
	tce.Enable(obls->Group());
}

void TEditPaymentDialog::NoENKillFocus()
{
	ProcessKey();
}

void TEditPaymentDialog::YearENKillFocus()
{
	ProcessKey();
}

void TEditPaymentDialog::YearENUpdate()
{
	long tYear = year->L();

	if (tYear >= YEAR_MIN && tYear <= YEAR_MAX)
		ProcessKey();
}

void TEditPaymentDialog::SearchOblBNClicked()
{
	TTransGroup ucnGroup;

	if (get_ucn(this, &ucnGroup, UCN_REAL_SIDES, true))
	{
		obls->Select(ucnGroup.ucn, ucnGroup.ucnType);

		if (!ucns->GetCount())
			ucns->Select(ucnGroup.ucn, ucnGroup.ucnType);
	}
}

void TEditPaymentDialog::SearchUCNBNClicked()
{
	TTransGroup ucnGroup;

	if (get_ucn(this, &ucnGroup, UCN_REAL_SIDES, true))
		ucns->Select(ucnGroup.ucn, ucnGroup.ucnType);
}
