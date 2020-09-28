DEFINE_RESPONSE_TABLE1(TElectricListWindow, TGroupListWindow)
//{{TElectricListWindowRSP_TBL_BEGIN}}
	EV_WM_RBUTTONUP,
	EV_WM_RBUTTONDOWN,
	EV_WM_KEYUP,
	EV_WM_KEYDOWN,
	EV_WM_SYSKEYUP,
	EV_WM_SYSKEYDOWN,
//{{TElectricListWindowRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TElectricListWindow Implementation}}

TElectricListWindow::TElectricListWindow(TElectricListDialog *parent, int resourceId, TRCDKeyContainer *container)
:
	TGroupListWindow(parent, resourceId, new TGSArray<TRCDKeyContainer>, 232), TElectricCollect(container),
		primary(container), renumber(NULL)
{
	Add(
		new TGroupListColumn("Дата", container_event_date, 52, LVCFMT_CENTER),
		new TGroupListColumn("Събитие", container_event_text, 168),
		NULL
	);

	loadData = false;
	CheckAccess();
}

TElectricListWindow::~TElectricListWindow()
{
	array->DeleteExtraText();
}

void TElectricListWindow::Signal(int sigtype, const TRCDKeyContainer *container, const void *data)
{
	bool primeq = container && conteq(container, primary);

	switch (sigtype)
	{
		case ELECTRIC_START :
		case ELECTRIC_ACCESS : break;
		case ELECTRIC_ADD :
		{
			TRCDKey origin;

			if (strchr(kinds, container->kind) &&
				(strchr(TYPE_ORIGINS, primary->key.type) ? find_origin(container, origin) && origin == primary->key :
				container->key == primary->key))	// 2013:309 LPR: direct key compare for inreg, outreg and prove
			{
				select = true;
				AddCopy(container);
				UpdateVisual();
			}

			break;
		}
		case ELECTRIC_EDIT :
		{
			const char *fields = (const char *) data;

			if (primeq)
			{
				if (container != primary)
					primary->ImportFields(container, fields);

				(dynamic_cast<TElectricListDialog *> (Parent))->SetupGlobalText();
			}

			select = (container == Container());

			if (fields)
			{
				if (UpdateFields(container, fields))
					UpdateVisual();
			}
			else if (Dispose(container))
			{
				AddCopy(container);
				UpdateVisual();
			}

			break;
		}
		case ELECTRIC_RENUMBER :
		{
			if (primeq)
			{
				TRCDKeyContainer *target = ((TRenumberContainer *) container)->target;
				renumber = kind2RCDKeyContainer(target->kind);
				*renumber << target;
			}

			break;
		}
		case ELECTRIC_DELETE :
		{
			if (primeq)
				Parent->ShowWindow(SW_HIDE);
			else if (Dispose(container))
				UpdateVisual();

			break;
		}
		case ELECTRIC_FINAL :
		{
			garbage.DeleteExtraText();
			garbage.Flush();

			if (sigmain == ELECTRIC_EDIT || sigmain == ELECTRIC_ACCESS)
			{
				cause.clear();

				if (sigmain == ELECTRIC_ACCESS &&
					(!can_menu(RIGHT_QUERY) || !can_access_exact(primary, false, true, &cause)))
				{
					Parent->PostMessage(WM_CLOSE);
				}
				else
					CheckAccess();
			}
			else if (sigmain == ELECTRIC_DELETE)
			{
				Parent->PostMessage(WM_CLOSE);

				if (renumber)
					electric_blue(AppMainWindow, renumber);
			}

			// failed _DELETE possible -> delete always
			delete renumber;
			renumber = NULL;
			break;
		}
		default : error("%d: unknown e-signal", sigtype);
	}

	if (!container || primeq)
		sigmain = sigtype;
}

void TElectricListWindow::ElectricList()
{
	electric_items_interact(this, Container(), false, dynamic_cast<const TLawsuit *> (primary));
}

void TElectricListWindow::ElectricBlue()
{
	// N.B. en/disabled by dialog enabler
	electric_blue(AppMainWindow, Container());
}

