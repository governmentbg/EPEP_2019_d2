#include "cac.h"

// Deletion rewritten. No SQL procedures, no transactions, unsuccessful deletion
// of sub-objects does not cancel the operation. The only status carried is that
// of the main object. There are too many externals (files, transfers) for
// anything else. Externals are executed on successful main object deletion.

constant Q_RESTORE_CONTINUE = "няма да може да се възстанови. Желаете ли да продължите?";

static void log_deleted(const TSubpoena *subpoena)
{
	mstr m;

	SubpoenaKind->Seek(subpoena->subpoenaKind);
	m.printf("Призовка %s %s %s", SubpoenaKind->alias, UCNType->Seek(subpoena->ucnType), subpoena->ucn);
	log(LOG_DELETE, IDDELETE, &subpoena->key, subpoena->kind, &subpoena->date, str(m));
}

static bool delete_subpoenas(const char *expr, TGArray<TUCNGroup> *affecteds, const TSubpoena *tSubpoena, const TLawsuit *lawsuit)
{
	TQuery q(expr);
	TSubpoena subpoena;
	bool deleted = false;

	while (q.Read())
	{
		subpoena << q;

		bool leading = tSubpoena && !strcmp(subpoena.ucn, tSubpoena->ucn) && subpoena.ucnType == tSubpoena->ucnType &&
			subpoena.involvement == tSubpoena->involvement && subpoena.eventStatus == tSubpoena->eventStatus &&
			subpoena.subpoenaNo == tSubpoena->subpoenaNo;

		if (subpoena.Delete(true))
		{
			log_deleted(&subpoena);
			eportal_send(&subpoena, lawsuit, LOG_DELETE, "");
			TSubpoenaVariantGroup::Affected(affecteds, &subpoena);

			if (leading)
				deleted = true;
		}
		else if (leading && !subpoena.Try("F_NO"))
			deleted = true;
	}

	return deleted;
}

bool delete_subpoena(TSubpoena *subpoena, TGArray<TUCNGroup> *affecteds, const TLawsuit *lawsuit)
{
	msql m(subpoena->Name);
	sign deleted = -1;
	bool legalAid = strchr(SUBPOENA_KIND_LEGAL_AIDS, subpoena->subpoenaKind);

	// 2008:178 LPR: delete multi subpoena elements
	if (subpoena->multi == SUBPOENA_LEADER)
	{
		subpoena->MultiCriteria(m);
		delete_subpoenas(str(m), affecteds, NULL, lawsuit);
	}
	// 2014:006
	else if (subpoena->requestId && legalAid)
	{
		m.Add(subpoena, "F_NO, F_YEAR, F_TYPE, F_REQUEST_ID");
		deleted = delete_subpoenas(str(m), affecteds, subpoena, lawsuit);
	}
	// 2014:248
	else if (subpoena->execListNo)
	{
		m.Add(subpoena, "F_NO, F_YEAR, F_TYPE, F_KIND, F_DATE, F_SUBPOENA_NO");
		deleted = delete_subpoenas(str(m), affecteds, subpoena, lawsuit);
	}

	if (deleted == -1)
	{
		deleted = subpoena->Delete(true);

		if (deleted)
		{
			eportal_send(subpoena, lawsuit, LOG_DELETE, "");
			log_deleted(subpoena);
			TSubpoenaVariantGroup::Affected(affecteds, subpoena);
		}
	}

	if (subpoena->outRegNo)
	{
		TOutReg outReg;
		TSideWind sideWind;

		outReg.key.no = subpoena->outRegNo;
		outReg.key.year = subpoena->outRegYear;
		outReg.Delete(true);

		sideWind.key = outReg.key;
		sideWind.kind = KIND_OUT_LETTER;
		sideWind.date = outReg.date;
		sideWind.Delete(false);
	}

	// 2014:248
	if (subpoena->execListNo)
	{
		TExecList execList;

		execList << subpoena;
		execList.execListYear = subpoena->subpoenaDate.year;

		if (execList.Delete(true))
		{
			m.clear();
			m.printf("Изпълн. лист %ld %ld", execList.execListNo, execList.execListKind);
			log(LOG_DELETE, IDDELETE, &subpoena->key, subpoena->kind, &subpoena->date, str(m));
		}
		else
		{
			m.clear();
			m.printf("Грешка ИЛ %ld %c %ld", execList.execListNo, execList.type, execList.execListKind);
			log(LOG_DELETE, IDDELETE, &subpoena->key, subpoena->kind, &subpoena->date, str(m));
		}
	}
	// 2014:006
	else if (deleted && subpoena->requestId && legalAid && any(XFER_LEGAL_AID_PATH))
	{
		char requestId[SIZE_OF_LEGAL_AID_ID];
		char name[SIZE_OF_PATH];

		subpoena_legal_name(name, requestId, &subpoena->key, LEGAL_AID_CREATED, subpoena->requestId);

		if (exist(name))
			remove_file(name, "legalaid");
		// 2014:324 LPR: handle various transfer stages
		else if (subpoena->outRegNo)
		{
			TRCDKeyContainer *const container = kind2RCDKeyContainer(subpoena->kind);
			TVirtPtr VP(container);
			TXMLFile x;

			*container << subpoena;
			subpoena_legal_name(name, requestId, &subpoena->key, LEGAL_AID_KILLED, subpoena->requestId);

			if (x.OpenLegal(m, name, container))
			{
				x.OpenTag("CancelRequests");
				{
					x.OpenTag("Request");
					{
						x.Emit("RequestID", requestId);
						x.OpenTag("InstanceData");
						{
							x.Emit("InstanceIdentifier", COURT_CODE);
						}
						x.CloseTag("InstanceData");
						printsub_emit_creator(x, "CancelledBy");
						x.Emit("Reason", "Искането е изтрито от потребител на САС");
						x.Emit("DateCancelled", Today, Now());
					}
					x.CloseTag("Request");
				}
				x.CloseTag("CancelRequests");
			}
			x.Close();
		}
	}

	return deleted;
}

