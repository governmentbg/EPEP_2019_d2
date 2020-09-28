#include "cac.h"

#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL

const char *origin_provable(TRCDKeyGroup *group, char status)
{
	TProve prove;

	if (prove.TryBy(group))
		return group->key.type == TYPE_REQUEST ? "По документа вече има вписано веществено доказателство" : "По делото вече има вписано веществено доказателство";

	if (group->key.type == TYPE_REQUEST)
	{
		if (status == RESOLUTION_LAWED)
			return "Документът е образуван в дело. Вещественото доказателство следва да бъде вписано към делото";
		if (!strchr(KIND_PUNISHMENT_INDOCS, ((TRequest *) group)->kind))
			return "Веществени доказателсттва могат да се вписват само към наказателни входящи документи";
	}

	return NULL;
}

#include "EditProveDialog.h"
#include "EditProveDialog.cpp"

#include "ProveSideListDialog.h"
#include "ProveSideListDialog.cpp"

void new_prove(TWindow *parent, TRCDKeyContainer *container)
{
	TProve prove;	// 2006:044 LRQ/BUGFIX: moved here for reinit

	prove.lawsuit = container->key;
	if (TEditProveDialog(parent, &prove, container, false).Execute() == IDOK)
	{
		log(LOG_ENTER, CM_ARCH_NEW_PROVE, &prove);
		TProveSideListDialog(parent, &prove, SIDE_MODE_ADD).Execute();
	}
}

bool edit_prove(TWindow *parent, TProve *prove)
{
	if (TEditProveDialog(parent, prove, NULL, true, IDD_EDIT_PROVE).Execute() == IDOK)
	{
		log(LOG_EDIT, CM_ARCH_EDIT_PROVE, prove);
		TProveSideListDialog(parent, prove, SIDE_MODE_EDIT).Execute();
		return true;
	}

	return false;
}

#include "ProveActionDialog.h"
#include "ProveActionDialog.cpp"

void new_prove_action(TWindow *parent, TProve *prove)
{
	TProveAction proveAction;
	proveAction.key = prove->key;

	if (TProveActionDialog(parent, prove, &proveAction, false, IDD_NEW_PROVE_ACTION).Execute() == IDOK)
		log(LOG_ENTER, CM_ARCH_NEW_PROVE_ACTION, &proveAction);
}

bool edit_prove_action(TWindow *parent, TProveAction *proveAction)
{
	if (TProveActionDialog(parent, PrimaryGroup(TProve), proveAction, true, IDD_EDIT_PROVE_ACTION).Execute() == IDOK)
	{
		log(LOG_EDIT, CM_ARCH_EDIT_PROVE_ACTION, proveAction);
		return true;
	}

	return false;
}

#endif  // APPEAL || AREAL || MILITARY || REGIONAL
