DEFINE_RESPONSE_TABLE1(TNkzPneListDialog, TFzlGroupListDialog)
//{{TNkzPneListDialogRSP_TBL_BEGIN}}
	EV_COMMAND_ENABLE(IDDELETE, DeleteBNEnable),
	EV_BN_CLICKED(IDCANCEL, CancelBNClicked),
	EV_WM_CLOSE,
//{{TNkzPneListDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TNkzPneListDialog Implementation}}

TNkzPneListDialog::TNkzPneListDialog(TWindow* parent, NPRFZLPNE *nprfzlpne, FZL *fzl, NPR *npr, bool tEdit, int resId)
:
	TFzlGroupListDialog(parent, fzl, npr, resId), edit(tEdit)
{
	nkzpnes = new TNkzPneListWindow(this, IDC_GROUP_LIST_WINDOW, nprfzlpne, fzl, npr);
}

bool TNkzPneListDialog::CanClose()
{
	bool result = TFzlGroupListDialog::CanClose();

	if (result)
	{
		if (edit && !nkzpnes->Count())
			result = error("Няма въведени наказания.");
	}

	return result;
}

void TNkzPneListDialog::DeleteBNEnable(TCommandEnabler &tce)
{
	tce.Enable(nkzpnes->Group() && (!edit || nkzpnes->Count() > 1));
}

void TNkzPneListDialog::CancelBNClicked()
{
	EvClose();
}

void TNkzPneListDialog::EvClose()
{
	if (CanClose())
	{
		if (nkzpnes->Count() ||
			warn("Няма въведени наказания.\n\nОтказвате ли се от изпращането на ЕИСПП данни за престъплението?"))
		{
			TFzlGroupListDialog::EvClose();
		}
	}
}
