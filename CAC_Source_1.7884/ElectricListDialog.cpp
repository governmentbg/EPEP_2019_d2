DEFINE_RESPONSE_TABLE1(TElectricListDialog, TGroupListDialog)
//{{TElectricListDialogRSP_TBL_BEGIN}}
	EV_COMMAND_ENABLE(IDLIST, SelectEnable),
	EV_BN_CLICKED(IDLIST, ListBNClicked),
	EV_COMMAND_ENABLE(IDBLUE, BlueBNEnable),
	EV_BN_CLICKED(IDBLUE, BlueBNClicked),
	EV_COMMAND_ENABLE(IDSAVE, SaveBNEnable),
	EV_BN_CLICKED(IDSAVE, SaveBNClicked),
	EV_COMMAND_ENABLE(IDQUERY, SelectEnable),
	EV_BN_CLICKED(IDQUERY, QueryBNClicked),
	EV_COMMAND_ENABLE(IDLOCAL, SelectEnable),
	EV_BN_CLICKED(IDLOCAL, LocalBNClicked),
	EV_BN_CLICKED(IDGLOBAL, GlobalBNClicked),
	EV_COMMAND_ENABLE(IDEISPP, EisppBNEnable),
	EV_BN_CLICKED(IDEISPP, EisppBNClicked),
//{{TElectricListDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TElectricListDialog Implementation}}

TElectricListDialog::TElectricListDialog(TWindow* parent, TRCDKeyContainer *container, TPoint *tWhere)
	: TGroupListDialog(parent, IDD_ELECTRIC_LIST), lastTicks(0), where(tWhere)
{
	primary = kind2RCDKeyContainer(container->kind);
	*primary << container;

	label = new TStatic(this, IDC_ELECTRIC_LIST_LABEL);
	electrics = new TElectricListWindow(this, IDC_GROUP_LIST_WINDOW, primary);
	new TElectricPushButton(this, IDLIST);
	new TElectricPushButton(this, IDBLUE);
	new TElectricPushButton(this, IDQUERY);
	save = new TElectricPushButton(this, IDSAVE);
	local = new TElectricPushButton(this, IDLOCAL);
	eispp = new TElectricPushButton(this, IDEISPP);

	if (CHROME_EFILE_MENUS)
		global = new TColorButton(this, IDGLOBAL, COLOR_BUTTON_TEXT);
	else
		global = new TStatic(this, IDC_ELECTRIC_LIST_HEADER);
}

TElectricListDialog::~TElectricListDialog()
{
	delete where;
	delete primary;
}

void TElectricListDialog::SetupGlobalText()
{
	mstr m;
	m.printf("%s %ld/%ld", Kind->Seek(primary->kind), primary->key.no, primary->key.year);

	if (strchr(KIND_LAWSUITS, primary->kind))
	{
		const TLawsuit *lawsuit = TYPECHECK_PTR_CAST(TLawsuit, primary);
		TArchive archive;
		archive.key = lawsuit->key;
		bool archived = archive.Try("F_KILL_DATE");

		if ((lawsuit->flags & LAWSUIT_LIMITED) || archived)
		{
			m.cat("\n");

			if (lawsuit->flags & LAWSUIT_LIMITED)
			{
				m.cat("ÄÅËÎ Ñ ÎÃÐÀÍÈ×ÅÍ ÄÎÑÒÚÏ");
				if (archived)
					m.cat(", ");
			}
			if (archived)
				m.cat(archive.killDate.Empty() ? "Àðõèâèðàíî" : "ÓÍÈÙÎÆÅÍÎ");
		}
	}

	global->SetCaption(str(m));
}

TIArray<TElectricListDialog> TElectricListDialog::Dialogs;

void TElectricListDialog::Signal(int sigtype, const TRCDKeyContainer *container, const void *data)
{
	for (int i = 0; i < Dialogs.Count(); i++)
		Dialogs[i]->SignalElectric(sigtype, container, data);
}

void TElectricListDialog::SignalElectric(int sigtype, const TRCDKeyContainer *container, const void *data)
{
	try
	{
		electrics->Signal(sigtype, container, data);
	}
	catch (xmsg& x)
	{
		HandleException(x.why().c_str());
	}
	catch (...)
	{
		HandleException(UNHANDLED_EXCEPT);
	}
}

int TElectricListDialog::ForEach(int command, void *argument)
{
	for (int i = 0; i < Dialogs.Count(); i++)
	{
		TElectricListDialog *dialog = Dialogs[i];

		switch (command)
		{
			case ELECTRIC_COUNT : break;
			case ELECTRIC_SHOW : dialog->ShowWindow(argument == VOID_TRUE ? SW_SHOW : SW_HIDE); break;
			case ELECTRIC_ENABLE : dialog->EnableWindow(argument == VOID_TRUE); break;
			case ELECTRIC_REMOVE : if (argument == dialog) Dialogs.Detach(i); break;
			case ELECTRIC_CLOSE : dialog->CloseDialog(IDCANCEL); break;
			case ELECTRIC_VERIFY : if (argument == dialog) return -1; break;
			case ELECTRIC_HANDLE : if (argument == dialog->Handle) return -1; break;
			default : error("unknown e-command %d", command);
		}
	}

	return Dialogs.Count();
}

