DEFINE_RESPONSE_TABLE1(TSurroundSideListDialog, TSideWindListDialog)
//{{TSurroundSideListDialogRSP_TBL_BEGIN}}
	EV_COMMAND_ENABLE(IDAPPEALER, SelectEnable),
	EV_BN_CLICKED(IDAPPEALER, AppealerBNClicked),
	EV_COMMAND_ENABLE(IDMONEY, SelectEnable),
	EV_BN_CLICKED(IDMONEY, MoneyBNClicked),
	EV_BN_CLICKED(IDATTACH, AttachBNClicked),
//{{TSurroundSideListDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TSurroundSideListDialog Implementation}}

TSurroundSideListDialog::TSurroundSideListDialog(TWindow* parent, TSurround *surround, TRCDKeyContainer *tSurrounded,
	long sideMode)
:
	TSideWindListDialog(parent, surround, NULL, sideMode | SIDE_LIST_KIND | SIDE_LIST_DATE, "документа",
		IDD_SURROUND_SIDE_LIST), surrounded(tSurrounded), lawsuit(NULL)
{
#if COURT_TYPE == COURT_AREAL
	if (container->kind == KIND_CHANGE_SURROUND)
		primaries = "заявители";
	else
#endif  // AREAL
		primaries = "податели";

	appealer = new TButton(this, IDAPPEALER);

	// 2018-09-26: LPR: moved here from Header2() for eportal
	// 2018-10-24: FIX: properly copied Header2() conditions
	if (surrounded && strchr(KIND_LAWSUITS, surrounded->kind))
		lawsuit = (TLawsuit *) surrounded;
	else if (strchr(TYPE_LAWSUITS, surround->lawsuit.type))
	{
		lawsuit = new TLawsuit;
		lawsuit->key = surround->lawsuit;

		if (!lawsuit->Get())
			fatal("Няма достъп до делото, по което е документа.");
	}
}

TSurroundSideListDialog::~TSurroundSideListDialog()
{
	if (lawsuit != surrounded)
		delete lawsuit;
}

void TSurroundSideListDialog::Header2(mstr &m)
{
	m.printf("%s %ld/%ldг.", Type->Seek(Surround()->lawsuit.type), Surround()->lawsuit.no, Surround()->lawsuit.year);

	// 2014:309; 2015:071
	if (PRINT_COMPOSITIONS && lawsuit)
		m.printf(", %s състав", Composition->Seek(lawsuit->composition));
}

bool TSurroundSideListDialog::Primary(TSideWind *sideWind) const
{
	return sideWind->Appealer();
}

void TSurroundSideListDialog::Accept()
{
	if (lawsuit)
		eportal_send(this, Surround(), lawsuit, LOG_WRITE);
	if (!edit)
		send_sms(Surround()->lawsuit, "Постъпване на", SMS_SURROUND, SMS_NEW, Surround());
	xfer_bank_surround(NULL, Surround(), sideWinds->Array(), edit ? XFER_EDIT : XFER_ADD);
}

void TSurroundSideListDialog::SetupWindow()
{
	TSideWindListDialog::SetupWindow();

#if COURT_TYPE == COURT_AREAL
	if (container->kind == KIND_CHANGE_SURROUND)
		appealer->SetCaption("&Заявител");
#endif  // AREAL
}

void TSurroundSideListDialog::AppealerBNClicked()
{
	TSideWind *sideWind = sideWinds->SideWind();
	bool state = sideWind->state;

		// 2009:140 LPR: surround_side() -> simple brand new logic
#if COURT_TYPE == COURT_AREAL
	if (Surround()->kind == KIND_CHANGE_SURROUND)
		sideWind->state = sideWind->state == STATE_CHANGE ? STATE_NONE : STATE_CHANGE;
	else
#endif  // AREAL
		sideWind->state = sideWind->state == STATE_SURROUND ? STATE_NONE : STATE_SURROUND;

	// 2009:147 TRQ: FIX: update sidewind (and revert state on error)
	if (sideWind->Update("F_STATE"))
		sideWinds->RedrawGroup();
	else
		sideWind->state = state;
}

void TSurroundSideListDialog::MoneyBNClicked()
{
	sideWinds->Expend(NULL, NULL);
}

void TSurroundSideListDialog::AttachBNClicked()
{
	sideWinds->Attach();
}
