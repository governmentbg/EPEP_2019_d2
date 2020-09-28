DEFINE_RESPONSE_TABLE1(TSubpoenaDialog, TResizeDialog)
//{{TSubpoenaDialogRSP_TBL_BEGIN}}
	EV_LVN_ITEMCHANGED(IDC_SUBPOENA_SUBPOENAS, SubpoenasLVNItemChanged),
	EV_LVN_ITEMCHANGING(IDC_SUBPOENA_SUBPOENAS, SubpoenasLVNItemChanging),
	EV_BN_CLICKED(IDNOTIFY, NotifyBNClicked),
	EV_BN_CLICKED(IDDELETE, DeleteBNClicked),
	EV_BN_CLICKED(IDLETTER, LetterBNClicked),
	EV_BN_CLICKED(IDPRINT, PrintBNClicked),
	EV_BN_CLICKED(IDRETURN, ReturnBNClicked),
	EV_BN_CLICKED(IDOUTREG, OutRegBNClicked),
	EV_BN_CLICKED(IDNUMBER, NumberBNClicked),
	EV_BN_CLICKED(IDVIEW, ViewBNClicked),
	EV_BN_CLICKED(IDEDIT, EditBNClicked),
//{{TSubpoenaDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TSubpoenaDialog Implementation}}

TSubpoenaDialog::TSubpoenaDialog(TWindow* parent, TRCDKeyContainer *tContainer, char tStatus, TSideWind *tSideWind,
	int tSideIndex, TGArray<TLinkWind> *tLinks, TGArray<TSideWind> *allSides, TGArray<TUCNGroup> *affecteds,
	const TLawsuit *lawsuit)
:
	TResizeDialog(parent, SHOW_LARGER, IDD_SUBPOENA), container(tContainer), status(tStatus), sideWind(tSideWind),
		sideIndex(tSideIndex), links(tLinks), variant(sideWind, links, allSides, affecteds, tContainer, lawsuit),
		apcTemplate(false)
{
#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_ADMIN || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY
	get_container_kinds(container, originKind, secondKind);
#endif  // APPEAL || ADMIN || AREAL || MILITARY
#if COURT_TYPE == COURT_REGIONAL
	TRCDKey origin;	// 2014:035 check regional subject

	if (find_origin(container, origin))
	{
		get_container_kinds(container, originKind, secondKind, &origin);

		char apcType = kind2type(originKind);

		if (!strchr(TYPE_APC_LAWS, apcType))
			apcType = kind2type(secondKind);

		if (strchr(TYPE_APC_LAWS, apcType))
		{
			TOriginContainer *const container = (TOriginContainer *) type2RCDKeyContainer(origin.type);
			TVirtPtr VP(container);

			container->key = origin;

			if (container->Get("F_SUBJECT") && container->subject)
			{
				Subject->Seek(apcType, container->subject);
				apcTemplate = Subject->sd[0].flags & SUBJECT_APC_TMPL;
			}
		}
	}
	else
		get_container_kinds(container, originKind, secondKind);
#endif  // REGIONAL

	name = new TStatic(this, IDC_SUBPOENA_NAME);
	text = new TEdit(this, IDC_SUBPOENA_TEXT);
	count = new TStatic(this, IDC_SUBPOENA_COUNT);
	autoSize = subpoenas = new TSubpoenaListWindow(this, IDC_SUBPOENA_SUBPOENAS, container, sideWind);
	cancel = new TButton(this, IDCANCEL);

	autoDown[0] = print = new TButton(this, IDPRINT);
	autoDown[1] = notify = new TButton(this, IDNOTIFY);
	autoDown[2] = remand = new TButton(this, IDRETURN);
	autoDown[3] = erase = new TButton(this, IDDELETE);
	autoDown[4] = letter = new TButton(this, IDLETTER);
	autoDown[5] = outreg = new TButton(this, IDOUTREG);
	autoDown[6] = view = new TButton(this, IDVIEW);
	autoDown[7] = number = new TButton(this, IDNUMBER);
	autoDown[8] = edit = new TButton(this, IDEDIT);
}

void TSubpoenaDialog::Resize()
{
	TResizeDialog::Resize();
	TResizeDialog::Resize(text, RESIZE_WIDTH);
	TResizeDialog::Resize(cancel, RESIZE_EDGE);
}