void TElectricListDialog::ElectricButton(void (TElectricListWindow::*ElectricAction)())
{
	if (big_sas_lock(this))
	{
		(electrics->*ElectricAction)();
		big_sas_unlock();
	}
}

void TElectricListDialog::ElectraButton(TControl *button, TRCDKeyContainer *container, const TElectra *electras)
{
	if (GetTickCount() > lastTicks + 100)
	{
		TRect rect = button->GetWindowRect();
		electrics->ElectraMenu(container, electras, (rect.left + rect.right) / 2, rect.bottom, TPM_CENTERALIGN);
		lastTicks = GetTickCount();
	}
}

int TElectricListDialog::lastWin = 0;

void TElectricListDialog::SetupWindow()
{
	TGroupListDialog::SetupWindow();

	save->EnableWindow(Default->post == POST_ADMIN);
	SetupGlobalText();
	global->ShowWindow(SW_SHOW);

	if (CHROME_EFILE_MENUS)
		local->ShowWindow(SW_SHOW);

#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
	if (primary->key.type == TYPE_PUNISHMENT_LAW)
		eispp->ShowWindow(SW_SHOW);
#endif  // APPEAL || AREAL || MILITARY || REGIONAL

	if (!where)
	{
		where = new TPoint(0, 0);

		if (!Dialogs.Count())
			lastWin = 0;

		int CX = GetSystemMetrics(SM_CXMAXIMIZED);
		int CY = GetSystemMetrics(SM_CYMAXIMIZED);

		where->x = label->Attr.Y * lastWin * (CX - TWindow::Attr.W) / (CY - TWindow::Attr.H - AppMainBottom);
		where->y = AppMainBottom + label->Attr.Y * lastWin;
		// [unneeded] where->x + TWindow::Attr.W > CX ||
		if (where->y + TWindow::Attr.H > CY || where->x < 0 || where->y < 0)
		{
			lastWin = 0;
			where->x = 0;
			where->y = AppMainBottom;
		}
		else
			lastWin++;
	}

	MoveWindow(where->x, where->y, TWindow::Attr.W, TWindow::Attr.H);
	SendMessage(WM_SETICON, TRUE, (LPARAM) AppNameIcon);
	Dialogs.Add(this);
}

void TElectricListDialog::CleanupWindow()
{
	// simulate appname repeater; foreach must be checked 1st
	if (!electric_foreach(ELECTRIC_REMOVE, this))
		AppMainWindow->PostMessage(WM_COMMAND, CM_ELECTRIC_BLUE, NULL);

	TGroupListDialog::CleanupWindow();
}

void TElectricListDialog::ListBNClicked()
{
	ElectricButton(&TElectricListWindow::ElectricList);
}

void TElectricListDialog::BlueBNEnable(TCommandEnabler &tce)
{
	TRCDKeyContainer *container = electrics->Container();
	tce.Enable(container && container->key != primary->key && strchr(TYPE_ELECTRICABLES, container->key.type));
}

void TElectricListDialog::BlueBNClicked()
{
	ElectricButton(&TElectricListWindow::ElectricBlue);
}

void TElectricListDialog::SaveBNEnable(TCommandEnabler &tce)
{
	tce.Enable(can_func(RIGHT_SAVELECTR));
}

void TElectricListDialog::SaveBNClicked()
{
	ElectricButton(&TElectricListWindow::ElectricSave);
}

void TElectricListDialog::QueryBNClicked()
{
	ElectricButton(&TElectricListWindow::ElectricQuery);
}

void TElectricListDialog::LocalBNClicked()
{
	TRCDKeyContainer *container = electrics->Container();

	if (!conteq(container, primary))
		ElectraButton(local, container, electra_locals);
}

void TElectricListDialog::GlobalBNClicked()
{
	ElectraButton(global, primary, electra_globals);
}

void TElectricListDialog::EisppBNEnable(TCommandEnabler &tce)
{
	TRCDKeyContainer *container = electrics->Container();
	char func = '\0';

	if (container && primary->key.type == TYPE_PUNISHMENT_LAW)
	{
		TLawsuit *lawsuit = (TLawsuit *) primary;

		if (EISPP_ACTIVE && any(lawsuit->eispp_nmr))	// from eispp
		{
			// 2013:037 IRQ: check RESULT_ENTERABLES
			if (strchr(KIND_SESSIONS, container->kind))
			{
				if (!strchr(RESULT_ENTERABLES, ((TSession *) container)->result))
					func = strchr(KIND_OPEN_SESSIONS, container->kind) ? RIGHT_OPEN : RIGHT_CLOSED;
			}
			else if (strchr(KIND_ENDOCS, container->kind))
				func = RIGHT_DECISION;
			else if (container->key.type == TYPE_INREG)
				func = RIGHT_INREG;
			else if (container->key.type == TYPE_OUTREG)
				func = RIGHT_OUTREG;
		}
	}

	tce.Enable(func && can_func(func));
}

void TElectricListDialog::EisppBNClicked()
{
	ElectricButton(&TElectricListWindow::ElectricEispp);
}

TResult TElectricListDialog::EvNotify(uint id, TNotify far& info)
{
	return window_handle_color(info, IDGLOBAL, IDGLOBAL) ? CDRF_NOTIFYPOSTPAINT : TGroupListDialog::EvNotify(id, info);
}
