DEFINE_RESPONSE_TABLE1(TInRegEventListDialog, TGroupListDialog)
//{{TInRegEventListDialogRSP_TBL_BEGIN}}
	EV_COMMAND_ENABLE(IDADD, AddBNEnable),
	EV_COMMAND_ENABLE(IDDELETE, DeleteBNEnable),
	EV_COMMAND_ENABLE(IDPRINT, PrintBNEnable),
	EV_BN_CLICKED(IDPRINT, PrintBNClicked),
	EV_BN_CLICKED(IDCANCEL, CancelBNClicked),
	EV_WM_CLOSE,
//{{TInRegEventListDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TInRegEventListDialog Implementation}}

TInRegEventListDialog::TInRegEventListDialog(TWindow* parent, TOutReg *tOutReg, TInReg *tInReg, TRCDKeyContainer *tEvent,
	sign tSave)
:
	TGroupListDialog(parent, IDD_INREG_EVENT_LIST), outReg(tOutReg), inReg(tInReg), event(tEvent), save(tSave),
		lastTicks(0), publInet(NULL)
{
	header1 = new TStatic(this, IDC_INREG_EVENT_LIST_HEADER_1);
	header2 = new TStatic(this, IDC_INREG_EVENT_LIST_HEADER_2);
	inRegEvents = new TInRegEventListWindow(this, IDC_GROUP_LIST_WINDOW, outReg, inReg, save);
	print = new TPushButton(this, IDPRINT);
#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
	if (save && outReg->receiverType == UCN_INSTANCE_HIGHER)
	{
		lawsuit.key = inReg->lawsuit;

		if (lawsuit.Try("F_KIND, F_FLAGS") && !lawsuit.LimitedNotAdmin())  // 2018-01-02: +P64Connect()
			if (decision.P64Wrong(&lawsuit, inReg, false) || decision.P64Connect(lawsuit.kind))
				publInet = new TCheckFace(this, IDC_INREG_EVENT_LIST_PUBLINET, "F_FLAGS_PUBLIC");
	}
#endif  // APPEAL || AREAL || MILITARY || REGIONAL
}

bool TInRegEventListDialog::Appeal() const
{
	// 2012:152 LRQ: all non-decision inReg-s are single
	//return inReg && !strchr(KIND_TEXTUAL_IN_DOCS, inReg->kind) && outReg->Status() == RESULT_IOREG_SEND;
	return inReg && inReg->kind && !inReg->decisionDate.Empty() && outReg->Status() == RESULT_IOREG_SEND;
}

void TInRegEventListDialog::PrintOut()
{
	TGArray<TInRegEvent> events;

	inRegEvents->GetEvents(&events);
	print_outreg(this, outReg, inReg, event, save, &events, NULL);
}

void TInRegEventListDialog::SetupWindow()
{
	TGroupListDialog::SetupWindow();

	mstr m(Kind->Seek(outReg->kind));

	if (outReg->key.no)
		m.printf(" %ld", outReg->key.no);
	else
		m.cat(" .....");

	m.printf("/%ld", outReg->key.year);
	header1->SetCaption(str(m));

	if (outReg->lawsuit.type)
	{
		m.clear();
		m.printf("%s %ld/%ldã.", Type->Seek(outReg->lawsuit.type), outReg->lawsuit.no, outReg->lawsuit.year);
		header2->SetCaption(str(m));
	}

	inRegEvents->EnableWindow(Appeal());

	if (publInet)
	{
		publInet->EnableWindow(true);
		// 2017:342 IRQ/LPR: check if 1st appeal; 2017:347 FIX: also if limited
		publInet->SetCheck(!(lawsuit.flags & LAWSUIT_LIMITED) &&
			((decision.flags & DECISION_PUBLIC) || (save == true && decision.Appealed() == true)) ?
			BF_CHECKED : BF_UNCHECKED);
		publInet->ShowWindow(SW_SHOW);
	}
}

void TInRegEventListDialog::AddBNEnable(TCommandEnabler &tce)
{
	tce.Enable(Appeal());
}

void TInRegEventListDialog::DeleteBNEnable(TCommandEnabler &tce)
{
	TInRegEvent *inRegEvent = (TInRegEvent *) inRegEvents->Group();
	tce.Enable(Appeal() && inRegEvent && inRegEvent->key != inReg->key);
}

void TInRegEventListDialog::PrintBNEnable(TCommandEnabler &tce)
{
	tce.Enable(atof(outReg->receiver));
}

void TInRegEventListDialog::PrintBNClicked()
{
	if (outReg->receiverType == UCN_INSTANCE_HIGHER && !strcmp(outReg->receiver, INSTANCE_SUPER))
	{
		if (GetTickCount() > lastTicks + 100)
		{
			char subpoenaKind = outreg_subpoena_kind(outReg);
			TRect rect = print->GetWindowRect();
			//uint flags = TPM_CENTERALIGN | TPM_TOPALIGN | TPM_NONOTIFY | TPM_RETURNCMD | TPM_LEFTBUTTON;
			uint flags = TPM_LEFTALIGN | TPM_TOPALIGN | TPM_NONOTIFY | TPM_RETURNCMD | TPM_LEFTBUTTON;
			TPopupMenu menu;

			SubpoenaKind->Seek(subpoenaKind);
			menu.AppendMenu(MF_STRING, subpoenaKind, SubpoenaKind->name);
			SubpoenaKind->Seek(SUBPOENA_KIND_SUPER_APPENDIX);
			menu.AppendMenu(MF_STRING, SUBPOENA_KIND_SUPER_APPENDIX, SubpoenaKind->name);

			//int command = TrackPopupMenu(menu.GetHandle(), flags, (rect.left + rect.right + rect.right) / 3,
			int command = TrackPopupMenu(menu.GetHandle(), flags, rect.left - (rect.right - rect.left) / 4,
				rect.bottom, 0, Handle, NULL);

			if (command == subpoenaKind)
				PrintOut();
			else if (command == SUBPOENA_KIND_SUPER_APPENDIX)
				outreg_send_appeal(this, outReg, inRegEvents->Events(), save);

			lastTicks = GetTickCount();
		}
	}
	else
		PrintOut();
}

void TInRegEventListDialog::CancelBNClicked()
{
	EvClose();
}

void TInRegEventListDialog::EvClose()
{
	if (CanClose())
	{
		if (publInet)
		{
			long decisionFlags = publInet->GetCheck() == BF_CHECKED ? (decision.flags | DECISION_PUBLIC) :
				(decision.flags & ~DECISION_PUBLIC);

			if (decisionFlags != decision.flags)
			{
				decision.flags = decisionFlags;
				// 2018-01-08: FIX: +publicDate
				decision.publicDate = (decisionFlags & DECISION_PUBLIC) ? Today : CDate();

				if (decision.Update("F_FLAGS, F_PUBLIC_DATE"))
				{
					log_event(LOG_EDIT, 0, &decision, decision.flags & DECISION_PUBLIC);

					if (!(decisionFlags & DECISION_PUBLIC))
					{
						xfer_web_delete(&decision, &lawsuit);
						eportal_send(NULL, &decision, &lawsuit, LOG_DELETE);  // 2018-10-19
					}
				}
			}
		}

		TGroupListDialog::EvClose();
	}
}
