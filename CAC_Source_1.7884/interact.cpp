#include "cac.h"

#include "JudicialActDialog.h"
#include "JudicialActDialog.cpp"

#include "EditMotivesDialog.h"	// 2006:187; 2009:149 LPR: and opinion
#include "EditMotivesDialog.cpp"

#include "StandardActDialog.h"
#include "StandardActDialog.cpp"

#if COURT_TYPE == COURT_AREAL
#include "TradeSyndicActDialog.h"
#include "TradeSyndicActDialog.cpp"

#include "FirmAcceptActDialog.h"
#include "FirmAcceptActDialog.cpp"
#endif  // AREAL

// 2008:196 Moved to a separate module with all dialogs
bool inter_act(TWindow *parent, TDecision *decision, TSession *session, TLawsuit *lawsuit, const char *kinds)
{
	bool entered = false;

	CDate completed = lawsuit->completed;
	// 2008:025 LRQ: revived ungrantable session results
	bool gr1 = !strchr(RESULT_UNGRANTABLES, session->result);

	switch (decision->JudicialActType(lawsuit, session))
	{
		case JUDICIAL_ACT_STANDARD :
		{
			// 2008:212 LPR: DECIMENTS -> DECORENTS
			// 2010:050 LPR: GRANTABLES
			bool gr2 = strchr(KIND_GRANTABLES, decision->kind) &&
				(!decision->SeekSubject(lawsuit) || !strchr(Subject->Attrib(lawsuit->kind), POST_ALTERNATE));

			entered = TStandardActDialog(parent, decision, lawsuit, kinds, gr1 && gr2,
				strchr(TYPE_FLAGABLE_LAWS, decision->key.type) != NULL && (lawsuit->flags & LAWSUIT_NEWGPK),
				session).Execute() == IDOK;
			break;
		}
	#if COURT_TYPE == COURT_AREAL
		case JUDICIAL_ACT_TRADE_SYNDIC :
		{
			bool gr2 =	// 2013:186 TRQ/LPR: +settlements
				(strchr(KIND_DECISIONS, decision->kind) || strchr(KIND_SETTLEMENTS, decision->kind)) &&
				session->result != RESULT_INCONSISTENCE;

			entered = TTradeSyndicActDialog(parent, decision, lawsuit, kinds, gr1 && gr2, session).Execute() == IDOK;
			break;
		}
		case JUDICIAL_ACT_FIRM_ACCEPT :
		{
			if (kinds)		// 2013:129 FIX: for new decisions only, not edit
			{
				msql m;
				TDecision tDecision;

				lawsuit->LastKey(m, &tDecision, "F_REGISTER, F_VOLUME, F_PAGE, F_PARTITION, F_LEVEL");
				TQuery q(str(m));

				if (q.Read())
				{
					tDecision << q;
					decision->registry = tDecision.registry;
					decision->volume = tDecision.volume;
					decision->page = tDecision.page;
					decision->partition = tDecision.partition;
					decision->level = tDecision.level;
				}
			}

			if (!decision->registry)
			{
				TDamn damn;

				if (find_damn(lawsuit, &damn))
				{
					CompanyType->Seek(damn.companyType);
					decision->registry = CompanyType->registry;
				}
			}

			entered = TFirmAcceptActDialog(parent, decision, lawsuit, kinds, session).Execute() == IDOK;
			break;
		}
	#endif  // AREAL
		default : error("%d: kind not supported by inter_act", decision->kind);
	}

	if (entered)
	{
		if (kinds)
			send_sms(lawsuit, "Вписано/а", SMS_ENDOC, SMS_NEW, decision);

		//eportal_send(parent, decision, lawsuit, LOG_WRITE);  // 2018-10-19: LPR: comout
		xfer_bank_event(lawsuit, session, NULL, decision, NULL, NULL, NULL, kinds ? XFER_ADD : XFER_EDIT);
		eispp_accept(parent, lawsuit, session, decision);	// 2012:332
	}

	if (entered && completed != lawsuit->completed && !lawsuit->Update("F_COMPLETED"))
		error("Данните датата на приключване на делото не се актуализираха!");

	return entered;
}

