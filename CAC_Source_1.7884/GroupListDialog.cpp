#include "cac.h"

DEFINE_RESPONSE_TABLE1(TGroupListDialog, TQuickDialog)
//{{TGroupListDialogRSP_TBL_BEGIN}}
	EV_BN_CLICKED(IDADD, AddBNClicked),
	EV_BN_CLICKED(IDEDIT, EditBNClicked),
	EV_BN_CLICKED(IDDELETE, DeleteBNClicked),
	EV_COMMAND_ENABLE(IDEDIT, SelectEnable),
	EV_COMMAND_ENABLE(IDDELETE, SelectEnable),
	EV_LVN_ITEMCHANGING(IDC_GROUP_LIST_WINDOW, ListLVNItemChanging),
//{{TGroupListDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TGroupListDialog Implementation}}

TGroupListDialog::TGroupListDialog(TWindow* parent, int resId)
:
	TQuickDialog(parent, resId), glist(NULL)
{
}

void TGroupListDialog::SelectEnable(TCommandEnabler &tce)
{
	tce.Enable(glist->Group() != NULL);
}

void TGroupListDialog::SetupWindow()
{
	if (!glist)
	{
		TWindow *window = FirstThat(MatchesId, (void *) IDC_GROUP_LIST_WINDOW);

#if RANGECHECK
		if (!window)
			fatal("group list dialog: no list");
#endif  // RANGECHECK
		// static cast does not work
		glist = dynamic_cast<TGroupListWindow *> (window);
#if TYPECHECK
		if (!glist)
			fatal("group list dialog: bad list");
#endif  // TYPECHECK
	}

	TQuickDialog::SetupWindow();
}

void TGroupListDialog::AddBNClicked()
{
	glist->Add();
}

void TGroupListDialog::EditBNClicked()
{
	glist->Edit();
}

void TGroupListDialog::DeleteBNClicked()
{
	glist->Delete();
}

bool TGroupListDialog::ListLVNItemChanging(TLwNotify &lwn)
{
	return glist->ItemChanging(lwn);
}