void TSubpoenaDialog::ComputeKinds()
{
	bool motivated = strchr(KIND_ENDOCS, container->kind) && ((TDecision *) container)->Motivated();
	char sk;

	for (int i = 0; (sk = SUBPOENA_KIND_ALLS[i]) != '\0'; i++)
	{
		// TODO? replace with T_SUBPOENA_KIND metachar for "any non-zero lawsuit kind"
		if (sk == SUBPOENA_KIND_LEGAL_AID_TYPED && strchr(KIND_INREG_EVENTS, container->kind))
		{
			TInReg inReg;
			inReg.key = container->key;

			if (inReg.Try("F_LAWSUIT_TYPE") && !inReg.lawsuit.type)
				continue;
		}

		SubpoenaKind->Seek(sk);

		// 2018-04-19: LPR: block all _ elements
		if (SubpoenaKind->proc != SK_PROC_NOTIFYING && !any(SubpoenaKind->htemplate))
			continue;

		// 2013:071 LPR: made primary
		if (SubpoenaKind->ForceDate())
		{
			TDate *forceDate = (TDate *) container->Find("F_FORCE_DATE");
			if (!forceDate || forceDate->Date().Empty())
				continue;
		}

	#if COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
		// 2009:231; 2011:230 IRQ: all courts; 2012:045 LPR: specific courts
		// 2013:071 LPR: made separate-secondary
		if (strchr(SUBPOENA_KIND_CONVICT_BULLETS, sk))
		{
			// local judgement force date, not container date as above
			TDate *forceDate = (TDate *) sideWind->Find("F_FORCE_DATE");
			if (!forceDate || forceDate->Date().Empty())
				continue;
		}
	#endif  // AREAL || MILITARY || REGIONAL

		if (COURT_LOCK && strchr(SUBPOENA_KIND_LOCKEDS, sk))
			continue;

	#if COURT_TYPE == COURT_REGIONAL
		if (strchr(SUBPOENA_KIND_APC_TEMPLATES, sk) && !apcTemplate)
			continue;
		// 2018-08-09: to avoid subpoena kind changes on result chahges
		if (sk == SUBPOENA_KIND_MESSAGE_415_19B && strchr(KIND_SESSIONS, container->kind))
		{
			bool matches = strchr(RESULT_FINALIZEDS, status) &&
				(strchr(RESULT_SETTLEMENTABLES, status) || strchr(RESULT_ORDERABLES, status));

			if (!matches)
				continue;
		}
	#endif  // REGIONAL

	#if TESTVER
		if (sk == '?')
		{
			message("k = %d, s = %d, i = %d, lk = %d",
				strinex(SubpoenaKind->kinds, kind2VisibleKind(container->kind)),
				strinex(SubpoenaKind->statuses, status),
				strinex(SubpoenaKind->involvements, sideWind->involvement),
				strinex2(SubpoenaKind->lawsuitKinds, originKind, secondKind));
		}
	#endif  // TESTVER

		if (sk == SUBPOENA_KIND_LEGAL_AID_TYPED && !strchr(UCN_CITIZENS, sideWind->ucnType))
			continue;

		if (strinex(SubpoenaKind->kinds, kind2VisibleKind(container->kind)) &&
			strinex(SubpoenaKind->statuses, status) &&
			strinex(SubpoenaKind->involvements, sideWind->involvement) &&
			// 2012:039 !any() || strchr() -> strinex(); 2012:132 inex2
			strinex2(SubpoenaKind->lawsuitKinds, originKind, secondKind))
		{
			mstr *subpKinds;

		#if RANGECHECK
			if (sk == SUBPOENA_KIND_MOTIVES_NOTIFY && !strchr(KIND_ENDOCS, container->kind))
				fatal("%d: kind %d is not an endoc", SUBPOENA_KIND_MOTIVES_NOTIFY, container->kind);
		#endif  // RANGECHECK

			// 2016:069 TRQ: FIX: after strinex() checks
			if (sk == SUBPOENA_KIND_MOTIVES_NOTIFY && !motivated)
				continue;

			if (strchr(SUBPOENA_KIND_STICKERS, sk))
				subpKinds = &(LINK_NOTIFICATION ? stickerKinds : printKinds);
			else
			{
				switch (SubpoenaKind->proc)
				{
					case SK_PROC_LETTER_TO : subpKinds = &letterKinds; break;
					case SK_PROC_STANDALONE_LETTER : subpKinds = &aloneKinds; break;
					case SK_PROC_NOTIFYING :
					case SK_PROC_DELIVER_COPY : subpKinds = &notifyKinds; break;
					default : subpKinds = SubpoenaKind->PrintingRequired() ? &printKinds : NULL;
				}
			}

			if (subpKinds)
				subpKinds->cach(sk);
		}
	}
}

