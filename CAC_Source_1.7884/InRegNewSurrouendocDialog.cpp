DEFINE_RESPONSE_TABLE1(TInRegNewSurrouendocDialog, TInRegNewBaseDialog)
//{{TInRegNewSurrouendocDialogRSP_TBL_BEGIN}}
	EV_CBN_SELCHANGE(IDC_EDIT_INCOMING_KIND, KindCBNSelChange),
	EV_BN_CLICKED(IDOK, OKBNClicked),
//{{TInRegNewSurrouendocDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TInRegNewDialog Implementation}}

TInRegNewSurrouendocDialog::TInRegNewSurrouendocDialog(TWindow* parent, TInReg *inReg, const char *kinds,
	const TLawsuit *lawsuit, const TSession *tSession, const TSurroundment *tSurrouendoc)
:
	TInRegNewBaseDialog(parent, inReg, kinds, KIND_SURROUENDOCS, KIND_SESSIONS, lawsuit), session(tSession),
		surrouendoc(tSurrouendoc)
{
}

void TInRegNewSurrouendocDialog::SetupWindow()
{
	TInRegNewBaseDialog::SetupWindow();

	sessionKind->EnableWindow(false);
	sessionDate->EnableWindow(false);
	decisionKind->EnableWindow(false);
	decisionNo->EnableWindow(false);
	decisionNo->SetL(surrouendoc->eventNo);
	receiver->SetXCrit(C2S[UCN_INSTANCE_HIGHER]);
	sessionKind->SetC(kind2VisibleKind(session->kind));
	sessionIndex->SetL(kind2index(session->kind, KIND_CLOSED_SESSIONS));
	KindCBNSelChange();
}

void TInRegNewSurrouendocDialog::KindCBNSelChange()
{
	SetReceivers();
}

void TInRegNewSurrouendocDialog::OKBNClicked()
{
	InReg()->receiverType = UCN_INSTANCE_HIGHER;
	sessionKind->SetC(session->kind);
	TInRegNewBaseDialog::OKBNClicked();
	sessionKind->SetC(kind2VisibleKind(session->kind));
}
