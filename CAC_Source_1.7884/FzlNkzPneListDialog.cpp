DEFINE_RESPONSE_TABLE1(TFzlNkzPneListDialog, TFzlDialog)
//{{TFzlNkzPneListDialogRSP_TBL_BEGIN}}
	EV_BN_CLICKED(IDC_FZL_NKZ_PNE_LIST_ADD_NKZ, AddNkzBNClicked),
	EV_BN_CLICKED(IDC_FZL_NKZ_PNE_LIST_EDIT_NKZ, EditNkzBNClicked),
	EV_BN_CLICKED(IDC_FZL_NKZ_PNE_LIST_DELETE_NKZ, DeleteNkzBNClicked),
	EV_COMMAND_ENABLE(IDC_FZL_NKZ_PNE_LIST_ADD_NKZ, AddNkzBNEnable),
	EV_COMMAND_ENABLE(IDC_FZL_NKZ_PNE_LIST_EDIT_NKZ, EditNkzBNEnable),
	EV_COMMAND_ENABLE(IDC_FZL_NKZ_PNE_LIST_DELETE_NKZ, DeleteNkzBNEnable),
	EV_LVN_ITEMCHANGING(IDC_FZL_NKZ_PNE_LIST_NKZS, NkzsLVNItemChanging),
	EV_BN_CLICKED(IDC_FZL_NKZ_PNE_LIST_ADD_NPRFZLPNE, AddNprFzlPneBNClicked),
	EV_BN_CLICKED(IDC_FZL_NKZ_PNE_LIST_EDIT_NPRFZLPNE, EditNprFzlPneBNClicked),
	EV_BN_CLICKED(IDC_FZL_NKZ_PNE_LIST_DELETE_NPRFZLPNE, DeleteNprFzlPneBNClicked),
	EV_COMMAND_ENABLE(IDC_FZL_NKZ_PNE_LIST_ADD_NPRFZLPNE, AddNprFzlPneBNEnable),
	EV_COMMAND_ENABLE(IDC_FZL_NKZ_PNE_LIST_EDIT_NPRFZLPNE, EditNprFzlPneBNEnable),
	EV_COMMAND_ENABLE(IDC_FZL_NKZ_PNE_LIST_DELETE_NPRFZLPNE, DeleteNprFzlPneBNEnable),
	EV_LVN_ITEMCHANGING(IDC_FZL_NKZ_PNE_LIST_NPRFZLPNES, NprFzlPnesLVNItemChanging),
	EV_BN_CLICKED(IDCANCEL, CancelBNClicked),
	EV_WM_CLOSE,
//{{TFzlNkzPneListDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TFzlNkzPneListDialog Implementation}}

TFzlNkzPneListDialog::TFzlNkzPneListDialog(TWindow* parent, FZL *fzl, SBE *sbe, bool tHasNkzs, bool tHasNprFzlPnes, bool tEdit)
:
	TFzlDialog(parent, fzl, sbe->npr, -1, sbe->npr->nmr, IDD_FZL_NKZ_PNE_LIST), edit(tEdit), hasNkzs(tHasNkzs),
		hasNprFzlPnes(tHasNprFzlPnes)
{
	nkzs = new TNkzListWindow(this, IDC_FZL_NKZ_PNE_LIST_NKZS, fzl, sbe);
	new TPushButton(this, IDC_FZL_NKZ_PNE_LIST_ADD_NKZ, IDADD);
	new TPushButton(this, IDC_FZL_NKZ_PNE_LIST_EDIT_NKZ, IDEDIT);
	new TPushButton(this, IDC_FZL_NKZ_PNE_LIST_DELETE_NKZ, IDDELETE);
	nprfzlpnes = new TFzlNprPneListWindow(this, IDC_FZL_NKZ_PNE_LIST_NPRFZLPNES, fzl, sbe);
	new TPushButton(this, IDC_FZL_NKZ_PNE_LIST_ADD_NPRFZLPNE, IDADD);
	new TPushButton(this, IDC_FZL_NKZ_PNE_LIST_EDIT_NPRFZLPNE, IDEDIT);
	new TPushButton(this, IDC_FZL_NKZ_PNE_LIST_DELETE_NPRFZLPNE, IDDELETE);
}