#include "DecisionSideListDialog.h"
#include "DecisionSideListDialog.cpp"

#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
bool mustbe_personal_data(TDecision *decision, char involvement)
{
	return strchr(INVOLVEMENT_PERSONALS, involvement) || decision->Defendant(involvement);
}

sign maybe_personal_data(TDecision *decision, char involvement)
{
	if (decision->kind == KIND_SETTLEMENT && involvement == INVOLVEMENT_REQUESTER)
		return -1;

	return mustbe_personal_data(decision, involvement);
}

static const char *check_personal_data(TDecision *decision, const TJudgement *judgement)
{
	if (!strchr(UCN_CITIZENS, judgement->ucnType))
		return "Видът на лицето не предполага вписване на индивидуален съдебен акт.";

	if (!maybe_personal_data(decision, judgement->involvement))
		return "Участието на лицето не предполага вписване на индивидуален съдебен акт.";

	return NULL;
}

#include "ArrestDialog.h"
#include "ArrestDialog.cpp"

bool arrest_sidewind(TWindow *parent, TLawsuit *lawsuit, TSideWind *sideWind, bool start)
{
	TAccusation accusation;
	accusation << sideWind;
	return TArrestDialog(parent, lawsuit, &accusation, accusation.Try(), start).Execute() == IDOK;
}

#define judgement OFFICE_GROUP_CAST(TJudgement)

static void judgement_judged(mstr &m, TGroup *group)
{
	m.cat(Judged->Seek(judgement->judged));
	m.cut(10);
}

#undef judgement

#include "ExecJudgementDialog.h"
#include "ExecJudgementDialog.cpp"
#include "LocalJudgementDialog.h"
#include "LocalJudgementDialog.cpp"
#include "JudgementListWindow.h"
#include "JudgementListWindow.cpp"
#include "JudgementListDialog.h"
#include "JudgementListDialog.cpp"
#endif  // APPEAL || AREAL || MILITARY || REGIONAL

#if COURT_TYPE == COURT_AREAL
#include "LocalSurroundmentDialog.h"
#include "LocalSurroundmentDialog.cpp"
#include "SurroundmentListWindow.h"
#include "SurroundmentListWindow.cpp"
#include "SurroundmentListDialog.h"
#include "SurroundmentListDialog.cpp"
#endif  // AREAL

#if COURT_TYPE == COURT_ADMIN
static bool second_dialog(TWindow *, TDecision *, TSession *, TLawsuit *, long)
{
	return false;
}
#endif  // ADMIN

#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
static bool specific_side_dialog(TWindow *parent, TDecision *decision, TSession *session, TLawsuit *lawsuit, long sideMode)
{
#if COURT_TYPE == COURT_REGIONAL
	if (lawsuit->sourceKind == KIND_HERITAGE_REQ_51 && strchr(RESULT_WITH_READY_ANYS, decision->result))
	{
		TDecisionSideListDialog(parent, decision, session, lawsuit, sideMode, IDD_HERITAGE_SIDE_LIST).Execute();
		return true;
	}
#endif  // REGIONAL

	if (decision->PersonalData(session))
	{
		long wasPublic = decision->flags & DECISION_PUBLIC;

		TJudgementListDialog(parent, decision, session, lawsuit, sideMode).Execute();

		if (decision->flags & DECISION_PUBLIC)
		{
			if (decision->P64Direct())
			{
				if (!wasPublic)
				{
					decision->publicDate = Today;
					decision->Update("F_FLAGS, F_PUBLIC_DATE");
					log_event(LOG_TRANSFER, 0, decision, true);
				}

				xfer_web(decision, session, lawsuit);
				eportal_send(NULL, decision, lawsuit, LOG_WRITE);  // 2018-10-19
			}
		}
		else
		{
			if (wasPublic)
			{
				decision->publicDate.Clear();
				decision->Update("F_FLAGS, F_PUBLIC_DATE");
				log_event(LOG_TRANSFER, 0, decision, false);
				xfer_web_delete(decision, lawsuit);
			}

			eportal_send(NULL, decision, lawsuit, LOG_DELETE);  // 2018-10-19
		}

		return true;
	}

	return false;
}

