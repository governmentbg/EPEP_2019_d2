DEFINE_RESPONSE_TABLE1(TDecisionSideListDialog, TSideWindListDialog)
//{{TDecisionSideListDialogRSP_TBL_BEGIN}}
	EV_BN_CLICKED(IDMESSAGES, MessagesBNClicked),
	EV_BN_CLICKED(IDLIST, ListBNClicked),
	EV_BN_CLICKED(IDRIGHT, RightBNClicked),
	EV_BN_CLICKED(IDSCHED, SchedBNClicked),
	EV_COMMAND_ENABLE(IDMONEY, SelectEnable),
	EV_BN_CLICKED(IDMONEY, MoneyBNClicked),
	EV_COMMAND_ENABLE(IDCHILD, ChildBNEnable),
	EV_BN_CLICKED(IDCHILD, ChildBNClicked),
	EV_COMMAND_ENABLE(IDC_HERITAGE_SIDE_LIST_HERITAGE, HeritageBNEnable),
	EV_BN_CLICKED(IDC_HERITAGE_SIDE_LIST_HERITAGE, HeritageBNClicked),
//{{TDecisionSideListDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TDecisionSideListDialog Implementation}}

TDecisionSideListDialog::TDecisionSideListDialog(TWindow* parent, TIndexContainer *container, TSession *tSession,
	TLawsuit *lawsuit, long sideMode, int resId)
:
	TSideWindListDialog(parent, container, lawsuit, sideMode, "съдебния акт", resId), session(tSession)
{
	new TTwinButton(this, IDLIST, IDRIGHT);
	child = new TPushButton(this, IDCHILD);
	heritage = (resId == IDD_HERITAGE_SIDE_LIST) ? new TPushButton(this, IDC_HERITAGE_SIDE_LIST_HERITAGE) : NULL;
}

void TDecisionSideListDialog::Header2(mstr &m)
{
	m.printf("%s от %Dг.", Kind->Seek(session->kind), &session->date);
	if (!session->time.Empty())
		m.printf(", %T часа", &session->time);

	if (IndexContainer()->eventNo)
	{
		TDate *fromDate = (TDate *) IndexContainer()->Find("F_FROM_DATE");

		m.printf(", %s", Kind->Seek(IndexContainer()->IndexKind()));
		// 2007:213 LRQ: show eventNo only if config
		// 2007:239 LRQ: show year only if config
		if (PRINT_JUDGEMENTS)
		{
			m.printf(" %ld/", IndexContainer()->eventNo);
			m.printf("%ldг.", fromDate ? fromDate->Date().year : IndexContainer()->date.year);
		}
	}
}

void TDecisionSideListDialog::SetupWindow()
{
	TSideWindListDialog::SetupWindow();
	
#if COURT_TYPE == COURT_REGIONAL
	if (heritage)
		heritage->ShowWindow(SW_SHOW);
#endif  // REGIONAL
}

void TDecisionSideListDialog::MessagesBNClicked()
{
	sideWinds->Messages();
}

void TDecisionSideListDialog::ListBNClicked()
{
	sideWinds->Roll();
}

void TDecisionSideListDialog::RightBNClicked()
{
	sideWinds->List(true);
}

void TDecisionSideListDialog::SchedBNClicked()
{
	TSched sched;

	sched << container;
	sched.lawsuit = container->key;
	edit_scheds(this, &sched);
}

void TDecisionSideListDialog::MoneyBNClicked()
{
	sideWinds->Expend(session, sideWinds->Lawsuit());
}

void TDecisionSideListDialog::HeritageBNEnable(TCommandEnabler &tce)
{
#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_ADMIN || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY
	tce.Enable(false);
#endif  // APPEAL || ADMIN || AREAL || MILITARY
#if COURT_TYPE == COURT_REGIONAL
	TSideWind *sideWind = sideWinds->SideWind();
	tce.Enable(sideWind && strchr(INVOLVEMENT_HERITAGES, sideWind->involvement));
#endif  // REGIONAL
}

void TDecisionSideListDialog::HeritageBNClicked()
{
#if COURT_TYPE == COURT_REGIONAL
	if (GetTickCount() > lastTicks + 100)
	{
		TRect rect = heritage->GetWindowRect();
		uint flags = TPM_CENTERALIGN | TPM_TOPALIGN | TPM_NONOTIFY | TPM_RETURNCMD | TPM_LEFTBUTTON;
		TPopupMenu menu;
		mstr results(RESULT_HERITAGE_STATES);

		results.cach(IndexContainer()->Status());

		for (const char *result = str(results); *result; result++)
			menu.AppendMenu(MF_STRING, *result, Result->Seek(*result));

		int command = TrackPopupMenu(menu.GetHandle(), flags, (rect.left + rect.right) / 2, rect.bottom, 0, Handle, NULL);

		sideWinds->Heritage(command);
		lastTicks = GetTickCount();
	}
#endif  // REGIONAL
}
