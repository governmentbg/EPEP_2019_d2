#include "cac.h"

// 2003:287 BRW: we should be able to use this as a more generic chainer
// 2007:016 LPR: static
void chain_action(TWindow *parent, TRCDKeyContainer *container, next_action action)
{
	if (AUTORUN_BASICS && action && container->Try())
		action(parent, container);
}

#include "CalendarDialog.h"
#include "CalendarDialog.cpp"

void edit_calendar(TWindow *parent)
{
	TCalendar calendar;

	calendar.year = Today.year;
	if (get_year(parent, &calendar))
	{
		if (!calendar.Try())
			calendar.Insert();

		log(LOG_NOM, CM_ADMIN_CALENDAR, "календар");
		if (TCalendarDialog(parent, &calendar).Execute() == IDOK)
			if (calendar.year == Calendar->year)
				*Calendar << calendar;
	}
}

#include "EditLawerDataDialog.h"
#include "EditLawerDataDialog.cpp"

bool edit_lawer_data(TWindow *parent, TKnownPerson *person, bool eportal)
{
	if (eportal && (is_real_ucn(person) || !person->birthDate.Empty()) && any(person->personalNo))
		return true;

	return TEditLawerDataDialog(parent, person, eportal).Execute() == IDOK;
}

// 2003:352 BRW: inspired by the need to skip involvement for judgements
static bool one_involvement(TGroup *group, const char *involvements)
{
	if (strlen(involvements) != 1 || strchr(INVOLVEMENT_FROMABLE_SIDES, *involvements))
		return false;

	TChar *involvement = (TChar *) group->Find("F_INVOLVEMENT");

	if (involvement)
		involvement->SetC(*involvements);

	TChar *fromSide = (TChar *) group->Find("F_FROM_SIDE");

	if (fromSide)
		fromSide->SetC(INVOLVEMENT_FROM_NULL);

	return true;
}

static bool edit_sidewind(TWindow *parent, TSideWind *sideWind, const char *involvements, bool unwanted, sign stat,
	char originKind)
{
	if (one_involvement(sideWind, involvements) && !unwanted)
		return true;

	if (TEditSideWindDialog(parent, sideWind, involvements, unwanted).Execute() != IDOK)
		return false;

	if (!check_involvement(parent, originKind, sideWind))
		return false;

	if (strchr(INVOLVEMENT_CHECK_PERSONS, sideWind->involvement))
	{
		TKnownPerson person;

		person << sideWind;
		person.involvement = involvement2known(sideWind->involvement);

		if (!person.Try())
		{
			return error("Лицето не е регистрирано в класификатор \"Участници в съдебния процес\" и участието "
				"му не може да бъде променено на \"%s\". Обърнете се към администратора на системата.",
				 Involvement->Seek(sideWind->involvement));
		}

		if (stat == -1 && person.involvement == INVOLVEMENT_LEGALESE && !edit_lawer_data(parent, &person, true))
			return false;
	}

	return true;
}

bool edit_sidewind(TWindow *parent, TRCDKeyContainer *container, TSideWind *source, TSideWind *target,
	const char *involvements, bool unwanted, TLinkWindArray *links, char status, sign stat)
{
	char originKind = strchr(KIND_LAWSUITS, container->kind) ? ((TLawsuit *) container)->sourceKind : container->kind;

	// 2008:169 LPR: allow edit unwanted even if notified
	if (blocked_sidewind(target, status, true))
		involvements = C2S[source->involvement];

	if (edit_sidewind(parent, target, involvements, unwanted, stat, originKind) && update_sidewind(source, target, links))
	{
		// 2009:047 LPR: always, not if edit_ucn/address
		log(LOG_EDIT, IDEDIT, container, target);

		// 2011:075 LRQ: allow non-name edits for users without rename
		// 2017:093 FIX: block rename for the special ~UCN_DATED_SIDES
		if (edit_ucn(parent, target, container, true, can_rename() && strchr(UCN_DATED_SIDES, target->ucnType),
			stat) && stat != -1)
		{
			edit_addresses(parent, target, ADDRESS_ALLOW_FOREIGN);
		}

		return true;
	}

	return false;
}