static bool second_dialog(TWindow *parent, TDecision *decision, TSession *session, TLawsuit *lawsuit, long sideMode)
{
	if (specific_side_dialog(parent, decision, session, lawsuit, sideMode))
		return true;

#if COURT_TYPE == COURT_AREAL
	if (strchr(RESULT_SURROUNDMENTABLES, session->result))
	{
		TGArray<TSurroundment> surroundments;

		if (try2surround(session, &surroundments))
		{
			TSurroundmentListDialog(parent, session, decision, &surroundments).Execute();
			return true;
		}
	}
#endif  // AREAL

	return false;
}
#endif  // APPEAL || AREAL || MILITARY || REGIONAL

struct TAfterOffer
{
	char after;
	char offer;
};

static const TAfterOffer AfterOffers[] =
{
#if COURT_TYPE == COURT_REGIONAL
	// 2008:087 LRQ; 2008:316 LRQ: protective after decision, not order
	{ KIND_ORDER,      KIND_EXECUTIVE },
	{ KIND_DECISION,   KIND_PROTECTIVE },
	{ KIND_SETTLEMENT, KIND_IMMED_PROTECT },
	{ KIND_DECISION,   KIND_EUHER_CERT },
	{ KIND_SETTLEMENT, KIND_EUHER_CERT },	// 2017:263
#endif  // REGIONAL
	{ '\0', '\0' },
};

// 2008:087 LRQ: redirect/relaunch EXECUTIVE
static void enter_act(TWindow *parent, TSession *session, TLawsuit *lawsuit, TDecision &decision, const char *kinds)
{
	decision.key = session->key;
	decision.kind = *kinds;
	decision.date = session->date;
	decision.sessionKind = session->kind;
	// 2006:206 LRQ: today for public decision
	if (session->result == RESULT_PUBLIC_DECISION)
		decision.fromDate = Today;
	else if (!session->end.Empty())
		decision.fromDate = session->end;
#if COURT_TYPE == COURT_REGIONAL
	else if (session->result == RESULT_ALLOW_SPLIT)
		decision.fromDate = Today;
#endif  // REGIONAL
	else
		decision.fromDate = session->date;

	if (inter_act(parent, &decision, session, lawsuit, kinds))
	{
		log_event(LOG_ENTER, 0, &decision, decision.flags & DECISION_PUBLIC);
		xfer_email(&decision);
		unschedule(SCHED_DPREPARE, session);
	#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
		if (strchr(KIND_JUDGEMENTS, decision.kind))
		{
			if (decision.returned.Empty())
				schedule(SCHED_JPREPARE, &decision, &decision.fromDate, NULL, session->judge);

			schedule(SCHED_JUDGEMENT, &decision, &decision.fromDate);
		}
	#endif  // APPEAL || AREAL || MILITARY || REGIONAL

		if (session->returned.Empty() && !decision.returned.Empty())
		{
			session->returned = decision.returned;
			session->Update("F_RETURNED");
		}

		if (decision.flags & DECISION_PUBLIC)
		{
			xfer_web(&decision, session, lawsuit);
			eportal_send(NULL, &decision, lawsuit, LOG_WRITE);  // 2018-10-19
		}

		// 2009:225 LPR/LRQ: else, aka judgement xor decision dialog
		if (!second_dialog(parent, &decision, session, lawsuit, SIDE_MODE_ADD) && AUTORUN_MDRS &&
			ask("Желаете ли да изготвите съобщения?"))
		{
			if (decision.kind == KIND_SETTLEMENT &&
				(strchr(RESULT_CANCELAWS, decision.result) || session->result == RESULT_CANCELED))
			{
				edit_sides(parent, session, lawsuit);
			}
			else //if (!decision.PersonalData())
				TDecisionSideListDialog(parent, &decision, session, lawsuit, SIDE_MODE_ADD).Execute();
		}

		for (const TAfterOffer *ao = AfterOffers; ao->after; ao++)
		{
			if (decision.kind == ao->after && strchr(kinds, ao->offer))
			{
				TDecision tDecision;
				enter_act(parent, session, lawsuit, tDecision, C2S[ao->offer]);
			}
		}
	}
}

