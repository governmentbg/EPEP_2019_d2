#include "cac.h"

#include "ForceDateDialog.h"
#include "ForceDateDialog.cpp"

bool force_session(TWindow *parent, TSession *session, TLawsuit *lawsuit)
{
	if (TForceDateDialog(parent, session, lawsuit, "връщане на заседанието").Execute() == IDOK)
	{
		xfer_bank_event(lawsuit, session, NULL, NULL, NULL, NULL, NULL, XFER_EDIT);
		return true;
	}

	return false;
}

void force_decision_xfer(TWindow *parent, TDecision *decision, TLawsuit *lawsuit)
{
	// 2010:029 TRQ: BUGIFX: PUBLIC only!
	if (decision->flags & DECISION_PUBLIC)
	{
		TSession session;
		// 2009:238 LRQ: web
		session << decision;
		session.kind = decision->sessionKind;
		if (session.Get())
			xfer_web(decision, &session, lawsuit);

		eportal_send(parent, decision, lawsuit, LOG_WRITE);  // 2018-10-19
	}

	//eportal_send(parent, decision, lawsuit, LOG_WRITE);  // 2018-10-19: LPR: comout
}

#include "ForceDecisionDialog.h"
#include "ForceDecisionDialog.cpp"

bool force_decision(TWindow *parent, TDecision *decision, TLawsuit *lawsuit)
{
	CDate lawsuitForceDate = lawsuit->forceDate;

	if (TForceDecisionDialog(parent, decision, lawsuit).Execute() == IDOK)
	{
		if (lawsuit->forceDate != lawsuitForceDate && lawsuit->Update("F_FORCE_DATE"))
		{
			// copied from judicial act dialog
			mstr m("Дата на влизане в ЗС: ");

			if (lawsuit->forceDate.Empty())
				m.cat("изтрита");
			else
				lawsuit->forceDate.Print(m);

			log(LOG_ENTER, CM_CLERK_FORCE_DATE, decision, str(m));
		}

		if (!lawsuit->forceDate.Empty())
		{
			unschedule(SCHED_DECIDE_14, decision);
			unschedule(SCHED_DECIDE_30, decision);
			// 2010:350 FIX: was missing
			unschedule(SCHED_JUDGEMENT, decision);
		}

		force_decision_xfer(parent, decision, lawsuit);
		return true;
	}

	return false;
}

#include "AccusationDialog.h"
#include "AccusationDialog.cpp"

// 2003:295 BRW: based on accuse_side, but with no dynamic (or any other) cast
bool accuse_sidewind(TWindow *parent, TRCDKeyContainer *container, TSideWind *sideWind)
{
	TAccusation accusation;
	bool edit = false;	// Generally

	accusation << sideWind;

	if (accusation.Try())
		edit = true;
	else
		accusation << sideWind;

	return TAccusationDialog(parent, container, &accusation, edit).Execute() == IDOK;
}

#include "PunishmentDialog.h"
#include "PunishmentDialog.cpp"

bool punish_side(TWindow *parent, TDecision *decision, const TJudgement *judgement, TLawsuit *lawsuit)
{
	TAccusation accusation;
	accusation << judgement;

	if (accusation.Get())
		return TPunishmentDialog(parent, decision, &accusation, lawsuit).Execute() == IDOK;

	return false;
}
