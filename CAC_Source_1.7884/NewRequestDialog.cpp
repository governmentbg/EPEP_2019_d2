DEFINE_RESPONSE_TABLE1(TNewRequestDialog, TEditIncomingDialog)
//{{TNewRequestDialogRSP_TBL_BEGIN}}
	EV_BN_CLICKED(IDOK, OKBNClicked),
	EV_CBN_SELCHANGE(IDC_EDIT_INCOMING_KIND, KindCBNSelChange),
	EV_BN_CLICKED(IDC_REGISTER_REQUEST_QUICK, QuickBNClicked),
	EV_BN_CLICKED(IDADD, AddBNClicked),
	EV_BN_CLICKED(IDEDIT, EditBNClicked),
	EV_COMMAND_ENABLE(IDEDIT, EditBNEnable),
	EV_BN_CLICKED(IDDELETE, DeleteBNClicked),
	EV_COMMAND_ENABLE(IDDELETE, DeleteBNEnable),
	EV_LVN_ITEMCHANGING(IDC_GROUP_LIST_WINDOW, ListLVNItemChanging),
	EV_BN_CLICKED(IDLOAD, LoadBNClicked),
	EV_CBN_SELCHANGE(IDC_REGISTER_REQUEST_LAW_KIND, LawKindCBNSelChange),
//{{TNewRequestDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TNewRequestDialog Implementation}}

TNewRequestDialog::TNewRequestDialog(TWindow* parent, TRequest *request, const char *kinds, char type)
:
	TEditIncomingDialog(parent, request, kinds, Default->colleges, false, IDD_REGISTER_REQUEST), types(C2S[type])
{
	subject = new TSubjectAliasFace(this, IDC_REGISTER_REQUEST_SUBJECT, "F_SUBJECT", kind2LawKind(*kinds, type));
	quick = new TQuickFace(this, IDC_REGISTER_REQUEST_QUICK, "F_QUICK");
	new TStringFace(this, IDC_REGISTER_REQUEST_TEXT, "F_TEXT", SIZE_OF_ORIGIN_TEXT);
	eispp_nmr = new TEisNmrFace(this, IDC_REGISTER_REQUEST_EISPP_NMR, "F_EISPP_NMR");
	eispp_nmr_label = new TStatic(this, IDC_REGISTER_REQUEST_EISPP_NMR_LABEL);
	connects = new TConnectListWindow(this, IDC_GROUP_LIST_WINDOW, NULL, NULL, eispp_nmr);
	lawKind = new TCharAliasFace(this, IDC_REGISTER_REQUEST_LAW_KIND, "F_LAW_KIND", Kind, "", ALIAS_OPTION);
	load = new TPushButton(this, IDLOAD);
}

bool TNewRequestDialog::IsValid()
{
	bool isValid = TEditIncomingDialog::IsValid();

	if (isValid)
		isValid = connects->IsValid();

	return isValid;
}

void TNewRequestDialog::SetupWindow()
{
	TEditIncomingDialog::SetupWindow();

	SetCaption("Регистриране на входящ документ");
	direction->SetS("На доклад за разпределение");
	KindCBNSelChange();

#if COURT_TYPE == COURT_ADMIN
	eispp_nmr->ShowWindow(SW_HIDE);
	eispp_nmr_label->ShowWindow(SW_HIDE);
#endif  // ADMIN
}

void TNewRequestDialog::OKBNClicked()
{
	if (IsValid() && CheckDirection() && ask_college(lawKind->C(), college->C(), "входящия документ"))
	{
		*Request() << this;

		if (Request()->Insert())
		{
			write_connects(Request(), connects->Connects(), "регистрирания документ");
			trouble.Move(Request(), date->Date());
			CmOk();
		}
	}
}

void TNewRequestDialog::KindCBNSelChange()
{
	char tKind = kind->C();
	kind2LawKinds(tKind, types, lawKinds);
	lawKind->SetXCrit(str(lawKinds));
	LawKindCBNSelChange();
	quick->SetKind(tKind);
	connects->SetIndocKind(tKind);
	connects->AdjustJurisd();
}

void TNewRequestDialog::QuickBNClicked()
{
	quick->AlterText();
}

void TNewRequestDialog::AddBNClicked()
{
	connects->Add();
}

void TNewRequestDialog::EditBNClicked()
{
	connects->Edit();
}

void TNewRequestDialog::DeleteBNClicked()
{
	connects->Delete();

	if (!connects->MainType())
		xfer_reject(Request());
}

void TNewRequestDialog::LoadBNClicked()
{
	TGArray<TConnect> *tConnects = new TGArray<TConnect>;
	Request()->college = college->C();

	if (xfer_load(this, Request(), tConnects, &trouble))
	{
		connects->ResetConnects(tConnects);
		eispp_nmr->SetS(Request()->eispp_nmr);

		if (Request()->kind)
		{
			kind->SetC(Request()->kind);
			lawKind->SetXCrit(KIND_LAWSUITS);
			lawKind->SetC(Request()->lawKind);
			KindCBNSelChange();
		}
		else
			Request()->kind = kind->C();
	}
	else
		delete tConnects;
}

void TNewRequestDialog::EditBNEnable(TCommandEnabler &tce)
{
	tce.Enable(connects->MayEdit());
}

void TNewRequestDialog::DeleteBNEnable(TCommandEnabler &tce)
{
	tce.Enable(connects->MayDelete());
}

bool TNewRequestDialog::ListLVNItemChanging(TLwNotify &lwn)
{
	  return connects->ItemChanging(lwn);
}

void TNewRequestDialog::LawKindCBNSelChange()
{
	char tLawKind = lawKind->C();

	adjust_law_college(tLawKind, college);
	subject->SetKind(tLawKind);
	connects->SetLawKind(tLawKind);
	eispp_nmr->SetKinds(kind->C(), tLawKind, false);
}
