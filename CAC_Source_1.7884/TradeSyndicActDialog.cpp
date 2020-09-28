DEFINE_RESPONSE_TABLE1(TTradeSyndicActDialog, TJudicialActDialog)
//{{TTradeSyndicActDialogRSP_TBL_BEGIN}}
	EV_BN_CLICKED(IDOK, OKBNClicked),
	EV_BN_CLICKED(IDPRINT, PrintBNClicked),
	EV_BN_CLICKED(IDC_TRADE_SYNDIC_ACT_SYNDICATE, SyndicateBNClicked),
	EV_LBN_SELCHANGE(IDC_TRADE_SYNDIC_ACT_ACCOMPLYS, AccomplysLBNSelChange),
//{{TTradeSyndicActDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TTradeSyndicActDialog Implementation}}

TTradeSyndicActDialog::TTradeSyndicActDialog(TWindow* parent, TDecision *decision, TLawsuit *lawsuit, const char *kinds,
	bool grant, TSession *session)
:
	TJudicialActDialog(parent, decision, lawsuit, kinds, grant, session, IDD_TRADE_SYNDIC_ACT)
{
	CreateAgeFinished();
	accomplys = new TCharListFace(this, IDC_TRADE_SYNDIC_ACT_ACCOMPLYS, "F_ACCOMPLYS", Accomply,
		decision->Accomplys(lawsuit, session), SIZE_OF_ACCOMPLYS, edit ? FLAG_NULL : FLAG_ACTIVE);
	completed = new TDateFace(this, IDC_TRADE_SYNDIC_ACT_COMPLETED, "F_COMPLETED", false);
	debtorStatus = new TCharAliasFace(this, IDC_TRADE_SYNDIC_ACT_DEBTOR_STATUS, "F_DEBTOR_STATUS", DebtorStatus, NULL,
		ALIAS_EMPTY);
	new TButton(this, IDC_TRADE_SYNDIC_ACT_SYNDICATE);
	publTreg = new TCheckFace(this, IDC_TROUBLE_PUBLTREG, "F_FLAGS_PUBLTREG");
}

bool TTradeSyndicActDialog::DefaultPublic()
{
	return true;
}

void TTradeSyndicActDialog::OKBNClicked()
{
	lawsuit->completed = completed->Date();
	TJudicialActDialog::OKBNClicked();
}

void TTradeSyndicActDialog::SetupWindow()
{
	TJudicialActDialog::SetupWindow();

	if (Decision()->Accomplys(lawsuit, session) == ACCOMPLY_NULLS)
		accomplys->EnableWindow(false);

	completed->SetDate(lawsuit->completed);

	if (grant)
	{
	#if COURT_TYPE == COURT_AREAL
		grant->EnableWindow(session->result != RESULT_INCONSISTENCE);
	#endif  // AREAL
		grant->SetC(lawsuit->grant);
	}
}

void TTradeSyndicActDialog::PrintBNClicked()
{
	lawsuit->completed = completed->Date();
	TJudicialActDialog::PrintBNClicked();
}

void TTradeSyndicActDialog::ResultChanged()
{
	TJudicialActDialog::ResultChanged();
	transfer_dec_text(result, text);
}

void TTradeSyndicActDialog::SyndicateBNClicked()
{
	TWhateverTextBaseDialog(this, Group(), "Данни за синдик", "F_SYNDICATE", SIZE_OF_SYNDICATE).Execute();
}

void TTradeSyndicActDialog::AccomplysLBNSelChange()
{
	debtorStatus->SetC(accomplys2DebtorStatus(accomplys->S()));
}
