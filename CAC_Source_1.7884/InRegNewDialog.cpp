DEFINE_RESPONSE_TABLE1(TInRegNewDialog, TInRegNewBaseDialog)
//{{TInRegNewDialogRSP_TBL_BEGIN}}
	EV_BN_CLICKED(IDOK, OKBNClicked),
	EV_CBN_SELCHANGE(IDC_EDIT_INCOMING_KIND, KindCBNSelChange),
	EV_CBN_SELCHANGE(IDC_INREG_NEW_LAWSUIT_TYPE, LawsuitTypeCBNSelChange),
	EV_CBN_SELCHANGE(IDC_INREG_NEW_SESSION_KIND, SessionKindCBNSelChange),
	EV_CBN_SELCHANGE(IDC_INREG_NEW_DECISION_KIND, DecisionKindCBNSelChange),
	EV_COMMAND_ENABLE(IDSEARCH, SearchBNEnable),
	EV_BN_CLICKED(IDSEARCH, SearchBNClicked),
//{{TInRegNewDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TInRegNewDialog Implementation}}

TInRegNewDialog::TInRegNewDialog(TWindow* parent, TInReg *inReg, const char *kinds, const TLawsuit *lawsuit)
:
	TInRegNewBaseDialog(parent, inReg, kinds, KIND_APPEALABLE_SURROUNDS, KIND_APPEALABLE_SESSIONS, lawsuit)
{
	search = new TPushButton(this, IDSEARCH);
}

void TInRegNewDialog::SetupWindow()
{
	TInRegNewBaseDialog::SetupWindow();

	KindCBNSelChange();
	sessionDate->SetS("");
	SessionKindCBNSelChange();
	search->ShowWindow(SW_SHOW);
}

// 2004:047 LRQ: hijacking...
static bool allowinc(char kind, const TLawsuit *lawsuit)
{
#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_ADMIN || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
	(void) kind;
	(void) lawsuit;
	return false;
#endif  // APPEAL || ADMIN || MILITARY || REGIONAL
#if COURT_TYPE == COURT_AREAL
	return strchr(KIND_INCONSISTENCE_INREGS, kind) && strchr(KIND_INCONSISTENCE_INDOCS, lawsuit->kind);
#endif  // AREAL
}

