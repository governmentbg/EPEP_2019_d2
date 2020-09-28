DEFINE_RESPONSE_TABLE1(TJudicialActDialog, TTroubleDialog)
//{{TJudicialActDialogRSP_TBL_BEGIN}}
	EV_BN_CLICKED(IDOK, OKBNClicked),
	EV_BN_CLICKED(IDPRINT, PrintBNClicked),
	EV_CBN_SELCHANGE(IDC_JUDICIAL_ACT_RESULT, ResultCBNSelChange),
	EV_CBN_SELCHANGE(IDC_JUDICIAL_ACT_KIND, KindCBNSelChange),
	EV_EN_KILLFOCUS(IDC_JUDICIAL_ACT_RETURNED, ReturnedKillFocus),
	EV_BN_CLICKED(IDRIGHT, RightBNClicked),
//{{TJudicialActDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TJudicialActDialog Implementation}}

TJudicialActDialog::TJudicialActDialog(TWindow* parent, TDecision *decision, TLawsuit *tLawsuit, const char *kinds,
	bool tGrant, TSession *tSession, int resId)
:
	TTroubleDialog(parent, decision, (kinds ? false : true) | ATTACH_RIGHT, TYPE_TEXT, resId), lawsuit(tLawsuit),
		session(tSession), previous(decision->returned), previousResult(decision->result), selectStandard(false),
		forceChange(false), age(NULL), finished(NULL), personalData(false)
{
	interval.key = lawsuit->key;
	interval.date = session->date;

	if (!interval.Read(NULL, false))
		failure();

	alreadyPublic = edit && (decision->flags & DECISION_PUBLIC);
	p64Connect = !alreadyPublic && decision->P64Connect(lawsuit->kind);

	if (edit)
		personalData = decision->PersonalData(session);
	else if (session->result == RESULT_PUBLIC_DECISION)
	{
		msql m(decision->Name, "F_RESULT");
		TDecision tDecision;

		m.Add(decision, "F_NO, F_YEAR, F_TYPE, F_DATE");
		TQuery q(str(m));

		while (q.Read())
		{
			tDecision << q;

			if (!strchr(RESULT_WITH_READY_ANYS, tDecision.result))
			{
				selectStandard = true;
				break;
			}
		}
	}

	header = new TStatic(this, IDC_JUDICIAL_ACT_HEADER);
	returnedLabel = new TStatic(this, IDC_JUDICIAL_ACT_RETURNED_LABEL);
	returned = new TDateFace(this, IDC_JUDICIAL_ACT_RETURNED, "F_RETURNED", false);
	kind = new TCharAliasFace(this, IDC_JUDICIAL_ACT_KIND, "F_KIND", Kind, kinds);
	fromDate = new TDateFace(this, IDC_JUDICIAL_ACT_FROM_DATE, "F_FROM_DATE");
	eventNo = new TLongFace(this, IDC_JUDICIAL_ACT_EVENT_NO, "F_EVENT_NO", SIZE_OF_EVENT_NO);
	// N.B. SetupWindow() -> KindCBNSelChange() -> KindChanged() sets results
	// 2012:144 LPR: init w/ RESULT_READY_DECREES -> RESULT_READY_SETTLEMENTS
	result = new TCharAliasFace(this, IDC_JUDICIAL_ACT_RESULT, "F_RESULT", Result, C2S[RESULT_READY_SETTLEMENT]);
	slcReason = new TSLCReasonAliasFace(this, IDC_JUDICIAL_ACT_SLC_REASON, "F_SLC_REASON", lawsuit->kind,
		edit ? FLAG_NULL : FLAG_ACTIVE);

	// 2005:053 LPR: grant is a flag now
	if (tGrant)
	{
		grant = new TCharAliasFace(this, IDC_JUDICIAL_ACT_GRANT, "F_GRANT", Grant, GRANT_DECISIONS);
		grantLabel = new TStatic(this, IDC_JUDICIAL_ACT_GRANT_LABEL);
	}
	else
	{
		grant = NULL;
		grantLabel = NULL;
	}

	text = new TStringFace(this, IDC_JUDICIAL_ACT_TEXT, "F_TEXT", SIZE_OF_DECISION_TEXT);
	print = new TPushButton(this, IDPRINT);

	if (lawsuit->key.type == TYPE_PUNISHMENT_LAW)
	{
		registerLabel = NULL;
		volume = NULL;
		volumeLabel = NULL;
		page = NULL;
		pageLabel = NULL;
		partition = NULL;
		partitionLabel = NULL;
		damages = new TMoneyFace(this, IDC_JUDICIAL_ACT_DAMAGES, "F_DAMAGES", SIZE_OF_DAMAGES);
		damagesLabel = new TStatic(this, IDC_JUDICIAL_ACT_DAMAGES_LABEL);
	}
	else
	{
		registerLabel = new TStatic(this, IDC_JUDICIAL_ACT_REGISTER_LABEL);
		volume = new TLongFace(this, IDC_JUDICIAL_ACT_VOLUME, "F_VOLUME", SIZE_OF_DAMN_VOLUME);
		volumeLabel = new TStatic(this, IDC_JUDICIAL_ACT_VOLUME_LABEL);
		page = new TLongFace(this, IDC_JUDICIAL_ACT_PAGE, "F_PAGE", SIZE_OF_DAMN_PAGE);
		pageLabel = new TStatic(this, IDC_JUDICIAL_ACT_PAGE_LABEL);
		partition = new TLongFace(this, IDC_JUDICIAL_ACT_PARTITION, "F_PARTITION", SIZE_OF_DAMN_PARTITION);
		partitionLabel = new TStatic(this, IDC_JUDICIAL_ACT_PARTITION_LABEL);
		damages = NULL;
		damagesLabel = NULL;
	}

	publInet = new TCheckFace(this, IDC_TROUBLE_PUBLINET, "F_FLAGS_PUBLIC");
	appeal = new TCheckFace(this, IDC_JUDICIAL_ACT_APPEAL, "F_FLAGS_APPEAL");
}