void TSubpoenaDialog::AddNotifyKinds(mstr &tNotifyKinds, const TSubpoena *sentSubpoena)
{
	if (sentSubpoena && sentSubpoena->eventStatus == status)
	{
		if ((sentSubpoena->byPost & OUTREG_WRITE_MAIL))
			tNotifyKinds.cach(SUBPOENA_KIND_DELIVERY_NOTICE);

		SubpoenaKind->Seek(sentSubpoena->subpoenaKind);

		if (SubpoenaKind->AddressRequired() && SubpoenaKind->PrintingRequired() &&
			!strchr(SUBPOENA_KIND_STICKERS, sentSubpoena->subpoenaKind))  // 2018-05-21 
		{
			tNotifyKinds.cat(stickerKinds);	// LINK_NOTIFICATION checked on compute
		}
	}
}

void TSubpoenaDialog::ItemChanged()
{
	TSubpoena *subpoena = subpoenas->Subpoena();

	// 2010:025 LPR/LRQ: lock everything for MULTI
	if (subpoena && subpoena->multi != SUBPOENA_MULTI)
	{
		SubpoenaKind->Seek(subpoena->subpoenaKind);

		// 2010:020 LPR: remark non-servables instead of return
		bool tRemand = !subpoena->requestId || !strchr(SUBPOENA_KIND_LEGAL_AIDS, subpoena->subpoenaKind);

		if (strchr(SUBPOENA_KIND_STICKERS, subpoena->subpoenaKind) && subpoena->regard)
			tRemand = false;

		remand->SetCaption(SubpoenaKind->MayServe() ? "&Връщане" : "&Забележка");
		remand->EnableWindow(tRemand);
		view->EnableWindow(true);

		// 2011:202 LRQ: support 2+ statuses
		if (subpoena->eventStatus == status)
		{
			erase->EnableWindow(!subpoena->outRegNo || can_func(RIGHT_DEL_IO));
			letter->EnableWindow(any(letterKinds) && SubpoenaKind->LetterAllowed());
			outreg->EnableWindow(!subpoena->outRegNo && can_func(RIGHT_OUTREG) && SubpoenaKind->OutRegAllowed());
			// 2015:033
			bool editable = false;
			// 2018-07-23: IRQ: re-allow edit exec list with letters for admins
			if (can_func(RIGHT_EXLSTEDIT) && strchr(SUBPOENA_KIND_EXEC_LISTS, subpoena->subpoenaKind) &&
				strchr(SRS_INITIALS, subpoena->receivedStatus) && subpoena->subpoenaDate.year == Today.year &&
				(!subpoenas->FindLetter(subpoena) || Default->post == POST_ADMIN))
			{
				TExecList execList;

				execList << subpoena;
				execList.execListYear = subpoena->subpoenaDate.year;
				editable = !(subpoena->subpoenaKind == SUBPOENA_KIND_EXEC_COUNTRY && execList.Try("F_NIA_NO") &&
					execList.nia.no);
			}

			edit->EnableWindow(editable);
		#if COURT_TYPE == COURT_REGIONAL
			bool numbered = subpoena->subpoenaKind == SUBPOENA_KIND_DIVORCE_MESSAGE;

			number->ShowWindow(numbered ? SW_SHOW : SW_HIDE);
			edit->ShowWindow(numbered ? SW_HIDE : SW_SHOW);
			number->EnableWindow(numbered && !subpoena->announceNo);
		#endif  // REGIONAL
		}
		else
		{
			erase->EnableWindow(false);
			letter->EnableWindow(false);
			outreg->EnableWindow(false);
			number->EnableWindow(false);
			edit->EnableWindow(false);
		}
	}
	else
	{
		remand->EnableWindow(false);
		remand->SetCaption("&Връщане");
		erase->EnableWindow(false);
		letter->EnableWindow(any(aloneKinds));
		outreg->EnableWindow(false);
		number->EnableWindow(false);
		view->EnableWindow(false);
		edit->EnableWindow(false);
	}

	mstr m;

	if (subpoena)
	{
		SubpoenaKind->Seek(subpoena->subpoenaKind);
		subpoena->PrintDescription(m);
	}

	text->SetText(str(m));
	m.clear();
	m.printf("Брой: %d", subpoenas->Count());
	count->SetText(str(m));
}

