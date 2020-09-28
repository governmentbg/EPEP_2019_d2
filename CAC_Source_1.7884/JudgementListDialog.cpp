DEFINE_RESPONSE_TABLE1(TJudgementListDialog, TSideWindListDialog)
//{{TJudgementListDialogRSP_TBL_BEGIN}}
	EV_BN_CLICKED(IDCHILD, ChildBNClicked),
	EV_BN_CLICKED(IDENTER, EnterBNClicked),
	EV_BN_CLICKED(IDEXEC, ExecBNClicked),
	EV_BN_CLICKED(IDPRINT, PrintBNClicked),
	EV_BN_CLICKED(IDLIST, ListBNClicked),
	EV_BN_CLICKED(IDRIGHT, RightBNClicked),
	EV_BN_CLICKED(IDEXTRADATA, ExtraDataBNClicked),
	EV_BN_CLICKED(IDMONEY, MoneyBNClicked),
	EV_BN_CLICKED(IDCANCEL, CancelBNClicked),
	EV_BN_CLICKED(IDADD, AddBNClicked),
	EV_BN_CLICKED(IDEDIT, EditBNClicked),
	EV_BN_CLICKED(IDDELETE, DeleteBNClicked),
	EV_COMMAND_ENABLE(IDCHILD, ChildBNEnable),
	EV_COMMAND_ENABLE(IDENTER, SelectEnable),
	EV_COMMAND_ENABLE(IDEXEC, SelectEnable),
	EV_COMMAND_ENABLE(IDPRINT, SelectEnable),
	EV_COMMAND_ENABLE(IDEXTRADATA, SelectEnable),
	EV_COMMAND_ENABLE(IDMONEY, SelectEnable),
//{{TJudgementListDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TJudgementListDialog Implementation}}

static TSideWindListWindow *createJudgements(TWindow* parent, long sideMode, TRCDKeyContainer *container, TLawsuit *lawsuit)
{
	return new TJudgementListWindow(parent, sideMode, IDC_GROUP_LIST_WINDOW, (TDecision *) container, lawsuit);
}

TJudgementListDialog::TJudgementListDialog(TWindow* parent, TDecision *decision, TSession *tSession, TLawsuit *lawsuit,
	long sideMode)
:
	TSideWindListDialog(parent, decision, lawsuit, sideMode, "акта", IDD_JUDGEMENT_LIST, createJudgements),
		session(tSession), defaultPublic(BF_CHECKED)
{
	alreadyPublic = edit && (decision->flags & DECISION_PUBLIC);
	extraData = lawsuit->sourceKind == KIND_NULL ? NULL : new TButton(this, IDEXTRADATA);
	// 2003:333; 2010:082 from TJudgementDialog
	//delete sideWinds;
	// Note: remove == true only allows side wind list to delete all sides
	//sideWinds = new TJudgementListWindow(this, sideMode, IDC_GROUP_LIST_WINDOW, decision, lawsuit);
	new TTwinButton(this, IDLIST, IDRIGHT);
	child = new TPushButton(this, IDCHILD);
	noPublInet = new TStatic(this, IDC_JUDGEMENT_LIST_NO_PUBLINET);
	publInet = new TCheckFace(this, IDC_JUDGEMENT_LIST_PUBLINET, "F_FLAGS_PUBLIC");
	p64Appealed = decision->P64Direct(false) && sideMode == SIDE_MODE_EDIT && decision->Appealed(); 
}

void TJudgementListDialog::Header2(mstr &m)
{
	m.printf("%s от %Dг.", Kind->Seek(session->kind), &session->date);
	session->time.Print(m, ", %T часа");

	if (Decision()->eventNo)
	{
		// 2007:213 LRQ: show eventNo only if config
		m.printf(", %s ", Kind->Seek(Decision()->IndexKind()));
		if (PRINT_JUDGEMENTS)
			m.printf("%ld/", Decision()->eventNo);
		m.printf("%ldг.", Decision()->date.year);
	}
}

void TJudgementListDialog::Accept()
{
	if (Decision()->P64Direct(false))
	{
		if (publInet->GetCheck() == BF_CHECKED)
			Decision()->flags |= DECISION_PUBLIC;
		else
			Decision()->flags &= ~DECISION_PUBLIC;
	}
}