bool TInRegNewDialog::IsValid()
{
	bool isValid = TInRegNewBaseDialog::IsValid();

	if (isValid)
	{
		char tDecisionKind = decisionKind->C();
		// TODO: tSessionKind
		int tDecisionCount =
			(lawsuit->key.type != '\0') +
			(lawsuit->key.no != 0) +
			(lawsuit->key.year != 0) +
			(sessionKind->C() != '\0') +
			(!sessionDate->Date().Empty()) +
			(tDecisionKind != '\0');

		int tSurroundCount = (surroundKind->C() != '\0') + (surroundNo->L() != 0) + (surroundYear->L() != 0);
		TSession session;
		TDecision decision;
		TSurroundment surroundment;
		sign iRead;
		mstr decisionKinds;
		char tKind = kind->C();

		if (tDecisionCount != 0 && tDecisionCount != 3 && tDecisionCount != 6)
		{
			isValid = error("Полетата вид дело, номер, година, вид заседание, дата, обжалван акт и номер не си "
				"съответстват.");
		}
		else if (tDecisionCount == 6 &&
			(
				session.key = lawsuit->key,
				// 2007:338 FIX: adjust session kind
				session.kind = adjust_xxx_kind(sessionKind->C(), sessionIndex, KIND_CLOSED_SESSIONS),
				session.date = sessionDate->Date(),
				!session.Try()
			)
		)
		{
			isValid = say_no_data(session.Name);
		}
		// 2005:076 LRQ: reject fixed/unfixed/aborted/... sessions
		else if (tDecisionCount == 6 && strchr(RESULT_UNCARRIEDS, session.Status()))
			isValid = error("Заседанието е %s и не подлежи на обжалване.", Result->Seek(session.Status()));
		// 2004:047 LRQ: allowinc() any/any bloat
		else if (tDecisionCount == 6 &&
			strchr(RESULT_ENTERABLES, session.Status()) != NULL &&
			!strchr(session.DecisionKinds(decisionKinds, lawsuit), tDecisionKind) &&
			!allowinc(tKind, lawsuit) &&
		#if COURT_TYPE == COURT_REGIONAL
			// 2013:008 IRQ
			!(tKind == KIND_PRIVATE_CALL_APPEAL && strchr(RESULT_DIV_PSEUDOSETS, session.result) &&
				tDecisionKind == KIND_SETTLEMENT) &&
		#endif  // REGIONAL
			// 2006:202 LRQ
			!(strchr(session.DecisionKinds(decisionKinds, lawsuit), KIND_BULWARK) && tDecisionKind == KIND_SETTLEMENT)
		)
		{
			isValid = error("Полетата вид заседание и вид документ не си съответстват.");
		}
		// 2004:047 LPR: allowinc() any/any bloat
		else if (tDecisionCount == 6 &&
			!strchr(RESULT_ENTERABLES, session.Status()) &&
			!strchr(KIND_IN_UNDECIDABLES, tDecisionKind) &&
			!allowinc(tKind, lawsuit)
		)
		{
			isValid = error("Полетата вид заседание и вид документ не си съответстват.");
		}
		// 2004:047 LRQ: allowinc() any/any bloat
		else if (tDecisionCount == 6 &&
			strchr(KIND_PRIVATE_IN_REG_DOCS, tKind) &&
			session.Status() == RESULT_PUBLIC_DECISION &&
			tDecisionKind != KIND_SETTLEMENT &&
		#if COURT_TYPE == COURT_APPEAL
			tKind != KIND_PRIVATE_CONNECT_APPEAL &&
		#endif  // APPEAL
			!allowinc(tKind, lawsuit)
		)
		{
			isValid = error("Полетата вид заседание и вид документ не си съответстват.");
		}
		// 2004:047 LRQ: allowinc() any/any bloat
		else if (tDecisionCount == 6 &&
			strchr(RESULT_ENTERABLES, session.Status()) &&
			(!strchr(KIND_PRIVATE_IN_REG_DOCS, tKind) || session.Status() != RESULT_PUBLIC_DECISION) &&
			!allowinc(tKind, lawsuit) &&
		#if COURT_TYPE == COURT_REGIONAL
			// 2013:008 IRQ
			!(tKind == KIND_PRIVATE_CALL_APPEAL && strchr(RESULT_DIV_PSEUDOSETS, session.result) &&
				tDecisionKind == KIND_SETTLEMENT) &&
		#endif  // REGIONAL
			// 2006:202 LRQ
			!(strchr(session.DecisionKinds(decisionKinds, lawsuit), KIND_BULWARK) &&
				tDecisionKind == KIND_SETTLEMENT) &&
			// 2013:303 LRQ
			!(strchr(session.DecisionKinds(decisionKinds, lawsuit), KIND_SETTLEMENT) &&
				tDecisionKind == KIND_SETTLEMENT) &&
			(
				decision.key = session.key,
				decision.kind = tDecisionKind,
				decision.date = session.date,
				// 2005:042 LPR: // !decision.Try()
				// 2005:056 LPR: // !decision.IndexRead(decisionIndex, false)
				decision.eventNo = decisionNo->L(),
				(iRead = decision.IndexRead(false)) < true
			)
		)
		{
			if (!iRead)
				say_no_data(decision.Name);
			isValid = false;
		}
		// 2004:047 LRQ: allowinc() any/any bloat
		// ----- TYPE_ ---------------------------------------------------
		else if (tDecisionCount == 6 && strchr(KIND_OBJECTION_IN_DOCS, tKind) != NULL &&
			lawsuit->key.type != TYPE_PUNISHMENT_LAW &&
		#if COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
			!(tKind == KIND_PROTEST && lawsuit->key.type == TYPE_CITIZEN_LAW) &&
		#endif  // AREAL || MILITARY || REGIONAL
			!allowinc(tKind, lawsuit)
		)
		{
			isValid = error("Полетата вид документ и вид дело не си съответстват.");
		}
		// 2008:060 FIX: 3-field SLOWDOWN is appeal
		else if ((tDecisionCount == 6 || (tDecisionCount == 3 && tKind == KIND_INREG_SLOWDOWN)) &&
			!obtain_appeal_instance(lawsuit->key.type, tKind, tDecisionKind))
		{
			isValid = error("Полетата вид документ и вид дело не си съответстват.");
		}
		else if (tDecisionCount != 6 && !strchr(KIND_IN_NOENDOCS, tKind))
			isValid = error("Полетата вид документ и обжалван акт не си съответстват.");
		else if (date->Date() < sessionDate->Date())
			isValid = error("Дата на документа е по-малка от дата на заседание.");
		// 2005:042 comout; 2005:074 uncomout
		else if (decisionNo->L() && !tDecisionKind)
			isValid = error("Полетата обжалван акт и номер не си съответстват.");
		else if (tSurroundCount != 0 && tSurroundCount != 3)
			isValid = error("Полетата съпровождащ документ, номер и година не си съответстват.");
		else if (tSurroundCount == 3 && tDecisionCount == 0)
		{
			// 2008:036 LPR: lacked isValid = false, may be a bug
			error("Не са попълнени данните за заседанието, на което е разглеждан съпровождащия документ.");
		}
		else if (tSurroundCount == 3 &&
			(
				surroundment.key.type = TYPE_SURROUND,
				surroundment.key.no = surroundNo->L(),
				surroundment.key.year = surroundYear->L(),
				surroundment.kind = surroundKind->C(),
				surroundment.lawsuit = lawsuit->key,
				// 2007:338 FIX: adjust session kind
				surroundment.sessionKind = adjust_xxx_kind(sessionKind->C(), sessionIndex, KIND_CLOSED_SESSIONS),
				surroundment.sessionDate = sessionDate->Date(),
				!surroundment.Try()
			)
		)
		{
			isValid = say_no_data(surroundment.Name);
		}
		// 2016:251 LPR: archeck is on select / electric action
	//	else if ((tDecisionCount == 3 || tDecisionCount == 6) && !can_access_exact(lawsuit, true, true))
	//		isValid = false;
		// 2009:145 LPR: surroundment.lawsuit already checked, move along
#if COURT_TYPE == COURT_AREAL
		// 2007:288 LRQ
		else if (tDecisionCount == 6 && strchr(KIND_IN_REG_REQ_2NDS, tKind) && !strchr(KIND_2ND_LAWSUITS, lawsuit->kind))
			isValid = error("Полетата вид документ и вид дело не си съответстват.");
#endif  // AREAL
		// 2014:036 IRQ/URQ: check no if decision exists
		else if (decisionNo->L() &&
			tDecisionCount == 6 &&
			tDecisionKind == KIND_SETTLEMENT &&
			(
				decision.key = session.key,
				decision.kind = tDecisionKind,
				decision.date = session.date,
				decision.Try("F_EVENT_NO") && decision.eventNo != decisionNo->L()
			)
		)
		{
			isValid = error("Въведеният номер на съдебен акт не е верен.");
		}
		else if (!college->C())		// 2016:097 JRQ: human readable error
			isValid = error("Полето колегия/отделение е задължително.");
		else if (tKind == KIND_EPORTAL_REQUEST && college->C() != COLLEGE_COMMON_ADMIN)
			isValid = error("Полетата вид документ и колегия/отделение не си съответстват.");
		else
			isValid = CheckDirection();

		if (tDecisionCount == 6)
		{
			strcpy(InReg()->judge, session.judge);
			// 2005:054 To avoid data loss in OK
			InReg()->decisionKind = decision.kind;
		}
	}

	return isValid;
}

