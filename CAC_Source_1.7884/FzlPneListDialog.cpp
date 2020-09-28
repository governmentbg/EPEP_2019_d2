DEFINE_RESPONSE_TABLE1(TFzlPneListDialog, TEisDialog)
//{{TFzlPneListDialogRSP_TBL_BEGIN}}
	EV_BN_CLICKED(IDC_FZL_PNE_LIST_ADD_FZL, AddFzlBNClicked),
	EV_COMMAND_ENABLE(IDC_FZL_PNE_LIST_ADD_FZL, AddFzlBNEnable),
	EV_BN_CLICKED(IDC_FZL_PNE_LIST_LINKS, LinksBNClicked),
	EV_COMMAND_ENABLE(IDC_FZL_PNE_LIST_LINKS, LinksBNEnable),
	EV_BN_CLICKED(IDC_FZL_PNE_LIST_EDIT_FZL, EditFzlBNClicked),
	EV_COMMAND_ENABLE(IDC_FZL_PNE_LIST_EDIT_FZL, EditFzlBNEnable),
	EV_BN_CLICKED(IDC_FZL_PNE_LIST_DELETE_FZL, DeleteFzlBNClicked),
	EV_COMMAND_ENABLE(IDC_FZL_PNE_LIST_DELETE_FZL, DeleteFzlBNEnable),
	EV_LVN_ITEMCHANGING(IDC_FZL_PNE_LIST_FZLS, FzlsLVNItemChanging),
	EV_BN_CLICKED(IDC_FZL_PNE_LIST_ADD_PNE, AddPneBNClicked),
	EV_COMMAND_ENABLE(IDC_FZL_PNE_LIST_ADD_PNE, AddPneBNEnable),
	EV_BN_CLICKED(IDC_FZL_PNE_LIST_EDIT_PNE, EditPneBNClicked),
	EV_COMMAND_ENABLE(IDC_FZL_PNE_LIST_EDIT_PNE, EditPneBNEnable),
	EV_BN_CLICKED(IDC_FZL_PNE_LIST_DELETE_PNE, DeletePneBNClicked),
	EV_COMMAND_ENABLE(IDC_FZL_PNE_LIST_DELETE_PNE, DeletePneBNEnable),
	EV_LVN_ITEMCHANGING(IDC_FZL_PNE_LIST_PNES, PnesLVNItemChanging),
	EV_MESSAGE(WM_FOCUS_FACE, EvFocusFace),
	EV_BN_CLICKED(IDCANCEL, CancelBNClicked),
	EV_WM_CLOSE,
//{{TFzlPneListDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TFzlPneListDialog Implementation}}

TFzlPneListDialog::TFzlPneListDialog(TWindow* parent, SBE *sbe, TRCDKeyContainer *container, bool tEdit, int tPneMode,
	int resId)
	: TEisDialog(parent, sbe->npr, -1, sbe->npr->nmr, resId), edit(tEdit), pneMode(tPneMode), fzlmin(sbe->npr->fzlmin),
		vid(sbe->vid == sbe_onesprtrnsfr ? sbe_onedlo : sbe->vid)
{
	sbe->npr->InitGenerator();
	fzls = new TFzlListWindow(this, IDC_FZL_PNE_LIST_FZLS, sbe, container);
	new TPushButton(this, IDC_FZL_PNE_LIST_ADD_FZL, IDADD);
	new TPushButton(this, IDC_FZL_PNE_LIST_EDIT_FZL, IDEDIT);
	new TPushButton(this, IDC_FZL_PNE_LIST_DELETE_FZL, IDDELETE);
	pnes = new TPneListWindow(this, IDC_FZL_PNE_LIST_PNES, sbe, pneMode, container);
	new TPushButton(this, IDC_FZL_PNE_LIST_ADD_PNE, IDADD);
	new TPushButton(this, IDC_FZL_PNE_LIST_EDIT_PNE, IDEDIT);
	new TPushButton(this, IDC_FZL_PNE_LIST_DELETE_PNE, IDDELETE);
}