void TElectricListWindow::ElectricSave()
{
	TPeriodGroup period("T_ELECTRIC");

	if (Container())
		period.minDate = Container()->date;
	else if (array->Count())
		period.minDate = ElementAt(0)->date;

	if (array->Count())
		period.maxDate = ElementAt(array->Count() - 1)->date;

	if (!get_period(this, &period))
		return;

	char fileName[SIZE_OF_DIR];

	if (GetSaveFileName(this, fileName, "HTML файлове", OUT_PATH, "HTML", sizeof fileName))
	{
		TUserFile uf("HTML");
		FILE *f = uf.Open(fileName, "w");

		if (f)
		{
			char dir[0x100];
			char *base = dir;

			strcpy(dir, fileName);
			for (char *s = dir; *s; s++)
				if (strchr(":\\/", *s))
					base = s + 1;

			*base = '\0';
			if (!any(dir))
				strcpy(dir, ".");

			Save(f, &period, dir);
			uf.Close();

			mstr m;
			m.printf("електронна папка %D-%D", &period.minDate, &period.maxDate);
			log(LOG_WRITE, 0, &primary->key, str(m));
		}
	}
}

void TElectricListWindow::ElectricQuery()
{
	TRCDKeyContainer *container = Container();

	// 2002:295 Moved to new electric 2009:071 VIRTUAL check first
	// 2009:150 inline + rider support
	if (strchr(KIND_VIRTUALS, container->kind))
		info("Липсва допълнителна информация.");
	else if (decompressKind(container))
	{
		query_surrouendocs(this, (TSession *) container);
		compressKind(container);
	}
	else if (strchr(KIND_ELEQUERABLES, container->kind))
		query_group(this, container, false);
	else
		query_event(container);
}

void TElectricListWindow::ElectricEispp()
{
	TRCDKeyContainer *container = Container();

	if (strchr(KIND_ENDOCS, container->kind))
	{
		TDecision *decision = (TDecision *) container;
		TSession *session = NULL;
		int index;

		for (index = 0; index < array->Count(); index++)
		{
			TRCDKeyContainer *tContainer = ElementAt(index);

			if (strchr(KIND_SESSIONS, tContainer->kind))
			{
				TSession *session = (TSession *) tContainer;

				if (session->date == decision->date && session->kind == decision->sessionKind)
					break;
			}
		}

		if (index < array->Count())
			eispp_accept(this, (TLawsuit *) primary, session, decision, true);
		else
			error("В електронната папка липсва заседанието, за което е съдебния акт.");
	}
	else if (strchr(KIND_SESSIONS, container->kind))
		eispp_accept(this, (TLawsuit *) primary, (TSession *) container, NULL, true);
}

static bool match1(const TElectra *electra, TRCDKeyContainer *container)
{
	return strchr(electra->types, container->key.type) && strmatch(container->kind, electra->kinds);
}

static const char *match2(const TElectra *electra, TRCDKeyContainer *container, char status)
{
	return !strmatch(status, electra->statuses) ? wrong_status : electra->match ? electra->match(container, status) : NULL;
}

void TElectricListWindow::ElectraMenu(TRCDKeyContainer *container, const TElectra *electras, int x, int y, uint align)
{
	if (compressedKind(container))
		electras = electra_surrouendocs;

	if (CHROME_EFILE_MENUS && big_sas_lock(Parent))
	{
		if (write)
		{
			SetWaitCursor(true);
			char status = '\0';
			TPopupMenu menu;

			SetPrimaryGroup(type2RCDKeyContainer(primary->key.type));
			*_primary_group << primary;

			// create command list
			for (const TElectra *electra = electras; electra->name; electra++)
			{
				if (match1(electra, container))
				{
					uint flags = MF_STRING;

					// 2010:211 FIX: was unchecked in economic mode
					if (!command_enabled(electra->command, electra->menus))
						flags |= MF_GRAYED;
					// jit-check object
					else if (CHROME_EFILE_MENUS == 1)
					{
						if (!status)
							status = container->Status();

						if (match2(electra, container, status) != NULL)
							flags |= MF_GRAYED;
					}

					if (!menu.AppendMenu(flags, electra->command, electra->name))
						fatal("%d: failed to append menu item", electra->command);
				}
			}

			SetWaitCursor(false);

			if (!menu.GetMenuItemCount())
			{
				// no actions available
				MessageBeep(0xFFFFFFFF);
			}
			else
			{
				// select and execute command
				uint flags = align | TPM_TOPALIGN | TPM_NONOTIFY | TPM_RETURNCMD | TPM_LEFTBUTTON;
				// can't use TPopupMenu::TrackPopupMenu(), it returns bool even if TPM_RETURNCMD
				int command = TrackPopupMenu(menu.GetHandle(), flags, x, y, 0, Parent->Handle, NULL);

				if (command > 0 && CHROME_EFILE_MENUS == 2)
				{
					const TElectra *electra;
					const char *ts;

					status = container->Status();

					for (electra = electras; electra->name; electra++)
						if (electra->command == command && match1(electra, container))
							break;

					if (!electra->name)
						fatal("%d: unable to refind command", command);
					else if ((ts = match2(electra, container, status)) != NULL)
					{
						error("%s %D - %s - %s.", Kind->Seek(container->kind), &container->date,
							rrseek(container, status), ts);
						command = -1;
					}
				}

				if (command > 0 && ask_cause(cause))
				{
					PrimaryCommand = command;
					*PrimaryTitle = '\0';

					electric_signal(ELECTRIC_START, NULL);

					try
					{
						electra_action(Parent, command, container);
					}
					catch (xmsg& x)
					{
						handle_exception(x.why().c_str());
						Parent->SetActiveWindow();
					}
					catch (...)
					{
						handle_exception(UNHANDLED_EXCEPT);
						Parent->SetActiveWindow();
					}

					electric_signal(ELECTRIC_FINAL, NULL);
				}
			}

			SetPrimaryGroup(NULL);
		}
		else if (any(cause))
			error("%s.", str(cause));

		big_sas_unlock();
	}
}