void TJudicialActDialog::CreateAgeFinished()
{
	age = new TCharAliasFace(this, IDC_JUDICIAL_ACT_AGE, "F_AGE", Age, AGE_AGES);
	finished = new TDateFace(this, IDC_JUDICIAL_ACT_FINISHED, "F_FINISHED", false);
}

void TJudicialActDialog::AttachRight(TWindow *parent, TElectricTrouble &trouble, char motivesKind)
{
	if (returned->Date().Empty())
		error("Не е вписана дата на предаване на мотивите.");
	else
	{
		attach_right(parent, trouble, motivesKind, returned->Date(), session);
		returned->EnableWindow(!trouble.act);
	}
}

bool TJudicialActDialog::DefaultPublic()
{
	return !p64Connect && session->DefaultPublic();
}

void TJudicialActDialog::DynamicPublic()
{
	bool publicable = !Decision()->P64Direct(edit, kind->C()) || alreadyPublic;

#if COURT_TYPE == COURT_REGIONAL
	if (kind->C() == KIND_EUHER_CERT)
		publicable = false;
#endif  // REGIONAL

	if (publicable)
		SetupPublic(DefaultPublic(), Decision()->flagsX & DECISION_X_PUBLTREG, lawsuit);
	else
	{
		publInet->EnableWindow(false);
		publInet->SetCheck(BF_UNCHECKED);
		SetupPublic(false);
	}
}

