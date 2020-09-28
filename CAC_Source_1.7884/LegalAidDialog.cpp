DEFINE_RESPONSE_TABLE1(TLegalAidDialog, TGroupDialog)
//{{TLegalAidDialogRSP_TBL_BEGIN}}
	//EV_COMMAND_ENABLE(IDOK, OKBNEnable),
	EV_BN_CLICKED(IDOK, OKBNClicked),
	EV_COMMAND_ENABLE(IDADDRESS, AddressBNEnable),
	EV_BN_CLICKED(IDADDRESS, AddressBNClicked),
	EV_COMMAND_ENABLE(IDC_LEGAL_AID_SPECIFIED_LAWER, LawerBNEnable),
	EV_BN_CLICKED(IDC_LEGAL_AID_SPECIFIED_LAWER, LawerBNClicked),
	EV_BN_CLICKED(IDRIGHT, RightBNClicked),
	EV_LVN_ITEMCHANGING(IDC_LEGAL_AID_LAIDS, ListLVNItemChanging),
//{{TLegalAidDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TLegalAidDialog Implementation}}

TLegalAidDialog::TLegalAidDialog(TWindow* parent, TPrintSubpoena *printsub, int resId)
:
	TGroupDialog(parent, printsub, resId)
{
	legalAidType = new TAliasListBox(this, IDC_LEGAL_AID_TYPE, LegalAidType, NULL, FLAG_NULL);
	specialRepresentative = new TCheckFace(this, IDC_LEGAL_AID_SPECIAL_REPRESENTATIVE, "F_SPECIAL_REPRESENTATIVE");
	reserveDefender = new TCheckFace(this, IDC_LEGAL_AID_RESERVE_DEFENDER, "F_RESERVE_DEFENDER");
	onDuty = new TCheckFace(this, IDC_LEGAL_AID_ON_DUTY, "F_ON_DUTY");
	conflictingInterests = new TCheckFace(this, IDC_LEGAL_AID_CONFLICTING_INTERESTS, "F_CONFLICTING_INTERESTS");
	laids = new TLegalAidSideListWindow(this, IDC_LEGAL_AID_LAIDS, printsub->laids);
	new TStringFace(this, IDC_LEGAL_AID_DESCRIPTION, "F_DESCRIPTION", SIZE_OF_LEGAL_AID_DESCRIPTION);
	new TTwinButton(this, IDC_LEGAL_AID_SPECIFIED_LAWER, IDRIGHT);
	lawerCount = new TLongFace(this, IDC_LEGAL_AID_LAWER_COUNT, "F_LAWER_COUNT", SIZE_OF_LAWER_COUNT);
}

bool TLegalAidDialog::IsValid()
{
	bool isValid = TGroupDialog::IsValid();

	if (isValid)
	{
		if (legalAidType->IsWindowEnabled() && !legalAidType->Group())
			isValid = error("Не е избран вид правна помощ.");
		else if (lawerCount->L() < LAWER_COUNT_MIN || lawerCount->L() > LAWER_COUNT_MAX)
			isValid = error("Брой адвокати трябва да бъде между %d и %d.", LAWER_COUNT_MIN, LAWER_COUNT_MAX);
		else
			isValid = laids->IsValid();
	}

	return isValid;
}

void TLegalAidDialog::SetupWindow()
{
	TGroupDialog::SetupWindow();

	mstr m;
	m.printf("Правна помощ - %s", SubpoenaKind->Seek(PrintSub()->subpoenaKind));
	SetCaption(str(m));

	if (PrintSub()->subpoenaKind == SUBPOENA_KIND_LEGAL_AID_TYPED)
		legalAidType->EnableWindow(true);
	else
		specialRepresentative->SetCheck(BF_CHECKED);
}

//void TLegalAidDialog::OKBNEnable(TCommandEnabler &tce)
//{
//	tce.Enable(!legalAidType->IsWindowEnabled() || legalAidType->Group());
//}

void TLegalAidDialog::OKBNClicked()
{
	if (IsValid())
	{
		TLegalAidType *tLegalAidType = (TLegalAidType *) legalAidType->Group();

		*Group() << this;
		PrintSub()->legalAidType = tLegalAidType ? tLegalAidType->legalAidType : 0;
		CmOk();
	}
}

void TLegalAidDialog::AddressBNEnable(TCommandEnabler &tce)
{
	tce.Enable(laids->Group());
}

void TLegalAidDialog::AddressBNClicked()
{
	laids->Address();
}

void TLegalAidDialog::LawerBNEnable(TCommandEnabler &tce)
{
	tce.Enable(PrintSub()->subpoenaKind == SUBPOENA_KIND_LEGAL_AID_TYPED && laids->Group());
}

void TLegalAidDialog::LawerBNClicked()
{
	laids->Lawer(true);
}

void TLegalAidDialog::RightBNClicked()
{
	laids->Lawer(false);
}

bool TLegalAidDialog::ListLVNItemChanging(TLwNotify &lwn)
{
	return laids->ItemChanging(lwn);
}