// 2008:171 LPR: multi-notify and new link selection
bool TSubpoenaDialog::ChooseSubpoenaVariant(TSubpoena *subpoena, const char *subpoenaKinds, bool allowLinks)
{
	variant.flags = 0;
	variant.flagsX = 0;
	delete variant.execList;
	variant.execList = NULL;
	variant.receiver.clear();
	variant.subpoenaDate = Today;

	if (TSubpoenaVariantDialog(this, &variant, subpoenaKinds, allowLinks).Execute() != IDOK)
		return false;

	subpoena->subpoenaKind = variant.subpoenaKind;
	subpoena->byPost = variant.flags;
	subpoena->byPostX = variant.flagsX;
	subpoena->subpoenaDate = variant.subpoenaDate;
	subpoena->validUntil = variant.validDate;
	return true;
}

void TSubpoenaDialog::PrintSubpoena(TSubpoena &subpoena)
{
	sign result = -1;
	SubpoenaKind->Seek(subpoena.subpoenaKind);

	switch (SubpoenaKind->proc)
	{
		case SK_PROC_TELEGRAM :
		{
			result = TTelegram(this, sideIndex, status, subpoena, container).Notify(&variant);
			break;
		}
		case SK_PROC_STATE_PAPER :
		{
			result = TStatePaper(this, sideIndex, status, subpoena, container).Notify(&variant);
			break;
		}
		case SK_PROC_ANNOUNCE :
		{
			if (strchr(KIND_INREG_EVENTS, container->kind))
				result = TInRegEventAnnounce(this, sideIndex, status, subpoena, container).Notify(&variant);
			else if (container->kind == KIND_REQUEST_RESOLUTION)
				result = TResolutionAnnounce(this, sideIndex, status, subpoena, container).Notify(&variant);
			else if (strchr(KIND_ENDOCS, container->kind))
				result = TDecisionAnnounce(this, sideIndex, status, subpoena, container).Notify(&variant);
			else if (strchr(KIND_SESSIONS, container->kind))
			{
				result = TNotifySubpoena(this, sideIndex, status, subpoena, container).Notify(&variant);
			}
			// else: remains -1
		}
		// default: remains -1
	}

	if (result == -1)
		fatal("proc %d, kind %d not supported by print subpoena", SubpoenaKind->proc, container->kind);
	else if (result)
	{
		subpoenas->InsertGroup(&subpoena);
		ItemChanged();
	}
}

void TSubpoenaDialog::SetupWindow()
{
	TResizeDialog::SetupWindow();

	ComputeKinds();
	print->EnableWindow(any(printKinds));
	ItemChanged();

	// 2009:327 LPR: was missing
	mstr m;

	sideWind->Print(m);
	m.compress();

	// 2018-10-25
	if (variant.lawsuit && strchr(KIND_LAWSUITS, variant.lawsuit->kind) && eportal_side_assigned(sideWind) &&
		eportal_invoke_granted(variant.lawsuit->key, sideWind->ucnType, sideWind->ucn, sideWind->involvement))
	{
		m.cat(" - ЕЛЕКТРОННО ПРИЗОВАВАНЕ ПО ДЕЛОТО");
	}

	name->SetText(str(m));
}

TResult TSubpoenaDialog::EvNotify(uint id, TNotify far& notifyInfo)
{
	return (notifyInfo.code == HDN_BEGINTRACK || notifyInfo.code == HDN_ITEMDBLCLICK) &&
		((THdrNotify &) notifyInfo).iItem == 0 ? TRUE : TResizeDialog::EvNotify(id, notifyInfo);
}

void TSubpoenaDialog::SubpoenasLVNItemChanged(TLwNotify &)
{
	ItemChanged();
}

bool TSubpoenaDialog::SubpoenasLVNItemChanging(TLwNotify &lwn)
{
	return subpoenas->ItemChanging(lwn);
}