// 2003:332 BRW: new unified wind remover; 2004:014 BRW: subpoena support
// 2005:311 LPR: removed special inreg event handling, using new events
// 2011:291 LPR: buffer deleted sideWinds for bank xfer
void delete_winds(TRCDKeyContainer *container, TGArray<TSideWind> *sideWinds, bool unlinkOnly)
{
	TSideWind sideWind;
	TLinkWind linkWind;
	TBaseWind expense("T_EXPENSE", NULL);

	sideWind << container;
	linkWind << container;
	expense << container;

	if (sideWinds)
		sideWind.LoadFloat(sideWinds);

	sideWind.Delete(false);
	linkWind.Delete(false);
	expense.Delete(false);

	if (strchr(KIND_ENDOCS, container->kind))
	{
		TJudgement judgement;
		judgement << container;
		judgement.Delete(false);
	}

	TBaseWind tSubpoena("T_SUBPOENA", NULL);

	if (unlinkOnly)
		tSubpoena.Delete(false);
	else
	{
		TGArray<TUCNGroup> affecteds;
		TSubpoena subpoena;
		msql m;

		tSubpoena << container;
		tSubpoena.FloatCriteria(m);

		TQuery q(str(m));
		while (q.Read())
		{
			subpoena << q;

			if (subpoena.Try())
				delete_subpoena(&subpoena, &affecteds, dynamic_cast<TLawsuit *> (container));
		}
	}
}

bool delete_decision(TDecision *decision, TLawsuit *lawsuit)
{
	if (decision->flags & DECISION_PUBLIC)
		xfer_web_delete(decision, lawsuit);

#if COURT_TYPE == COURT_REGIONAL
	TGArray<TSubpoena> subpoenas;

	if (strchr(KIND_DECISIONS, decision->kind))
	{
		TSubpoena subpoena;
		msql m;

		subpoena << decision;
		subpoena.eventStatus = decision->result;
		subpoena.FloatCriteria(m, "F_ANNOUNCE_NO, F_SUBPOENA_DATE");
		m.AddIsNull("F_ANNOUNCE_NO", false);
		subpoena.LoadArray(subpoenas, str(m));
	}
#endif  // REGIONAL

	// 2017:089 judgement moved to delete_winds
	delete_winds(decision);
	unschedule(SCHED_NULL, decision);

	if (!decision->Delete(true))
		return false;

	send_sms(decision, "Изтриване на", SMS_ENDOC, SMS_DELETE, decision);

#if COURT_TYPE == COURT_REGIONAL
	for (int i = 0; i < subpoenas.Count(); i++)
	{
		AnnounceIndex->no = subpoenas[i]->announceNo;
		AnnounceIndex->year = subpoenas[i]->subpoenaDate.year;
		AnnounceIndex->Delete(true);
	}
#endif  // REGIONAL

	eportal_send(NULL, decision, lawsuit, LOG_DELETE);  // ignore PUBLIC
	xfer_email(decision, LOG_DELETE);
	eispp_delete(decision);
	return true;
}

void delete_finalization(const TSession *session, const TLawsuit *lawsuit)
{
	TLawInterval interval(lawsuit->key, session->date);

	if (interval.Read(NULL, true) && !interval.finished.Empty() &&
		ask("Желаете ли да се изтрият и данните за продължителността и края на делото?"))
	{
		interval.age = AGE_NONE_YET;
		interval.finished.Clear();
		interval.grant = GRANT_NONE_YET; 
		interval.Update("F_AGE, F_FINISHED, F_GRANTED");
	}
}

bool delete_surroundment(TSurroundment *surroundment, TLawsuit *lawsuit, sign bank_event)
{
	if (surroundment->decisionKind)
	{
		TDecision decision;
		decision.Init(surroundment);

		if (decision.flags & DECISION_PUBLIC)
			xfer_web_delete(&decision, lawsuit);

		if (bank_event)
			xfer_bank_event(lawsuit, NULL, NULL, &decision, NULL, NULL, NULL, XFER_DELETE);
	}

	bool result = bank_event == -1 ? surroundment->ClearSurrouendoc() : surroundment->Delete(true);

	if (result && surroundment->decisionKind)
		log_surrouendoc(LOG_DELETE, CM_ADMIN_DELETE_LAWSUIT_EVENT, surroundment);

	return result;
}

static bool delete_session(TSession *session, TLawsuit *lawsuit)
{
	TLawInterval interval(lawsuit->key, session->date);

	// 2017:037 LPR: delete 2ndary interval
	if (session->flagsY & SESSION_Y_RESTART)
		interval.Delete(true);
	// 2009:202; 2010:286 TRQ: FIX: update firstSet for secondary interval
	else if ((session->flagsX & SESSION_X_FIRSET) && interval.Read(NULL, true))
	{
		interval.firstSet.Clear();
		interval.Update("F_FIRST_SET");
	}

	// 2016:118 delete individually because of eventNo
	TSurroundment surroundment;
	msql m(surroundment.Name);

	surroundment.SessionCriteria(m, session);
	TQuery q(str(m));

	while (q.Read())
	{
		surroundment << q;
		// false because deleting session is enough for all endocs
		delete_surroundment(&surroundment, lawsuit, false);
	}

	delete_winds(session);

	// 2008:287 result check(s) -> final date
	// 2009:202 moved to delete_finalization
	if (!session->final.Empty())
		delete_finalization(session, lawsuit);

	unschedule(SCHED_NULL, session);

	TSessionTimes sessionTimes;
	sessionTimes << session;
	sessionTimes.Delete(false);

	if (session->Delete(true))
	{
		if (strchr(KIND_OPEN_SESSIONS, session->kind) && session->result == RESULT_FIXED)
			send_sms(session, "Изтриване на насрочено за", SMS_SET, SMS_DELETE, session);
		else if (strchr(KIND_CLOSED_SESSIONS, session->kind))
		{
			if (strchr(RESULT_HOLDS, session->result))
				send_sms(session, "Изтриване на обявено без движение на", SMS_HOLD, SMS_DELETE, session);
			else if (session->result == RESULT_STOPPED)
				send_sms(session, "Изтриване на спряно на", SMS_HOLD, SMS_DELETE, session);
		}

		// 2018-10-19: LPR: unconditional
		//if (session->IsEPortSessEndoc())
			eportal_send(NULL, session, lawsuit, WHAT_SESS_ENDOC, LOG_DELETE);

		// 2018-10-19: LPR: unconditional
		//if (session->IsEPortSession())
			eportal_send(NULL, session, lawsuit, WHAT_SESSION, LOG_DELETE);

		xfer_email(session, LOG_DELETE);

		if (!strchr(RESULT_ENTERABLES, session->result))
			eispp_delete(session);

		return true;
	}

	return false;
}