bool TJudicialActDialog::IsValid()
{
	bool isValid = TTroubleDialog::IsValid();

	if (isValid)
	{
		const char *what = motiving ? "предаване на мотивите" : "връщане";

		if (!motiving && returned->Date().Empty())
			isValid = error("Датата на %s е задължителна.", what);
		else if (!returned->Date().Empty() && returned->Date() < session->date)
			isValid = error("Дата на %s е преди датата на заседание.", what);
		else if (!returned->Date().Empty() && returned->Date() < fromDate->Date())
			isValid = error("Дата на %s е преди датата на документа.", what);
		else if (fromDate->Date() > Today)
			isValid = error("Дата на документа е по-голяма от текущата дата.");
		else if (returned->Date() > Today)
			isValid = error("Дата на %s е по-голяма от текущата дата.", what);
		else if (finished && ((age->C() == AGE_NONE_YET) != finished->Date().Empty()))
			isValid = error("Продължителност и край на делото не си съответстват.");
		else if (lawsuit->key.type != TYPE_FIRM_LAW)
		{
			int tRegisterCount = volume ? (volume->L() != 0) + (page->L() != 0) + (partition->L() != 0) : 0;

			if (tRegisterCount != 0 && tRegisterCount != 3)
				isValid = error("Полетата том, страница и номер от регистъра на решенията не са комплектни.");
			else if (tRegisterCount == 3 && returned->Date().Empty())
			{
				isValid = error("За вписването на том, страница и номер от регистъра на решенията е необходимо "
					"да е попълнена датата на %s.", what);
			}
			else if (tRegisterCount == 3)
			{
				msql m(Decision()->Name, "F_NO");

				m.AddLong("F_PARTITION", partition->L());
				// 2008:316 LRQ: FIX: lawsuit year -> fromDate.year
				// 2009:105 TRQ: fromDate.year -> returned.year
				//" AND F_YEAR = %ld "
				// "AND F_FROM_DATE BETWEEN %ld "
				// 2017:017 FIX: fromDate->Date().year -> returned
				m.AddRange("F_RETURNED", CDate(1, 1, returned->Date().year), CDate(31, 12, returned->Date().year));
				m.AddChar("F_TYPE", session->key.type);

				if (edit)
				{
					// not the same act...
					m.AddLong("NOT (F_NO", session->key.no);
					m.Add(session, "F_YEAR, F_TYPE, F_DATE");
					m.AddChar("F_KIND", kind->C());
					m.cat(")");
				}

				TQuery q(str(m));
				if (q.Read())
				{
					isValid = error("Вече има решение с номер %ld от регистъра за %ldг.", partition->L(),
						returned->Date().year);
				}
			}
		}
		// 2008:318 LPR: still valid, moved from standard act
		if (isValid && finished)
		{
			if (GetFocus() == GetDlgItem(IDC_JUDICIAL_ACT_RETURNED))
				ReturnedKillFocus();

			if (!PublicDecision() || finished->Date() != returned->Date())
				isValid = lawsuit_finished_valid(interval, finished);
		}
	}

	return isValid;
}

