DEFINE_RESPONSE_TABLE1(TInRegSideListDialog, TSideWindListDialog)
//{{TInRegSideListDialogRSP_TBL_BEGIN}}
	EV_COMMAND_ENABLE(IDAPPEALER, AppealerBNEnable),
	EV_BN_CLICKED(IDAPPEALER, AppealerBNClicked),
	EV_COMMAND_ENABLE(IDMONEY, SelectEnable),
	EV_BN_CLICKED(IDMONEY, MoneyBNClicked),
	EV_BN_CLICKED(IDATTACH, AttachBNClicked),
//{{TInRegSideListDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TInRegSideListDialog Implementation}}

TInRegSideListDialog::TInRegSideListDialog(TWindow* parent, TInReg *inReg, TLawsuit *lawsuit, long sideMode)
:
	TSideWindListDialog(parent, inReg, lawsuit, sideMode | SIDE_LIST_KIND | SIDE_LIST_DATE, "документа",
		IDD_INREG_SIDE_LIST), eportalSendable(inReg->EPortalSendable())
{
	primaries = strchr(KIND_APPEAL_IN_DOCS, inReg->kind) ? "жалбоподатели" : "податели";
	editor = new TPushButton(this, IDEDIT);
	appealer = new TButton(this, IDAPPEALER);
}

void TInRegSideListDialog::Header2(mstr &m)
{
	const TInReg *inReg = InReg();

	if (inReg->lawsuit.no)
	{
		m.printf("%s %ld/%ldг.", Type->Seek(inReg->lawsuit.type), inReg->lawsuit.no, inReg->lawsuit.year);

		TIndexContainer *container = inReg->AppealedAct(true, true);

		if (container)
		{
			m.printf(", %s ", Kind->Seek(container->kind));

			if (PRINT_JUDGEMENTS && container->eventNo)
			{
				m.printf("%ld", container->eventNo);
				if (container->date.year)
					m.printf("/%ldг.", container->date.year);
			}
			else
				container->date.Print(m, "от %Dг.");

			delete container;
		}

		if (inReg->extraNo)
			m.printf(" по Възражение %ld/%ldг.", inReg->surroundNo, inReg->surroundYear);
	}
	else if (inReg->surroundKind)
		m.printf("%s %ld/%ldг.", Kind->Seek(inReg->surroundKind), inReg->surroundNo, inReg->surroundYear);
}

bool TInRegSideListDialog::Primary(TSideWind *sideWind) const
{
	return sideWind->Appealer();
}

void TInRegSideListDialog::Accept()
{
	TInReg *inReg = InReg();

	if (!edit && strchr(KIND_APPEAL_IN_DOCS, inReg->kind))
		send_sms(inReg->lawsuit, "Постъпване на", SMS_INREG, SMS_NEW, inReg);

	eportal_send(this, inReg, sideWinds->Lawsuit(), LOG_WRITE, eportalSendable);
	xfer_bank_event(NULL, NULL, NULL, NULL, inReg, sideWinds->Array(), NULL, edit ? XFER_EDIT : XFER_ADD);
	eispp_accept(this, InReg());
}

void TInRegSideListDialog::SetupWindow()
{
	TSideWindListDialog::SetupWindow();

	if (strchr(KIND_TEXTUAL_IN_DOCS, container->kind))
	{
		editor->SetStyle(editor->GetStyle() | BS_DEFPUSHBUTTON, true);
		appealer->SetStyle(appealer->GetStyle() & ~BS_DEFPUSHBUTTON, true);
	}
}

void TInRegSideListDialog::AppealerBNEnable(TCommandEnabler &tce)
{
	tce.Enable(sideWinds->SideWind() && !strchr(KIND_TEXTUAL_IN_DOCS, container->kind));
}

void TInRegSideListDialog::AppealerBNClicked()
{
	TSideWind *sideWind = sideWinds->SideWind();

	// 2004:005 LPR: Update() to be sure
	if (inreg_side(this, InReg(), sideWind) && sideWind->Update("F_STATE"))
		sideWinds->RedrawGroup();
}

void TInRegSideListDialog::MoneyBNClicked()
{
	sideWinds->Expend(NULL, NULL);
}

void TInRegSideListDialog::AttachBNClicked()
{
	sideWinds->Attach();
}
