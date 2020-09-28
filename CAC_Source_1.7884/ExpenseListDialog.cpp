DEFINE_RESPONSE_TABLE1(TExpenseListDialog, TGroupListDialog)
//{{TExpenseListDialogRSP_TBL_BEGIN}}
	EV_BN_CLICKED(IDPENALITY, PenalityBNClicked),
	EV_BN_CLICKED(IDREWARD, RewardBNClicked),
	EV_COMMAND_ENABLE(IDREWARD, RewardBNEnable),
//{{TExpenseListDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TExpenseListDialog Implementation}}

TExpenseListDialog::TExpenseListDialog(TWindow* parent, TSideWind *tSideWind, TRCDKeyContainer *tContainer,
	TSession *session, TLawsuit *lawsuit, int resId)
:
	TGroupListDialog(parent, resId), sideWind(tSideWind), container(tContainer)
{
	header = new TStatic(this, IDC_EXPENSE_LIST_HEADER);
	expenses = new TExpenseListWindow(this, IDC_GROUP_LIST_WINDOW, sideWind, container, session, lawsuit);
	penality = new TButton(this, IDPENALITY);
}

void TExpenseListDialog::SetupWindow()
{
	TGroupListDialog::SetupWindow();

	mstr m;
	sideWind->Print(m);
	header->SetCaption(str(m));

	if (container->Status() != RESULT_FIXED)
		penality->EnableWindow(true);
}

void TExpenseListDialog::PenalityBNClicked()
{
	expenses->Penality();
}

void TExpenseListDialog::RewardBNClicked()
{
	expenses->Reward();
}

void TExpenseListDialog::RewardBNEnable(TCommandEnabler &tce)
{
	tce.Enable(strchr(INVOLVEMENT_MONEYABLES, sideWind->involvement));
}
