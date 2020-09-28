DEFINE_RESPONSE_TABLE1(TSelectRegisterActDialog, TGroupListDialog)
//{{TSelectContainerDialogRSP_TBL_BEGIN}}
	EV_BN_CLICKED(IDOK, OKBNClicked),
	EV_COMMAND_ENABLE(IDVIEW, SelectEnable),
	EV_BN_CLICKED(IDVIEW, ViewBNClicked),
	EV_COMMAND_ENABLE(IDOTHER, OtherBNEnable),
	EV_BN_CLICKED(IDOTHER, OtherBNClicked),
	EV_LVN_ITEMCHANGING(IDC_SELECT_REGISTER_ACT_ACTS, ListLVNItemChanging),
//{{TSelectContainerDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TSelectRegisterActDialog Implementation}}

TSelectRegisterActDialog::TSelectRegisterActDialog(TWindow* parent, TElectricTrouble *tTrouble, TGArray<TRegisterAct> *tActs,
	mstr &tOthers, int resId)
:
	TGroupListDialog(parent, resId), trouble(tTrouble), others(tOthers)
{
	glist = acts = new TSelectRegisterActListWindow(this, IDC_SELECT_REGISTER_ACT_ACTS, tActs);
	new TPushButton(this, IDVIEW, IDQUERY);
}

void TSelectRegisterActDialog::OKBNClicked()
{
	TRegisterAct *act = (TRegisterAct *) acts->Group();

	trouble->ClearAct();

	if (act)
	{
		trouble->act = (TRegisterAct *) act->Clone();
		*trouble->act << act;

		if (trouble->act->FetchFiles(trouble))
			CmOk();
		else
			trouble->ClearAct();
	}
	else
		CmOk();
}

void TSelectRegisterActDialog::ViewBNClicked()
{
	acts->View();
}

void TSelectRegisterActDialog::OtherBNEnable(TCommandEnabler &tce)
{
	tce.Enable(any(others));
}

void TSelectRegisterActDialog::OtherBNClicked()
{
	message("%s.", str(others));
}
