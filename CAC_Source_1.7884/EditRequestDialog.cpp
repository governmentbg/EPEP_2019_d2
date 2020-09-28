DEFINE_RESPONSE_TABLE1(TEditRequestDialog, TEditIncomingDialog)
//{{TEditRequestDialogRSP_TBL_BEGIN}}
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
//{{TEditRequestDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TEditRequestDialog Implementation}}

TEditRequestDialog::TEditRequestDialog(TWindow* parent, TRequest *request, char tStatus, int resId)
:
	TEditIncomingDialog(parent, request, NULL, Default->colleges, true, resId), status(tStatus),
		has_eispp_nmr(any(request->eispp_nmr)), types(Default->colleges)
{
	subject = new TSubjectAliasFace(this, IDC_REGISTER_REQUEST_SUBJECT, "F_SUBJECT", request->lawKind, request->subject);
	quick = new TQuickFace(this, IDC_REGISTER_REQUEST_QUICK, "F_QUICK");
	new TStringFace(this, IDC_REGISTER_REQUEST_TEXT, "F_TEXT", SIZE_OF_ORIGIN_TEXT);
	tConnects = new TGArray<TConnect>;
	load_connects(request, tConnects);
	eispp_nmr = new TEisNmrFace(this, IDC_REGISTER_REQUEST_EISPP_NMR, "F_EISPP_NMR");
	eispp_nmr_label = new TStatic(this, IDC_REGISTER_REQUEST_EISPP_NMR_LABEL);
	connects = new TConnectListWindow(this, IDC_GROUP_LIST_WINDOW, tConnects, NULL, eispp_nmr);
	add = new TPushButton(this, IDADD);
	kind2LawKinds(request->kind, types, lawKinds);
	lawKind = new TCharAliasFace(this, IDC_REGISTER_REQUEST_LAW_KIND, "F_LAW_KIND", Kind, str(lawKinds));
}

TEditRequestDialog::~TEditRequestDialog()
{
	delete tConnects;
}

bool TEditRequestDialog::IsValid()
{
	bool isValid = TEditIncomingDialog::IsValid();

	if (isValid)
		isValid = connects->IsValid();

	return isValid;
}

bool TEditRequestDialog::IsClean()
{
	bool isClean = TEditIncomingDialog::IsClean();

	if (isClean)
	{
		isClean = !connects->dirty || ask("Промените в адресантите се записват само при потвърждение. "
			"Желаете ли да прекъснете редактирането?");
	}

	return isClean;
}

void TEditRequestDialog::SetupWindow()
{
	TEditIncomingDialog::SetupWindow();

	SetCaption("Редактиране на входящ документ");
	KindCBNSelChange();

	quick->SetL(Request()->quick);
	QuickBNClicked();		// 2011:004 FIX: was missing
	// 2001:268 Allow kind change; 2001:275 For new requests only
	// 2003:262 QUICK_CONVERSION_INDOCS -> adjust_indoc_kinds()
	// 2007:219 LPR: removed IDD_EDIT_REQUEST: date, no, kind changes
	const char *kinds = adjust_indoc_kinds(kind->C());

	if (status == RESOLUTION_NEW && can_func(RIGHT_EDIT_KIND) && kinds)
	{
		kind->EnableWindow(true);
		kind->SetXCrit(kinds);
	}

	if (status == RESOLUTION_LAWED)
	{
		subject->EnableWindow(false);
		add->EnableWindow(false);
	}

	// 2010:053 LRQ
	if (!any(Request()->direction))
		direction->SetS("На доклад за разпределение");

#if COURT_TYPE == COURT_ADMIN
	eispp_nmr->ShowWindow(SW_HIDE);
	eispp_nmr_label->ShowWindow(SW_HIDE);
#endif  // ADMIN
}

void TEditRequestDialog::OKBNClicked()
{
	if (IsValid() && CheckDirection())
	{
		*Request() << this;

		if (Request()->Update() && ask_college(lawKind->C(), college->C(), "входящия документ"))
		{
			write_connects(Request(), connects->Connects(), "документа");
			connects->dirty = false;
			trouble.Move(Request(), date->Date());
			CmOk();
		}
	}
}

void TEditRequestDialog::KindCBNSelChange()
{
	char tKind = kind->C();
	kind2LawKinds(tKind, types, lawKinds);
	lawKind->SetXCrit(str(lawKinds));
	LawKindCBNSelChange();
	quick->SetKind(tKind);
	connects->SetIndocKind(tKind);
}

void TEditRequestDialog::QuickBNClicked()
{
	quick->AlterText();
}

void TEditRequestDialog::AddBNClicked()
{
	connects->Add();
}

void TEditRequestDialog::EditBNClicked()
{
	connects->Edit();
}

void TEditRequestDialog::DeleteBNClicked()
{
	connects->Delete();
	// 2016:196 FIX: was missing
	if (!connects->MainType())
		xfer_reject(Request());
}

void TEditRequestDialog::EditBNEnable(TCommandEnabler &tce)
{
	tce.Enable(status != RESOLUTION_LAWED && connects->MayEdit());
}

void TEditRequestDialog::DeleteBNEnable(TCommandEnabler &tce)
{
	tce.Enable(status != RESOLUTION_LAWED && connects->MayDelete());
}

bool TEditRequestDialog::ListLVNItemChanging(TLwNotify &lwn)
{
	  return connects->ItemChanging(lwn);
}

void TEditRequestDialog::LoadBNClicked()
{
	TGArray<TConnect> tConnects;
	// << this will restore request kind and eispp_nmr
	xfer_load(this, Request(), &tConnects, &trouble);
}

void TEditRequestDialog::LawKindCBNSelChange()
{
	char tLawKind = lawKind->C();

	adjust_law_college(tLawKind, college);
	subject->SetKind(tLawKind);
	connects->SetLawKind(tLawKind);
	eispp_nmr->SetKinds(kind->C(), tLawKind, has_eispp_nmr);
}
