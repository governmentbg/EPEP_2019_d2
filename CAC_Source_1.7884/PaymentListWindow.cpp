//{{TPaymentListWindow Implementation}}

TPaymentListWindow::TPaymentListWindow(TWindow* parent, int resourceId, TPayment *tSearch)
:
	TDoubleListWindow(parent, resourceId, new TGSArray<TPayment>, 356), search(tSearch)
{
	Add(
		new TGroupListColumn("", generic_null, 0),
		new TGroupListColumn("Транзакция", payment_trans_id, 80),
		new TGroupListColumn("Сума", "F_SUM", 40),
		new TGroupListColumn("Дата и час", payment_stamp, 64, LVCFMT_RIGHT),
		new TGroupListColumn("Други данни", payment_key_short, 160),
		NULL
	);
}

void TPaymentListWindow::Refresh()
{
	QuickRefresh();
	DeleteAllItems();
	array->Flush();
	LoadData();

	for (int index = 0; index < array->Count(); index++)
		InsertVisual(GroupAt(index), index << 1);
}

void TPaymentListWindow::AddRepeat()
{
	do
	{
		TPayment *payment = new TPayment;
		TPayment *tPayment = Payment();

		if (tPayment)
		{
			payment->sumType = tPayment->sumType;
			payment->key = tPayment->key;
			strcpy(payment->obl, tPayment->obl);
			payment->oblType = tPayment->oblType;
			strcpy(payment->ucn, tPayment->ucn);
			payment->ucnType = tPayment->ucnType;
		}

		if (TEditPaymentDialog(this, payment, false).Execute() == IDCANCEL)
		{
			delete payment;
			break;
		}

		InsertGroup(payment);

	} while (REPEAT_PAYMENTS);
}

void TPaymentListWindow::Edit()
{
	int index = GetVisualIndex();

	if (index >= 0)
	{
		TPayment *payment = PaymentAt(index >> 1);
		TPayment tPayment;

		tPayment << payment;

		if (TEditPaymentDialog(this, &tPayment, true).Execute() == IDOK)
		{
			*payment << tPayment;
			DrawGroupAt(payment, index, false);
		}
	}
}

void TPaymentListWindow::Delete()
{
	TPayment *payment = Payment();

	if (payment && ask("Желаете ли да изтриете избраното плащане?") && payment->Delete(true))
		RemoveGroup(IDDELETE);
}

void TPaymentListWindow::LoadData()
{
	msql m(search->Name);

	if (!strcmp(search->transId, TRANS_ID_PENDING))
		m.AddString("F_TRANS_ID", search->transId);
	else
		m.AddLike("F_TRANS_ID", search->transId, false);

	if (search->ecr)
		m.AddLong("F_ECR", search->ecr);

	m.AddDate("F_DATE", search->date);
	search->LoadArray(array, str(m));
}

void TPaymentListWindow::DrawSecond(TListWindItem &lwi, TGroup *group)
{
	TPayment *payment = (TPayment *) group;
	mstr m;

	if (strcmp(payment->transId, TRANS_ID_PENDING))
	{
		m.printf("АК %-6s, Т %ld", payment->authId, payment->termId);
		lwi.SetText(str(m));
		SetItem(lwi);
		m.clear();
	}

	lwi.iSubItem++;
	lwi.SetText(AccountType->Seek(payment->accountType));
	SetItem(lwi);

	lwi.iSubItem++;
	SumType->Seek(payment->sumType);
	lwi.SetText(SumType->alias);
	SetItem(lwi);

	lwi.iSubItem++;
	payment_ucn_long(m, payment);
	lwi.SetText(str(m));
	SetItem(lwi);
}

void TPaymentListWindow::SetupWindow()
{
	TDoubleListWindow::SetupWindow();
	LoadData();
	TDoubleListWindow::Refresh(GetClientRect().Width());
}

TGroup *TPaymentListWindow::NewGroup() const
{
	return new TPayment;
}

bool TPaymentListWindow::EditGroup(TGroup *group)
{
	return TEditPaymentDialog(this, (TPayment *) group, true).Execute() == IDOK;
}

void TPaymentListWindow::PayPending()
{
	TPayment *payment = Payment();
	TIntegerArray indexes;
	double total = 0.0;

	for (int i = 0; i < array->Count(); i++)
	{
		TPayment *tPayment = PaymentAt(i);

		if (tPayment->accountType == payment->accountType && tPayment->ucnType == payment->ucnType &&
			!strcmp(tPayment->ucn, payment->ucn) && !strcmp(tPayment->transId, TRANS_ID_PENDING))
		{
			indexes.Add(i);
			total += tPayment->sum;
		}
	}

	mstr m;
	long termId;
	char transId[SIZE_OF_TRANS_ID];
	char authId[SIZE_OF_PAYMENT_AUTH_ID];
	char cardId[SIZE_OF_PAYMENT_CARD_ID];

	payment_ucn_long(m, payment);

	if (ask("Плащане на %.2lfлв. по сметка %s\nот лицето %s.\n\nЖелаете ли да продължите?", total,
		AccountType->Seek(payment->accountType), str(m)) && TPosTerm::Approval(this, payment->PosTerm()) &&
		(termId = TPosTerm::Payment(this, payment->PosTerm(), total, transId, authId, cardId)) != 0)
	{
		TRCDKey key = { payment->key.no, payment->key.year, connectKind2Type(payment->key.kind) };
		CDateTime::SyncServer(SYNC_EXACT);

		if (termId < 0)
		{
			log(LOG_POSTERM, 0, &key, "прекъсване с клавиши");
			error("Прекъсване на плащането с клавишна комбинация!");
		}
		else
		{
			m.cpy(transId);
			m.printf(" %s", authId);
			log(LOG_POSTERM, 0, &key, str(m));

			for (int i = 0; i < indexes.Count(); i++)
			{
				TPayment tPayment;

				payment = PaymentAt(indexes[i]);
				tPayment << payment;
				strcpy(tPayment.transId, transId);
				strcpy(tPayment.authId, authId);
				tPayment.ecr = POSTERM_ECR;
				tPayment.termId = termId;
				strcpy(tPayment.cardId, cardId);

				if (tPayment.Insert())
				{
					payment->Delete(true);
					*payment << tPayment;
					DrawGroupAt(payment, indexes[i] << 1, false);
				}
			}

			print_payments(this, payment);
		}
	}
}