void TElectricListWindow::CheckAccess()
{
	cause.clear();
	write = can_access_exact(primary, true, true, &cause);
}

static bool riderq(const TRCDKeyContainer *container, const TRCDKeyContainer *rider)
{
	constant sessionKind = strchr(KIND_SESSIONS, container->kind);

	return (strchr(KIND_ENDOC_RIDERS, rider->kind) && conteq(container, ((const TDecisionRider *) rider)->decision)) ||
		(sessionKind && compressedKind(rider) && rider->kind == KIND_SURROUENDOC_BASE + (sessionKind - KIND_SESSIONS) &&
		container->key == rider->key && container->date == rider->date);
}

bool TElectricListWindow::Dispose(const TRCDKeyContainer *filter)
{
	bool disposed = false;
	int index = 0;

	while (index < array->Count())
	{
		TRCDKeyContainer *element = ElementAt(index);

		if (conteq(element, filter) || riderq(filter, element))	// 2011:234 LPR: +riderq()
		{
			RemoveVisual(index);
			garbage.Add(element);
			array->Detach(index);
			disposed = true;
		}
		else
			index++;
	}

	return disposed;
}

bool TElectricListWindow::UpdateFields(const TRCDKeyContainer *container, const char *fields)
{
	for (int index = 0; index < array->Count(); index++)
	{
		TRCDKeyContainer *element = ElementAt(index);

		if (conteq(element, container))
		{
			element->ImportFields(container, fields);
			return true;
		}
	}

	return false;
}

void TElectricListWindow::ElectraDevice(TPoint &point)
{
	TRCDKeyContainer *container = Container();

	if (container && !conteq(container, primary))
		ElectraMenu(container, electra_locals, point.x, point.y, TPM_LEFTALIGN);
}

void TElectricListWindow::ElectraButton(TPoint &point, bool action)
{
	int index = HitTest(TLwHitTestInfo(point));

	if (index >= 0)
	{
		SetVisualIndex(index);

		if (action)
		{
			ClientToScreen(point);
			ElectraDevice(point);
		}
	}
}

