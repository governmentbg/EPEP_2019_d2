#include "cac.h"

#include "ChooseDepartDialog.h"
#include "ChooseDepartDialog.cpp"

static bool add_sender(TWindow *parent, TSender *sender, char ucnType)
{
	if (strchr(UCN_COURT_TEMPL_SENDERS, ucnType))
	{
		if (get_ucn(parent, sender, C2S[UCN_INSTANCE_EQUAL], false, ucnType == UCN_INSTANCE_EXTERNAL))
		{
			TAddress address;
			address << sender;
			bool tAddress = address.Try();

			sender->ucnType = ucnType;
			if (sender->Insert())
			{
				if (tAddress)
				{
					address.ucnType = ucnType;
					address.Insert();
				}

				return true;
			}
		}
	}
	else
	{
		sender->ucnType = ucnType;

		if (strchr(EISPP_SENDERS, ucnType) && TChooseDepartDialog(parent, sender).Execute() != IDOK)
			return false;

		if (add_ucn(parent, sender))
		{
			if (strchr(UCN_EDIT_ADDR_SENDERS, ucnType))
			{
				if (edit_address(parent, sender))
					return true;

				if (strchr(UCN_ADDR_REQR_SENDERS, ucnType) && sender->Delete(true))
					return false;  // un-added
			}

			return true;
		}
	}

	return false;
}

static bool edit_sender(TWindow *parent, TSender *sender)
{
	bool edited = false;

	if (strchr(UCN_EDIT_NAME_SENDERS, sender->ucnType))
	{
		if (edit_ucn(parent, sender, NULL, false) != IDOK)
			return false;

		edited = true;
	}

	if (strchr(UCN_EDIT_ADDR_SENDERS, sender->ucnType) && edit_address(parent, sender))
		edited = true;

	return edited;
}

#include "SenderListWindow.h"
#include "SenderListWindow.cpp"

#include "SenderListDialog.h"
#include "SenderListDialog.cpp"

void edit_senders(TWindow *parent)
{
	log(LOG_NOM, CM_ADMIN_SENDERS, "адресанти");
	TSenderListDialog(parent).Execute();
	// 2017:089 self, InstanceIII... may be changed
	reload_title(parent);
}