void TJudicialActDialog::OKBNClicked()
{
	if (IsValid() && (!grant || !grant->IsWindowEnabled() ||
		// 2011:185 cancelaw support, should cleanup then lock grant
		grant->C() != GRANT_NONE_YET || strchr(RESULT_CANCELAWS, result->C()) ||
		ask("Не е вписана степен на уважаване на иска. %s", Q_CONTINUE)))
	{
		if (p64Connect && !Decision()->P64Direct(edit, kind->C()) && publInet->GetCheck() == BF_CHECKED &&
			!message(MB_WARN | MB_DEFBUTTON2, "Актът е по наказателно дело. Желаете ли да бъде публикуван?"))
		{
			publInet->SetCheck(BF_UNCHECKED);
		}

		TDecision *decision = Decision();
		*decision << this;

		if (motiving)
			decision->flags |= DECISION_MOTIVES;
		else
			decision->flags &= ~DECISION_MOTIVES;

		if (decision->kind == KIND_BULWARK && !edit)
			decision->forceDate = decision->fromDate;

		if (decision->flags & DECISION_PUBLIC)
		{
			// 2017:320 PRQ: decision->fromDate -> Today of 1st public
			if (!alreadyPublic)
				decision->publicDate = Today;
		}
		else
			decision->publicDate.Clear();

		if (decision->Save(edit))
		{
			//check_ses_returned(session, returned);
			PostMove();
			// 2008:319 LPR: motives/finchange moved from standard act
			// 2006:188 LRQ: log motives date
			if (previous != returned->Date() && motiving)
			{
				mstr m("Дата на мотиви: ");

				if (returned->Date().Empty())
					m.cat("изтрита");
				else
				{
					returned->Date().Print(m);
					// 2014:296 v.1.6275 p.42.1
					send_sms(decision, "Вписани мотиви към", SMS_ENDOC, SMS_NEW, decision);
				}

				log(LOG_CHANGE, CM_JUDGE_MOTIVES, &decision->key, KIND_MOTIVES, &decision->date, str(m));
			}

			if (finished)
			{
				if (finished->Date() != interval.finished && !lawsuit->finished.Empty())
					finished_judge(lawsuit, session);

				bool grantChanged = grant && grant->C() != interval.grant;

				if (age->C() != interval.age || finished->Date() != interval.finished || grantChanged)
				{
					mstr fields("F_AGE, F_FINISHED");

					interval.age = age->C();
					interval.finished = finished->Date();

					if (grantChanged)
					{
						interval.grant = grant->C();
						fields.cat(", F_GRANTED");
					}

					interval.Update(str(fields));
				}
			}

			if (PublicDecision())
				attach_final(session, decision, returned->Date(), finished ? finished->Date() : CDate(), edit);

			CmOk();
		}
	}
}

void TJudicialActDialog::SetupWindow()
{
	TTroubleDialog::SetupWindow();

	mstr m;
	m.printf("%s %ld/%ldг., %s от %Dг.", Type->Seek(session->key.type), session->key.no, session->key.year,
		Kind->Seek(session->kind), &session->date);
	header->SetCaption(str(m));

	ResultCBNSelChange();

	if (edit)
	{
		kind->EnableWindow(false);
		eventNo->EnableWindow(false);
		trouble.Name(Decision());	// 2008:101 LPR: FIX: was missing
	}
	else if (session->result == RESULT_CANCELED)
		kind->SetC(KIND_SETTLEMENT);	// 2017:157

	if (grant)
	{
		grant->ShowWindow(SW_SHOW);
		grantLabel->ShowWindow(SW_SHOW);
		// 2008:077 LPR: en/disable now in kind change
	}

	KindChanged();

	// 2008:077 LPR: revive some invisibility
	if (volume)
	{
		registerLabel->ShowWindow(SW_SHOW);
		volume->ShowWindow(SW_SHOW);
		volumeLabel->ShowWindow(SW_SHOW);
		page->ShowWindow(SW_SHOW);
		pageLabel->ShowWindow(SW_SHOW);
		partition->ShowWindow(SW_SHOW);
		partitionLabel->ShowWindow(SW_SHOW);
	}

	if (damages)
	{
		damagesLabel->ShowWindow(SW_SHOW);
		damages->ShowWindow(SW_SHOW);
	}

	if (finished)
	{
		age->SetC(interval.age);
		finished->SetDate(interval.finished);
		ReturnedKillFocus();
	}

	// 2009:069 LPR/LRQ: for eventNo fixage to fromDate.year
	fromDate->EnableWindow(!edit);

	if (!COURT_LOCK)
	{
		appeal->ShowWindow(SW_SHOW);
		appeal->EnableWindow(true);
	}

	DynamicPublic();
}