// ----- delete actions ------------------------------------------------------
static void delete_message(const char *message, bool deleted)
{
	if (deleted)
		info(message, "");
	else
		error(message, "НЕ ");
}

void delete_archive(TWindow *, TArchive *archive, TExternContainer *lawsuit)
{
	if (ask("Внимание! След изтриване данните за архивиране на делото няма да могат да се възстановят. %s", Q_CONTINUE))
	{
		bool deleted = archive->Delete(true);

		if (deleted)
			archive->Signal(ELECTRIC_EDIT, lawsuit);

		delete_message("Данните за архивиране на делото %sса изтрити.", deleted);
	}
}

void delete_mails(TOutReg *outReg)
{
	// 2014:023 LPR: char[] -> msql
	TRegisteredMail mail;
	msql m(mail.Name, MQ_DELETE);

	m.AddLong("F_DOC_NO", outReg->key.no);
	m.AddLong("F_DOC_YEAR", outReg->key.year);
	m.AddChar("F_DOC_TYPE", outReg->key.type);
	mail.DataBase->Execute(str(m), ET_ERROR);
}

bool delete_inreg_event(TInRegEvent *inRegEvent)
{
	delete_winds(inRegEvent);
	unschedule(SCHED_NULL, inRegEvent);
	return inRegEvent->Delete(true);
}

static bool delete_moves(TRCDKeyGroup *group)
{
	TMovement movement;
	movement.key = group->key;
	return movement.Delete(false);
}

static void delete_repairs(TRCDKeyContainer *container)
{
	TReplacePairs repairs;
	repairs.key = container->key;
	repairs.Delete(false);
}

#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
static void delete_eispps(TOriginContainer *container)
{
	if (strchr(KIND_PUNISHMENT_INDOCS, container->kind) || strchr(KIND_PUNISHMENT_LAWS, container->kind))
	{
		for (constant *name = EISPP_TABLE_NAMES; *name; name++)
		{
			msql m(*name, MQ_DELETE);
			m.Add(container, "F_NO, F_YEAR, F_TYPE");
			container->DataBase->Execute(str(m), ET_ERROR);
		}
	}
}
#endif  // APPEAL || TYPE == AREAL || TYPE == MILITARY || REGIONAL

#if COURT_TYPE == COURT_ADMIN
static void delete_eispps(TOriginContainer *)
{
}
#endif  // ADMIN

static bool delete_inreg(TInReg *inReg, TGArray<TSideWind> *sideWinds, TLawsuit *lawsuit, bool sms)
{
	TInRegEvent inRegEvent;
	msql m;

	inRegEvent.key = inReg->key;
	inRegEvent.FloatCriteria(m);

	TQuery q(str(m));
	while (q.Read())
	{
		inRegEvent << q;
		delete_inreg_event(&inRegEvent);
	}

	delete_moves(inReg);
	delete_repairs(inReg);
	delete_winds(inReg, sideWinds);

	if (inReg->Delete(true))
	{
		if (inReg->kind == KIND_EPORTAL_REQUEST)
		{
			TEPortAssign assign;
			assign.key = inReg->key;
			assign.Delete(false);
		}
		else	// 2018-10-03: sendable as argument
			eportal_send(NULL, inReg, lawsuit, LOG_DELETE, inReg->EPortalSendable());

		if (strchr(KIND_APPEAL_IN_DOCS, inReg->kind))
		{
			if (sms)
				send_sms(inReg->lawsuit, "Изтриване на", SMS_INREG, SMS_DELETE, inReg);
			xfer_email(inReg, LOG_DELETE);
			eispp_delete(inReg);
		}

	#if COURT_TYPE == COURT_ADMIN
		// 2007:221 LPR/LRQ: don't free index if # remains in use
		TRequest request;

		request.key.no = inReg->key.no;
		request.key.year = inReg->key.year;

		if (!request.Try("F_NO"))
	#endif  // ADMIN
		{
			*RequestIndex << inReg;
			RequestIndex->Delete(true);
		}

		return true;
	}

	return false;
}

static bool table_clean(const char *s, const char *what, const char *with)
{
	TQuery q(s);

	if (q.Read())
		return with ? error("Изтрийте първо %s, свързани %s.", what, with) : false;

	return true;
}

// 2008:008 LRQ: FIX: decision kind support; 2009:150 count -> check
// 2010:286 LPR: removed decisionKind argument, not used
// 2011:272 check -> clean, interactive
bool inreg_clean(TIndexContainer *container, const char *with)
{
	msql m("T_INREG", "F_NO");
	bool decision = strchr(KIND_DECISIONS, container->kind);

	m.AddLong("F_LAWSUIT_NO", container->key.no);
	m.AddLong("F_LAWSUIT_YEAR", container->key.year);
	m.AddChar("F_LAWSUIT_TYPE", container->key.type);

	// 2016:1034 URQ/TRQ/FIX? decision criteria
	if (decision)
	{
		m.Add(container, "F_SESSION_KIND");
		m.AddChar("F_DECISION_KIND", container->kind);
	}
	else
		m.AddChar("F_SESSION_KIND", container->kind);

	m.AddDate("F_DECISION_DATE", container->date);
	return table_clean(str(m), "документите от входящ регистър", with);
}

// 2011:272 check -> clean, interactive
bool surround_clean(TIndexContainer *container, const char *with)
{
	msql m("T_SURROUND", "F_NO");

	m.AddLong("F_LAWSUIT_NO", container->key.no);
	m.AddLong("F_LAWSUIT_YEAR", container->key.year);
	m.AddChar("F_LAWSUIT_TYPE", container->key.type);
	m.AddDate("F_DECISION_DATE", container->date);
	m.AddChar("F_DECISION_KIND", container->kind);
	return table_clean(str(m), "съпровождащите документи", with);
}