bool ask_join(TUCNGroup *source, TUCNGroup *target)
{
	mstr m("Желаете ли да присъедините ");

	source->Print(m);
	print_join_name(m, source);
	m.cat(" към ");
	target->Print(m);
	print_join_name(m, target);
	m.compress();
	m.trim_trail();
	return ask("%s?", str(m));
}

bool ask_renumber(TUCNGroup *source, TUCNGroup *target)
{
	mstr m("Желаете ли да преномерирате ");
	TNamedGroup *group = TNamedGroup::NewGroup(source->ucnType);
	TVirtPtr VP(group);

	source->Print(m);
	*group << source;

	if (group->Get("F_UCN, F_UCN_TYPE"))
	{
		TSideWind sideWind;

		sideWind << group;
		m.cat(" ");
		sideWind.PrintNames(m);
	}

	m.printf(" на %s %s?", UCNType->Seek(target->ucnType), target->ucn);
	return ask(str(m));
}

bool ask_move(TDecision &source, TSession &session)
{
	mstr m("Желаете ли да преместите ");

	m.printf("%s по %s %ld/%ld ", Kind->Seek(source.kind), Type->Seek(source.key.type), source.key.no, source.key.year);
	m.printf("от %s с дата %D ", Kind->Seek(source.sessionKind), &source.date);
	m.printf("към %s с дата %D", Kind->Seek(session.kind), &session.date);
	return ask("%s?", str(m));
}

void attach_final(TSession *session, const TDecision *decision, const CDate &tReturned, const CDate &finished, bool edit)
{
	CDate minReturned = decision->MinReturned(edit);

	if (minReturned.Empty() || (!tReturned.Empty() && tReturned < minReturned))
		minReturned = tReturned;

	// 2017:044
	if (!finished.Empty() && finished == tReturned)
		minReturned = tReturned;

	if (session->final != minReturned)
	{
		session->final = tReturned;
		session->Update("F_FINAL");
	}
}

void barcode_scan(TWindow *parent)
{
	static constant MUST_ACCESS = "Нямате право да работите с тази призовка.";
	static constant MUST_WRIT = "Баркода изисква призовкарския модул да е активен.";

	TAnnounce announce;
	TGroupArray deliverers;
	TAnnounce tAnnounce;

	tAnnounce.subpoenaDate = Today;
	tAnnounce.Remove("F_COURT");
	tAnnounce.Remove("F_REQUEST_ID");
	tAnnounce.Remove("F_BARCODE");

	while (get_barcode(parent, &announce.court, &announce.requestId, announce.barcode))
	{
		msql m(announce.Name);

		m.Add(&announce, any(announce.barcode) ? "F_BARCODE" : "F_COURT, F_REQUEST_ID");
		TQuery q(str(m));

		if (q.Read())
		{
			if (COURT_WRIT)
			{
				announce << q;

				if (Default->post == POST_DELIVERER && strcmp(announce.delivererUcn, Default->ucn))
					error(MUST_ACCESS);
				else
					serve_announce(parent, &announce);
			}
			else
				error(MUST_WRIT);
		}
		else if (any(announce.barcode) || announce.court != COURT_CODE)
		{
			if (COURT_WRIT)
			{
				if (!deliverers.Count())
					UserAliasGroup->LoadData(&deliverers, C2S[POST_DELIVERER], FLAG_ACTIVE);

				if (any(announce.barcode))
					announce.court = 0;

				// 2016:103 JRQ/IRQ: clear data on different/unknown court
				// 2017:321 JRQ/URQ: clear data on configuration setting
				if (!announce.court || announce.court != tAnnounce.court || !REPEAT_BARCODES)
				{
					tAnnounce.court = announce.court;
					announce << tAnnounce;
				}

				announce.receivedStatus = SRS_FOR_DELIVER;
				edit_announce(parent, &announce, false, &deliverers, CONNECT_KIND_ANNOUNCES, true);
			}
			else
				error(MUST_WRIT);
		}
		else
		{
			TSubpoena subpoena;
			msql m(subpoena.Name);

			m.Add(&announce, "F_REQUEST_ID");
			TQuery q(str(m));

			if (q.Read())
			{
				subpoena << q;

				if (Default->post == POST_DELIVERER && strcmp(subpoena.delivererUcn, Default->ucn))
					error(MUST_ACCESS);
				else
					serve_subpoena(parent, &subpoena, -1);
			}
			else
				say_no_data(subpoena.Name);
		}
	}
}

