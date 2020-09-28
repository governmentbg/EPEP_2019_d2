#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
DEFINE_RESPONSE_TABLE1(TProveActionDialog, TTroubleDialog)
//{{TProveActionDialogRSP_TBL_BEGIN}}
	EV_BN_CLICKED(IDOK, OKBNClicked),
	EV_CBN_SELCHANGE(IDC_EDIT_PROVE_ACTION_RESOLUTION, ResolutionCBNSelChange),
	EV_CBN_SELCHANGE(IDC_EDIT_PROVE_ACTION_SESSION_KIND, SessionKindCBNSelChange),
//{{TProveActionDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TProveActionDialog Implementation}}

TProveActionDialog::TProveActionDialog(TWindow* parent, TProve *tProve, TProveAction *proveAction, bool edit, int resId)
:
	TTroubleDialog(parent, proveAction, edit, TYPE_TEXT, resId), prove(tProve)
{
	new TCharAliasFace(this, IDC_EDIT_PROVE_ACTION_LAWSUIT_TYPE, "F_LAWSUIT_TYPE", Type, NULL);
	new TLongFace(this, IDC_EDIT_PROVE_ACTION_LAWSUIT_NO, "F_LAWSUIT_NO", SIZE_OF_NO);
	new TYearFace(this, IDC_EDIT_PROVE_ACTION_LAWSUIT_YEAR, "F_LAWSUIT_YEAR");

	new TLongFace(this, IDC_EDIT_PROVE_ACTION_NO, "F_NO", SIZE_OF_NO);
	new TYearFace(this, IDC_EDIT_PROVE_ACTION_YEAR, "F_YEAR");
	resolution = new TCharAliasFace(this, IDC_EDIT_PROVE_ACTION_RESOLUTION, "F_RESOLUTION", Resolution, RESOLUTION_PROVE_ACTIONS);
	date = new TDateFace(this, IDC_EDIT_PROVE_ACTION_DATE, "F_DATE");
	flagsInstance = new TCheckFace(this, IDC_EDIT_PROVE_ACTION_INSTANCE, "F_FLAGS_INSTANCE");
	// 2006:038  KIND_VISIBLE_SESSIONS -> KIND_PROVABLE_SESS
	sessionKind = new TCharAliasFace(this, IDC_EDIT_PROVE_ACTION_SESSION_KIND, "F_SESSION_KIND", Kind, KIND_PROVABLE_SESS, ALIAS_EMPTY);
	sessionDate = new TDateFace(this, IDC_EDIT_PROVE_ACTION_SESSION_DATE, "F_SESSION_DATE", false);
	decisionKind = new TCharAliasFace(this, IDC_EDIT_PROVE_ACTION_DECISION_KIND, "F_DECISION_KIND", Kind, KIND_VISIBLE_ENDOCS, ALIAS_EMPTY);
	outNo = new TLongFace(this, IDC_EDIT_PROVE_ACTION_OUT_NO, "F_OUT_NO", SIZE_OF_NO);
	outYear = new TYearFace(this, IDC_EDIT_PROVE_ACTION_OUT_YEAR, "F_OUT_YEAR");
	index = new TIndexFace(this, IDC_EDIT_PROVE_ACTION_INDEX, "F_INDEX", KIND_CLOSED_SESSIONS, SIZE_OF_SESSION_INDEX);

	sessionKindLabel = new TStatic(this, IDC_EDIT_PROVE_ACTION_SESSION_KIND_LABEL);
	sessionDateLabel = new TStatic(this, IDC_EDIT_PROVE_ACTION_SESSION_DATE_LABEL);
	decisionKindLabel = new TStatic(this, IDC_EDIT_PROVE_ACTION_DECISION_KIND_LABEL);
	new TStringFace(this, IDC_EDIT_PROVE_ACTION_TEXT, "F_TEXT", SIZE_OF_PROVE_ACTION_TEXT);
}

void TProveActionDialog::SetupWindow()
{
	TTroubleDialog::SetupWindow();

	*prove >> this;		// for the lawsuit fields
	*ProveAction() >> this;

	if (strchr(TYPE_LAWSUITS, prove->lawsuit.type))
	{
		decisionKindLabel->ShowWindow(SW_SHOW);
		decisionKind->ShowWindow(SW_SHOW);
	}
	else
	{
		sessionKindLabel->SetText("Вид документ");
		sessionDateLabel->SetText("Дата на ре&зол.");
	}

	ResolutionCBNSelChange();
	SessionKindCBNSelChange();
}

void TProveActionDialog::OKBNClicked()
{
	if (IsValid())
	{
		*ProveAction() << this;
		ProveAction()->kind = KIND_PROVE_ACTION;
		ProveAction()->sessionKind = adjust_xxx_kind(sessionKind->C(), index, KIND_CLOSED_SESSIONS);

		if (ProveAction()->Save(edit))
		{
			trouble.Move(ProveAction(), ProveAction()->date);
			CmOk();
		}
	}
}