void enter_act(TWindow *parent, TSession *session, TLawsuit *lawsuit)
{
	if (strchr(RESULT_SURROUENDOCS, session->result))
		edit_surrouendocs(parent, session, lawsuit);
	else
	{
		TDecision decision;
		mstr decisionKinds;
		enter_act(parent, session, lawsuit, decision, session->DecisionKinds(decisionKinds, lawsuit));
	}
}

bool edit_act(TWindow *parent, TDecision *decision, TLawsuit *lawsuit)
{
	TSession session;
	long tFlags = decision->flags;

	session.key = decision->key;
	session.date = decision->date;
	session.kind = decision->sessionKind;

	if (session.Get() && inter_act(parent, decision, &session, lawsuit, NULL))
	{
		// 2014:120 TRQ/FIX: restored
	#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
		if (strchr(KIND_JUDGEMENTS, decision->kind) && !decision->returned.Empty())
			unschedule(SCHED_JPREPARE, decision);
	#endif  // APPEAL || AREAL || MILITARY || REGIONAL

		log_event(LOG_EDIT, 0, decision, decision->flags & DECISION_PUBLIC);

		if (session.returned.Empty() && !decision->returned.Empty())
		{
			session.returned = decision->returned;
			session.Update("F_RETURNED");
		}

		if (decision->flags & DECISION_PUBLIC)
		{
			// 2017:303
			if (!decision->P64Direct())
			{
				xfer_web(decision, &session, lawsuit);
				eportal_send(NULL, decision, lawsuit, LOG_WRITE);  // 2018-10-19
			}
		}
		else
		{
			if (tFlags & DECISION_PUBLIC)
				xfer_web_delete(decision, lawsuit);

			eportal_send(NULL, decision, lawsuit, LOG_DELETE);  // 2018-10-19
		}

		second_dialog(parent, decision, &session, lawsuit, SIDE_MODE_EDIT);
		return true;
	}

	return false;
}

void edit_sides(TWindow *parent, TDecision *decision, TLawsuit *lawsuit, long sideMode)
{
	TSession session;

	session.key = decision->key;
	session.kind = decision->sessionKind;
	session.date = decision->date;

	if (session.Get())
	{
		log(LOG_PRINT, 0, decision, "обявления (СРГ)");

	#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
		if (specific_side_dialog(parent, decision, &session, lawsuit, sideMode))
			return;
	#endif // APPEAL || AREAL || MILITARY || REGIONAL

		TDecisionSideListDialog(parent, decision, &session, lawsuit, sideMode).Execute();
	}
}

#include "SessionSideListDialog.h"
#include "SessionSideListDialog.cpp"

void edit_sides(TWindow *parent, TSession *session, TLawsuit *lawsuit)
{
	long sideMode = session->result == RESULT_FIXED ? SIDE_MODE_ADD : SIDE_MODE_EDIT;

	log(LOG_PRINT, 0, session, "призовки/съобщения/писма");
#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_ADMIN || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY
	TSessionSideListDialog(parent, session, lawsuit, sideMode).Execute();
#endif  // APPEAL || ADMIN || AREAL || MILITARY
#if COURT_TYPE == COURT_REGIONAL
	if (lawsuit->sourceKind == KIND_HERITAGE_REQ_51 && session->result != RESULT_FIXED)
		TSessionSideListDialog(parent, session, lawsuit, sideMode, IDD_HERITAGE_SIDE_LIST).Execute();
	else
		TSessionSideListDialog(parent, session, lawsuit, sideMode).Execute();
#endif  // REGIONAL
}