static void delete_session_event(TLawsuit *lawsuit, TSession *session)
{
	char status = session->Status();

	if (strchr(RESULT_SURROUENDOCS, status) && try2surround(session, NULL, NULL, KIND_SURROUENDOCS, true) &&
		!warn("По заседанието има вписани съдебни актове основно възражения. %s", Q_CONTINUE))
	{
		return;
	}

	if (check_subpoenas(session) && warn("Заседанието е %s%s. %s", strchr(RESULT_UNCARRIEDS, status) ? "" : "с резултат ",
		Result->Seek(status), Q_CONTINUE))
	{
		if (delete_session(session, lawsuit))
		{
			xfer_bank_event(lawsuit, session, NULL, NULL, NULL, NULL, NULL, XFER_DELETE);
			log(LOG_DELETE, CM_ADMIN_DELETE_LAWSUIT_EVENT, session, Result->Seek(session->result));
			info("Заседанието е изтрито.");
		}
	}
}

static void delete_decision_event(TLawsuit *lawsuit, TDecision *decision)
{
	if (delete_decision(decision, lawsuit))
	{
		xfer_bank_event(lawsuit, NULL, NULL, decision, NULL, NULL, NULL, XFER_DELETE);
		log(LOG_DELETE, CM_ADMIN_DELETE_LAWSUIT_EVENT, decision, Result->Seek(decision->result));

		// 2012:082 LPR/LRQ: rewrite: full set of final dates
		if (!decision->returned.Empty())
		{
			TSession session;

			session.key = decision->key;
			session.date = decision->date;
			session.kind = decision->sessionKind;

			if (session.Get("F_RESULT, F_FINAL") && detached_final(&session, decision))
				delete_finalization(&session, lawsuit);
		}

		info("Доkументът е изтрит.");
	}
}

void delete_lawsuit_event(TWindow *, TLawsuit *lawsuit, TIndexContainer *container)
{
	if (strchr(KIND_SESSIONS, container->kind))
	{
		if (inreg_clean(container, "със заседанието") && surround_clean(container, "със заседанието"))
		{
			TSession *session = (TSession *) container;

			if (try2decide(session, NULL))
				error("Изтрийте първо крайните документи по заседанието.");
			else
				delete_session_event(lawsuit, session);
		}
	}
	else if (strchr(KIND_ENDOCS, container->kind))
	{
		if (inreg_clean(container, "със съдебния акт") && surround_clean(container, "със съдебния акт") &&
			check_subpoenas(container) && warn("След изтриване акта %s", Q_RESTORE_CONTINUE))
		{
			delete_decision_event(lawsuit, (TDecision *) container);
		}
	}
	else
		fatal("%d: kind not supported by delete lawsuit event", container->kind);
}

bool delete_resolution(TRequestResolution *resolution)
{
	// 2007:180 ASSIGNED support
	if (resolution->resolution == RESOLUTION_LAWED && atob(resolution->assign))
	{
		const char *s;

		if ((s = resolution->text.rchr('\n')) != NULL)
			resolution->text.delead(s + 1);
		resolution->resolution = RESOLUTION_ASSIGNED;
		return resolution->Update("F_RESOLUTION, F_TEXT");
	}

	delete_winds(resolution);
	return resolution->Delete(true);
}

static void delete_outreg_restart(const TOutReg *outReg)
{
	if (outReg->flagsX & OUTREG_X_RESTART)
	{
		TLawInterval interval(outReg->lawsuit, outReg->returned);
		interval.Delete(true);
	}
}

static bool delete_outreg(TOutReg *outReg, TLawsuit *lawsuit)
{
	TConnect connect;

	connect.key = outReg->key;
	connect.Delete(false);
	delete_mails(outReg);
	delete_winds(outReg, NULL, true);	// 2017:243 LPR: set unlink only

	if (strchr(RESULT_IOREG_RETURNEDS, outReg->result))
	{
		delete_receive_list(outReg);
		outReg->result = RESULT_IOREG_SEND;		// for delete_send_list()
		delete_outreg_restart(outReg);
	}

	delete_send_list(outReg);
	delete_repairs(outReg);

	if (outReg->Delete(true))
	{
		bool p64WrongDelete = false;
		TLawsuit tLawsuit;

		if (!lawsuit && strchr(TYPE_LAWSUITS, outReg->lawsuit.type))
		{
			tLawsuit.key = outReg->lawsuit;
			if (tLawsuit.Get())
				lawsuit = &tLawsuit;
		}

		eportal_send(NULL, outReg, lawsuit, LOG_DELETE);
		eispp_delete(outReg, true);

		if (atob(outReg->receiver))
		{
		#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
			if (lawsuit && outReg->receiverType == UCN_INSTANCE_HIGHER)
			{
				TInReg inReg;

				inReg.key.no = outReg->inNo;
				inReg.key.year = outReg->inYear;

				if (inReg.Try("F_DECISION_KIND, F_DECISION_DATE"))
				{
					TDecision decision;

					if (decision.P64Wrong(lawsuit, &inReg, true) && (decision.flags & DECISION_PUBLIC))
					{
						decision.flags &= ~DECISION_PUBLIC;

						if (decision.Update("F_FLAGS"))
						{
							log_event(LOG_EDIT, 0, &decision, decision.flags & DECISION_PUBLIC);
							xfer_web_delete(&decision, lawsuit);
							p64WrongDelete = true;
							eportal_send(NULL, &decision, lawsuit, LOG_DELETE);  // 2018-10-19
						}
					}
				}
			}
		#endif  // APPEAL || AREAL || MILITARY || REGIONAL
			xfer_delete(outReg);
		}

		if (outReg->inNo && !p64WrongDelete)
			xfer_web(outReg, true);

		if (outReg->inNo)
			xfer_email(outReg, LOG_DELETE);

		return true;
	}

	return false;
}

// 2011:291 LPR: load to-be-deleted sideWinds for bank xfer
static bool delete_surround(TSurround *surround, TGArray<TSideWind> *sideWinds, bool sms = true)
{
	TSurroundment surroundment;

	surroundment << surround;
	surroundment.Delete(false);
	delete_moves(surround);
	delete_winds(surround, sideWinds);

	if (surround->Delete(true))
	{
		*RequestIndex << surround;
		RequestIndex->Delete(true);
		unschedule(SCHED_NULL, surround);
		xfer_email(surround, LOG_DELETE);
		if (sms)
			send_sms(surround->lawsuit, "Изтриване на", SMS_SURROUND, SMS_DELETE, surround);
		if (strchr(TYPE_LAWSUITS, surround->lawsuit.type))
		{
			TLawsuit lawsuit;
			lawsuit.key = surround->lawsuit;

			if (lawsuit.Get("F_FLAGS_Y"))
				eportal_send(NULL, surround, &lawsuit, LOG_DELETE);
		}
		return true;
	}

	return false;
}

