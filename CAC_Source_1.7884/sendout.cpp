#include "cac.h"

TReceiveInstance::TReceiveInstance(char connectType, TOutReg *outReg)
{
	connect.connectType = connectType;
	connect.key = outReg->key;
	lawsuitType = outReg->lawsuit.type;
}

void TReceiveInstance::LawsuitKindChange()
{
	bool enabled = xferKind ? xferKind->L() : (long) lawsuitKind->C();

	lawsuitNo->EnableWindow(enabled);
	lawsuitYear->EnableWindow(enabled);
	decisionKind->EnableWindow(enabled);
	decisionDate->EnableWindow(enabled);
	remark->EnableWindow(enabled);

	if (!enabled)
	{
		lawsuitNo->SetL(0);
		lawsuitYear->SetL(0);
		decisionKind->SetC('\0');
		decisionDate->SetS("");
	}
}

void TReceiveInstance::CourtChange(bool setDefaultKind)
{
	bool enabled = atob(court->S());

	lawsuitKind->EnableWindow(enabled);

	if (xferKind)
		xferKind->EnableWindow(enabled);

	if (enabled)
	{
		if (setDefaultKind && strchr(TYPE_LAWSUITS, lawsuitType))
		{
			lawsuitKind->SetDefaultKind(lawsuitType);
			if (xferKind)
				xferKind->SetDefaultKind(lawsuitType);
		}
	}
	else
	{
		lawsuitKind->SetC('\0');

		if (xferKind)
			xferKind->SetL(0);
	}

	LawsuitKindChange();
}

bool TReceiveInstance::Valid()
{
	int count = (xferKind ? xferKind->L() != 0 : lawsuitKind->C() != '\0') + (lawsuitNo->L() != 0) +
		(lawsuitYear->L() != 0);

	return !(count % 3);
}

void TReceiveInstance::Try()
{
	hasConnect = connect.Try();

	if (hasConnect)
	{
		lawsuitKind->SetC(connect.connectKind);

		if (xferKind)
			xferKind->SetL(connect.xferKind);

		lawsuitNo->SetL(atol(connect.connectNo));
		lawsuitYear->SetL(connect.connectYear);
		decisionKind->SetC(connect.decisionKind);
		decisionDate->SetDate(connect.decisionDate);
		court->SetS(connect.ucn);
	}
	else
		decisionDate->SetS("");
}

void TReceiveInstance::Save()
{
	if (lawsuitNo->L())
	{
		connect.ucnType = UCN_INSTANCE_HIGHER;
		strcpy(connect.ucn, court->S());
		sprintf(connect.connectNo, "%ld", lawsuitNo->L());
		connect.connectYear = lawsuitYear->L();

		if (xferKind && xferKind->L())
		{
			XferKind->Seek(xferKind->L());
			connect.connectKind = XferKind->connectKind;
			connect.xferKind = xferKind->L();
		}
		else
		{
			connect.connectKind = lawsuitKind->C();
			connect.xferKind = 0;
		}

		connect.decisionKind = decisionKind->C();
		connect.decisionDate = decisionDate->Date();

		if (hasConnect)
			connect.Update();
		else
			connect.Insert();
	}
	else if (hasConnect)
		connect.Delete(true);
}

#include "OutRegReceiveBaseDialog.h"
#include "OutRegReceiveBaseDialog.cpp"

#include "OutRegReceiveDialog.h"
#include "OutRegReceiveDialog.cpp"

#define inRegEvent OFFICE_GROUP_CAST(TInRegEvent)

static void inreg_event_kind(mstr &m, TGroup *group)
{
	TInReg inReg;
	inReg.key = inRegEvent->key;
	m.cpy(Kind->Seek(inReg.Try("F_KIND") ? inReg.kind : inRegEvent->kind));
}

static void inreg_event_no_year(mstr &m, TGroup *group)
{
	m.printf("%ld/%ld", inRegEvent->key.no, inRegEvent->key.year);
}