bool big_sas_lock(TWindow *parent)
{
	static bool BigSasLock = false;
	bool lock = parent != NULL;
	bool doit = lock != BigSasLock;

	if (doit)
	{
		BigSasLock = lock;

		if (lock)
		{
			CDateTime::SyncServer(SYNC_AVERAGE);
			TShowDialog::Cleanup();
		}
		else
		{
			// 2017:072 moved here from electric
			PrimaryCommand = 0;
		}

		AppMainWindow->EnableWindow(!lock);
		electric_foreach(ELECTRIC_ENABLE, lock ? VOID_FALSE : VOID_TRUE);
	}

	return doit;
}

bool check_config_dates()
{
	bool changed = false;

	TConfigData configData(CONFIG_NAME_GLOBAL);
	strcpy(configData.section, "serial");

	// 2013:325 LPR/LRQ: config parameter to avoid slow startup
	if (FULL_KIND_DATE.Empty())
	{
		TWaitWindow wait(NULL, "Еднократна проверка на системния журнал...");
		TLog log;
		msql m(log.Name, "MAX(F_STAMP) AS F_STAMP");

		m.Add("(F_STAMP");
		m.cat("BETWEEN '1.7.2011' AND '1.7.2012') "
			"AND ((F_DATE IS NULL AND NOT F_KIND IS NULL) OR (F_KIND IS NULL AND NOT F_DATE IS NULL)) "
			"AND (F_GOP IN ('n', 'E', 'l') OR (F_GOP IN ('s', 'r') AND F_TYPE = 'o'))");

		TQuery q(str(m));
		if (q.Read())
			log << q;

		if (log.stamp.Empty())
		{
			FULL_KIND_DATE.day = 1;
			FULL_KIND_DATE.month = 7;
			FULL_KIND_DATE.year = 2011;
		}
		else
			FULL_KIND_DATE = (CDate &) log.stamp;

		strcpy(configData.name, "FULL_KIND_DATE");
		sprintf(configData.value, "%0D", &FULL_KIND_DATE);

		if (configData.Insert())
			changed = true;
	}
#if TESTVER
	if (XFER_BANK_DATE.Empty())
	{
		strcpy(configData.name, "XFER_BANK_DATE");
		sprintf(configData.value, "%0D", &Today);

		if (configData.Insert())
			changed = true;
	}
#endif  // TESTVER
	return changed;
}

bool check_involvement(TWindow *parent, char originKind, const TSideWind *sideWind)
{
#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_ADMIN || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY
	(void) parent;
	(void) originKind;
	(void) sideWind;
#endif  // APPEAL || ADMIN || AREAL || MILITARY
#if COURT_TYPE == COURT_REGIONAL
	if (originKind == KIND_EUHER_REQUEST && strchr(UCN_CITIZENS, sideWind->ucnType) &&
		sideWind->involvement == INVOLVEMENT_LEGATOR)
	{
		msql m("T_SIDE_WIND W "
			"JOIN T_REQUEST R ON R.F_NO = W.F_NO AND R.F_YEAR = W.F_YEAR AND R.F_TYPE = W.F_TYPE "
				"AND R.F_KIND = W.F_KIND AND R.F_DATE = W.F_DATE "
			"JOIN T_CITIZEN C ON C.F_UCN = W.F_UCN AND C.F_UCN_TYPE = W.F_UCN_TYPE",
			"DISTINCT F_NO, F_YEAR, F_TYPE");
		TCitizen citizen;

		m.AddChar("W.F_KIND", KIND_EUHER_REQUEST);
		m.AddChar("W.F_INVOLVEMENT", INVOLVEMENT_LEGATOR);

		if (sideWind->Find("F_FAMILY"))
			citizen << sideWind;
		else
		{
			strcpy(citizen.ucn, sideWind->ucn);
			citizen.ucnType = sideWind->ucnType;

			if (!citizen.Get())
				return true;
		}

		m.AddString("C.F_NAME", citizen.name);
		m.AddString("C.F_FAMILY", citizen.family);

		if (any(citizen.reName))
		{
			m.AddString("(C.F_RENAME", citizen.reName);
			m.NextOr();
			m.AddIsNull("C.F_RENAME", true);
			m.cat(")");
		}

		{
			TWaitWindow wait(parent, WAIT);
			TRequest request;
			TLawsuit lawsuit;

			TQuery q(str(m));
			m.clear();

			while (q.Read())
			{
				request << q;

				if (request.key == sideWind->key)
					continue;

				if (lawsuit.TryBy(&request, "F_NO, F_YEAR, F_TYPE"))
				{
					if (lawsuit.key == sideWind->key)
						continue;

					request.key = lawsuit.key;
				}

				m.printf("%s %ld/%ldг.\n", Type->Seek(request.key.type), request.key.no, request.key.year);
			}
		}

		return !any(m) || ask("Дела/документи, съдържащи %s със същите или подобни имена:\n\n%s\n%s",
			Involvement->Seek(INVOLVEMENT_LEGATOR), str(m), Q_CONTINUE);
	}
#endif  // REGIONAL
	return true;
}

