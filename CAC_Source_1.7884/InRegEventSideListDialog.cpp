DEFINE_RESPONSE_TABLE1(TInRegEventSideListDialog, TSideWindListDialog)
//{{TInRegEventSideListDialogRSP_TBL_BEGIN}}
	EV_BN_CLICKED(IDMESSAGES, MessagesBNClicked),
	EV_BN_CLICKED(IDLIST, ListBNClicked),
	EV_BN_CLICKED(IDRIGHT, RightBNClicked),
	EV_BN_CLICKED(IDSCHED, SchedBNClicked),
//{{TInRegEventSideListDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TInRegEventSideListDialog Implementation}}

TInRegEventSideListDialog::TInRegEventSideListDialog(TWindow* parent, TInRegEvent *inRegEvent, TInReg *tInReg, long sideMode)
:
	TSideWindListDialog(parent, inRegEvent, NULL, sideMode | SIDE_LIST_KIND, "разпореждането", IDD_INREG_EVENT_SIDE_LIST),
		inReg(tInReg), eportalSendable(inReg->EPortalSendable())
{
	new TTwinButton(this, IDLIST, IDRIGHT);

	if (eportalSendable)
	{
		TLawsuit *tLawsuit = new TLawsuit;
		tLawsuit->key = inReg->lawsuit;	// 2018-11-08: FIX: was "key" 

		if (tLawsuit->Get())
			sideWinds->SetLawsuit(tLawsuit);
		else
			delete tLawsuit;
	}
}

TInRegEventSideListDialog::~TInRegEventSideListDialog()
{
	delete sideWinds->Lawsuit();
}

void TInRegEventSideListDialog::Header1(mstr &m)
{
	m.printf("%s %ld/%ldг.", Kind->Seek(inReg->kind), inReg->key.no, inReg->key.year);
}

void TInRegEventSideListDialog::Header2(mstr &m)
{
	m.cat(InRegEvent()->Status() == RESULT_RESO_SETTLEMENT ? "Определение " : "Разпореждане ");

	if (InRegEvent()->eventNo)
		m.printf("%ld/%ldг.", InRegEvent()->eventNo, InRegEvent()->date.year);
	else
		InRegEvent()->date.Print(m, C_DATE_GOD);
}

void TInRegEventSideListDialog::MessagesBNClicked()
{
	sideWinds->Messages();
}

void TInRegEventSideListDialog::ListBNClicked()
{
	// 2004:021 LRQ: from decision dialog
	sideWinds->Roll();
}

void TInRegEventSideListDialog::RightBNClicked()
{
	// 2004:229 LPR: or something else?
	sideWinds->List(true);
}

void TInRegEventSideListDialog::SchedBNClicked()
{
	edit_scheds(this, InRegEvent());
}