bool TFzlPneListDialog::CanClose()
{
	bool result = TEisDialog::CanClose();

	if (result)
	{
		if (edit)
		{
			if (!fzls->Count())
				result = error(fzlmin ? "Няма добавени лица." : "Няма лица.");
			else if (!pnes->Count())
				result = error("Няма престъпления.");
		}

		if (result)
			result = fzls->IsValid() && pnes->IsValid();
	}

	return result;
}

void TFzlPneListDialog::AddFzlBNClicked()
{
	fzls->Add();
}

void TFzlPneListDialog::AddFzlBNEnable(TCommandEnabler &tce)
{
	tce.Enable(vid != sbe_edit_fzlpne);
}

void TFzlPneListDialog::LinksBNClicked()
{
	fzls->Links();
}

void TFzlPneListDialog::LinksBNEnable(TCommandEnabler &tce)
{
	FZL *fzl = fzls->Fzl();
	tce.Enable(fzl && (vid == sbe_onedlo || vid == sbe_ovadlo || (vid == sbe_vklnzv && fzl->sid >= fzlmin)) &&
		pnes->Array()->Count());
}

void TFzlPneListDialog::EditFzlBNClicked()
{
	fzls->Edit();
}

void TFzlPneListDialog::EditFzlBNEnable(TCommandEnabler &tce)
{
	FZL *fzl = fzls->Fzl();
	tce.Enable(fzl && (fzl->dirty || vid == sbe_edit_fzlpne));
}

void TFzlPneListDialog::DeleteFzlBNClicked()
{
	fzls->Delete();
}

void TFzlPneListDialog::DeleteFzlBNEnable(TCommandEnabler &tce)
{
	FZL *fzl = fzls->Fzl();
	tce.Enable(fzl && (vid != sbe_edit_fzlpne) && fzl->sid >= fzlmin && (!edit || fzls->Count() > 1));
}

bool TFzlPneListDialog::FzlsLVNItemChanging(TLwNotify &lwn)
{
	return fzls->ItemChanging(lwn);
}

void TFzlPneListDialog::AddPneBNClicked()
{
	pnes->Add();
}

void TFzlPneListDialog::AddPneBNEnable(TCommandEnabler &tce)
{
	tce.Enable(vid == sbe_onedlo);
}

void TFzlPneListDialog::EditPneBNClicked()
{
	pnes->Edit();
}

void TFzlPneListDialog::EditPneBNEnable(TCommandEnabler &tce)
{
	PNE *pne = pnes->Pne();
	tce.Enable(pne && (pne->dirty || vid == sbe_edit_fzlpne));
}

void TFzlPneListDialog::DeletePneBNClicked()
{
	pnes->Delete();
}

void TFzlPneListDialog::DeletePneBNEnable(TCommandEnabler &tce)
{
	tce.Enable(vid == sbe_onedlo && pnes->Pne() && (!edit || pnes->Count() > 1));
}

bool TFzlPneListDialog::PnesLVNItemChanging(TLwNotify &lwn)
{
	return pnes->ItemChanging(lwn);
}

long TFzlPneListDialog::EvFocusFace(TParam1 wParam, TParam2 lParam)
{
	if (wParam)
	{
		if (lParam == IDC_FZL_PNE_LIST_FZLS)
			pnes->Deselect();
		else
			fzls->Deselect();
	}

	return MW_FACE_FOCUSED;
}

void TFzlPneListDialog::CancelBNClicked()
{
	EvClose();
}

void TFzlPneListDialog::EvClose()
{
	if (CanClose())
	{
		if (!fzls->Count())
		{
			if (warn(fzlmin ?
				"Няма добавени лица.\n\nОтказвате ли се от изпращането на ЕИСПП данни за събитието?" :
				"Няма лица и престъпления.\n\nОтказвате ли се от изпращането на ЕИСПП данни за делото?"))
			{
				if (!fzlmin)
					Group()->Delete(true);
			}
			else
				return;
		}

		TEisDialog::EvClose();
	}
}
