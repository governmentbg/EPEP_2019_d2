DEFINE_RESPONSE_TABLE1(TFzlNprPneListDialog, TFzlGroupListDialog)
//{{TFzlNprPneListDialogRSP_TBL_BEGIN}}
	EV_BN_CLICKED(IDADD, AddBNClicked),
	EV_COMMAND_ENABLE(IDADD, AddBNEnable),
	EV_BN_CLICKED(IDDELETE, DeleteBNClicked),
	EV_COMMAND_ENABLE(IDDELETE, DeleteBNEnable),
	EV_BN_CLICKED(IDCANCEL, CancelBNClicked),
	EV_WM_CLOSE,
//{{TFzlNprPneListDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TFzlNprPneListDialog Implementation}}

TFzlNprPneListDialog::TFzlNprPneListDialog(TWindow* parent, FZL *fzl, SBE *sbe, TXArray<NPRFZLPNE> *allLinks, bool tEdit,
	int resId)
:
	TFzlGroupListDialog(parent, fzl, sbe->npr, resId), edit(tEdit)
{
	nprfzlpne = new TNprFzlPneComboBox(this, IDC_FZLNPRPNE_LIST_NPRFZLPNE, fzl, allLinks, npr);
	nprfzlpnes = new TNprFzlPneListWindow(this, IDC_GROUP_LIST_WINDOW, fzl, sbe, nprfzlpne);
}

bool TFzlNprPneListDialog::CanClose()
{
	bool result = TFzlGroupListDialog::CanClose();

	if (result)
	{
		if (edit && !nprfzlpnes->Count())
			result = error("Няма въведени престъпления.");
	}

	return result;
}

void TFzlNprPneListDialog::AddBNClicked()
{
	TGroupListDialog::AddBNClicked();
	nprfzlpne->Refresh();
}

void TFzlNprPneListDialog::AddBNEnable(TCommandEnabler &tce)
{
	tce.Enable(nprfzlpne->NprFzlPne());
}

void TFzlNprPneListDialog::DeleteBNClicked()
{
	TGroupListDialog::DeleteBNClicked();
	nprfzlpne->Refresh();
}

void TFzlNprPneListDialog::DeleteBNEnable(TCommandEnabler &tce)
{
	tce.Enable(nprfzlpnes->Group() && (!edit || nprfzlpnes->Count() > 1));
}

void TFzlNprPneListDialog::CancelBNClicked()
{
	EvClose();
}

void TFzlNprPneListDialog::EvClose()
{
	if (CanClose())
	{
		if (nprfzlpnes->Count() ||
			warn("Няма въведени престъпления.\n\nОтказвате ли се от изпращането на ЕИСПП данни за лицето?"))
		{
			TFzlGroupListDialog::EvClose();
		}
	}
}
