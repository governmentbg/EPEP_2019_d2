DEFINE_RESPONSE_TABLE1(TChooseDepartDialog, TScaleDialog)
//{{TChooseDepartDialogRSP_TBL_BEGIN}}
	EV_BN_CLICKED(IDOK, OKBNClicked),
	EV_LBN_SELCHANGE(IDC_CHOOSE_DEPART_DEPARTS, DepartsLBNSelChange),
	EV_BN_CLICKED(IDSEARCH, SearchBNClicked),
	EV_BN_CLICKED(IDC_CHOOSE_DEPART_RESEARCH, ResearchBNClicked),
	EV_EN_UPDATE(IDC_CHOOSE_DEPART_NAME, NameENUpdate),
//{{TChooseDepartDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TChooseDepartDialog Implementation}}

TChooseDepartDialog::TChooseDepartDialog(TWindow* parent, TSender *tSender, int resId)
:
	TScaleDialog(parent, resId), sender(tSender)
{
	ucnType = new TStringFace(this, IDC_CHOOSE_DEPART_UCN_TYPE, "F_UCN_TYPE", SIZE_OF_ALIAS);
	eispp = new TLongFace(this, IDC_CHOOSE_DEPART_EISPP, "F_EISPP", SIZE_OF_EISPP_DEPART);
	name = new TStringFace(this, IDC_CHOOSE_DEPART_NAME, "F_NAME", SIZE_OF_EISPP_NAME);
	departs = new TEisDepartListBox(this, IDC_CHOOSE_DEPART_DEPARTS, tSender->ucnType, NULL);
	ok = new TPushButton(this, IDOK);
	search = new TPushButton(this, IDSEARCH);
	research = new TButton(this, IDC_CHOOSE_DEPART_RESEARCH);
}

void TChooseDepartDialog::SetDefault(int resourceId)
{
	if (resourceId == IDSEARCH)
	{
		SetDefaultId(search->GetId());
		search->SetStyle(search->GetStyle() | BS_DEFPUSHBUTTON, true);
		ok->SetStyle(ok->GetStyle() & ~BS_DEFPUSHBUTTON, true);
	}
	else
	{
		SetDefaultId(ok->GetId());
		search->SetStyle(search->GetStyle() & ~BS_DEFPUSHBUTTON, true);
		ok->SetStyle(ok->GetStyle() | BS_DEFPUSHBUTTON, true);
	}
}

void TChooseDepartDialog::SetupWindow()
{
	TScaleDialog::SetupWindow();
	ucnType->SetS(UCNType->Seek(sender->ucnType));
}

void TChooseDepartDialog::OKBNClicked()
{
	sender->eispp = eispp->L();
	strzcpy(sender->name, name->S(), sizeof sender->name);
	CmOk();
}

void TChooseDepartDialog::DepartsLBNSelChange()
{
	if (departs->Depart())
	{
		SetDefault(IDOK);
		eispp->SetL(departs->Depart()->code);
	}
}

void TChooseDepartDialog::SearchBNClicked()
{
	departs->Search(name->S(), false);
	ok->EnableWindow(true);
}

void TChooseDepartDialog::ResearchBNClicked()
{
	departs->Search(name->S(), true);
	ok->EnableWindow(true);
}

void TChooseDepartDialog::NameENUpdate()
{
	ok->EnableWindow(false);
	SetDefault(IDSEARCH);
}
