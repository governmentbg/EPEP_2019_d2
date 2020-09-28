DEFINE_RESPONSE_TABLE1(TStandardActDialog, TJudicialActDialog)
//{{TStandardActDialogRSP_TBL_BEGIN}}
	EV_BN_CLICKED(IDC_STANDARD_ACT_MOTIVES, MotivesBNClicked),
	EV_BN_CLICKED(IDC_STANDARD_ACT_ABSENCE, AbsenceBNClicked),
	EV_BN_CLICKED(IDC_STANDARD_ACT_CONFESS, ConfessBNClicked),
	EV_BN_CLICKED(IDC_STANDARD_ACT_OPINION, OpinionBNClicked),
	EV_BN_CLICKED(IDC_STANDARD_ACT_RETURNED_TYPE, ReturnedTypeBNClicked),
//{{TStandardActDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TStandardActDialog Implementation}}

TStandardActDialog::TStandardActDialog(TWindow* parent, TDecision *decision, TLawsuit *lawsuit, const char *kinds, bool grant,
	bool flags, TSession *session)
:
	TJudicialActDialog(parent, decision, lawsuit, kinds, grant, session, IDD_STANDARD_ACT)
{
	CreateAgeFinished();
	motivesLabel = new TStatic(this, IDC_STANDARD_ACT_MOTIVES_LABEL);
	returnedType = new TButton(this, IDC_STANDARD_ACT_RETURNED_TYPE);

	// 2008:028
	if (flags)
	{
		confess = new TCheckFace(this, IDC_STANDARD_ACT_CONFESS, "F_FLAGS_CONFESS");
		absence = new TCheckFace(this, IDC_STANDARD_ACT_ABSENCE, "F_FLAGS_ABSENCE");
	}
	else
	{
		confess = NULL;
		absence = NULL;
	}

	motives = new TButton(this, IDC_STANDARD_ACT_MOTIVES);
	opinion = new TButton(this, IDC_STANDARD_ACT_OPINION);
	keyWords = new TKeyWordsFace(this, IDC_STANDARD_ACT_KEYWORDS, "F_KEYWORDS");

#if COURT_TYPE == COURT_APPEAL
	if (strchr(KIND_INCONSISTENCE_REQUESTS, lawsuit->sourceKind))
		publTreg = new TCheckFace(this, IDC_TROUBLE_PUBLTREG, "F_FLAGS_PUBLTREG");
#endif  // APPEAL
}

void TStandardActDialog::KindChanged()
{
	TJudicialActDialog::KindChanged();

	// 2008:072 LRQ: FIX: only handle existing confess/absence
	if (strchr(KIND_DECISIONS, Decision()->kind))
	{
		if (confess)
		{
			absence->SetCheck(Decision()->flags & DECISION_ABSENCE ? BF_CHECKED : BF_UNCHECKED);
			absence->EnableWindow(true);
			ConfessBNClicked();
			confess->SetCheck(Decision()->flags & DECISION_CONFESS ? BF_CHECKED : BF_UNCHECKED);
			confess->EnableWindow(true);
			AbsenceBNClicked();
		}
	}
	else
	{
		if (confess)
		{
			absence->EnableWindow(false);
			absence->SetCheck(BF_UNCHECKED);
			confess->EnableWindow(false);
			confess->SetCheck(BF_UNCHECKED);
		}
	}
}

void TStandardActDialog::MotivesBNClicked()
{
	TEditMotivesDialog(this, Decision(), edit, KIND_MOTIVES, "Мотиви", "F_MOTIVES").Execute();
}

void TStandardActDialog::ConfessBNClicked()
{
	if (confess->GetCheck() == BF_CHECKED)
	{
		absence->EnableWindow(false);
		absence->SetCheck(BF_UNCHECKED);
	}
	else
	{
		absence->EnableWindow(true);
		if (Decision()->flags & DECISION_ABSENCE)
			absence->SetCheck(BF_CHECKED);
	}
}

void TStandardActDialog::AbsenceBNClicked()
{
	if (absence->GetCheck() == BF_CHECKED)
	{
		confess->EnableWindow(false);
		confess->SetCheck(BF_UNCHECKED);
	}
	else
	{
		confess->EnableWindow(true);
		if (Decision()->flags & DECISION_CONFESS)
			confess->SetCheck(BF_CHECKED);
	}
}

void TStandardActDialog::SetupWindow()
{
	TJudicialActDialog::SetupWindow();

	constant jurors[JUROR_COUNT] = { session->juror, session->reJuror, session->jurorX, session->reserve };

	if ((atob(session->judgeX) && strcmp(session->judgeX, session->judge)) ||
		(atob(session->reJudgeX) && strcmp(session->reJudgeX, session->judge)) ||
		(atob(session->president) && strcmp(session->president, session->judge)) ||
		atobc(jurors, JUROR_COUNT))
	{
		opinion->EnableWindow(true);
	}
}

void TStandardActDialog::OpinionBNClicked()
{
	TEditMotivesDialog(this, Decision(), edit, KIND_OPINION, "Особено мнение", "F_OPINION").Execute();
}

void TStandardActDialog::SetMotiving(bool tMotiving)
{
	TJudicialActDialog::SetMotiving(tMotiving);

	if (motiving)
	{
		returnedLabel->ShowWindow(SW_HIDE);
		if (edit)
			motivesLabel->ShowWindow(SW_SHOW);
		else
		{
			returnedType->SetCaption("Дата на предаване на &мотивите");
			returnedType->ShowWindow(SW_SHOW);
			returnedType->EnableWindow(true);
		}
		print->ShowWindow(SW_HIDE);
		print->EnableWindow(false);
		motives->ShowWindow(SW_SHOW);
		motives->EnableWindow(true);
	}
	else
	{
		if (edit || !Decision()->Motivable())
		{
			returnedType->ShowWindow(SW_HIDE);
			returnedType->EnableWindow(false);
			returnedLabel->ShowWindow(SW_SHOW);
		}
		else
		{
			returnedLabel->ShowWindow(SW_HIDE);
			returnedType->SetCaption("Дата на &връщане");
			returnedType->ShowWindow(SW_SHOW);
			returnedType->EnableWindow(true);
		}
		print->ShowWindow(SW_SHOW);
		print->EnableWindow(true);
		motives->ShowWindow(SW_HIDE);
		motives->EnableWindow(false);
	}
}

void TStandardActDialog::ReturnedTypeBNClicked()
{
	SetMotiving(!motiving);
}