void TJudgementListDialog::UpdatePublInet()
{
	if (!Decision()->P64Direct(false))
		return;

	constant prefix = Decision()->flags & DECISION_PUBLIC ? "Данните за публикуване ще бъдат изтрити - " :
		"Не подлежи на публикуване в интернет - ";
	mstr reason;

	if (Lawsuit()->LimitedNotAdmin())
		reason.cat("Делото е с ограничен достъп");
	else if (!p64Appealed)
		Judgements()->P64Check(reason);

	if (any(reason))
	{
		publInet->ShowWindow(SW_HIDE);
		publInet->SetCheck(BF_UNCHECKED);
		mbk_insert(&reason, prefix, 0, strlen(prefix));
		noPublInet->SetCaption(str(reason));
		noPublInet->ShowWindow(SW_SHOW);
	}
	else
	{
		// 2017:313 IRQ: display return indexes
		if (!any(publINetText))
		{
			mstr m;
			Decision()->OutRegIndexes(m);

			publINetText.cat("Подлежи на публикуване в интернет /ППИ/");

			if (any(m))
			{
				//publINetText.printf("\nОбжалван%c, индекс%s %s", "оа"[Decision()->kind == KIND_JUDGEMENT],
				//	"и" + (m.index(",") == INT_MAX), str(m));
				publINetText.printf("\nРезултат%s от обжалване: %s", "и" + (m.index(",") == INT_MAX), str(m));
			}
			else if (p64Appealed)
				publINetText.cat(" (обжалван)");

			publInet->SetCaption(str(publINetText));
		}

		noPublInet->ShowWindow(SW_HIDE);
		publInet->SetCheck(!(Lawsuit()->flags & LAWSUIT_LIMITED) || alreadyPublic ? defaultPublic : BF_UNCHECKED);
		publInet->ShowWindow(SW_SHOW);
	}
}


void TJudgementListDialog::SetupWindow()
{
	TSideWindListDialog::SetupWindow();

	if (extraData)
		extraData->EnableWindow(true);

	// 2017:303 char[] -> mstr
	mstr title("Вписване на ");
	mstr label("&Списък на ");

	switch (kind2VisibleKind(Decision()->kind))
	{
		case KIND_DECISION :
		{
			title.cat("решения");
			label.cat("решенията");
			break;
		}
	#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
		case KIND_JUDGEMENT :
		{
			title.cat("присъди");
			label.cat("присъдите");
			break;
		}
		case KIND_BULWARK :
		{
			title.cat("споразумения");
			label.cat("споразуменията");
			break;
		}
	#endif  // APPEAL || AREAL || MILITARY || REGIONAL
		case KIND_SETTLEMENT :
		{
			title.cat("определения");
			label.cat("определенията");
			break;
		}
		case KIND_ORDER :
		{
			title.cat("разпореждания");
			label.cat("разпорежданията");
			break;
		}
		default : error("%d: kind not supported by JudgementListDialog", Decision()->kind);
	}

	SetCaption(str(title));
	sideLabel->SetCaption(str(label));
	// 2017:311
	UpdatePublInet();
	// 2017:328 URQ/LPR: annoying re-offer public
	if (edit && publInet->GetCheck() == BF_CHECKED && !alreadyPublic)
	{
		publInet->SetCheck(BF_UNCHECKED);
		defaultPublic = BF_UNCHECKED;
	}
}

void TJudgementListDialog::EnterBNClicked()
{
	Judgements()->Enter();
	UpdatePublInet();
}

void TJudgementListDialog::ExecBNClicked()
{
	Judgements()->Exec();
	UpdatePublInet();
}

void TJudgementListDialog::PrintBNClicked()
{
	Judgements()->Messages();
}

void TJudgementListDialog::ListBNClicked()
{
	sideWinds->Roll();
}

void TJudgementListDialog::RightBNClicked()
{
	sideWinds->List(true);
}

void TJudgementListDialog::ExtraDataBNClicked()
{
	Judgements()->Extra();
}

void TJudgementListDialog::MoneyBNClicked()
{
	sideWinds->Expend(NULL, NULL);
}

void TJudgementListDialog::CancelBNClicked()
{
	EvClose();
}

void TJudgementListDialog::AddBNClicked()
{
	TSideWindListDialog::AddBNClicked();
	UpdatePublInet();
}

void TJudgementListDialog::EditBNClicked()
{
	TSideWindListDialog::EditBNClicked();
	UpdatePublInet();
}

void TJudgementListDialog::DeleteBNClicked()
{
	TSideWindListDialog::DeleteBNClicked();
	UpdatePublInet();
}
