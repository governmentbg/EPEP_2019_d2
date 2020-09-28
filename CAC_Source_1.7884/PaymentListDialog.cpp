DEFINE_RESPONSE_TABLE1(TPaymentListDialog, TQuickDialog)
//{{TPaymentListDialogRSP_TBL_BEGIN}}
	EV_LVN_ITEMCHANGING(IDC_PAYMENT_LIST_PAYMENTS, PaymentsLVNItemChanging),
	EV_BN_CLICKED(IDPREV, PrevBNClicked),
	EV_COMMAND_ENABLE(IDPREV, FindBNEnable),
	EV_BN_CLICKED(IDNEXT, NextBNClicked),
	EV_COMMAND_ENABLE(IDNEXT, FindBNEnable),
	EV_BN_CLICKED(IDSEARCH, SearchBNClicked),
	EV_BN_CLICKED(IDADD, AddBNClicked),
	EV_COMMAND_ENABLE(IDADD, AddBNEnable),
	EV_BN_CLICKED(IDEDIT, EditBNClicked),
	EV_COMMAND_ENABLE(IDEDIT, EditBNEnable),
	EV_BN_CLICKED(IDDELETE, DeleteBNClicked),
	EV_COMMAND_ENABLE(IDDELETE, DeleteBNEnable),
	EV_EN_SETFOCUS(IDC_PAYMENT_LIST_DATE, SetSearch),
	EV_EN_SETFOCUS(IDC_PAYMENT_LIST_TRANS_ID, SetSearch),
	EV_EN_SETFOCUS(IDC_PAYMENT_LIST_ECR, SetSearch),
	EV_BN_CLICKED(IDPAYMENT, PaymentBNClicked),
	EV_COMMAND_ENABLE(IDPAYMENT, PaymentBNEnable),
	EV_BN_CLICKED(IDPRINT, PrintBNClicked),
	EV_COMMAND_ENABLE(IDPRINT, PrintBNEnable),
	EV_BN_CLICKED(IDQUERY, QueryBNClicked),
	EV_MESSAGE(WM_FOCUS_FACE, EvFocusFace),
	EV_BN_CLICKED(IDCANCEL, CancelBNClicked),
	EV_WM_CLOSE,
//{{TPaymentListDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TPaymentListDialog Implementation}}

TPaymentListDialog::TPaymentListDialog(TWindow* parent, int resId)
:
	TQuickDialog(parent, resId)
{
	date = new TDateFace(this, IDC_PAYMENT_LIST_DATE, "F_DATE");
	new TDateFaceUpDown(this, IDC_PAYMENT_LIST_DATE_UPDN, date);

	transId = new TTransIdFace(this, IDC_PAYMENT_LIST_TRANS_ID, "F_TRANS_ID", true);
	strcpy(payment.transId, "");
	ecr = new TLongFace(this, IDC_PAYMENT_LIST_ECR, "F_ECR", SIZE_OF_ECR);
	payments = new TPaymentListWindow(this, IDC_PAYMENT_LIST_PAYMENTS, &payment);
	search = new TPushButton(this, IDSEARCH);
	add = new TPushButton(this, IDADD);
	edit = new TPushButton(this, IDEDIT);
}

void TPaymentListDialog::SetSearch()
{
	SetDefault(add, search);
}