static bool delete_prove(TProve *prove)
{
	TProveAction proveAction;

	proveAction.key = prove->key;
	proveAction.Delete(false);
	delete_winds(prove);
	delete_moves(prove);
	return prove->Delete(true);
}

static bool delete_container(TRCDKeyContainer *container, bool sms, TLawsuit *lawsuit)
{
	if (strchr(KIND_SURROUNDS, container->kind))
		return delete_surround((TSurround *) container, NULL, sms);

	if (strchr(KIND_SESSIONS, container->kind))
		return delete_session((TSession *) container, lawsuit);

	// 2005:311 LPR: separated inregs and new inreg events
	if (strchr(KIND_INREG_EVENTS, container->kind))
		return delete_inreg_event((TInRegEvent *) container);

	// 2005:311 LPR: using KIND_ instead of TYPE_-plus-dynacast
	if (strchr(KIND_IN_REG_DOCS, container->kind))
		return delete_inreg((TInReg *) container, NULL, lawsuit, sms);

	if (strchr(KIND_OUT_REG_DOCS, container->kind))
		return delete_outreg((TOutReg *) container, lawsuit);

	// 2005:318 LPR: BUGFIX: was missing
	if (strchr(KIND_REQUEST_RESOLUTIONS, container->kind))
		return delete_resolution((TRequestResolution *) container);

	// 2003:287 BRW: check if last remaining are decisions
	// 2004:055 BUGFIX: KIND_DECISIONS -> KIND_ENDOCS
	if (strchr(KIND_ENDOCS, container->kind))
		return delete_decision((TDecision *) container, lawsuit);

	if (strchr(KIND_PROVES, container->kind))
		return delete_prove((TProve *) container);

	fatal("%d: kind not supported by delete_container", container->kind);
	return false;
}

#if COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
void delete_distrib(const TBaseSession *session)
{
	TJurorDistrib distrib;

	if (distrib.Find(session, true))
	{
		bool deleted = distrib.Delete(true);

		if (deleted)
			log_juror_double(LOG_DELETE, 0, session, "на разпределени заседатели", JUROR_DISTRIB_DELETE);

		delete_message("Разпределението %sе изтрито.", deleted);
	}
}

void delete_distrib(TWindow *, const TBaseSession *session)
{
	if (warn("Внимание! След изтриване разпределението %s", Q_RESTORE_CONTINUE))
		delete_distrib(session);
}
#endif  // AREAL || MILITARY || REGIONAL

void delete_foreign(TWindow *, TForeignAddress *foreignAddress)
{
	if (warn("Внимание! След изтриване адреса %s", Q_RESTORE_CONTINUE))
		delete_message("Адреса %sе изтрит.", foreignAddress->Delete(true));
}

static void delete_movement(TRequest *request)
{
	TGSArray<TRCDKeyContainer> array;
	query_events(request, &array);

	for (int i = 0; i < array.Count(); i++)
		delete_container((TDatedContainer *) array[i], true, NULL);
}

static bool delete_request(TRequest *request)
{
	TAccusation accusation;
	TConnect connect;
	TProve prove;

	delete_moves(request);
	delete_movement(request);
	delete_winds(request);
	accusation.key = request->key;
	accusation.Delete(false);
	connect.key = request->key;
	connect.Delete(false);
	delete_repairs(request);
	delete_eispps(request);
	if (prove.TryBy(request))
		delete_prove(&prove);

	// 2004:355 delete common index
	if (request->Delete(true))
	{
	#if COURT_TYPE == COURT_ADMIN
		// 2007:221 LPR/LRQ: don't free index if # remains in use
		TInReg inReg;

		inReg.key.no = request->key.no;
		inReg.key.year = request->key.year;

		if (!inReg.Try("F_NO"))
	#endif  // ADMIN
		{
			*RequestIndex << request;
			RequestIndex->Delete(true);
		}

		eportal_send(request, LOG_DELETE);
		return true;
	}

	return false;
}

void delete_indoc(TWindow *, TRCDKeyContainer *container)
{
	if (check_subpoenas(container) && warn("Внимание! След изтриване документа %s", Q_RESTORE_CONTINUE))
	{
		const char *what = NULL;	// also deleted flag
		TGArray<TSideWind> sideWinds;

		switch (container->key.type)
		{
			case TYPE_REQUEST :
			{
				if (delete_request((TRequest *) container))
					what = "на входящ документ";
				eispp_delete(container);
				break;
			}
			case TYPE_SURROUND :
			{
				if (delete_surround((TSurround *) container, &sideWinds))
				{
					what = "на съпровождащ документ";
					xfer_bank_surround(NULL, (TSurround *) container, &sideWinds, XFER_DELETE);
				}

				break;
			}
			case TYPE_INREG :
			{
				TInReg *inReg = (TInReg *) container;

				if (delete_inreg(inReg, &sideWinds, NULL, true))
				{
					what = "на документ от входящ регистър";
					xfer_bank_event(NULL, NULL, NULL, NULL, inReg, &sideWinds, NULL, XFER_DELETE);
				}

				break;
			}
			default : fatal("%d: unsupported delete indoc type", container->key.type);
		}

		if (what)
			log(LOG_DELETE, CM_ADMIN_DELETE_INDOC, container, what);

		delete_message("Доkументът %sе изтрит.", what);
	}
}

void delete_old_names(TWindow *, TNamedGroup *oldGroup)
{
	if (warn("Внимание! След изтриване предишното име %s", Q_RESTORE_CONTINUE))
		delete_message("Името %sе изтрито.", oldGroup->Delete(true));
}

