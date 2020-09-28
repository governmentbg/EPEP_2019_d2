#include "cac.h"

#include "SurroundSideListDialog.h"
#include "SurroundSideListDialog.cpp"

#include "EditSurroundDialog.h"
#include "EditSurroundDialog.cpp"

bool edit_surround(TWindow *parent, TSurround *surround, TRCDKeyContainer *container)
{
	if (TEditSurroundDialog(parent, surround, NULL).Execute() == IDOK)
	{
		log(LOG_EDIT, CM_REG_EDIT_SURROUND, surround);
		TSurroundSideListDialog(parent, surround, container, SIDE_MODE_EDIT).Execute();
		return true;
	}

	return false;
}

void new_surround(TWindow *parent, TRCDKeyContainer *container)
{
	TSurround surround;

	surround.lawsuit = container->key;
	surround.college = find_college(container);

	if (strchr(KIND_SESSIONS_ENDOCS, container->kind))
	{
		surround.decisionKind = container->kind;
		surround.decisionDate = container->date;
	}

	if (TEditSurroundDialog(parent, &surround, adjust_surround_kinds(container)).Execute() == IDOK)
	{
		log(LOG_NEW_DOC, CM_REG_NEW_SURROUND, &surround);
		xfer_email(&surround);
#if COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_REGIONAL
		if (surround.kind == KIND_REPLY_INDOC_SURROUND && strchr(KIND_LAWSUITS, container->kind))
			unschedule(SCHED_REP_INDOC, (TLawsuit *) container);
#endif  // AREAL || REGIONAL
#if COURT_TYPE == COURT_AREAL
		if (surround.kind == KIND_CHANGE_SURROUND)
			schedule(SCHED_FCHANGE, &surround, &surround.date, NULL, surround.judge);
#endif  // AREAL
#if COURT_TYPE == COURT_REGIONAL
		if (surround.kind == KIND_OBJECTION_SURROUND)
			unschedule(SCHED_APP_EXECOR, (TLawsuit *) container);
		else if (surround.kind == KIND_CLAIM_SURROUND)
			unschedule(SCHED_FILE_CLAIM, (TLawsuit *) container);
#endif  // REGIONAL
		TSurroundSideListDialog(parent, &surround, container, SIDE_MODE_ADD).Execute();
	}
}