void TElectricListWindow::Save(FILE *f, TPeriodGroup *period, const char *dir)
{
	mstr m;

	fputs("<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\">\n", f);
	fputs("<html>\n<head>\n", f);
	fputs("<meta http-equiv=\"Content-Type\" content=\"text/html; charset=windows-1251\">\n", f);
	m.printf("Движение на %s %ld/%ldг. в периода от %D до %D", Type->Seek(primary->key.type), primary->key.no,
		primary->key.year, &period->minDate, &period->maxDate);
	fprintf(f, "<title>%s</title>\n</head>\n<body>\n<center>\n<p><b>%s</b></p></center>\n<table>\n\n", str(m), str(m));

	for (int index = 0; index < array->Count(); index++)
	{
		TRCDKeyContainer *container = ElementAt(index);

		if (container->date >= period->minDate && container->date <= period->maxDate)
		{
			mstr date, event;

			container_event_date(date, container);
			container_event_text(event, container);
			fprintf(f, "<tr>\n<td>%s </td><td> %s", str(date), str(event));

			TGSArray<TElectric> electrics;
			electric_items_collect(container, electrics);

			for (int i = 0; i < electrics.Count(); i++)
			{
				TElectric *electric = electrics[i];
				const char *source = electric->extraName;
				char target[SIZE_OF_PATH];

				electric_name(container, dir, target, electric->extraType,
					electric->extraNo ? electric->extraNo : -1);	// avoid "*"

				// 2017:003 TRQ/FIX: preserve secondary ext-s
				strcpy(file_ext(target), file_ext(electric->extraName));

				if (copy_overead_file(source, target, false, true) && electric->extraType == TYPE_HTML)
					html_group_action(source, copy_overead_file, target);

				if (exist(target))
				{
					char *relname = target + strlen(dir);
					mstr m;

					for (char *url = relname; (url = strchr(url, '\\')) != NULL; *url = '/');
					fprintf(f, "%s <a href=\"%s\">", i ? "," : " -", relname);

					if (electric->extraNo)
					{
						electric->Print(m);
						m.cat(" - ");
						THTMLFile::puts(f, str(m), '\n');
					}

					fprintf(f, "%s документ", Electrics[electric->extraType].what);
					fputs("</a>", f);
				}
			}

			fputs("</td>\n", f);
		}
	}

	fputs("\n</table>\n</body>\n</html>\n", f);
}

void TElectricListWindow::AddItem(TRCDKeyContainer *container)
{
	// 2013:242 LPR:
	if (inReg && container->key.type == TYPE_OUTREG && !((TOutReg *) container)->aret)
	{
		for (int i = 0; i < array->Count(); i++)
		{
			TRCDKeyContainer *tContainer = ElementAt(i);

			if (tContainer->key == container->key && !((TOutReg *) tContainer)->aret)
			{
				delete container;		// 2016:271 LPR: fix leak
				return;
			}
		}
	}

#if TESTVER
	int index = ((TGSArray<TRCDKeyContainer> *) array)->Add(container);

	InsertVisual(index);
#else  // TESTVER
	int index = InsertGroup(container);
#endif  // TESTVER

	if (select)
		SetVisualIndex(index);
}

TGroup *TElectricListWindow::NewGroup() const
{
	return new TDummyContainer;
}

void TElectricListWindow::Refresh()
{
	select = false;
	Collect();
	TGroupListWindow::Refresh();
}

void TElectricListWindow::EvRButtonUp(uint, TPoint &point)
{
	ElectraButton(point, false);
}

void TElectricListWindow::EvRButtonDown(uint, TPoint &point)
{
	ElectraButton(point, true);
}

void TElectricListWindow::EvKeyUp(uint key, uint repeatCount, uint flags)
{
	TGroupListWindow::EvKeyUp(key, repeatCount, flags);

	if (key == VK_INSERT || key == VK_APPS)
	{
		TRect rect;
		TPoint point;

		if (GetItemRect(GetVisualIndex(), &rect, Bounds))
		{
			point.x = rect.left;
			point.y = rect.bottom;
			ClientToScreen(point);
		}
		else
			GetCursorPos(point);

		ElectraDevice(point);
	}
	else if (key >= VK_F1 && key <= VK_F12)
		AppMainWindow->PostMessage(WM_KEYUP, key, repeatCount + (flags << 16));
}

void TElectricListWindow::EvKeyDown(uint key, uint repeatCount, uint flags)
{
	TGroupListWindow::EvKeyDown(key, repeatCount, flags);

	if (key >= VK_F1 && key <= VK_F12)
		AppMainWindow->PostMessage(WM_KEYDOWN, key, repeatCount + (flags << 16));
}

void TElectricListWindow::EvSysKeyUp(uint key, uint repeatCount, uint flags)
{
	TGroupListWindow::EvSysKeyUp(key, repeatCount, flags);

	if (key >= VK_F1 && key <= VK_F12)
		AppMainWindow->PostMessage(WM_SYSKEYUP, key, repeatCount + (flags << 16));
}

void TElectricListWindow::EvSysKeyDown(uint key, uint repeatCount, uint flags)
{
	TGroupListWindow::EvSysKeyDown(key, repeatCount, flags);

	if (key >= VK_F1 && key <= VK_F12)
		AppMainWindow->PostMessage(WM_SYSKEYDOWN, key, repeatCount + (flags << 16));
}