bool check_subpoenas(const TRCDKeyContainer *container)
{
	TBaseWind tSubpoena("T_SUBPOENA", NULL);
	msql m;

	tSubpoena << container;
	tSubpoena.FloatCriteria(m);
	m.AddIsNull("(F_OUTREG_NO", false);
	m.NextOr();
	m.AddIsNull("F_REQUEST_ID", false);
	m.NextOr();
	m.AddIsNull("F_EXEC_LIST_NO", false);
	m.cat(")");

	TSubpoena subpoena;
	TStringArray emited;

	TQuery q(str(m));
	while (q.Read())
	{
		if (emited.Count() == 15)
		{
			emited.Add("Други призовки/съобщения/писма, свързани с допълнителни обекти.");
			break;
		}

		subpoena << q;
		SubpoenaKind->Seek(subpoena.subpoenaKind);
		m.clear();

		if (subpoena.outRegNo)
		{
			m.printf("Изготвено \"%s\" с изходящ номер №%ld/%ldг.", SubpoenaKind->name, subpoena.outRegNo,
				subpoena.outRegYear);
		}

		if (subpoena.requestId && strchr(SUBPOENA_KIND_LEGAL_AIDS, subpoena.subpoenaKind))
			m.printf("Изготвено \"%s\" със заявка към АК за осъществяване на правна помощ.", SubpoenaKind->name);
		else if (subpoena.execListNo)
			m.printf("Издаден \"%s\" №%ld/%ldг.", SubpoenaKind->name, subpoena.execListNo, subpoena.subpoenaDate.year);

		if (any(m))
			emited.Add(strnew(str(m)));
	}

	if (emited.Count())
	{
		m.clear();
		m.printf("За %s от %D има", Kind->Seek(container->kind), &container->date);

		if (emited.Count() == 1)
			m.printf(" и%s", emited[0] + 1);
		else
		{
			m.cat(":\n");

			for (int i = 0; i < emited.Count(); i++)
				m.printf("\n- %s", emited[i]);
		}

		return ask("%s\n\n%s", str(m), Q_CONTINUE);
	}

	return true;
}

static inline char child_mark_ad(const TSideWind *sideWind)
{
	return TSideWind::ChildText(sideWind->flags & SIDE_WIND_CHILD ? sideWind->flags : SIDE_WIND_ADMNU)[0];
}