bool TFzlNkzPneListDialog::CanClose()
{
	bool result = TFzlDialog::CanClose();

	if (result)
	{
		if (edit)
		{
			if (hasNkzs && !nkzs->Count())
				result = error("Няма въведени наказания за лицето.");
			else if (hasNprFzlPnes && !nprfzlpnes->Count())
				result = error("Няма данни за поне едно престъпление.");
		}
		else if (hasNkzs && hasNprFzlPnes)
		{
			if (!nkzs->Count() != !nprfzlpnes->Count())
				result = error("Трябва да се въведат данни и в двата списъка.");
		}
	}

	return result;
}

void TFzlNkzPneListDialog::SetupWindow()
{
	TFzlDialog::SetupWindow();
	nkzs->EnableWindow(hasNkzs);
	nprfzlpnes->EnableWindow(hasNprFzlPnes);
}

void TFzlNkzPneListDialog::AddNkzBNClicked()
{
	nkzs->Add();
}

void TFzlNkzPneListDialog::EditNkzBNClicked()
{
	nkzs->Edit();
}

void TFzlNkzPneListDialog::DeleteNkzBNClicked()
{
	nkzs->Delete();
}

void TFzlNkzPneListDialog::AddNkzBNEnable(TCommandEnabler &tce)
{
	tce.Enable(hasNkzs);
}

void TFzlNkzPneListDialog::EditNkzBNEnable(TCommandEnabler &tce)
{
	tce.Enable(hasNkzs && nkzs->Group());
}

void TFzlNkzPneListDialog::DeleteNkzBNEnable(TCommandEnabler &tce)
{
	tce.Enable(hasNkzs && nkzs->Group() && (!edit || nkzs->Count() > 1));
}

bool TFzlNkzPneListDialog::NkzsLVNItemChanging(TLwNotify &lwn)
{
	return nkzs->ItemChanging(lwn);
}

void TFzlNkzPneListDialog::AddNprFzlPneBNClicked()
{
	nprfzlpnes->Add();
}

void TFzlNkzPneListDialog::EditNprFzlPneBNClicked()
{
	nprfzlpnes->Edit();
}

void TFzlNkzPneListDialog::DeleteNprFzlPneBNClicked()
{
	nprfzlpnes->Delete();
}

void TFzlNkzPneListDialog::AddNprFzlPneBNEnable(TCommandEnabler &tce)
{
	tce.Enable(hasNprFzlPnes && nprfzlpnes->LinkCount() > 0);
}

void TFzlNkzPneListDialog::EditNprFzlPneBNEnable(TCommandEnabler &tce)
{
	tce.Enable(hasNprFzlPnes && nprfzlpnes->Group());
}

void TFzlNkzPneListDialog::DeleteNprFzlPneBNEnable(TCommandEnabler &tce)
{
	tce.Enable(hasNprFzlPnes && nprfzlpnes->Group() && (!edit || nprfzlpnes->Count() > 1));
}

bool TFzlNkzPneListDialog::NprFzlPnesLVNItemChanging(TLwNotify &lwn)
{
	return nprfzlpnes->ItemChanging(lwn);
}

void TFzlNkzPneListDialog::CancelBNClicked()
{
	EvClose();
}

void TFzlNkzPneListDialog::EvClose()
{
	if (CanClose())
	{
		if (nkzs->Count() || nprfzlpnes->Count() ||
			warn("Няма въведени данни.\n\nОтказвате ли се от изпращането на ЕИСПП данни за събитието?"))
		{
			TFzlDialog::EvClose();
		}
	}
}