static void inreg_event_status(mstr &m, TGroup *group)
{
	msql w(inRegEvent->Name, "F_RESULT");

	w.Add(inRegEvent, "F_NO, F_YEAR");
	w.Add("F_DATE");
	w.printf("< %ld", inRegEvent->date.Value());
	w.AddOrder("F_DATE DESC");

	TQuery q(str(w));
	if (q.Read())
	{
		TInRegEvent tInRegEvent;
		tInRegEvent << q;
		m.cpy(Result->Seek(tInRegEvent.result));
	}
}

#undef inRegEvent

#include "InRegEventListWindow.h"
#include "InRegEventListWindow.cpp"
#include "InRegEventListDialog.h"
#include "InRegEventListDialog.cpp"

bool receive_out(TWindow *parent, TOutReg *outReg)
{
	const char *results, *complects, *services, *procedures;
	// 2007:305 +related: single full backup object
	TOutReg tOutReg;
	tOutReg << outReg;

	switch (outReg->kind)
	{
		case KIND_APPEAL_LETTER : results = complects = services = procedures = RESULT_APPEAL_LETTERS; break;
		case KIND_PROPOSITION_LETTER : results = complects = services = procedures = RESULT_PROPOSITION_LETTERS; break;
		default :
		{
			TLawsuit lawsuit;
			lawsuit.key = outReg->lawsuit;

			if (lawsuit.key.type && lawsuit.Try("F_FINISHED") && !lawsuit.finished.Empty())
			{
				results = RESULT_IOREG_FINISHEDS;
				complects = RESULT_COMPLECT_FINISHEDS;
				services = RESULT_SERVICE_FINISHEDS;
				procedures = RESULT_PROCEDURE_FINISHEDS;
			}
			else
			{
				results = RESULT_IOREG_CONTINUEDS;
				complects = RESULT_COMPLECT_CONTINUEDS;
				services = RESULT_SERVICE_CONTINUEDS;
				procedures = RESULT_PROCEDURE_CONTINUEDS;
			}
		}
	}

	TInReg inReg;
	inReg.key.no = outReg->inNo;
	inReg.key.year = outReg->inYear;
	bool tInReg = outReg->inNo && inReg.Get();
	int result;		// 2010:069 LPR/URQ: FIX: bool -> int +related

	// 2009:286 LPR: HIGHER
	if (tInReg && outReg->receiverType == UCN_INSTANCE_HIGHER)
	{
		const TAppealInstance *hinst = obtain_appeal_instance(&inReg);

		if (!hinst)
			fatal("%ld/%ld: unable to obtain appeal instance for inreg", inReg.key.no, inReg.key.year);

		const char *resultsVZ = C2S[RESOLUTION_FAKE];
		const char *resultsKS = hinst->ks_res;
		bool ksRecv = hinst->KSRecv(outReg->receiver);
		TFixeiver receiverVZ(UCN_INSTANCE_HIGHER);
		TFixeiver receiverKS(UCN_INSTANCE_HIGHER);

		if (ksRecv)
			receiverKS.ucns[0] = outReg->receiver;
		else
		{
			resultsVZ = hinst->vz_res;
			receiverVZ.ucns[0] = outReg->receiver;
			// 2012:003 LPR: including jumpy
			receiverKS.ucns[0] = hinst->ks0;
			receiverKS.ucns[1] = hinst->ks1;
		}

		result = TOutRegReceiveDialog(parent, outReg, results, complects, services, procedures, resultsVZ, resultsKS,
			ksRecv, &receiverVZ, &receiverKS).Execute();
	}
	else
	{	// OTHERS
		result = TOutRegReceiveBaseDialog(parent, outReg, results).Execute();
	}

	if (result == IDCANCEL)
	{
		xfer_reject(outReg);
		return false;
	}

	if (tInReg)
	{
		if (tOutReg.result == RESULT_IOREG_SEND)
			receive_list(outReg);
		// 2004:343 LPR: handle date changes
		// 2007:290 LRQ: also status changes
		else if (outReg->returned != tOutReg.returned || outReg->result != tOutReg.result)
		{
			revert_receive_list(&tOutReg);
			receive_list(outReg);
		}

		// 2012:018 LPR: moved after _list operations, requires event list
		xfer_bank_event(NULL, NULL, NULL, NULL, &inReg, NULL, outReg, XFER_EDIT);

		// 2011:318 LRQ/LPR: comout, lists send instead of received
		//TInRegEventListDialog(parent, outReg, &inReg).Execute();
		// 2009:286 LPR: HIGHER
		if (outReg->receiverType == UCN_INSTANCE_HIGHER)
			xfer_web(outReg);
	}

	log(LOG_RETURN, CM_REG_OUT_RECEIVE, outReg);

	if (result == IDREGISTRY)
	{
		TInReg tInReg;
		TLawsuit lawsuit;

		tInReg.college = outReg->college;
		lawsuit.key = outReg->lawsuit;

		if (!lawsuit.key.type || lawsuit.Get())
			new_inreg(parent, &tInReg, KIND_IN_LETTERS, &lawsuit);
	}

	handle_restart(outReg->lawsuit, tOutReg.flagsX & OUTREG_X_RESTART, outReg->flagsX & OUTREG_X_RESTART,
		outReg->returned, false);

	xfer_accept(outReg);
	outReg->aret = true;
	eispp_receive(parent, outReg);
	return true;
}

