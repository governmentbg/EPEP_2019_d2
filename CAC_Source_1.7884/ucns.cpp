#include "cac.h"

#include "SelectUCNListBox.h"
#include "SelectUCNListBox.cpp"

#include "SelectUCNBaseDialog.h"
#include "SelectUCNBaseDialog.cpp"

#include "SelectUCNDialog.h"
#include "SelectUCNDialog.cpp"

#include "EditNamedDialog.h"		// 2004:168
#include "EditNamedDialog.cpp"

#include "EditCitizenDialog.h"
#include "EditCitizenDialog.cpp"

#include "EditJudicialDialog.h"
#include "EditJudicialDialog.cpp"

#include "EditFirmDialog.h"
#include "EditFirmDialog.cpp"

#include "EditSenderDialog.h"
#include "EditSenderDialog.cpp"

#include "EditNIATDepDialog.h"
#include "EditNIATDepDialog.cpp"

#include "EditEISenderDialog.h"
#include "EditEISenderDialog.cpp"

#include "EditCompanyDialog.h"
#include "EditCompanyDialog.cpp"

bool get_ucn(TWindow *parent, TFoundGroup *ucnGroup, const char *ucnTypes, sign allowAdd, const char *involvements,
	bool active, ucn_action action, bool stat)
{
	switch (TSelectUCNDialog(parent, ucnGroup, ucnTypes, allowAdd, involvements, active, action, stat).Execute())
	{
		case IDOK :
		{
			// 2011:081 LPR: full stat support, removed stat from select
			// 2018-03-20: FIX: ignore sender stat == "no current court"
			if (stat && ucn2uxn(ucnGroup->ucnType) != UXN_SENDER)
			{
				const char *bulstat = ((TString *) ucnGroup->Find("F_BULSTAT"))->S();
				const char *country = ((TStringAlias *) ucnGroup->Find("F_COUNTRY"))->S();

				return any(bulstat) || strcmp(country, BUILDIN_COUNTRY) ||
					edit_ucn(parent, ucnGroup, NULL, false, can_rename(), true);
			}
			return true;
		}
		case IDADD :
		{
			if (add_ucn(parent, ucnGroup, stat))
				break;
			// falldown
		}
		default : return false;
	}

	if (allowAdd == true)
		edit_addresses(parent, ucnGroup, ADDRESS_ALLOW_FOREIGN);

	return true;
}

bool get_ucn(TWindow *parent, TFoundGroup *ucnGroup, const char *ucnTypes, sign allowAdd, bool stat)
{
	return get_ucn(parent, ucnGroup, ucnTypes, allowAdd, NULL, false, NULL, stat);
}

bool get_ucn(TWindow *parent, TTransGroup *ucnGroup, const char *ucnTypes, const char *involvements, bool active, bool stat,
	sign allowAdd)
{
	return get_ucn(parent, ucnGroup, ucnTypes, allowAdd, involvements, active, NULL, stat);
}

void repeat_ucn(TWindow *parent, TFoundGroup *ucnGroup, const char *ucnTypes, ucn_action action)
{
	get_ucn(parent, ucnGroup, ucnTypes, false, NULL, false, action, false);
}

