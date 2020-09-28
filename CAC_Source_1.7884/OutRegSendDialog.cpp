DEFINE_RESPONSE_TABLE1(TOutRegSendDialog, TOutRegSendBaseDialog)
//{{TOutRegSendDialogRSP_TBL_BEGIN}}
	EV_BN_CLICKED(IDENTER, EnterBNClicked),
	EV_BN_CLICKED(IDPRINT, PrintBNClicked),
	EV_CBN_SELCHANGE(IDC_OUTREG_SEND_KIND, KindCBNSelChange),
//{{TOutRegSendDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TOutRegSendDialog Implementation}}

TOutRegSendDialog::TOutRegSendDialog(TWindow* parent, TOutReg *outReg, TReceiver *tReceiver, const char *kinds,
	TInReg *inReg, TRCDKeyContainer *tEvent, TConnect **connects)
:
	TOutRegSendBaseDialog(parent, outReg, inReg, kinds, connects, false, IDD_OUTREG_SEND), event(tEvent)
{
	strcpy(types, Default->colleges);
	strcach(types, TYPE_REQUEST);
	lawsuitType = new TCharAliasFace(this, IDC_OUTREG_SEND_LAWSUIT_TYPE, "F_LAWSUIT_TYPE", Type, types, ALIAS_EMPTY);

	outReg->receiverType = tReceiver->ucnType;

	if (outReg->receiverType == UCN_INSTANCE_RESERVED)
		receiver = NULL;
	else
	{
		const char *ucnTypes = C2S[tReceiver->ucnType];

		if (strchr(UCN_INSTANCE_JURICONFS, tReceiver->ucnType))
		{
			receiver = new TReceiverAliasFace(this, IDC_OUTREG_SEND_RECEIVER, "F_RECEIVER", tReceiver, ucnTypes);
			fixeiver = new TComboBox(this, IDC_OUTREG_SEND_FIXEIVER);
		}
		else if (tReceiver->ucnType == UCN_INSTANCE_LOWER || strchr(UCN_EXECUTORS, tReceiver->ucnType))
			receiver = new TStringAliasFace(this, IDC_OUTREG_SEND_FIXEIVER, "F_RECEIVER", tReceiver, ucnTypes);
		else
			receiver = new TSenderAliasFace(this, IDC_OUTREG_SEND_FIXEIVER, "F_RECEIVER", tReceiver->ucnType);
	}

	college = new TCharAliasFace(this, IDC_OUTREG_SEND_COLLEGE, "F_COLLEGE", College, Default->xColleges, ALIAS_EMPTY);
	new TPushButton(this, IDCANCEL, IDI_CANCEL);
}

void TOutRegSendDialog::SetupWindow()
{
	TOutRegSendBaseDialog::SetupWindow();

	if (OutReg()->college)
		college->EnableWindow(false);

	// N.B. not automatic since receiver is not mandatory; 2013:039 +PROSEC
	if (!outreg_subpoena_kind(OutReg()))
		print->EnableWindow(false);
	else if (OutReg()->receiverType == UCN_INSTANCE_LOWER)
	{
		vssIndexLabel->ShowWindow(SW_SHOW);
		vssIndex->ShowWindow(SW_SHOW);
		receiver->EnableWindow(receiver->GetCount() >= 2);	// 2016:111
	}
	else if (receiver)
	{
		// 2011:150 LPR: full juriconf support
		if (strchr(UCN_INSTANCE_JURICONFS, OutReg()->receiverType))
		{
			fixeiver->ShowWindow(SW_HIDE);
			receiver->ShowWindow(SW_SHOW);
			receiver->EnableWindow(true);
		}
		else
			receiver->EnableWindow(receiver->GetCount() >= 2);

		// 2009:210
		if (OutReg()->receiverType == UCN_INSTANCE_CONFLICT)
		{
			TSender sender;

			sender.ucnType = UCN_INSTANCE_HIGHER;
		#if COURT_TYPE == COURT_APPEAL
			strcpy(sender.ucn, INSTANCE_SUPER);
		#endif  // APPEAL
		#if COURT_TYPE == COURT_ADMIN
			strcpy(sender.ucn, INSTANCE_SPEAL);
		#endif  // APPEAL
		#if COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
			strcpy(sender.ucn, INSTANCE_III);
		#endif  // AREAL || MILITARY || REGIONAL
			if (sender.Get())
			{
				char s[SIZE_OF_UCN];
				sprintf(s, "%010d", sender.court);
				receiver->SetS(s);
			}
		}
	}

	if (any(PrimaryTitle))
		SetCaption(PrimaryTitle);

	if (receiver)
		receiver->ShowWindow(SW_SHOW);

	// 2016:083
	if (inReg)
	{
		if (inReg->kind == KIND_INREG_SLOWDOWN)
			kind->SetC(KIND_APPEAL_LETTER);

		char tReceiver[SIZE_OF_UCN];

		if (inReg->LastReceiver(tReceiver, NULL))
			receiver->SetS(tReceiver);
		else if (any(inReg->receiver))
		{
			receiver->SetS(inReg->receiver);
			receiver->EnableWindow(false);
		}
	}

	KindCBNSelChange();
}