void TJudicialActDialog::PrintBNClicked()
{
	if (IsValid())
	{
		*Decision() << this;

	#if 20170243
		const char *name = NULL;

		switch (kind->C())
		{
		#if COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_REGIONAL
			case KIND_PROTECTIVE :
				name = print_protective(this, Decision(), session, lawsuit, "O_ZZDN.htm");
				break;
		#endif  // AREAL || REGIONAL
		#if COURT_TYPE == COURT_REGIONAL
			case KIND_IMMED_PROTECT :
				name = print_protective(this, Decision(), session, lawsuit, "ImmedProt.htm");
				break;
			case KIND_EXECUTIVE :
				name = print_executive(this, Decision(), session, lawsuit);
				break;
		#endif  // REGIONAL
			default : print_decision_judgement(this, Decision(), lawsuit, returned->Date());
		}

		if (name)
		{
			char tName[SIZE_OF_PATH];
			build_fn(tempdir, name, tName);
			trouble.name[TYPE_TEXT]->SetS(tName);
		}
	#else  // 20170243
		switch (kind->C())
		{
		#if COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_REGIONAL
			case KIND_PROTECTIVE : print_protective(this, Decision(), session, lawsuit, "O_ZZDN.htm"); break;
		#endif  // AREAL || REGIONAL
		#if COURT_TYPE == COURT_REGIONAL
			case KIND_IMMED_PROTECT : print_protective(this, Decision(), session, lawsuit, "ImmedProt.htm"); break;
			case KIND_EXECUTIVE : print_executive(this, Decision(), session, lawsuit); break;
		#endif  // REGIONAL
			default : print_decision_judgement(this, Decision(), lawsuit, returned->Date());
		}
	#endif  // 20170243
	}
}

void TJudicialActDialog::ResultCBNSelChange()
{
	ResultChanged();
}

void TJudicialActDialog::ResultChanged()
{
	if (grant)
		grant->SetC(interval.grant);

	// 2011:305 LRQ: also backported
	if (!edit)
		transfer_dec_text(result, text);

	if (strchr(RESULT_CANCELAWS, result->C()) || strchr(RESULT_ALL_CANCELEDS, session->result))
	{
		slcReason->SetXCrit(C2S[SLC_TYPE_CANCELED]);
		slcReason->SetL(Decision()->slcReason);
	}
	else
		slcReason->SetXCrit("");
}

void TJudicialActDialog::KindChanged()
{
	// 2017:300 LPR: replace all previous tweaks
	if (!edit)
	{
		Decision()->kind = kind->C();
		personalData = Decision()->PersonalData(session);
	}

	if (strchr(KIND_DECISIONS, Decision()->kind))
	{
		// 2008:078 LRQ: new GPK only; 2008:114 URQ: DEC_REG_NUM_ALL
		if (volume && (DEC_REG_NUM_ALL || (lawsuit->flags & LAWSUIT_NEWGPK)))
		{
			volume->EnableWindow(true);
			page->EnableWindow(true);
			partition->EnableWindow(true);
		}
	}
	else if (volume)
	{

		volume->EnableWindow(false);
		volume->SetL(0);
		page->EnableWindow(false);
		page->SetL(0);
		partition->EnableWindow(false);
		partition->SetL(0);
	}

	mstr title(edit ? "Редактиране на " : "Вписване на ");
	size_t len = title.len();

	title.cat(Kind->Seek(kind2VisibleKind(Decision()->kind)));
	strlwr(ncstr(title) + len);
	SetCaption(str(title));
	ProcessResults();

	if (grant)
	{
		// 2008:077 LRQ: LPR: one more hack
		grant->EnableWindow(lawsuit->key.type != TYPE_PUNISHMENT_LAW || kind->C() != KIND_SETTLEMENT);
		// 2008:076 LRQ: LPR: this is a hack
	#if COURT_TYPE == COURT_REGIONAL
		if (kind->C() == KIND_PROTECTIVE || (kind->C() == KIND_EXECUTIVE && session->result != RESULT_FORCE_EXECUTIVE))
			grant->EnableWindow(false);
	#endif  // REGIONAL
		if (!grant->IsWindowEnabled())
			grant->SetC(interval.grant);
	}

	bool tMotiving;

	if (edit)
		tMotiving = Decision()->flags & DECISION_MOTIVES;
	else
	{
		tMotiving = true;

		if (session->result == RESULT_WITH_JUDGEMENT)
		{
			TGArray<TDecision> decisions;
			try2decide(session, &decisions, NULL, "F_FLAGS");

			for (int i = 0; i < decisions.Count(); i++)
				if (decisions[i]->flags & DECISION_MOTIVES)
					tMotiving = false;
		}

		if (tMotiving)
			tMotiving = personalData && Decision()->PreferMotives(lawsuit);
	}

	// 2008:319 LPR: moved here from standard act; 2009:349 LPR: method
	SetMotiving(tMotiving);
	// 2015:295; 2015:296 FIX: no enable on edit
	trouble.IndexChanged(Decision()->IndexKind());
	fromDate->EnableWindow(!edit && !trouble.act);

	// 2017:300 LPR: moved here from setup window
	if (damages)
	{
		if (personalData)
			damages->EnableWindow(true);
		else
		{
			damages->EnableWindow(false);
			damages->SetM(0.0);
		}
	}
}