bool child_sidewind(TSideWind *sideWind, long flags)
{
	TSideWind tSideWind;
	TJudgement tJudgement;
	bool result = false;
	TDummyContainer container;

	for (int i = 0; i < 2; i++)
	{
		TSideWind *target = i ? (TSideWind *) &tJudgement : &tSideWind;
		msql m(target->Name, "F_KIND, F_DATE, F_FLAGS");

		m.Add(sideWind, "F_NO, F_YEAR, F_TYPE, F_UCN, F_UCN_TYPE, F_INVOLVEMENT");

		if (flags == SIDE_WIND_INFANT)
		{
			m.AddDate("F_DATE", sideWind->date);
			m.AddIsNull("(F_FLAGS", true);
			m.NextOr();
			m.AddFlags("NOT F_FLAGS", SIDE_WIND_CHILD, SIDE_WIND_INFANT, SIDE_WIND_ALL);
			m.cat(")");
		}
		else
		{
			m.AddDate("(F_DATE", sideWind->date);
			m.NextOr();
			m.Add("(F_DATE");
			m.printf("> %ld", sideWind->date.Value());

			if (flags == SIDE_WIND_MINOR)
				m.AddFlags("F_FLAGS", SIDE_WIND_CHILD, SIDE_WIND_INFANT, SIDE_WIND_ALL);
			else
				m.AddFlags("F_FLAGS", SIDE_WIND_YOUNG, SIDE_WIND_YOUNG, SIDE_WIND_ALL);

			m.cat("))");
		}

		TQuery q(str(m));
		while (q.Read())
		{
			*target << sideWind;
			*target << q;
			const char oldMark = child_mark_ad(target);
			target->flags = (target->flags & ~SIDE_WIND_CHILD) | flags;

			if (target->Update("F_FLAGS"))
			{
				m.clear();
				m.printf("%s %s %c пълнолетност %c -> %c", UCNType->Seek(target->ucnType), target->ucn,
					target->Name[2], oldMark, child_mark_ad(target));
				container << target;
				log(LOG_CHANGE, 0, &container, str(m));

				if (!strcmp(target->Name, sideWind->Name) && target->kind == sideWind->kind &&
					target->date == sideWind->date)
				{
					sideWind->flags = target->flags;
					result = true;
				}
			}
		}
	}

	return result;
}

bool detached_final(TSession *session, const TDecision *decision)
{
	// 2010:209 LPR: reverted !session.final.Empty() to match delete_finalization()
	if (public_decision(session->result, decision->kind) && !session->final.Empty())
	{
		CDate minReturned = decision->MinReturned(false);

		if (session->final != minReturned)
		{
			session->final = minReturned;
			return session->Update("F_FINAL");
		}
	}

	return false;
}

void move_act(TDecision &source, TSession &session)
{
	TDecision target;

	target << source;
	target.sessionKind = session.kind;

	mstr m;
	m.printf("към %s от %D", KindAlias->Seek(session.kind), &session.date);

	if (target.date == session.date)
	{
		target.Update("F_SESSION_KIND");
		log(LOG_MOVE, 0, &source, str(m));
		// TODO: xfer if updated
		return;
	}
	else if (!target.CopyTo(session.date))
		return;

	// electric; 2017:101 moved 1st, return if failed
	if (!electric_rekind(&source, source.kind, target.date))
	{
		target.eventNo = 0;
		target.Delete(true);
		return;
	}

	log(LOG_MOVE, 0, &source, str(m));
	move_winds(&source, &target);
	source.eventNo = 0;

	if (source.Delete(true))
	{
		if (!source.returned.Empty())
			detached_final(&session, &source);

		if (public_decision(session.result, target.kind))
			attach_final(&session, &target, target.returned, PrimaryGroup(TLawsuit)->finished, true);
	}

	// TODO: xfer
}

void move_winds(TRCDKeyContainer *source, TRCDKeyContainer *target, bool override)
{
	TSideWind sideWind;
	TLinkWind linkWind;
	TJudgement judgement;
	TSubpoena subpoena;
	TExpense expense;

	copy_winds(source, target, sideWind, override);
	copy_winds(source, target, linkWind, override);
	copy_winds(source, target, judgement, override);
	copy_winds(source, target, subpoena, override);
	copy_winds(source, target, expense, override);
	delete_winds(source, NULL, true);
}

#include "MovementDialog.h"
#include "MovementDialog.cpp"

bool movement(TWindow *parent, TRCDKeyGroup *group)
{
	TMovement movement;
	movement.key = group->key;

	if (TMovementDialog(parent, &movement, group).Execute())
	{
		char gop = movement.kind == KIND_OUT_MOVEMENT ? LOG_GIVE : LOG_RETURN;
		log(gop, CM_CLERK_IN_OUT_MOVEMENT, &movement, movement.text);
		return true;
	}

	return false;
}

