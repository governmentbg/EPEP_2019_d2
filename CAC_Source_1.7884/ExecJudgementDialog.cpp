DEFINE_RESPONSE_TABLE1(TExecJudgementDialog, TEditDialog)
//{{TExecJudgementDialogRSP_TBL_BEGIN}}
	EV_BN_CLICKED(IDPRINT, PrintBNClicked),
	EV_BN_CLICKED(IDEDIT, EditBNClicked),
//{{TExecJudgementDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TExecJudgementDialog Implementation}}

TExecJudgementDialog::TExecJudgementDialog(TWindow* parent, TJudgement *judgement, TDecision *tDecision)
:
	TEditDialog(parent, judgement, true, IDD_EXEC_JUDGEMENT), decision(tDecision),
		judged(strchr(JUDGED_RIGHT_JUDGEDS, judgement->judged) != NULL)
{
	new TCharAliasFace(this, IDC_EXEC_JUDGEMENT_UCN_TYPE, "F_UCN_TYPE", UCNType, NULL);
	new TStringFace(this, IDC_EXEC_JUDGEMENT_UCN, "F_UCN", SIZE_OF_UCN);
	forceDate = new TDateFace(this, IDC_EXEC_JUDGEMENT_FORCE_DATE, "F_FORCE_DATE", false);
	sendDate = new TDateFace(this, IDC_EXEC_JUDGEMENT_SEND_DATE, "F_SEND_DATE", false);
	sendTo = new TStringFace(this, IDC_EXEC_JUDGEMENT_SEND_TO, "F_SEND_TO", SIZE_OF_JUDGEMENT_SEND_TO);
	execDate = new TDateFace(this, IDC_EXEC_JUDGEMENT_EXEC_DATE, "F_EXEC_DATE", false);
	execBind = new TStringFace(this, IDC_EXEC_JUDGEMENT_EXEC_BIND, "F_EXEC_BIND", SIZE_OF_JUDGEMENT_EXEC_BIND);
	execPlace = new TStringFace(this, IDC_EXEC_JUDGEMENT_EXEC_PLACE, "F_EXEC_PLACE", SIZE_OF_JUDGEMENT_EXEC_PLACE);
	forgive = new TStringFace(this, IDC_EXEC_JUDGEMENT_FORGIVE, "F_FORGIVE", SIZE_OF_JUDGEMENT_FORGIVE);
	orderFrom = new TStringFace(this, IDC_EXEC_JUDGEMENT_ORDER_FROM, "F_ORDER_FROM", SIZE_OF_JUDGEMENT_ORDER_FROM);
	givenDate = new TDateFace(this, IDC_EXEC_JUDGEMENT_GIVEN_DATE, "F_GIVEN_DATE", false);
	sendText = new TStringFace(this, IDC_EXEC_JUDGEMENT_SEND_TEXT, "F_SEND_TEXT", SIZE_OF_JUDGEMENT_SEND_TEXT);
	countDate = new TDateFace(this, IDC_EXEC_JUDGEMENT_COUNT_DATE, "F_COUNT_DATE", false);
	eventNo = new TLongFace(this, IDC_EXEC_JUDGEMENT_EVENT_NO, "F_EVENT_NO", SIZE_OF_EVENT_NO);
	interval = new TCharAliasFace(this, IDC_EXEC_JUDGEMENT_INTERVAL, "F_INTERVAL", Interval, INTERVAL_DOCS);
	new TStringFace(this, IDC_EXEC_JUDGEMENT_REMARK, "F_EXEC_REMARK", SIZE_OF_JUDGEMENT_REMARK);
	notifyDate = new TDateFace(this, IDC_EXEC_JUDGEMENT_NOTIFY_DATE, "F_NOTIFY_DATE", false);
}

static constant
#if COURT_TYPE == COURT_APPEAL
	ORDER_FROM	= "АС",
	SEND_TO	= "АП",
#endif  // APPEAL
#if COURT_TYPE == COURT_ADMIN
	ORDER_FROM	= "",
	SEND_TO	= "",
#endif  // ADMIN
#if COURT_TYPE == COURT_AREAL
	ORDER_FROM	= "ОС",
	SEND_TO	= "ОП",
#endif  // AREAL
#if COURT_TYPE == COURT_MILITARY
#if SPECIAL
	ORDER_FROM	= "СНС",
	SEND_TO	= "СП",
#else  // SPECIAL
	ORDER_FROM	= "ВС",
	SEND_TO	= "ВП",
#endif  // SPECIAL
#endif  // MILITARY
#if COURT_TYPE == COURT_REGIONAL
	ORDER_FROM	= "РС",
	SEND_TO	= "РП",
#endif  // REGIONAL
	//EXEC_PLACE= "Затвора",
	SEND_TEXT	= "Преписка №  , 2 бр. присъди, мотиви, решения, споразумения, определения, свидетелства за съдимост";

