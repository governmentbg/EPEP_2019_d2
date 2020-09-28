DEFINE_RESPONSE_TABLE1(TLocalJudgementDialog, TGroupDialog)
//{{TLocalJudgementDialogRSP_TBL_BEGIN}}
	EV_BN_CLICKED(IDOK, OKBNClicked),
	EV_BN_CLICKED(IDVIEW, ViewBNClicked),
	EV_CBN_SELCHANGE(IDC_LOCAL_JUDGEMENT_JUDGED, JudgedCBNSelChange),
	EV_BN_CLICKED(IDEXTRADATA, ExtraDataBNClicked),
//{{TLocalJudgementDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TLocalJudgementDialog Implementation}}

TLocalJudgementDialog::TLocalJudgementDialog(TWindow* parent, TJudgement *judgement, TDecision *tDecision,
	TLawsuit *tLawsuit, bool edit)
:
	TGroupDialog(parent, judgement, IDD_LOCAL_JUDGEMENT), decision(tDecision), lawsuit(tLawsuit)
{
#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_ADMIN
	(void) edit;
#endif  // APPEAL || ADMIN
	const char *tPunishments = decision->Punishments(lawsuit);

	limited = !strcmp(tPunishments, PUNISHMENT_NOT_KNOWNS);
	header = new TStatic(this, IDC_LOCAL_JUDGEMENT_HEADER);
	judged = new TCharAliasFace(this, IDC_LOCAL_JUDGEMENT_JUDGED, "F_JUDGED", Judged,
		limited ? C2S[JUDGED_NOT_KNOWN] : NULL);
	punishment = new TCharAliasFace(this, IDC_LOCAL_JUDGEMENT_PUNISHMENT, "F_PUNISHMENT", Punishment, tPunishments);
	recidive = new TCharAliasFace(this, IDC_LOCAL_JUDGEMENT_RECIDIVE, "F_RECIDIVE", Recidive, NULL);
	text = new TStringFace(this, IDC_LOCAL_JUDGEMENT_TEXT, "F_TEXT", SIZE_OF_JUDGEMENT_TEXT);
	flagsNew = new TCheckFace(this, IDC_LOCAL_JUDGEMENT_FLAGS_NEW, "F_FLAGS_NEW");
	flagsPriority = new TCheckFace(this, IDC_LOCAL_JUDGEMENT_FLAGS_PRIORITY, "F_FLAGS_PRIORITY");
	flagsAppendix23 = new TCheckFace(this, IDC_LOCAL_JUDGEMENT_FLAGS_23, "F_FLAGS_APPENDIX_23");
	flagsAppendix24 = new TCheckFace(this, IDC_LOCAL_JUDGEMENT_FLAGS_24, "F_FLAGS_APPENDIX_24");
	flagsAppendix25 = new TCheckFace(this, IDC_LOCAL_JUDGEMENT_FLAGS_25, "F_FLAGS_APPENDIX_25");
	flagsAppendix26 = new TCheckFace(this, IDC_LOCAL_JUDGEMENT_FLAGS_26, "F_FLAGS_APPENDIX_26");
	punishmentsExtra = new TCharListFace(this, IDC_LOCAL_JUDGEMENT_PUNISHMENT_EXTRA, "F_PUNISHMENTS_X", Punishment,
		decision->ExtraPunishments(lawsuit), SIZE_OF_PUNISHMENTS);
	jailMode = new TCharAliasFace(this, IDC_LOCAL_JUDGEMENT_JAIL_MODE, "F_JAIL_MODE", JailMode, NULL);
	// 2007:284; 2009:315 LPR: if (CourtType -> #if COURT_TYPE
#if COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
	articleLabel = new TStatic(this, IDC_LOCAL_JUDGEMENT_ARTICLE_LABEL);
	article = new TArticleAliasFace(this, IDC_LOCAL_JUDGEMENT_ARTICLE, "F_ARTICLE", lawsuit->subject,
		edit ? ALIAS_EMPTY : FLAG_ACTIVE | ALIAS_EMPTY);
#endif  // AREAL || MILITARY || REGIONAL
#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_ADMIN
	articleLabel = NULL;
	article = NULL;
#endif  // APPEAL || ADMIN
}