static bool check_send(const TRCDKeyGroup *group, char receiverType, const char *what)
{
	TOutReg outReg;
	msql m(outReg.Name, "F_NO, F_YEAR");

	m.AddLong("F_LAWSUIT_NO", group->key.no);
	m.AddLong("F_LAWSUIT_YEAR", group->key.year);
	m.AddChar("F_LAWSUIT_TYPE", group->key.type);
	m.AddChar("F_RECEIVER_TYPE", receiverType);

	TQuery q(str(m));
	if (q.Read())
	{
		outReg << q;
		return ask("%s с изходящ номер %ld/%ldг. Желаете ли да продължите?", what, outReg.key.no, outReg.key.year);
	}

	return true;
}

static bool ask_print_outreg(TWindow *parent, const TOutReg *outReg, TRCDKeyContainer *event, const TConnect *lower)
{
	if (!outreg_subpoena_kind(outReg))
		info("%s Изх. № %ld от %Dг.", Kind->Seek(outReg->kind), outReg->key.no, &outReg->date);
	else if (ask("%s Изх. № %ld от %Dг.\n\nЖелаете ли да отпечатате документа?", Kind->Seek(outReg->kind), outReg->key.no,
		&outReg->date))
	{
		return print_outreg(parent, outReg, NULL, event, true, NULL, lower);
	}

	return false;
}

static void write_mail(const TOutReg *outReg, char ucnType, char *ucn)
{
	if (outReg->flags & OUTREG_WRITE_MAIL)
	{
		TRegisteredMail mail;
		TAddress address;

		mail.ucnType = ucnType;
		address.ucnType = receiver_ucn_type(ucnType);
		strcpy(address.ucn, strcpy(mail.ucn, ucn));

		if (address.Get())
		{
			mail.doc = outReg->key;
			mail.date = Today;
			mail.flags = outReg->flags;
			mail << address;
			// 2008:030 LPR: allow common colleges
			//mail.lawsuitType = outreg_to_law_type(*outReg);
			mail.lawsuitType = find_college(outReg);
			mail.Insert();
		}
	}
}

static void create_mails(TOutReg *outReg)
{
	msql m;
	TSideWind sideWind;

	sideWind << outReg;
	sideWind.FloatCriteria(m);

	TQuery q(str(m));
	while (q.Read())
	{
		sideWind << q;
		write_mail(outReg, sideWind.ucnType, sideWind.ucn);
	}
}

static TConnect *get_connect(TConnect **connects, const char *ucn)
{
	return connects ? connects[connects[1] && !strcmp(ucn, connects[1]->ucn)] : NULL;
}

#include "OutRegSendBaseDialog.h"
#include "OutRegSendBaseDialog.cpp"

#include "OutRegEditDialog.h"
#include "OutRegEditDialog.cpp"

#include "OutRegSideListDialog.h"
#include "OutRegSideListDialog.cpp"