// 2009:044 LPR: altered to store new persons and renames
static bool edit_ucn(TWindow *parent, TUCNGroup *ucnGroup, TRCDKeyContainer *container, bool edit, bool getit, bool full, bool stat)
{
	char uxnType = ucn2uxn(ucnGroup->ucnType);
	TNamedGroup *const newGroup = TNamedGroup::NewXGroup(uxnType);
	TVirtPtr VP_N(newGroup);
	bool result, reReg;

	if (uxnType == UXN_DAMN)
	{
		if (edit)
		{
			reReg = getit;
			getit = false;
		}
		else
			reReg = false;
	}

	*newGroup << ucnGroup;

	if (!getit || newGroup->Get())
	{
		bool autorun = false;
		TNamedGroup *const oldGroup = TNamedGroup::NewXGroup(uxnType);
		TVirtPtr VP_O(oldGroup);

		*oldGroup << newGroup;	// names loaded at this point

		switch (uxnType)
		{
			case UXN_CITIZEN :
			{
				if (ucnGroup->ucnType == UCN_CITIZEN_UCN && is_internal(ucnGroup->ucn))
					result = error("Лицето е забранено за модификация.");
				else
				{
					// 2009:027 LPR: lock users / known persons
					TUser user;
					TKnownAliasGroup person;	// 2013:248 FIX: was TFoundGroup

					user << ucnGroup;
					person << ucnGroup;

					if (!can_func(RIGHT_NOM) && (user.Try() || person.Try("F_UCN")))
						result = autorun = true;
					else
						result = TEditCitizenDialog(parent, (TCitizen *) newGroup, edit, full).Execute() == IDOK;
				}
				break;
			}
			case UXN_FIRM :
			{
				result = TEditFirmDialog(parent, (TFirm *) newGroup, edit, full, stat).Execute() == IDOK;
				break;
			}
			case UXN_ANONYMOUS :
			{
				newGroup->ucnType = UCN_ANONYMOUS_CODE;
				result = TEditSenderDialog(parent, (TSender *) newGroup, edit, full, IDD_EDIT_ANONYMOUS).Execute() == IDOK;
				break;
			}
			case UXN_UNKNOWN :
			{
				newGroup->ucnType = UCN_UNKNOWN_CODE;
				result = TEditSenderDialog(parent, (TSender *) newGroup, edit, full, IDD_EDIT_UNKNOWN).Execute() == IDOK;
				break;
			}
			case UXN_SENDER :
			{
				TSender *sender = (TSender *) newGroup;

				if (strchr(EISPP_SENDERS, ucnGroup->ucnType))
					result = TEditEISenderDialog(parent, sender, edit, full).Execute() == IDOK;
				else if (ucnGroup->ucnType == UCN_NIA_TDEP)
					result = TEditNIATDepDialog(parent, sender, edit).Execute() == IDOK;
				else
				{
					int resId = strchr(UCN_MIXED_CASE_SENDERS, sender->ucnType) ? IDD_EDIT_SENDER_MIXED :
						IDD_EDIT_SENDER_UPPER;	// 2017:089 courtset names are editable now

					result = TEditSenderDialog(parent, sender, edit, full, resId).Execute() == IDOK;
				}
				break;
			}
			case UXN_DAMN :
			{
				newGroup->ucnType = UCN_DAMN_CODE;
				result = TEditCompanyDialog(parent, (TDamn *) newGroup, edit, full, stat, reReg).Execute() == IDOK;
				break;
			}
			default :
			{
				fatal("%d: ucnType not supported by edit_ucn", ucnGroup->ucnType);
				result = false;
			}
		}

		if (result)
		{
			if (!autorun)
			{
				mstr newText;
				newGroup->Print(newText);

				if (edit)
				{
					mstr oldText;
					oldGroup->Print(oldText);

					if (oldText.cmp(newText))
					{
						// 2010:020 LPR/LRQ: oldText -> -old +new
						mbk_insert(&oldText, "-", 0, 1);

						if (container)
							log(LOG_RENAME, IDRENAME, container, str(oldText));
						else
							log(LOG_RENAME, IDRENAME, str(oldText));

						mbk_insert(&newText, "+", 0, 1);

						if (container)
							log(LOG_RENAME, IDRENAME, container, str(newText));
						else
							log(LOG_RENAME, IDRENAME, str(newText));
					}
				}
				else
					log(LOG_CREATE, IDCREATE, str(newText));
			}

			*ucnGroup << newGroup;
		}
	}
	else
		result = false;

	return result;
}

bool add_ucn(TWindow *parent, TUCNGroup *ucnGroup, bool stat)
{
	return edit_ucn(parent, ucnGroup, NULL, false, false, true, stat);
}

bool edit_ucn(TWindow *parent, TUCNGroup *ucnGroup, TRCDKeyContainer *container, bool getit, bool full, bool stat)
{
	return edit_ucn(parent, ucnGroup, container, true, getit, full, stat);
}

#if COURT_TYPE == COURT_AREAL
// 2011:109 LPR: cut/modified select ucn
#include "SelectFirmsDialog.h"
#include "SelectFirmsDialog.cpp"

void select_firms(TWindow *parent, firm_action action)
{
	TSelectFirmsDialog(parent, action).Execute();
}
#endif  // AREAL

#include "SelectUCNMultiDialog.h"
#include "SelectUCNMultiDialog.cpp"

void select_ucns(TWindow* parent, const char *ucnTypes, bool allowEmpty, const char *involvements, ucn_multi_action action)
{
	TSelectUCNMultiDialog(parent, ucnTypes, allowEmpty, involvements, action).Execute();
}