void TSubpoenaDialog::NotifyBNClicked()
{
	TSubpoena *sentSubpoena = subpoenas->Subpoena();
	TSubpoena subpoena;

	mstr tNotifyKinds(notifyKinds);
	AddNotifyKinds(tNotifyKinds, sentSubpoena);

	if (ChooseSubpoenaVariant(&subpoena, str(tNotifyKinds), true))
	{
		if (strchr(SUBPOENA_KIND_STICKERS, subpoena.subpoenaKind))
		{
			subpoena.regard = sentSubpoena->subpoenaNo;
			PrintSubpoena(subpoena);
			return;
		}

		SubpoenaKind->Seek(subpoena.subpoenaKind);

		if (TSWNotify(this, sideIndex, status, subpoena, container, sentSubpoena).Notify(&variant))
		{
			bool session = strchr(KIND_SESSIONS, container->kind);

			if (SubpoenaKind->MaySchedule() && !(session && status == RESULT_FIXED))
			{
				schedule(Container2Sched(container), container, &subpoena.subpoenaDate);

				if (session && container->key.type != TYPE_PUNISHMENT_LAW && status == RESULT_STOPPED)
					schedule(SCHED_STOPPED_M, container, &subpoena.subpoenaDate);
			}

			subpoenas->InsertGroup(&subpoena);
			ItemChanged();
		}
	}
}

void TSubpoenaDialog::DeleteBNClicked()
{
	if (subpoenas->Delete(&variant))
		ItemChanged();
}

void TSubpoenaDialog::LetterBNClicked()
{
	TSubpoena subpoena;
	TSubpoena *sentSubpoena = subpoenas->Subpoena();

	if (ChooseSubpoenaVariant(&subpoena, str(sentSubpoena ? letterKinds : aloneKinds), false))
	{
		TSubpoena fakeSubpoena;

		if (!sentSubpoena)
		{
			fakeSubpoena << sideWind;
			fakeSubpoena.eventStatus = status;
			fakeSubpoena.subpoenaNo = 0;
			fakeSubpoena.officiallyNotified = SUBPOENA_OFFICIALLY_NONNOTIFIED;	// ?-)
			sentSubpoena = &fakeSubpoena;
		}

		if (TLetterTo(this, sideIndex, status, subpoena, container, sentSubpoena).Notify(&variant))
		{
			subpoenas->InsertGroup(&subpoena);
			ItemChanged();
		}
	}
}

void TSubpoenaDialog::PrintBNClicked()
{
	TSubpoena subpoena;

	if (ChooseSubpoenaVariant(&subpoena, str(printKinds), true))
		PrintSubpoena(subpoena);
}

void TSubpoenaDialog::ReturnBNClicked()
{
	if (subpoenas->ReturnGroup(originKind, secondKind))
		ItemChanged();
}

void TSubpoenaDialog::OutRegBNClicked()
{
	TSubpoena *subpoena = subpoenas->Subpoena();

	variant.flags = subpoena->byPost & OUTREG_POSTAL;
	variant.flagsX = subpoena->byPost & ~OUTREG_POSTAL;

	if (TPostalDialog(this, &variant).Execute() == IDOK)
	{
		TOutReg outReg;

		outReg.kind = KIND_OUT_LETTER;
		outReg.date = can_back_date() ? subpoena->subpoenaDate : Today;
		outReg.lawsuit = container->key;
		// 2009:348
		outReg.flags = variant.flags;
		outReg.flagsX = variant.flagsX;

		outReg.text.cpy(SubpoenaKind->Seek(subpoena->subpoenaKind));
		outReg.text.cat(" на ");
		sideWind->PrintNames(outReg.text);
		outReg.resultII = ACCOMPLY_ACCOMPANY_LETTER;
		outReg.resultKS = ACCOMPLY_ACCOMPANY_LETTER;
		outReg.resultMJ = ACCOMPLY_ACCOMPANY_LETTER;

		char college = find_college(container);

		if (college)
			outReg.college = college;

		if (outReg.Insert())
		{
			log(LOG_SEND, 0, &outReg, "на писма");

			subpoena->outRegNo = outReg.key.no;
			subpoena->outRegYear = outReg.key.year;
			subpoena->byPost = variant.flags;
			subpoena->byPostX = variant.flagsX;

			if (subpoena->requestId && strchr(SUBPOENA_KIND_LEGAL_AIDS, subpoena->subpoenaKind))
			{
				msql m(subpoena->Name, MQ_UPDATE);

				subpoena->Quote(m, "F_OUTREG_NO, F_OUTREG_YEAR, F_BY_POST");
				m.Add(subpoena, "F_NO, F_YEAR, F_TYPE, F_REQUEST_ID");
				subpoena->DataBase->Execute(str(m), ET_ERROR);
			}
			else
				subpoena->Update("F_OUTREG_NO, F_OUTREG_YEAR, F_BY_POST");

			TSideWind tSideWind;

			tSideWind.key = outReg.key;
			tSideWind.kind = KIND_OUT_LETTER;
			tSideWind.date = outReg.date;

			if (atof(subpoena->lReceiverUcn))
			{
				tSideWind.ucnType = subpoena->lReceiverUcnType;
				strcpy(tSideWind.ucn, subpoena->lReceiverUcn);
			}
			// 2014:027 LPR: legal support
			else if (strchr(SUBPOENA_KIND_LEGAL_AIDS, subpoena->subpoenaKind))
			{
				TFirm firm;

				create_lawer_college(&firm);
				tSideWind.ucnType = firm.ucnType;
				strcpy(tSideWind.ucn, firm.ucn);
			}
			// 2009:239 LPR: non-lReceiver out# support
			else
			{
				tSideWind.ucnType = subpoena->ucnType;
				strcpy(tSideWind.ucn, subpoena->ucn);
			}
			tSideWind.involvement = INVOLVEMENT_RECEIVER;
			tSideWind.Insert();

			// 2013:163 IRQ/LRQ: attach to outreg
			char subName[SIZE_OF_PATH];
			char outName[SIZE_OF_PATH];

			electric_name(subpoena, subName, TYPE_TEXT);
			electric_name(&outReg, Electrics[TYPE_TEXT].docketPath, outName, TYPE_TEXT);

			if (exist(subName))
				copy_overead_file(subName, outName, false, true);

			// 2014:006 LRQ/IRQ: send legal aid
			legalaid_rename(subpoena, LEGAL_AID_CREATED, LEGAL_AID_QUEUED);

			message("%s   Изх.No %ld/%Dг.", str(outReg.text), outReg.key.no, &outReg.date);
			ItemChanged();
		}
	}
}