bool edit_outreg(TWindow *parent, TOutReg *outReg)
{
	TInReg *inReg = NULL;
	TInReg tInReg;

	if (outReg->inNo)
	{
		inReg = &tInReg;
		inReg->key.no = outReg->inNo;
		inReg->key.year = outReg->inYear;

		if (!inReg->Get())
			return false;
	}

	TConnect connect;
	TConnect *connects[2] = { &connect, NULL };

	// 2017:283 LPR: temporary fix; 2018-01-19: LPR: obsolete, disabled
	//outReg->text.trim_trail();

	if (outReg->receiverType == UCN_INSTANCE_LOWER && !outReg->TryLower(connect))
	{
		// 2015:253 FIX: the error message was missing
		say_no_data(connect.Name);
	}
	else if (TOutRegEditDialog(parent, outReg, inReg, connects).Execute() == IDOK)
	{
		log(LOG_EDIT, CM_REG_OUT_EDIT, outReg);	// 2011:290 moved at top
		eispp_send(parent, outReg, inReg);
		xfer_resend(outReg, inReg);

		if (outReg->receiverType != UCN_INSTANCE_RESERVED)
			eportal_send(parent, outReg, NULL, LOG_WRITE);

		if (outReg->receiverType == UCN_INSTANCE_HIGHER)
		{
			xfer_bank_event(NULL, NULL, NULL, NULL, inReg, NULL, outReg, XFER_EDIT);
			TInRegEventListDialog(parent, outReg, inReg, NULL, -1).Execute();
			delete_mails(outReg);
			// 2007:148 LPR: UCN_INSTANCE_THIS -> HIGHER
			write_mail(outReg, UCN_INSTANCE_HIGHER, outReg->receiver);
			// 2009:211 LPR: justin case public was set not long ago...
			if (outReg->Status() == RESULT_IOREG_SEND)
				xfer_web(outReg);
		}
		else
		{
			if (outReg->receiverType == UCN_INSTANCE_RESERVED)
				TOutRegSideListDialog(parent, outReg, NULL, SIDE_MODE_EDIT).Execute();

			delete_mails(outReg);
			create_mails(outReg);
		}

		return true;
	}

	return false;
}

#include "OutRegSendDialog.h"
#include "OutRegSendDialog.cpp"