void print_join_name(mstr &m, TUCNGroup *ucnGroup)
{
	switch (ucn2uxn(ucnGroup->ucnType))
	{
		case UXN_CITIZEN :
		{
			TCitizen citizen;
			citizen << ucnGroup;

			if (citizen.Get())
				m.printf("%s %s %s", citizen.name, citizen.reName, citizen.family);
			break;
		}
		case UXN_FIRM :
		{
			TFirm firm;
			firm << ucnGroup;

			if (firm.Get("F_NAME"))
				m.cat(firm.name);
			break;
		}
		case UXN_DAMN :
		{
			TDamn damn;
			damn << ucnGroup;

			if (damn.Get("F_NAME"))
				m.cat(damn.name);
			break;
		}
	}
}

#include "PrinterDialog.h"
#include "PrinterDialog.cpp"

void printer_setup(TWindow *parent, TPrinterGroup *group)
{
	// 2007:337 FIX: backward compatibility: handle alignment
	group->Load();
	group->alignto = DEFAULT_ALIGNMENT;
	if (TPrinterDialog(parent, group).Execute() == IDOK)
		group->Save();
}

void recompose_law(TLawsuit *source, TLawsuit *target)
{
	if (recompose(source, target))
	{
		log(LOG_CHANGE, CM_ADMIN_RECOMPOSE_LAWSUIT, source, "на състава на дело");
		info("Състава на делото е променен.");
	}
}

void reload_config()
{
	SetWaitCursor(true);
	TPosTerm::Shutdown();
	shutdown_config();
	TConfigure::Reset();
	initialize_config();
	TPosTerm::Initialize();
	SetWaitCursor(false);
#if TESTVER
	// 2018-03-12: LPR: moved to update_mini()
#else  // TESTVER
	check_config_dates();
#endif  // TESTVER	
	// 2015:331
	if (AppMainWindow)
		SetForegroundWindow(AppMainWindow->HWindow);
}

void reload_title(TWindow *parent)
{
	// 2004:191 VRQ: BUGFIX: was missing
	UCLP->Seek(COURT_UCLP);
	// 2004:114 LPR: refill uclp name after reload
	reset_cache_data();
	// 2001:295 Refresh title etc.
	parent->GetModule()->SetName(AppNameTitle);
	parent->SetCaption(parent->GetModule()->GetName());
}