void TProveActionDialog::ResolutionCBNSelChange()
{
	if (resolution->C() == RESOLUTION_PROVE_SEND)
	{
		outNo->SetL(ProveAction()->outNo);
		outNo->EnableWindow(true);
		outYear->SetL(ProveAction()->outYear);
		outYear->EnableWindow(true);
	}
	else
	{
		outNo->EnableWindow(false);
		outNo->SetL(0);
		outYear->EnableWindow(false);
		outYear->SetL(0);
	}

	if (resolution->C() == RESOLUTION_RETURNED)
	{
		sessionKind->EnableWindow(false);
		sessionKind->SetC('\0');
		sessionDate->EnableWindow(false);
		sessionDate->SetS("");
		decisionKind->EnableWindow(false);
		decisionKind->SetC('\0');
		flagsInstance->EnableWindow(!edit);
		index->EnableWindow(false);
		index->SetL(0);
	}
	else
	{
		sessionDate->SetDate(ProveAction()->sessionDate);
		sessionDate->EnableWindow(true);

		if (strchr(TYPE_LAWSUITS, prove->lawsuit.type))
		{
			sessionKind->SetXCrit(KIND_PROVABLE_SESS);
			sessionKind->SetC(ProveAction()->sessionKind);
			sessionKind->EnableWindow(true);
			decisionKind->SetC(ProveAction()->decisionKind);
			decisionKind->EnableWindow(true);
			index->EnableWindow(true);
		}
		else
		{
			sessionKind->SetXCrit(KIND_REQUEST_RESOLUTIONS);
			sessionKind->EnableWindow(false);
			sessionKind->SetC(KIND_REQUEST_RESOLUTION);
			decisionKind->EnableWindow(false);
			decisionKind->SetC('\0');
		}

		flagsInstance->EnableWindow(!edit && resolution->C() == RESOLUTION_PROVE_SEND);
	}
}

bool TProveActionDialog::IsValid()
{
	bool isValid = TTroubleDialog::IsValid();

	if (isValid)
	{
		int tOutCount = (outNo->L() != 0) + (outYear->L() != 0);
		TOutReg outReg;
		int tDecisionCount =
			(sessionKind->C() != '\0') +
			!sessionDate->Date().Empty() +
			(decisionKind->C() != '\0');
		TLawsuit lawsuit;
		TSession session;
		TDecision decision;
		TRequestResolution tResolution;
		TProveAction *proveAction = NULL;
		mstr decisionKinds;

		if (date->Date() > Today)
			isValid = error("Дата на действието е по-голяма от текущата дата.");
		else if (sessionDate->IsWindowEnabled() && date->Date() < sessionDate->Date())
			isValid = error("Дата на действието е по-малка от дата на заседание.");
		else if (outNo->IsWindowEnabled() && tOutCount != 2)
			isValid = error("Полетата изходящ номер и година са задължителни.");
		else if
		(
			tOutCount == 2  &&
			(
				outReg.key.no = outNo->L(),
				outReg.key.year = outYear->L(),
				!outReg.Get()
			)
		)
			isValid = false;
		else if (sessionDate->IsWindowEnabled() && sessionKind->IsWindowEnabled() && tDecisionCount != 3)
			isValid = error("Полетата вид заседание, дата на заседание и вид съдебен акт са задължителни.");
		else if (sessionDate->IsWindowEnabled() && !sessionKind->IsWindowEnabled() && tDecisionCount != 2)
			isValid = error("Полето дата на разпореждане е задължително.");
		else if
		(
			tDecisionCount == 3 &&
			(
				session.key = prove->lawsuit,
				session.kind = adjust_xxx_kind(sessionKind->C(), index, KIND_CLOSED_SESSIONS),
				session.date = sessionDate->Date(),
				!session.Get()
			)
		)
			isValid = false;
		else if (tDecisionCount == 3 && strchr(RESULT_UNCARRIEDS, session.Status()))
			isValid = error("Заседанието е %s и не може да се ползва за разпоредително действие.", Result->Seek(session.Status()));
		else if
		(
			tDecisionCount == 3 &&
			(
				lawsuit.key = prove->lawsuit,
				!lawsuit.Get()
			)
		)
			isValid = false;
		else if
		(
			tDecisionCount == 3 &&
			strchr(RESULT_ENTERABLES, session.Status()) != NULL &&
			strchr(session.DecisionKinds(decisionKinds, &lawsuit), decisionKind->C()) == NULL
		)
			isValid = error("Резултата от заседанието и вида на съдебния акт не си съответстват.");
		else if
		(
			tDecisionCount == 3 &&
			!strchr(RESULT_ENTERABLES, session.Status()) &&
			!strchr(KIND_IN_UNDECIDABLES, decisionKind->C())
		)
			isValid = error("Резултата от заседанието и вида на съдебния акт не си съответстват.");
		else if
		(
			tDecisionCount == 6 &&
			strchr(RESULT_ENTERABLES, session.Status()) &&
			(
				decision.key = session.key,
				decision.kind = decisionKind->C(),
				decision.date = session.date,
				!decision.Get()
			)
		)
			isValid = false;
		else if
		(
			tDecisionCount == 2 &&
			(
				tResolution.key.no = prove->lawsuit.no,
				tResolution.key.year = prove->lawsuit.year,
				tResolution.date = sessionDate->Date(),
				!tResolution.Get()
			)
		)
			isValid = false;
		// 2007:250 FIX: resolution is blocked and valid on edit
		else if (
			!edit &&
			(resolution->C() == RESOLUTION_PROVE_SEND ||
			resolution->C() == RESOLUTION_RETURNED) &&
			(proveAction = prove->LastAction("F_RESOLUTION", date->Date())) != NULL &&
			(proveAction->Status()) == resolution->C()
		)
			isValid = error("Доказателството вече е %s.", resolution->C() == RESOLUTION_PROVE_SEND ? "предоставено" : "върнато");

		delete proveAction;
	}

	return isValid;
}

void TProveActionDialog::SessionKindCBNSelChange()
{
	adjust_xxx_index(sessionKind->C(), index, KIND_CLOSED_SESSIONS);
}
#endif  // APPEAL || AREAL || MILITARY || REGIONAL