// 2009:167 LPR: init receiver/fixeiver here +other send/returns
void return_out(TWindow *parent, TRCDKeyContainer *container, TOriginContainer *origin, bool lower)
{
	TConnect connect, tConnect;
	TConnect *connects[2] = { &connect, NULL };
	const char *failure = NULL;

	// part 1: obtain fixeiver(s)
	if (lower)
	{
	#if COURT_TYPE == COURT_APPEAL
		if (!connect.TryBy(container, CONNECT_TYPE_LOWER_INSTANCE))
			failure = "Няма вписани данни за първоинстанционен/въззивен съд";
		else
		{
			bool renew = strchr(KIND_RENEW_LAWS, origin->kind) || strchr(KIND_RENEW_INDOCS, origin->kind);

			if (renew && container->Status() == RESULT_NOT_GRANTED)
				failure = connect.Returnable(RETURNABLE_FULL);
			else if (tConnect.TryBy(container, CONNECT_TYPE_RECALL_LAWSUIT))
			{
				if (renew)
				{
					// 2016:111 JRQ/FIX: Returnable() is string status
					if (tConnect.Returnable(RETURNABLE_LOWER) == NULL ||
						connect.Returnable(RETURNABLE_LOWER) != NULL)
					{
						connects[0] = &tConnect;
						connects[1] = &connect;
					}
					else
						connects[1] = &tConnect;
				}
				else
				{
					failure = tConnect.Returnable(RETURNABLE_FULL);
					connect << tConnect;
				}
			}
		}
	#endif  // APPEAL
	#if COURT_TYPE == COURT_ADMIN || COURT_TYPE == COURT_MILITARY
		if (!connect.TryBy(container, CONNECT_TYPE_LOWER_INSTANCE))
			failure = "Няма вписани данни за първоинстанционен съд";
	#endif  // ADMIN || MILITRY
	#if COURT_TYPE == COURT_AREAL
		if (!connect.TryBy(container, CONNECT_TYPE_LOWER_INSTANCE))
			failure = "Няма вписани данни за първоинстанционен съд / ЧСИ";
	#endif  // AREAL
	}
	else if (!connect.TryBy(container, CONNECT_TYPE_PROSECS))
		failure = "Няма вписани данни за изпращаща прокуратура";

	// part 2: send
	if (failure)
		error("%s.", failure);
	else if (check_send(container, connect.ucnType, "Делото вече е върнато"))
	{
		TFixeiver fixeiver(connect.ucnType);
		TConnect *rConnect = NULL;
		TOutReg outReg;

		fixeiver.ucns[0] = connects[0]->ucn;
		fixeiver.ucns[1] = connects[1] ? connects[1]->ucn : NULL;

		outReg.lawsuit = container->key;
		outReg.college = origin->college;
		outReg.decisionKind = container->kind;
		outReg.decisionDate = container->date;
		strcpy(outReg.receiver, connects[0]->ucn);

		if (TOutRegSendDialog(parent, &outReg, &fixeiver, KIND_OUT_REG_LETTERS, NULL, lower ? NULL : container,
			connects).Execute() == IDOK)
		{
			if (lower)
			{
				// 2009:047 LPR/LRQ: _SEND -> _RETURN to match interface/opc
				log(LOG_RETURN, CM_REG_OUT_RETURN_LOWER, &outReg, "в I-инст съд");
				xfer_email(&outReg);

				rConnect = get_connect(connects, outReg.receiver);
				// 2009:351 LRQ: don't xfer/return any EXECUTIVES to sender
				// 2015:253 LPR/XRQ: re-verify returnable, may be AppC RECALL
				if (!strchr(CONNECT_KIND_EXECUTIVES, connect.connectKind) &&
					rConnect->Returnable(RETURNABLE_LOWER) == NULL)
				{
					xfer_send(&outReg, NULL);
				}
			}
			else
				log(LOG_RETURN, CM_REG_OUT_RETURN_PROSEC, &outReg, "за доразследване");

			eportal_send(parent, &outReg, origin, LOG_WRITE);
			eispp_send(parent, &outReg);
			ask_print_outreg(parent, &outReg, lower ? NULL : container, rConnect);
			// 2017:244
			if (outReg.receiverType == UCN_INSTANCE_LOWER)
				write_mail(&outReg, outReg.receiverType, outReg.receiver);
		}
	}
}

void send_nosave(TWindow *parent, TOutReg *outReg, TInReg *inReg, TRCDKeyContainer *event)
{
	TInRegEventListDialog(parent, outReg, inReg, event, false).Execute();
}

void send_appeal(TWindow *parent, TOutReg *outReg, TInReg *inReg)
{
	TFixeiver fixeiver(UCN_INSTANCE_HIGHER);

	if (!fill_appeal_fixeivers(inReg, fixeiver, outReg->receiver))
		fatal("%ld/%ld: unable to obtain appeal receivers for inreg", inReg->key.no, inReg->key.year);

	outReg->inNo = inReg->key.no;
	outReg->inYear = inReg->key.year;
	outReg->lawsuit = inReg->lawsuit;
	outReg->college = inReg->college;

	TLawsuit lawsuit;
	bool tLawsuit;

	if (strchr(TYPE_LAWSUITS, outReg->lawsuit.type))
	{
		lawsuit.key = outReg->lawsuit;
		tLawsuit = lawsuit.Get();

		// 2014:004; 2016:083 moved here from OutRegSendDialog
	#if COURT_TYPE == COURT_REGIONAL
		if (lawsuit.key.type == TYPE_PUNISHMENT_LAW)
		{
			Subject->Seek(TYPE_PUNISHMENT_LAW, lawsuit.subject);

			if (strchr(Subject->sd[0].attrib, POST_SEND2ADMIN))
				strcpy(outReg->receiver, INSTANCE_ADMIN);
		}
	#endif  // REGIONAL
	}

	// 2008:086 LRQ; 2016:083 moved here from OutRegSendDialog
#if COURT_TYPE == COURT_ADMIN
	if (inReg->kind == KIND_INREG_SLOWDOWN)
		strcpy(outReg->receiver, INSTANCE_SPEAL);
#endif  // ADMIN

	if (TOutRegSendDialog(parent, outReg, &fixeiver, KIND_OUT_REG_APPS, inReg, NULL).Execute() == IDOK)
	{
		if (tLawsuit)
			for (const char *sched = SCHED_SENDENDS; *sched; sched++)
				unschedule(*sched, &lawsuit);

		// outReg is already written, so don't check send_list()
		send_list(outReg, inReg);
		log(LOG_SEND, CM_REG_OUT_APPEAL, outReg, "за обжалване");
		if (tLawsuit)
			eportal_send(parent, outReg, &lawsuit, LOG_WRITE);
		xfer_bank_event(NULL, NULL, NULL, NULL, inReg, NULL, outReg, XFER_ADD);
		TInRegEventListDialog(parent, outReg, inReg, NULL, true).Execute();
		xfer_send(outReg, inReg);
		xfer_email(outReg);
		eispp_send(parent, outReg, inReg);
		// 2009:223 FIX: send lawsuits only
		if (strchr(TYPE_LAWSUITS, outReg->lawsuit.type))
			xfer_web(outReg);
		// 2007:148 LPR: UCN_INSTANCE_THIS -> HIGHER
		write_mail(outReg, UCN_INSTANCE_HIGHER, outReg->receiver);
	}
}