void TInRegNewDialog::OKBNClicked()
{
	if (IsValid())
	{
		TInReg *inReg = InReg();

		*inReg << this;
		inReg->receiverType = atob(inReg->receiver) ? UCN_INSTANCE_HIGHER : UCN_INSTANCE_RESERVED;

		if (!strchr(KIND_TEXTUAL_IN_DOCS, inReg->kind))
			*inReg->text = '\0';

		// decision.no is fake! lawsuit.no completes the decision key
		inReg->decision.year = inReg->lawsuit.year;
		inReg->decision.type = inReg->lawsuit.type;
		inReg->decisionDate = sessionDate->Date();

		if (sessionIndex->L() > 0)
			inReg->sessionKind = index2kind(sessionIndex->L(), KIND_CLOSED_SESSIONS);

		if (inReg->Insert())
		{
			trouble.Move(Incoming(), date->Date());
			CmOk();
		}
	}
}

void TInRegNewDialog::KindCBNSelChange()
{
	char tKind = kind->C();

	if (strchr(KIND_TEXTUAL_IN_DOCS, tKind))
	{
		text->EnableWindow(true);
		sessionKind->SetC('\0');
		sessionKind->EnableWindow(false);
		sessionDate->SetS("");
		sessionDate->EnableWindow(false);
		sessionIndex->SetL(0);
		sessionIndex->EnableWindow(false);
		decisionKind->SetC('\0');
		decisionKind->EnableWindow(false);
		decisionNo->SetL(0);
		decisionNo->EnableWindow(false);
		surroundKind->SetC('\0');
		surroundKind->EnableWindow(false);
		surroundNo->SetL(0);
		surroundNo->EnableWindow(false);
		surroundYear->SetL(0);
		surroundYear->EnableWindow(false);
		receiver->SetXCrit(C2S[UCN_INSTANCE_RESERVED]);
	}
	else
	{
		text->EnableWindow(false);
		sessionKind->EnableWindow(true);
	#if TESTVER
		// 2016:097; 2016:313
		strcpy(sesKinds, kind2ses_kinds(lawsuit->kind));
		strcat(sesKinds, KIND_CLOSED_SESSIONS);

		for (char *sk = sesKinds; *sk; )
		{
			if (strchr(KIND_APPEALABLE_SESSIONS, *sk))
				sk++;
			else
				strmove(sk, sk + 1);
		}

		sessionKind->SetXCrit(sesKinds);
	#endif  // TESTVER
		sessionDate->EnableWindow(true);
		decisionKind->EnableWindow(true);
		decisionNo->EnableWindow(true);
	#if COURT_TYPE == COURT_ADMIN || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY
		surroundKind->EnableWindow(true);
		surroundNo->EnableWindow(true);
		surroundYear->EnableWindow(true);
	#endif  // ADMIN || AREAL || MILITARY
		receiver->SetXCrit(C2S[UCN_INSTANCE_HIGHER]);
		adjust_appeal_decisions(TYPE_V_LAWSUIT, kind, decisionKind);
		SetReceivers();
	}
}

void TInRegNewDialog::LawsuitTypeCBNSelChange()
{
	SetReceivers();
}

void TInRegNewDialog::SessionKindCBNSelChange()
{
	adjust_xxx_index(sessionKind->C(), sessionIndex, KIND_CLOSED_SESSIONS);
}

void TInRegNewDialog::DecisionKindCBNSelChange()
{
	SetReceivers();
}

void TInRegNewDialog::SearchBNEnable(TCommandEnabler &tce)
{
	tce.Enable(sessionKind->C());
}

void TInRegNewDialog::SearchBNClicked()
{
	TRCDKeyContainer *const session = select_event(this, lawsuit, sessionKind->C(), NULL, NULL, NULL, carried);
	TVirtPtr VP(session);

	if (session)
	{
		sessionKind->SetC(kind2VisibleKind(session->kind));
		adjust_xxx_index(session->kind, sessionIndex, KIND_CLOSED_SESSIONS);
		sessionDate->SetDate(session->date);
	}
}
