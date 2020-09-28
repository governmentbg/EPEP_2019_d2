DEFINE_RESPONSE_TABLE1(TSbeFzlListDialog, TGroupListDialog)
//{{TSbeFzlListDialogRSP_TBL_BEGIN}}
	EV_BN_CLICKED(IDADD, AddBNClicked),
	EV_COMMAND_ENABLE(IDADD, AddBNEnable),
	EV_BN_CLICKED(IDEDIT, EditBNClicked),
	EV_COMMAND_ENABLE(IDEDIT, EditBNEnable),
	EV_BN_CLICKED(IDDELETE, DeleteBNClicked),
	EV_COMMAND_ENABLE(IDDELETE, DeleteBNEnable),
	EV_BN_CLICKED(IDCANCEL, CancelBNClicked),
	EV_WM_CLOSE,
//{{TSbeFzlListDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TSbeFzlListDialog Implementation}}

TSbeFzlListDialog::TSbeFzlListDialog(TWindow* parent, SBE *sbe, bool tEdit, int resId)
:
	TGroupListDialog(parent, resId), npr(sbe->npr), include("T_FZL"), edit(tEdit), allLinks("T_NPRFZLPNE")
{
	FZL tFzl;
	PNE pne;
	NPRFZLPNE nprfzlpne;

	tFzl.LoadFloat(&include, npr, npr->dlo->fzl);
	npr->dlo->pne->Flush();		// 2013:031 LPR: don't combine xml/db loaded pne-s
	pne.LoadFloat(npr->dlo->pne, npr, npr->dlo->pne);
	nprfzlpne.LoadFloat(&allLinks, npr, npr->dlo->nprfzlpne);

	new TEisNmrFace(this, IDC_SBE_LIST_NPRNMR, "F_NPRNMR", EISPP_NPR_KERNEL);
	fzl = new TFzlComboBox(this, IDC_SBE_FZL_LIST_FZL, sbe, &include, NULL, npr->dlo->fzl);
	fzls = new TSbeFzlListWindow(this, IDC_GROUP_LIST_WINDOW, npr->dlo->fzl, &allLinks, fzl, sbe);
}

void TSbeFzlListDialog::SetupWindow()
{
	TGroupListDialog::SetupWindow();
	*npr >> this;
}

bool TSbeFzlListDialog::CanClose()
{
	bool result = TGroupListDialog::CanClose();

	if (result)
	{
		if (edit && !fzls->Count())
			result = error("Няма въведени лица.");
	}

	return result;
}

void TSbeFzlListDialog::AddBNClicked()
{
	TGroupListDialog::AddBNClicked();
	fzl->Refresh();
}

void TSbeFzlListDialog::AddBNEnable(TCommandEnabler &tce)
{
	tce.Enable(fzl->Fzl());
}

void TSbeFzlListDialog::EditBNClicked()
{
	fzls->Edit();
}

void TSbeFzlListDialog::EditBNEnable(TCommandEnabler &tce)
{
	tce.Enable(fzls->Group() && (fzls->Fzl()->Includes("NPRFZLSTA") || npr->Includes("NPRFZLPNE")));
}

void TSbeFzlListDialog::DeleteBNClicked()
{
	TGroupListDialog::DeleteBNClicked();
	fzl->Refresh();
}

void TSbeFzlListDialog::DeleteBNEnable(TCommandEnabler &tce)
{
	tce.Enable(fzls->Group() && (!edit || fzls->Count() > 1));
}

void TSbeFzlListDialog::CancelBNClicked()
{
	EvClose();
}

void TSbeFzlListDialog::EvClose()
{
	if (CanClose())
	{
		if (fzls->Count() || warn("Няма въведени лица.\n\nОтказвате ли се от изпращането на ЕИСПП данни за събитието?"))
			TGroupListDialog::EvClose();
	}
}