static void send_lowlevel(TWindow *parent, TRCDKeyGroup *group, TReceiver *receiver, const char *text)
{
	TOutReg outReg;

	if (check_send(group, receiver->ucnType, group->key.type == TYPE_REQUEST ? "Документът вече е изпратен" :
		"Делото вече е изпратено"))
	{
		outReg.lawsuit = group->key;
		outReg.college = ((TCharAlias *) group->Find("F_COLLEGE"))->C();

		if (TOutRegSendDialog(parent, &outReg, receiver, KIND_OUT_REG_LETTERS, NULL, NULL).Execute() == IDOK)
		{
			log(LOG_SEND, 0, &outReg, text);
			eportal_send(parent, &outReg, group, LOG_WRITE);
			xfer_send(&outReg, NULL);
			xfer_email(&outReg);
			eispp_send(parent, &outReg);
			ask_print_outreg(parent, &outReg, NULL, NULL);
			// 2017:244 FIX: EQUAL only for now
			if (outReg.receiverType == UCN_INSTANCE_EQUAL)
				write_mail(&outReg, outReg.receiverType, outReg.receiver);
		}
	}
}

static bool find_container(TRCDKeyContainer *container, TRCDKeyGroup *group, const char *results)
{
	msql m(container->Name, "F_DATE");

	m.AddLong("F_NO", group->key.no);
	m.AddLong("F_YEAR", group->key.year);
	m.AddChar("F_TYPE", group->key.type);
	m.AddChars(container->Find("F_RESULT") ? "F_RESULT" : "F_RESOLUTION", results);

	TQuery q(str(m));
	return q.Read();
}

static const char *send_canceled(TRCDKeyGroup *group, bool jurisd)
{
	TDatedContainer *container;
	char results[5];
	const char *result;

	*results = '\0';
	strcach(results, RESOLUTION_SEND);	// == RESULT_SEND

	if (group->key.type == TYPE_REQUEST)
	{
		container = new TRequestResolution;
		result = "Липсва разпореждане за изпращане на документа";
	}
	else
	{
		TDecision decision;

		if (find_container(&decision, group, RESULT_CANCELAWS))
			return NULL;

		container = new TSession;

		if (jurisd)
		{
#if COURT_TYPE == COURT_AREAL
			if (find_container(&decision, group, RESULT_FIRM_SENDECS))
				return NULL;
#endif  // AREAL
			result = "Липсва заседание с което делото е прекратено или изпратено по подсъдност";
			strcach(results, RESULT_CANCELED);
#if COURT_TYPE == COURT_AREAL
			strcach(results, RESULT_FIRM_SENDSES);
#endif  // AREAL
		}
		else
		{
			result = "Липсва заседание с което делото е прекратено";
			*results = RESULT_CANCELED;	// override RESULT_SEND
		}

		strcach(results, RESULT_WITH_SETTLEMENT);
	}

	if (find_container(container, group, results))
		result = NULL;

	delete container;
	return result;
}