//bool TLocalJudgementDialog::Limited()
//{
//#if COURT_TYPE == COURT_AREAL
//	if (lawsuit->kind == KIND_PRIVATE_PUNISHMENT_LAW && strchr(KIND_DECISIONS, decision->kind))
//		return true;
//#endif  // AREAL
//	return decision->kind == KIND_SETTLEMENT || decision->kind == KIND_ORDER;
//}

void TLocalJudgementDialog::OKBNClicked()
{
	if (IsValid())
	{
		*Judgement() << this;
		// 2001:194
		if (*punishmentsExtra->S() != '\0')
			Judgement()->flagsX |= JUDGEMENT_X_ADDITIONAL;

		if (Judgement()->Update())
			CmOk();
	}
}

void TLocalJudgementDialog::SetupWindow()
{
	TGroupDialog::SetupWindow();

	JudgedCBNSelChange();

#if INSTANCE  // APPEAL & AREA
	// There are no flags
#else  // REGION
	flagsNew->EnableWindow(true);
#endif  // INSTANCE

	TCitizen citizen;

	citizen.ucnType = Judgement()->ucnType;
	strcpy(citizen.ucn, Judgement()->ucn);
	citizen.Try();
	citizen >> this;

	if (limited)
	{
		punishmentsExtra->EnableWindow(false);
		jailMode->EnableWindow(false);
		flagsAppendix23->EnableWindow(false);	// 2011:322
		flagsAppendix24->EnableWindow(false);
		flagsAppendix25->EnableWindow(false);
		flagsAppendix26->EnableWindow(false);
	}
	else	// 2011:322 not if the punishment fields are locked
		adjust_judgement_priority(Judgement(), flagsPriority);

	// 2003:331 BRW: unify/remove TPersonal*Dialog()
	char title[0x100];

	GetWindowText(title, sizeof title);
	strcat(title, Kind->Seek(kind2VisibleKind(decision->kind)));
	strlwr(title + 1);
	SetCaption(title);

	// 2005:119 LRQ: using header
	mstr m;

	Judgement()->Print(m);
	header->SetCaption(str(m) + SIZE_OF_BASE_WIND_STAT + SIZE_OF_JUDGEMENT_STAT);

	if (article)
	{
		articleLabel->ShowWindow(SW_SHOW);
		article->ShowWindow(SW_SHOW);
	}
}

bool TLocalJudgementDialog::IsValid()
{
	bool isValid = TGroupDialog::IsValid();

	if (isValid)
	{
		bool convicted = strchr(PUNISHMENT_CONVICTIONS, punishment->C());

		if ((strchr(JUDGED_RIGHT_JUDGEDS, judged->C()) != NULL) != convicted)
			isValid = error("Полетата резултат от съдебното производство и наложено наказание общо не си съответстват.");
		if (article && article->L() && !convicted)
			isValid = error("Полетата наложено наказание общо и основание за налагане не си съответстват.");
		if (!isValid)
			isValid = ask("Не са попълнени коректно всички данни за лицето. Желаете ли да продължите?");
	}

	return isValid;
}

void TLocalJudgementDialog::JudgedCBNSelChange()
{
	if (strchr(JUDGED_RIGHT_JUDGEDS, judged->C()))
	{
		punishment->SetC(Judgement()->punishment);
		punishment->EnableWindow(true);
		recidive->SetC(Judgement()->recidive);
		recidive->EnableWindow(true);
	}
	else
	{
		punishment->SetC(PUNISHMENT_NOT_KNOWN);
		punishment->EnableWindow(false);
		recidive->SetC(RECIDIVE_NOT_KNOWN);
		recidive->EnableWindow(false);
	}
}

void TLocalJudgementDialog::ViewBNClicked()
{
	electric_action(decision, TYPE_TEXT, ACTION_VIEW);
}


void TLocalJudgementDialog::ExtraDataBNClicked()
{
	punish_side(this, decision, Judgement(), lawsuit);
}