void delete_inreg_event(TWindow *, TInRegEvent *inRegEvent)
{
	if (inreg_clean(inRegEvent, "с разпореждането") && check_subpoenas(inRegEvent) &&
		(inRegEvent->result != RESULT_EPORTAL_ALLOWED || warn("Разпореждането е със статус %s. %s",
			Result->Seek(RESULT_EPORTAL_ALLOWED), Q_CONTINUE)) &&
		warn("Внимание! След изтриване разпореждането %s", Q_RESTORE_CONTINUE))
	{
		bool deleted = delete_inreg_event(inRegEvent);

		if (deleted)
			log(LOG_DELETE, CM_ADMIN_DELETE_INREG_EVENT, inRegEvent, Result->Seek(inRegEvent->result));

		delete_message("Разпореждането %sе изтрито.", deleted);
	}
}

void delete_resolution(TWindow *, TRequestResolution *resolution)
{
	// 2011:272 LPR: was "със заседанието"
	if (inreg_clean(resolution, "с разпореждането") && check_subpoenas(resolution) &&
		warn("Внимание! След изтриване разпореждането %s", Q_RESTORE_CONTINUE))
	{
		bool deleted = delete_resolution(resolution);

		if (deleted)
			log(LOG_DELETE, CM_ADMIN_DELETE_REQUEST_RESOLUTION, resolution, Resolution->Seek(resolution->resolution));

		delete_message("Разпореждането %sе изтрито.", deleted);
	}
}

void delete_outreg_return(TWindow *, TOutReg *outReg)
{
	if (warn("Внимание! След изтриване данните за връщането на изходящия документ няма да могат да се възстановят. %s",
		Q_CONTINUE))
	{
		TConnect connect;

		connect.key = outReg->key;
		connect.Delete(false);
		revert_receive_list(outReg);
		delete_outreg_restart(outReg);

		outReg->returned.Clear();
		outReg->result = RESULT_IOREG_SEND;
		outReg->resultI = outReg->resultII = RESOLUTION_FAKE;
		outReg->resultKS = outReg->resultMJ = RESOLUTION_FAKE;
		*outReg->vssIndex = '\0';
		outReg->remark.clear();
		outReg->remarkVZ.clear();
		outReg->remarkKS.clear();

		bool deleted = outReg->Update();

		if (deleted)
		{
			eispp_delete(outReg, false);
			log(LOG_DELETE, CM_ADMIN_DELETE_OUTREG_RETURN, outReg, "на връщане");

			if (outReg->inNo)
				xfer_web(outReg);
		}

		delete_message("Данните за връщането на изходящия документ %sса изтрити.", deleted);
	}
}

static bool delete_lawsuit(TLawsuit *lawsuit, TGArray<TSideWind> *sideWinds)
{
#if COURT_TYPE == COURT_AREAL
	if (lawsuit->key.type == TYPE_FIRM_LAW)
	{
		TDamn damn;

		// 2004:167 LPR: conditional
		if (find_damn(lawsuit, &damn, false) && damn.Delete(true))
		{
			TAddress address;

			// 2009:168 FIX: F_NO fails
			address << damn;

			if (address.Try())
				address.Delete(true);
		}
	}
#endif  // AREAL

	// 2005:341 LPR: prove support
	TProve prove;

	if (prove.TryBy(lawsuit))
	{
		if (lawsuit->source.type == TYPE_REQUEST)
		{
			prove.lawsuit = lawsuit->source;

			if (!prove.Update("F_LAWSUIT_NO, F_LAWSUIT_YEAR, F_LAWSUIT_TYPE"))
				error("Данните за доказателството по делото не се прехвърлиха към входящия документ!");
		}
		else
			delete_prove(&prove);
	}

	TAccusation accusation;
	TConnect connect;
	TLawInterval interval;

	accusation.key = lawsuit->key;
	accusation.Delete(false);
	connect.key = lawsuit->key;
	connect.Delete(false);
	delete_repairs(lawsuit);
	interval.key = lawsuit->key;
	interval.Delete(false);
	delete_winds(lawsuit, sideWinds);
	delete_eispps(lawsuit);
	unschedule(SCHED_NULL, lawsuit);
	eportal_send(NULL, sideWinds, lawsuit, LOG_DELETE);

	if (lawsuit->Delete(true))
	{
		// 2004:009 LPR: revert_source() at this level
		if (lawsuit->source.type == TYPE_REQUEST)
			revert_source(lawsuit);

		eportal_send(NULL, lawsuit, LOG_DELETE);
		return true;
	}

	return false;
}

void delete_lawsuit(TWindow *, TLawsuit *lawsuit)
{
	mstr m;
	m.printf("%s %ld/%ld", Type->Seek(lawsuit->key.type), lawsuit->key.no, lawsuit->key.year); 

	if (check_subpoenas(lawsuit) && warn("Внимание! След изтриване %s %s", str(m), Q_RESTORE_CONTINUE))
	{
		TGArray<TSideWind> sideWinds;
		bool deleted = delete_lawsuit(lawsuit, &sideWinds);

		if (deleted)
		{
			xfer_bank_case(lawsuit, &sideWinds, NULL, XFER_DELETE);
			eispp_delete(lawsuit);
			log(LOG_DELETE, CM_ADMIN_DELETE_LAWSUIT, lawsuit);
		}

		m.cat(" %sе изтрито.");
		delete_message(str(m), deleted);
	}
}

bool delete_incomplete(TRCDKeyContainer *container)
{
	bool deleted;

	if (strchr(KIND_INDOCS, container->kind))
		deleted = delete_request((TRequest *) container);
	else if (strchr(KIND_LAWSUITS, container->kind))
	{
		TGArray<TSideWind> sideWinds;
		deleted = delete_lawsuit((TLawsuit *) container, &sideWinds);
	}
	else
		deleted = delete_container(container, false, NULL);

	if (deleted)
	{
		if (strchr(KIND_IN_REG_DOCS, container->kind))
			log(LOG_DELETE, CM_DELETE_EMPTY, (TInReg *) container);
		else if (strchr(KIND_SURROUNDS, container->kind))
			log(LOG_DELETE, CM_DELETE_EMPTY, (TSurround *) container);
		else
			log(LOG_DELETE, CM_DELETE_EMPTY, container);
	}

	return deleted;
}