void TExecJudgementDialog::SetupWindow()
{
	TEditDialog::SetupWindow();

	// 2005:154 LPR: judged checks (non-judged persons are now allowed)
	if (judged && !any(Judgement()->execText))
		Judgement()->execText.cpy(Judgement()->text);

	if (forceDate->Date().Empty())	// complect I
	{
		if (!any(orderFrom))
			orderFrom->SetS(ORDER_FROM);
	// II-pass/complect
	//}
	//else if (sendDate->Date().Empty())		// complect II
	//{
		if (judged)
		{
			if (sendDate->Date().Empty())
				sendDate->SetDate(Today);
			if (!any(sendTo))
				sendTo->SetS(SEND_TO);
			if (!any(sendText))
				sendText->SetS(SEND_TEXT);
		}

		// 2011:175
	#if COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_REGIONAL
		TBaseWind baseWind("T_SUBPOENA", NULL);
		msql m;

		baseWind << Judgement();
		// 2011:313 FIX: Float -> Exact
		baseWind.ExactCriteria(m, "F_SUBPOENA_DATE, F_LRECEIVER_UCN, F_LRECEIVER_UCN_TYPE");
		m.AddChars("F_SUBPOENA_KIND", SUBPOENA_KIND_JUDGEMENT_COPIES);

		TQuery q(str(m));
		if (q.Read())
		{
			TSubpoena subpoena;
			TSender sender;

			subpoena << q;
			sendDate->SetDate(subpoena.subpoenaDate);

			if (atob(subpoena.lReceiverUcn))
			{
				strcpy(baseWind.ucn, subpoena.lReceiverUcn);
				baseWind.ucnType = subpoena.lReceiverUcnType;
				m.clear();
				baseWind.PrintNames(m);
				m.cut(LENGTH_OF_JUDGEMENT_SEND_TO);
				// 2012:166 TRQ: FIX: cut long text
				sendTo->SetS(str(m));
			}
		}
	#endif  // AREAL || REGIONAL
		// 2017:234 IRQ
		if (decision->kind == KIND_BULWARK)
			forceDate->SetDate(decision->forceDate);
	}
	//else if (execDate->Date().Empty())		// complectIII
	//{
	//	execDate->SetDate(Today);
	//	if (execPlace->Empty())
	//		execPlace->SetS(EXEC_PLACE);
	//}

	if (judged)
		eventNo->SetL(decision->eventNo);
	// 2011:174 LRQ: removed !judged disable/clear eventNo..forgive

	// 2017:303 char[] -> mstr
	mstr title("Данни за изпълнението на ");
	size_t len = title.len();

	title.cat(Kind->Seek(kind2VisibleKind(decision->kind)));
	strlwr(ncstr(title) + len);
	SetCaption(str(title));
}

bool TExecJudgementDialog::IsValid()
{
	bool isValid = TEditDialog::IsValid();

	if (isValid)
	{
		static constant NOTIFY = "получаване на уведомление за изпълнение";
		int complectI = any(orderFrom) + !forceDate->Date().Empty() + !givenDate->Date().Empty();
		int complectII = !sendDate->Date().Empty() + (interval->C() != INTERVAL_NONE_YET) + any(sendTo) + any(sendText);
		int complectIII = !execDate->Date().Empty() + any(execBind) + !countDate->Date().Empty() + any(execPlace);

		// 2004:013 LRQ: orderFrom and forceDate (i.e. not givenDate)
		if (complectI == 2 && complectII == 0 && complectIII == 0 && givenDate->Date().Empty())
		{
			// 2008:290 LRQ: check for conditionals
			if ((!judged || strchr(PUNISHMENT_OPT_EXECS, Judgement()->punishment)) &&
				ask("Попълнени са само полетата постановяване и в законна сила. Желаете ли да продължите?"))
			{
				return true;
			}
		}

		if (complectI != 0 && complectI != 3)
			isValid = error("Полетата постановяване, в законна сила и предадена не си съответстват.");
		// II-pass/complect
		else if ((complectI == 0) != (complectII == 0))
			isValid = error("Данните за изпращане и за постановяване на присъдата не си съответстват.");
		else if (complectII != 0 && complectII != 4)
			isValid = error("Полетата изпратена, срок, орган и текст не си съответстват.");
		else if (complectII == 0 && complectIII != 0)
			isValid = error("Данните за изпълнение на присъдата не могат да се попълнят преди данните за изпращане.");
		else if (notifyDate->Date() > Today)
			isValid = error("Датата на %s е по-голяма от текущата дата.", NOTIFY);
		else if (complectII == 0 && !notifyDate->Date().Empty())
			isValid = error("Датата на %s не може да се попълни преди данните за изпращане.", NOTIFY);
		else if (notifyDate->Date().Empty() && decision->date >= P64ActDate && complectIII != 0)
			isValid = error("Данните за изпълнение на присъдата не могат да се попълнят преди датата на %s.", NOTIFY);
		else if (complectIII != 0 && complectIII != 4)
			isValid = error("Полетата приведена в изпълнение, преписка, зачита се и място на изпълнение не си съответстват.");
		else if (!forceDate->Date().Empty() && !givenDate->Date().Empty() && givenDate->Date() < forceDate->Date())
			isValid = error("Присъдата не може да се предаде за изпълнение преди да е влязла в законна сила.");
		else if (!givenDate->Date().Empty() && !sendDate->Date().Empty() && sendDate->Date() < givenDate->Date())
			isValid = error("Присъдата не може да се изпрати преди да е предадена за изпълнение.");
		else if (!sendDate->Date().Empty() && !execDate->Date().Empty() && execDate->Date() < sendDate->Date())
			isValid = error("Присъдата не може да се приведе в изпълнение преди да е изпратена.");
		else if (execDate->Date() > Today)
			isValid = error("Датата на привеждане в изпълнение е по-голяма от текущата дата.");
	}

	return isValid;
}

void TExecJudgementDialog::PrintBNClicked()
{
	if (IsValid())
	{
		*Judgement() << this;
		print_one_judgement(this, Judgement());
	}
}

void TExecJudgementDialog::EditBNClicked()
{
	TWhateverTextViewDialog(this, Judgement(), "Изпълнение на присъдата", "F_EXEC_TEXT", SIZE_OF_JUDGEMENT_TEXT,
		decision).Execute();
}