void TOutRegSendDialog::EnterBNClicked()
{
	// 2007:240 LRQ: check college against lawsuit type
	if (IsValid())
	{
		*OutReg() << this;

		if (OutReg()->Insert())
		{
			trouble.Move(OutReg());
			CmOk();
		}
	}
}

bool TOutRegSendDialog::IsValid()
{
	bool isValid = TOutRegSendBaseDialog::IsValid();

	if (isValid)
	{
		TLawsuit lawsuit;

		lawsuit.key.no = lawsuitNo->L();
		lawsuit.key.year = lawsuitYear->L();
		lawsuit.key.type = lawsuitType->C();

		if (kind->C() == KIND_APPEAL_LETTER && lawsuitType->C() == TYPE_PUNISHMENT_LAW)
		{
			isValid = error("Не може да се изпраща %s за %s.", Kind->Seek(KIND_APPEAL_LETTER),
				Type->Seek(TYPE_PUNISHMENT_LAW));
		}
		else if (date->Date() > Today)
			isValid = error("Датата на изпращане е по-голяма от текущата дата.");
		else if (kind->C() == KIND_PROPOSITION_LETTER && !lawsuit.Get("F_FINISHED"))
			isValid = false;
		else if (kind->C() == KIND_PROPOSITION_LETTER && lawsuit.finished.Empty())
		{
			isValid = error("Не може да се изпраща %s за дело, което не е свършило.",
				Kind->Seek(KIND_PROPOSITION_LETTER));
		}
		else if (inReg)
		{
			TInRegEvent *inRegEvent = inReg->LastEvent("F_DATE");	// 2005:052

			if (inRegEvent && date->Date() < inRegEvent->date)
			{
				isValid = error("Датата на изпращане е по-малка от датата на последното събитие по документа "
					"от входящ регистър.");
			}

			delete inRegEvent;
		}
		else if (OutReg()->receiverType == UCN_INSTANCE_LOWER && connects[1])
		{
			TConnect *connect = get_connect(connects, receiver->S());
			const char *failure;

			if (connect->connectType == CONNECT_TYPE_LOWER_INSTANCE)
				failure = connect->Returnable(RETURNABLE_FULL);
			else
			{
				failure = connect->Returnable(RETURNABLE_EISPP);

				if (!failure)
				{
					const char *s = connect->Returnable(RETURNABLE_LOWER);
					isValid = !s || ask("%s. Желаете ли да продължите?", s);
				}
			}

			if (failure)
				isValid = error("%s.", failure);
		}
	}

	return isValid;
}

void TOutRegSendDialog::PrintBNClicked()
{
	if (IsValid())
	{
		TOutReg *outReg = OutReg();

		*outReg << this;
		ShowWindow(SW_HIDE);

		if (outReg->receiverType == UCN_INSTANCE_HIGHER)
			send_nosave(this, outReg, inReg, event);
		else
		{
			TConnect *lower = outReg->receiverType == UCN_INSTANCE_LOWER ? get_connect(connects, receiver->S()) : NULL;
			print_outreg(this, outReg, inReg, event, false, NULL, lower);
		}

		// 2007:323 LRQ: do not close on sending to lower court
		// 2009:170 LPR/LRQ: removed, print after OK is available
		CloseDialog(IDPRINT);
	}
}

void TOutRegSendDialog::KindCBNSelChange()
{
	text->LimitText(kind->C() == KIND_OTHER_OUT_DOCUMENT ? SIZE_OF_ORIGIN_TEXT : SIZE_OF_OUTREG_REMARK);
}