static const char *delete_movement(TLawsuit *lawsuit, TGSArray<TRCDKeyContainer> *array)
{
	const char *undeleted = NULL;
	msql m;
	long BACK_EPORTAL = XFER_EPORTAL;

	XFER_EPORTAL = 0;

	if (!delete_moves(lawsuit))
		undeleted = "T_MOVEMENT";

	// Logical movement
	// 2009:246 FIX: doit backwards, delete outReg xfer requires inReg
	for (int index = array->Count() - 1; index >= 0; index--)
	{
		TDatedContainer *container = (TDatedContainer *) (*array)[index];

		if (delete_container(container, true, lawsuit))
			array->Remove(index);
		else
			undeleted = container->Name;
	}

	{
		// 2009:230 LPR: moved here from delete law
		TJurorDistrib distrib;

		distrib.key = lawsuit->key;
	#if 20170149
		distrib.FloatCriteria(m, MQ_DELETE);

		if (!lawsuit->DataBase->Execute(str(m), ET_ERROR))
			undeleted = distrib.Name;
	#else  // 20170149
		if (distrib.Try() && !distrib.Delete(true))
			undeleted = distrib.Name;
	#endif  // 20170149
	}

	// 2017:039 rewritten
	TLawInterval interval(lawsuit->key, lawsuit->date);

	interval.FloatCriteria(m, MQ_DELETE);
	m.AddFlags("F_FLAGS", LAW_INTERVAL_SECONDARY, LAW_INTERVAL_SECONDARY, LAW_INTERVAL_ALL);

	if (!lawsuit->DataBase->Execute(str(m), ET_ERROR))
		undeleted = interval.Name;

	if (!interval.Read(NULL, true))
		undeleted = interval.Name;
	else
	{
		interval.age = AGE_NONE_YET;
		interval.finished.Clear();
		interval.grant = GRANT_NONE_YET;
		interval.firstSet.Clear();

		if (!interval.Update("F_AGE, F_FINISHED, F_GRANTED, F_FIRST_SET"))
			undeleted = interval.Name;
	}

	XFER_EPORTAL = BACK_EPORTAL;
	eportal_delete_movement(lawsuit);
	
	return undeleted;
}

void delete_movement(TWindow *, TLawsuit *lawsuit)
{
	mstr m;
	m.printf("%s %ld/%ld", Type->Seek(lawsuit->key.type), lawsuit->key.no, lawsuit->key.year);

	if (warn("Внимание! След изтриване никакъв документ от движението на %s %s", str(m), Q_RESTORE_CONTINUE))
	{
		log(LOG_DELETE, CM_ADMIN_DELETE_LAWSUIT_MOVEMENT, lawsuit, "на движението на дело");

		TGSArray<TRCDKeyContainer> array;
		query_events(lawsuit, &array);

		int count = array.Count();
		const char *undeleted = delete_movement(lawsuit, &array);

		if (array.Count() < count)
		{
			xfer_bank_case(lawsuit, NULL, NULL, XFER_DELETE);
			xfer_bank_filter(&array);
			xfer_bank_export(lawsuit, &array);
		}
	#if TESTVER
		if (undeleted)
			error("Тестова версия: %s: грешка при изтриване.", undeleted);
	#endif  // TESTVER
		static constant MOVEMENT = "Движението на ";

		mbk_insert(&m, MOVEMENT, 0, strlen(MOVEMENT));
		m.cat(" %sе напълно изтрито.");
		delete_message(str(m), !undeleted);
	}
}

#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
static bool delete_eispp_nmr(TRequest *request)
{
	eispp_delete(request);
	strcpy(request->eispp_nmr, "");
	delete_eispps(request);			// 2014:349 FIX: was missing

	if (request->Update("F_EISPP_NMR"))
	{
		log(LOG_DELETE, CM_ADMIN_DELETE_EISPP_NMR, request, "на ЕИСПП номер на НП");
		return true;
	}

	return false;
}

static void delete_eispp_nmr(TWindow *, TRequest *request)
{
	if (warn("Внимание! След изтриване, събитието получаване на дело по ЕИСПП %s", Q_RESTORE_CONTINUE))
		delete_message("ЕИСПП номера %sе изтрит.", delete_eispp_nmr(request));
}

static void delete_eispp_nmr(TWindow *, TLawsuit *lawsuit)
{
	if (warn("Внимание! След изтриване събитията по ЕИСПП няма да могат да се възстановят. %s", Q_CONTINUE))
	{
		TGSArray<TRCDKeyContainer> array;
		query_events(lawsuit, &array);

		log(LOG_DELETE, CM_ADMIN_DELETE_EISPP_NMR, lawsuit, "на ЕИСПП номер на НП");

		for (int i = array.Count() - 1; i >= 0; i--)
		{
			TRCDKeyContainer *container = array[i];

			if (strchr(KIND_SESSIONS, container->kind))
			{
				TSession *session = (TSession *) container;

				if (!strchr(RESULT_ENTERABLES, session->result))
					eispp_delete(session);
			}
			else if (strchr(KIND_APPEAL_IN_DOCS, container->kind))
				eispp_delete((TInReg *) container);
			else if (strchr(KIND_OUT_REG_DOCS, container->kind))
				eispp_delete((TOutReg *) container, true);
			else if (strchr(KIND_DECISIONS, container->kind))
				eispp_delete((TDecision *) container);
		}

		eispp_delete(lawsuit);
		delete_eispps(lawsuit);			// 2014:349 FIX: was missing
		strcpy(lawsuit->eispp_nmr, "");

		if (lawsuit->Update("F_EISPP_NMR"))
		{
			log(LOG_DELETE, CM_ADMIN_DELETE_EISPP_NMR, lawsuit, "на ЕИСПП номер на НП");

			TRequest request;
			request.key = lawsuit->source;

			if (request.key.type != TYPE_REQUEST ||
				(request.Get() && (!any(request.eispp_nmr) || delete_eispp_nmr(&request))))
			{
				info("ЕИСПП номера е изтрит.");
			}
			else
			{
				error("ЕИСПП номера на делото е изтрит.\n\nЕИСПП номера на входящия документ НЕ е изтрит.");
			}
		}
		else
			error("ЕИСПП номера НЕ е изтрит.");
	}
}

