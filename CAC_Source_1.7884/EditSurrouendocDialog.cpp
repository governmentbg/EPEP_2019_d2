DEFINE_RESPONSE_TABLE1(TEditSurrouendocDialog, TTroubleDialog)
//{{TEditSurrouendocDialogRSP_TBL_BEGIN}}
	EV_CBN_SELCHANGE(IDC_EDIT_SURROUENDOC_DECISION_KIND, DecisionKindCBNSelChange),
	EV_LBN_SELCHANGE(IDC_EDIT_SURROUENDOC_ACCOMPLYS, AccomplysLBNSelChange),
	EV_BN_CLICKED(IDOK, OKBNClicked),
	EV_BN_CLICKED(IDRIGHT, RightBNClicked),
	EV_BN_CLICKED(IDPRINT, PrintBNClicked),
//{{TEditSurrouendocDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TEditSurrouendocDialog Implementation}}

TEditSurrouendocDialog::TEditSurrouendocDialog(TWindow* parent, TSurroundment *surrouendoc, const TSession *tSession,
	TLawsuit *tLawsuit)
:
	TTroubleDialog(parent, surrouendoc, (surrouendoc->decisionKind != '\0') | ATTACH_RIGHT, TYPE_TEXT,
		IDD_EDIT_SURROUENDOC), session(tSession), backupIndex(surrouendoc), lawsuit(tLawsuit)
{
	new TCharAliasFace(this, IDC_EDIT_SURROUENDOC_KIND, "F_KIND", Kind, NULL);
	new TLongFace(this, IDC_EDIT_SURROUENDOC_NO, "F_NO", SIZE_OF_NO);
	new TYearFace(this, IDC_EDIT_SURROUENDOC_YEAR, "F_YEAR");
	new TDateFace(this, IDC_EDIT_SURROUENDOC_DATE, "F_DATE");
	decisionKind = new TCharAliasFace(this, IDC_EDIT_SURROUENDOC_DECISION_KIND, "F_DECISION_KIND", Kind,
		KIND_SURROUENDOC_ACTS);
	eventNo = new TLongFace(this, IDC_EDIT_SURROUENDOC_EVENT_NO, "F_EVENT_NO", SIZE_OF_EVENT_NO);
	decisionDate = new TDateFace(this, IDC_EDIT_SURROUENDOC_DECISION_DATE, "F_DECISION_DATE", true);
	result = new TCharAliasFace(this, IDC_EDIT_SURROUENDOC_RESULT, "F_RESULT", Result, RESULT_SURROUENDOC_ENDOCS,
		ALIAS_OPTION);
	new TStringFace(this, IDC_EDIT_SURROUENDOC_TEXT, "F_TEXT", SIZE_OF_SURROUNDMENT_TEXT);
	accomplys = new TCharListFace(this, IDC_EDIT_SURROUENDOC_ACCOMPLYS, "F_ACCOMPLYS", Accomply, session->Accomplys(),
		SIZE_OF_ACCOMPLYS, edit ? FLAG_NULL : FLAG_ACTIVE);
	debtorStatus = new TCharAliasFace(this, IDC_EDIT_SURROUENDOC_DEBTOR_STATUS, "F_DEBTOR_STATUS", DebtorStatus, NULL,
		ALIAS_EMPTY);
	publInet = new TCheckFace(this, IDC_TROUBLE_PUBLINET, "F_FLAGS_PUBLIC");
	publTreg = new TCheckFace(this, IDC_TROUBLE_PUBLTREG, "F_FLAGS_PUBLTREG");
}

void TEditSurrouendocDialog::Commit()
{
	Parent->Show(SW_HIDE);
	PostMove();
	Parent->Show(SW_SHOW);
	CmOk();
}

void TEditSurrouendocDialog::SetupWindow()
{
	TTroubleDialog::SetupWindow();

	if (session->Accomplys() == ACCOMPLY_NULLS)
		accomplys->EnableWindow(false);

	DecisionKindCBNSelChange();
	decisionDate->EnableWindow(!backupIndex.EventNo());
	SetupPublic(true, Surrouendoc()->flagsX & DECISION_X_PUBLTREG, lawsuit);
	// 2016:174 
	if (decisionDate->Date().Empty())
		decisionDate->SetDate(Today);
}

void TEditSurrouendocDialog::DecisionKindCBNSelChange()
{
	Surrouendoc()->decisionKind = decisionKind->C();

	result->SetXCrit(Surrouendoc()->SurrouendocResults());
	backupIndex.CheckIndex(Surrouendoc(), eventNo);

	if (!edit && Surrouendoc()->decisionKind == KIND_SETTLEMENT)
	{
		char tAccomplys[SIZE_OF_ACCOMPLYS];

		strcpy(tAccomplys, accomplys->S());
		strcach_nx(tAccomplys, ACCOMPLY_CONFIRM);
		accomplys->SetS(tAccomplys);
	}
}

void TEditSurrouendocDialog::AccomplysLBNSelChange()
{
	debtorStatus->SetC(accomplys2DebtorStatus(accomplys->S()));
}

void TEditSurrouendocDialog::OKBNClicked()
{
	TSurroundment *surrouendoc = Surrouendoc();
	*surrouendoc << this;

	if (IsValid())
	{
		if (backupIndex.Update(surrouendoc))
			Commit();
	}
}

void TEditSurrouendocDialog::RightBNClicked()
{
	Surrouendoc()->decisionKind = decisionKind->C();
	attach_right(this, trouble, Surrouendoc()->IndexKind(), decisionDate->Date(), session);
	decisionDate->EnableWindow(!backupIndex.EventNo() && !trouble.act);
}

void TEditSurrouendocDialog::PrintBNClicked()
{
	TDecision decision;

	*Surrouendoc() << this;
	decision.Init(Surrouendoc());
	print_decision_judgement(this, &decision, lawsuit, decisionDate->Date());
}