void TJudicialActDialog::KindCBNSelChange()
{
	KindChanged();
	DynamicPublic();
}

void TJudicialActDialog::ReturnedKillFocus()
{
	// 2008:311 LPR: check for PUBLIC DECISION was missing
	// 2008:311 LPR: check for valid returned was missing
	if (date_valid(returned->S(), true) && finished && PublicDecision() && interval.finished.Empty())
	{
		finished->SetDate(returned->Date());
		char tAge = interval2LawAge(lawsuit->kind, interval.date, interval.firstSet, returned->Date());
		age->SetC(returned->Date().Empty() ? AGE_NONE_YET : tAge);
	}
}

void TJudicialActDialog::SetMotiving(bool tMotiving)
{
	motiving = tMotiving;

	if (motiving)
	{
		returned->EnableWindow(true);

		if (!edit)
		{
			returned->SetS("");	// 2005:103 LRQ
			ReturnedKillFocus();
		}
	}
	else
	{
		// 2006:018 LRQ
		returned->EnableWindow(!return_date_locked(lawsuit));

		if (!edit)
		{
			returned->SetDate(Today);
			ReturnedKillFocus();
		}
	}
}

void TJudicialActDialog::ProcessResults()
{
	char standardResult = Decision()->StandardResult();

	strcpy(results, Decision()->SpecificResults(lawsuit, session, &data));

	// 2016:351 local heritage
#if COURT_TYPE == COURT_REGIONAL
	if (lawsuit->sourceKind == KIND_HERITAGE_REQ_51)
	{
		if (!edit || previousResult == standardResult)
		{
			stremove(results, RESULT_HERITAGE_ENDOCS);
			// Note: protects resulting CANCELAWS-only
			strcach_nx(results, standardResult);
		}
	}
#endif  // REGIONAL

	// 2016:025 CANCELAW -> CANCELAWS
#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_ADMIN || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY
	if (not_inset(results, RESULT_CANCELAWS) && !strchr(results, standardResult) || !any(results))
#endif  // APPEAL || ADMIN || AREAL || MILITARY
#if COURT_TYPE == COURT_REGIONAL
	if ((not_inset(results, RESULT_CANCELAWS) && !strchr(results, standardResult) &&
		!intersect(results, RESULT_HERITAGE_ENDOCS)) || !any(results))
#endif  // REGIONAL
	{
		strcach_nx(results, standardResult);
	}

	strcach_nx(results, previousResult);
	if (strchr(KIND_DECISIONS, kind->C()) && strchr(RESULT_DECIDABLES, session->result))
		strcach_nx(results, RESULT_CANCEL_LAWSUIT);  // 2017:157
	result->SetXCrit(results);
	result->SetC(selectStandard ? standardResult : Decision()->result);
	ResultChanged();	// 2012:017 TRQ: was missing
}

void TJudicialActDialog::RightBNClicked()
{
	Decision()->kind = kind->C();
	attach_right(this, trouble, Decision()->IndexKind(), fromDate->Date(), session);
	fromDate->EnableWindow(!trouble.act);
}