void send_sms(const TRCDKey &key, const char *text, char action, int phase, const TDatedContainer *container)
{
	if (!SMS_ACTIVE || !strchr(TYPE_LAWSUITS, key.type))
		return;

	TXMLFile x;
	mstr m;
	char court[SIZE_OF_NAME];

	switch (COURT_CODE)
	{
		case COURT_SOFGS : strcpy(court, "SGS"); break;
		case COURT_SPNKS : strcpy(court, "SpNS"); break;
		default :
		{
		#if COURT_TYPE == COURT_APPEAL
			strcpy(court, "ApS-");
		#endif  // APPEAL
		#if COURT_TYPE == COURT_ADMIN
			strcpy(court, "AdmS-");
		#endif  // ADMIN
		#if COURT_TYPE == COURT_AREAL
			strcpy(court, "OS-");
		#endif  // AREAL
		#if COURT_TYPE == COURT_MILITARY
			strcpy(court, "VS-");
		#endif  // MILITARY
		#if COURT_TYPE == COURT_REGIONAL
			strcpy(court, "RS-");
		#endif  // REGIONAL
			UCLP->Seek(COURT_UCLP);
			filter_prefixes(court + strlen(court), UCLP->name, FILTER_UCLP_PREFIXES);
		}
	}

	m.clear();
	Type->Seek(key.type);
	m.printf("%s %s %ld/%ld %s ", court, Type->alias, key.no, key.year, text);

	if (container->key.type == TYPE_INREG || container->key.type == TYPE_SURROUND)
	{
		// KIND_TEXTUAL_IN_DOCS are not sent
		if (strchr(KIND_TEXTUAL_SURROUNDS, container->kind))
		{
			size_t len = m.len();
			const TSurround *surround = (TSurround *) container;

			m.cat(surround->text);
			m.cut(len + LENGTH_OF_NAME);
		}
		else
			m.cat(Kind->Seek(container->kind));

		m.printf(" вх.N %ld/", container->key.no);
	}

	if (strchr(KIND_ENDOCS, container->kind))
		m.printf("%s от %0Dг.", Kind->Seek(container->kind), &((TDecision *) container)->fromDate);
	else
		m.printf("%0Dг.", &container->date);

	if (strchr(KIND_OPEN_SESSIONS, container->kind))
		m.printf(", %0Tч.", &((const TSession *) container)->time);

	static constant CONTROL_ALLS = "\1\2\3\4\5\6\7\10\11\12\13\14\15\16\17\20\21\22\23\24\25\26\27\30\31\32\33\34\35\36\37";
	static CTime last;	// avoid 2+ same-time SMS
	static bool last_failed = false;

	char sms[SIZE_OF_SMS];
	transliterate(str(m), sms, LENGTH_OF_SMS);
	m.compress(0, CONTROL_ALLS);

	char name[SIZE_OF_PATH];
	char stamp[SIZE_OF_NAME];
	CTime time;
	bool failed = true;

	while ((time = Now()) == last)
		Sleep(100);
	last = time;

	sprintf(stamp, "%02d%02d%04ld_%02d%02d%02d", Today.day, Today.month, Today.year, time.hour, time.minute, time.second);
	sprintf(name, "%s\\%s_%s.xml", SMS_PATH, CONFIG_NAME_LOCAL, stamp);

	if (x.OpenSms(m, name))
	{
		x.OpenTag("Message");
		{
			x.Emit("Court", COURT_CODE);
			x.Emit("CaseNo", key.no);
			x.Emit("CaseYear", key.year);
			x.Emit("CaseType", C2S[key.type]);
			x.Emit("Action", C2S[action]);
			x.Emit("Phase", phase);
			x.Emit("Subject", "");
			x.Emit("BodyText", "");
			x.Emit("SMS", sms);
			x.Emit("Machine", CONFIG_NAME_LOCAL);
			x.Emit("Created", Today, time);
		}
		x.CloseTag("Message");

		if (x.Close())
			failed = false;
	}

	if (failed != last_failed)
	{
		char text[SIZE_OF_LOG_TEXT];

		if (failed)
			sprintf(text, "sms error %d at %s", errno, CONFIG_NAME_LOCAL);
		else
			sprintf(text, "sms created at %s", CONFIG_NAME_LOCAL);

		if (container)
			log(LOG_TRANSFER, 0, container, str(m));
		else
			log(LOG_TRANSFER, 0, &key, str(m));

		last_failed = failed;
	}
}

bool update_sidewind(TSideWind *source, TSideWind *target, TGArray<TLinkWind> *links)
{
	if (source->involvement == target->involvement)
		return target->Update();

	if (target->Insert())
	{
		// 2010:340: links array support
		// 2010:341: array only, proxy/redirect support
		for (int i = 0; i < links->Count(); i++)
		{
			TLinkWind *linkWind = (*links)[i];

			if (!strcmp(linkWind->ucn, source->ucn) && linkWind->ucnType == source->ucnType &&
				linkWind->involvement == source->involvement)
			{
				TLinkWind backup;

				backup << linkWind;
				linkWind->involvement = target->involvement;
				linkWind->Insert();
				backup.Delete(true);
			}
			else
			{
				if (!strcmp(linkWind->proxyUcn, source->ucn) &&
					linkWind->proxyUcnType == source->ucnType &&
					linkWind->proxyInvolvement == source->involvement)
				{
					linkWind->proxyInvolvement = target->involvement;
					linkWind->Update("F_PROXY_INVOLVEMENT");
				}
				else if (!strcmp(linkWind->redirectUcn, source->ucn) &&
					linkWind->redirectUcnType == source->ucnType &&
					linkWind->redirectInvolvement == source->involvement)
				{
					linkWind->redirectInvolvement = target->involvement;
					linkWind->Update("F_REDIRECT_INVOLVEMENT");
				}
			}
		}

		// N.B. expenses are stored for person, not person+involvement, so
		// updating them on involvement change is neither needed nor proper

		source->Delete(true);
		return true;
	}

	return false;
}