#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_ADMIN || COURT_TYPE == COURT_AREAL
static bool competent(TRCDKeyGroup *group)
{
	char inDocKind = group->key.type == TYPE_REQUEST ? ((TRequest *) group)->kind : ((TLawsuit *) group)->sourceKind;
	return strchr(KIND_COMPETENT_INDOCS, inDocKind);
}
#endif  // APPEAL || AREAL

static const char *jurisdable(TRCDKeyGroup *group, char)
{
#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_ADMIN || COURT_TYPE == COURT_AREAL
	// should be find_finalizer() after intervals are fully implemented
	if (group->key.type != TYPE_REQUEST && competent(group))
	{
		TSession session;
		msql m;

		session.key = group->key;
		session.FloatCriteria(m, "F_KIND, F_DATE, F_RESULT");
		m.AddIsNull("F_FINAL", false);

		TQuery q(str(m));
		while (q.Read())
		{
			session << q;
			adjust_cancelaw(&session);
			if (!strchr(RESULT_ALL_CANCELEDS, session.result))
				return NULL;
		}

		return "Делото не е решено по същество";
	}
#endif  // APPEAL || AREAL

	return send_canceled(group, true);
}

void send_juriconf(TWindow *parent)
{
	TRCDKeyGroup *const group = get_group(parent, NULL, TYPE_SEND_JURICONFS, NULL, jurisdable);
	TVirtPtr VP(group);

	if (group)
	{
		const char *text;

		// 2012:131 TRQ/LRQ: +ADMIN
#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_ADMIN || COURT_TYPE == COURT_AREAL
		if (competent(group))
		{
			Receiver->ucnType = UCN_INSTANCE_COMPETENT;
			text = "по компетентност";
		}
		else
#endif  // APPEAL || AREAL
		{
			Receiver->ucnType = UCN_INSTANCE_EQUAL;
			text = "по подсъдност";
		}

		send_lowlevel(parent, group, Receiver, text);
	}
}

static const char *conflictable(TRCDKeyGroup *group, char)
{
	return send_canceled(group, false);
}

void send_confhigh(TWindow *parent)
{
	TRCDKeyGroup *const group = get_group(parent, NULL, TYPE_SEND_JURICONFS, NULL, conflictable);
	TVirtPtr VP(group);

	if (group)
	{
		Receiver->ucnType = UCN_INSTANCE_CONFLICT;
		send_lowlevel(parent, group, Receiver, "за определяне на компетентен съд");
	}
}

void send_other(TWindow *parent, TOutReg *outReg, TRCDKeyGroup *group)
{
	TFixeiver fixeiver(UCN_INSTANCE_RESERVED);
	TInReg *inReg;

	if (group->key.type == TYPE_INREG)
	{
		inReg = (TInReg *) group;
		outReg->lawsuit = inReg->lawsuit;
		outReg->inNo = inReg->key.no;
		outReg->inYear = inReg->key.year;
	}
	else
	{
		inReg = NULL;
		if (group->key.type != TYPE_EMPTY)
			outReg->lawsuit = group->key;
	}

	TCharAlias *college = (TCharAlias *) group->Find("F_COLLEGE");
	if (college)
		outReg->college = college->C();

	if (TOutRegSendDialog(parent, outReg, &fixeiver, outReg->lawsuit.type ? KIND_OUT_REG_OUTS : KIND_OUT_REG_CUTS,
		inReg, NULL).Execute() == IDOK)
	{
		log(LOG_SEND, CM_REG_OUT_SEND_OTHER, outReg, "на писма и др.");

		if (inReg)
			send_list(outReg, inReg);
		// 2015:175 no side list if print lReceiver
		if (ask_print_outreg(parent, outReg, NULL, NULL))
			eportal_send(parent, outReg, group, LOG_WRITE);
		else
			TOutRegSideListDialog(parent, outReg, dynamic_cast<TLawsuit *> (group), SIDE_MODE_ADD).Execute();

		create_mails(outReg);
	}
}