void TSubpoenaDialog::NumberBNClicked()
{
	TSubpoena *subpoena = subpoenas->Subpoena();
	msql m(subpoena->Name, "F_NO");

	m.Add(subpoena, "F_NO, F_YEAR, F_TYPE, F_SUBPOENA_KIND");
	m.AddIsNull("F_ANNOUNCE_NO", false);
	TQuery q(str(m));

	if (q.Read())
		error("По делото вече има съобщение с номер.");
	else
	{
		AnnounceIndex->no = 0;
		AnnounceIndex->year = subpoena->subpoenaDate.year;

		if (TAnnounceNumberDialog(this).Execute() == IDOK)
		{
			subpoena->announceNo = AnnounceIndex->no;

			if (subpoena->Update("F_ANNOUNCE_NO"))
			{
				m.clear();
				subpoena->PrintNotifieds(m, ", ", " ");
				message("%s %s No %ld/%ldг.", SubpoenaKind->Seek(subpoena->subpoenaKind), str(m), AnnounceIndex->no,
					AnnounceIndex->year);
				ItemChanged();
			}
			else
				AnnounceIndex->Delete(true);
		}
	}
}

void TSubpoenaDialog::ViewBNClicked()
{
	bool found = false;

	for (int type = 0; type < TROUBLE_COUNT; type++)
	{
		char name[SIZE_OF_PATH];
		electric_name(subpoenas->Subpoena(), name, type);

		if (exist(name))
		{
			electric_action(name, type, ACTION_VIEW);
			found = true;
		}
	}

	if (!found)
		error("Няма присъединени документи.");
}

void TSubpoenaDialog::EditBNClicked()
{
	TSubpoena *subpoena = subpoenas->Subpoena();
	TSubpoena tSubpoena;
	msql m(subpoena->Name);

	variant.subpoenaKind = subpoena->subpoenaKind;
	variant.preLink << sideWind;
	delete variant.execList;
	variant.execList = new TExecList;	// mark as edit
	variant.flags = 0;
	variant.flagsX = 0;
	variant.notifySelect = NOTIFY_SELECT_EXEC_LIST;

	variant.links.Flush();
	m.Add(subpoena, "F_NO, F_YEAR, F_TYPE, F_KIND, F_DATE, F_EXEC_LIST_NO, F_EXEC_LIST_KIND, F_SUBPOENA_DATE");
	TQuery q(str(m));
	while (q.Read())
	{
		tSubpoena << q;
		variant.links.Add(new TExecLink(&tSubpoena));
	}

	if (TNotifySubpoena(this, sideIndex, status, *subpoena, container).Notify(&variant))
	{
		subpoenas->RedrawFocus();
		ItemChanged();
	}
}