bool TPaymentListDialog::CanClose()
{
	bool result = TQuickDialog::CanClose();

	if (result)
	{
		TPayment tPayment;
		msql m(tPayment.Name, "COUNT(F_ECR)");
	#if TESTVER  // 2018-07-09
		strcpy(tPayment.transId, TRANS_ID_PENDING);
		tPayment.ecr = POSTERM_ECR;
		m.Add(&tPayment, "F_TRANS_ID, F_ECR");

		if (tPayment.DataBase->GetLong(str(m), "COUNT") > 0)
	#else  // TESTVER
		long count;

		strcpy(tPayment.transId, TRANS_ID_PENDING);
		tPayment.ecr = POSTERM_ECR;
		m.Add(&tPayment, "F_TRANS_ID, F_ECR");
		TGetGroup g(str(m), new TLong("COUNT", &count));

		if (count)
	#endif  // TESTVER
		{
			result = ask("Неразплатените суми за електронна каса %ld ще бъдат изтрити. Желаете ли да продължите?",
				POSTERM_ECR);

			if (result)
			{
				m.Begin(payment.Name, MQ_DELETE);
				m.Add(&tPayment, "F_TRANS_ID, F_ECR");
				result = payment.DataBase->Execute(str(m), ET_ERROR);
			}
		}
	}

	return result;
}

void TPaymentListDialog::SetupWindow()
{
	TQuickDialog::SetupWindow();
	ecr->SetL(POSTERM_ECR);
}

bool TPaymentListDialog::PaymentsLVNItemChanging(TLwNotify &lwn)
{
	return payments->ItemChanging(lwn);
}

void TPaymentListDialog::DropCBNSelChange()
{
	SearchBNClicked();
}

void TPaymentListDialog::PrevBNClicked()
{
	payments->FindText(transId->S(), -1);
}

void TPaymentListDialog::NextBNClicked()
{
	payments->FindText(transId->S(), +1);
}

void TPaymentListDialog::FindBNEnable(TCommandEnabler &tce)
{
	tce.Enable(*transId->S());
}

void TPaymentListDialog::SearchBNClicked()
{
	if (IsValid())
	{
		payment << this;
		payments->Refresh();
	}
}

void TPaymentListDialog::AddBNClicked()
{
	payment << this;
	payments->AddRepeat();
}

void TPaymentListDialog::AddBNEnable(TCommandEnabler &tce)
{
	tce.Enable(date->Date() == Today);
}

void TPaymentListDialog::EditBNClicked()
{
	payments->Edit();
}

void TPaymentListDialog::EditBNEnable(TCommandEnabler &tce)
{
	TPayment *payment = payments->Payment();
	tce.Enable(payment && !strcmp(payment->transId, TRANS_ID_PENDING));
}

void TPaymentListDialog::DeleteBNClicked()
{
	payments->Delete();
}

void TPaymentListDialog::DeleteBNEnable(TCommandEnabler &tce)
{
	TPayment *payment = payments->Payment();
	tce.Enable(payment && !strcmp(payment->transId, TRANS_ID_PENDING) && can_func(RIGHT_DELETE));
}

void TPaymentListDialog::PaymentBNClicked()
{
	payments->PayPending();
}

void TPaymentListDialog::PaymentBNEnable(TCommandEnabler &tce)
{
	TPayment *payment = payments->Payment();
	tce.Enable(payment && !strcmp(payment->transId, TRANS_ID_PENDING) && TPosTerm::Active(payment->PosTerm()));
}

void TPaymentListDialog::PrintBNClicked()
{
	print_payments(this, payments->Payment());
}

void TPaymentListDialog::PrintBNEnable(TCommandEnabler &tce)
{
	TPayment *payment = payments->Payment();
	tce.Enable(payments->Payment() && strcmp(payment->transId, TRANS_ID_PENDING));
}

void TPaymentListDialog::QueryBNClicked()
{
	print_transacts(this, date->Date(), ecr->L());
}

void TPaymentListDialog::QueryBNEnable(TCommandEnabler &tce)
{
	tce.Enable(!date->Date().Empty() && date->Date().Valid());
}

long TPaymentListDialog::EvFocusFace(TParam1 wParam, TParam2)
{
	if (wParam)
		SetDefault(search, add);

	return MW_FACE_FOCUSED;
}

void TPaymentListDialog::CancelBNClicked()
{
	EvClose();
}

void TPaymentListDialog::EvClose()
{
	if (CanClose())
		TQuickDialog::EvClose();
}