void delete_eispp_nmr(TWindow *parent, TOriginContainer *container)
{
	if (container->key.type == TYPE_REQUEST)
		delete_eispp_nmr(parent, (TRequest *) container);
	else
		delete_eispp_nmr(parent, (TLawsuit *) container);
}
#endif  // APPEAL || AREAL || MILITARY || REGIONAL

void delete_movement(TWindow *, TMovement *movement)
{
	if (warn("Внимание! След изтриване предоставянето/връщането %s", Q_RESTORE_CONTINUE))
	{
		bool deleted = movement->Delete(true);

		if (deleted)
			log(LOG_DELETE, CM_ADMIN_DELETE_MOVEMENT, movement);

		delete_message("Предоставянето/връщането %sе изтрито.", deleted);
	}
}

void delete_outreg(TWindow *, TOutReg *outReg)
{
	msql m;
	TSubpoena subpoena;
	TNIAProtocol protocol;
	bool subexist = false;
	bool niaexist = false;

	if (outReg->lawsuit.type && outReg->lawsuit.no)
	{
		m.Begin(subpoena.Name);
		m.AddLong("F_NO", outReg->lawsuit.no);
		m.AddLong("F_YEAR", outReg->lawsuit.year);
		m.AddChar("F_TYPE", outReg->lawsuit.type);
		m.AddLong("F_OUTREG_NO", outReg->key.no);
		m.AddLong("F_OUTREG_YEAR", outReg->key.year);

		TQuery q(str(m));
		if (q.Read())
		{
			subexist = true;
			subpoena << q;
			m.clear();
			m.printf("Внимание! С посочения изходящ номер е изпратено придружително писмо относно призовка/съобщение "
				"за %s от %Dг. по %s %ld/%ldг. Относно придружителното писмо ще бъде изтрита само информацията за "
				"изходящ номер.", Kind->Seek(subpoena.kind), &subpoena.date, Type->Seek(subpoena.key.type),
				subpoena.key.no, subpoena.key.year);
		}
	}
	else
	{
		m.Begin(protocol.Name);
		m.AddLong("F_OUT_NO", outReg->key.no);
		m.AddLong("F_OUT_YEAR", outReg->key.year);

		TQuery q(str(m));
		if (q.Read())
		{
			niaexist = true;
			protocol << q;
			m.clear();
			m.printf("Внимание! С посочения изходящ номер е изпратен протокол %ld/%ldг. (",
				protocol.nia.no, protocol.nia.year);

			if (protocol.nia.type == TYPE_COMMON_ADM)
				m.cat("обща номерация");
			else
				m.printf("%s %s", College->Seek(protocol.nia.type), COLLEGE);

			m.cat(") за предаване на ИЛ в НАП. Относно протокола ще бъде изтрита само информацията за изходящ "
				"номер.");
		}
	}

	if (warn("%s %s", subexist || niaexist ? str(m) : "Внимание! След изтриване документа няма да може да се възстанови.",
		Q_CONTINUE))
	{
		if (subexist)
		{
			subpoena.outRegNo = 0;
			subpoena.outRegYear = 0;
			if (!subpoena.Update("F_OUTREG_NO, F_OUTREG_YEAR"))
				error("Внимание! Информацията за изходящия номер не се изтри от придружителното писмо!");
			// 2014:027
			if (subpoena.requestId && strchr(SUBPOENA_KIND_LEGAL_AIDS, subpoena.subpoenaKind))
				legalaid_rename(&subpoena, LEGAL_AID_QUEUED, LEGAL_AID_CREATED);
		}
		else if (niaexist)
		{
			protocol.outNo = 0;
			protocol.outYear = 0;
			if (!protocol.Update("F_OUT_NO, F_OUT_YEAR"))
				error("Внимание! Информацията за изходящия номер не се изтри от протокола за предаване на ИЛ в НАП!");
		}

		bool deleted = delete_outreg(outReg, NULL);

		if (deleted)
		{
			log(LOG_DELETE, CM_ADMIN_DELETE_OUTREG, outReg);
			xfer_bank_event(NULL, NULL, NULL, NULL, NULL, NULL, outReg, XFER_DELETE);
		}

		delete_message("Документът %sе изтрит.", deleted);
	}
}

#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
void delete_prove(TWindow *, TProve *prove)
{
	if (check_subpoenas(prove) && ask("Внимание! След изтриване доказателството %s", Q_RESTORE_CONTINUE))
	{
		bool deleted = delete_prove(prove);

		if (deleted)
		{
			mstr m;
			m.printf("веществено доказателство %ld/%ldг.", prove->key.no, prove->key.year);
			log(LOG_DELETE, CM_ADMIN_DELETE_PROVE, &prove->lawsuit, str(m));
		}

		delete_message("Доказателството %sе изтрито.", deleted);
	}
}

void delete_prove_action(TWindow *, TProveAction *proveAction)
{
	if (ask("Внимание! След изтриване действието няма да може да се възстанови. Желаете ли да продължите?"))
	{
		bool deleted = proveAction->Delete(true);

		if (deleted)
			log(LOG_DELETE, CM_ADMIN_DELETE_PROVE_ACTION, proveAction);

		delete_message("Действието %sе изтрито.", deleted);
	}
}
#endif  // APPEAL || AREAL || MILITARY || REGIONAL

void delete_nia_protocol(TWindow *, TNIAProtocol *protocol)
{
	if (warn("След изтриване протокола %s", Q_RESTORE_CONTINUE))
	{
		TExecList execList;
		msql m(execList.Name, MQ_UPDATE);

		execList.Quote(m, "F_NIA_NO, F_NIA_YEAR, F_TDEP");
		m.Add(protocol, "F_NIA_NO, F_NIA_YEAR, F_NIA_TYPE");
		execList.DataBase->Execute(str(m), ET_ERROR);

		if (protocol->outNo)
		{
			TOutReg outReg;

			outReg.key.no = protocol->outNo;
			outReg.key.year = protocol->outYear;

			if (outReg.Try() && !delete_outreg(&outReg, NULL))
				error("Изходящия документ по протокола не се изтри!");
		}

		bool deleted = protocol->Delete(true);

		if (deleted)
			log(LOG_DELETE, CM_ADMIN_DELETE_NIA_PROTOCOL, &protocol->nia, "на протокол за предаване на ИЛ в НАП");

		delete_message("Протоколът %sизтрит.", deleted);
	}
}
