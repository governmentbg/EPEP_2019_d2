DEFINE_RESPONSE_TABLE1(TEditPneDialog, TScaleDialog)
//{{TEditPneDialogRSP_TBL_BEGIN}}
	EV_BN_CLICKED(IDSEARCH, SearchBNClicked),
	EV_BN_CLICKED(IDOK, OKBNClicked),
	EV_EN_SETFOCUS(IDC_EDIT_PNE_KCQ_NAME, NameENSetFocus),
	EV_EN_KILLFOCUS(IDC_EDIT_PNE_KCQ_NAME, NameENKillFocus),
	EV_BN_CLICKED(IDADDRESS, AddressBNClicked),
//{{TEditPneDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TEditPneDialog Implementation}}

TEditPneDialog::TEditPneDialog(TWindow* parent, PNE *pne, bool edit, int tPneMode, const char *nmrnpr, int resId)
:
	TEisDialog(parent, pne, edit, nmrnpr, resId), pneMode(tPneMode)
{
	nmr = new TEisNmrFace(this, IDC_EDIT_PNE_NMR, "F_PNENMR", EISPP_PNE_KERNEL);
	nmr->SetStartHere(pneMode == PNE_NMR_ALLOWED);
	otdtip = new TEisAliasFace(this, IDC_EDIT_PNE_OTDTIP, "pneotdtip", nmk_pneotdtip, pne);
	dtaotdLabel = new TStatic(this, IDC_EDIT_PNE_DTAOTD_LABEL);
	dtaotd = new TDateFace(this, IDC_EDIT_PNE_DTAOTD, "F_DTAOTD", -1);
	dtadod = new TDateFace(this, IDC_EDIT_PNE_DTADOD, "F_DTADOD", -1);
	kcqLabel = new TStatic(this, IDC_EDIT_PNE_KCQ_LABEL);
	kcqName = new TStringFace(this, IDC_EDIT_PNE_KCQ_NAME, "F_NAME", SIZE_OF_EISPP_NAME);
	kcq = new TEisCrimeListBox(this, IDC_EDIT_PNE_KCQ, pne->pnesta->kcq);
	stpdvs = new TEisAliasFace(this, IDC_EDIT_PNE_STPDVS, "pnestpdvs", nmk_pnestpdvs, pne);
	sts = new TEisAliasFace(this, IDC_EDIT_PNE_STS, "pnests", nmk_pnests, pne);
	ok = new TPushButton(this, IDOK);
	search = new TPushButton(this, IDSEARCH);
}

bool TEditPneDialog::IsValid()
{
	bool isValid = TEisDialog::IsValid();

	if (isValid)
	{
		if (!*nmr->S() && pneMode == PNE_NMR_REQUIRED)
			isValid = error("Номера на престъпление е задължителен за дело което не започва в този съд.");
		else if (!(Pne()->adr->flags & XML_INCLUDED) && pneMode != PNE_NMR_REQUIRED)
		{
			isValid = error("Адреса е задължителен за дело, което започва в този съд, "
				"и при редактиране на престъпление.");
		}
		else if (otdtip->L() == pneotdper && dtadod->Date().Empty())
			isValid = error("Крайната дата е задължителна при тип дата = начална дата на извършване.");
		else if (!dtadod->Date().Empty() && dtadod->Date() < dtaotd->Date())
			isValid = error("Крайната дата е преди началната.");
	}

	return isValid;
}

void TEditPneDialog::SetupWindow()
{
	TEisDialog::SetupWindow();

	TXMLAttrib *attrib = XMLGroup()->FindAttrib("pnekcq");

	if (!(attrib->flags & XML_EDITABLE))
	{
		kcqName->EnableWindow(false);
		kcq->EnableWindow(false);
	}

	if (pneMode == PNE_NMR_BLOCKED)
		nmr->EnableWindow(false);
	// 2015:071
	if (!Pne()->pnesta->sts && sts->IsWindowEnabled())
		sts->SetL(sts_prestaplenie);
}

void TEditPneDialog::OKBNClicked()
{
	ADR *adr = Pne()->adr;

	if (adr->drj)
		adr->flags |= XML_INCLUDED;
	else
		adr->flags &= ~XML_INCLUDED;

	if (!kcq->Crime())	// 2012:339 IRQ/FIX: must be before kcq assign
		error("Квалификацията на престъпление е задължителна.");
	else
	{
		Pne()->pnesta->kcq = kcq->Crime()->code;
		TEisDialog::OKBNClicked();
	}
}

void TEditPneDialog::SearchBNClicked()
{
	kcq->Search(kcqName->S(), false);
}

void TEditPneDialog::NameENSetFocus()
{
	SetDefaultId(search->GetId());
	search->SetStyle(search->GetStyle() | BS_DEFPUSHBUTTON, true);
	ok->SetStyle(ok->GetStyle() & ~BS_DEFPUSHBUTTON, true);
}

void TEditPneDialog::NameENKillFocus()
{
	SetDefaultId(ok->GetId());
	search->SetStyle(search->GetStyle() & ~BS_DEFPUSHBUTTON, true);
	ok->SetStyle(ok->GetStyle() | BS_DEFPUSHBUTTON, true);
}

void TEditPneDialog::AddressBNClicked()
{
	TEditAdrDialog(this, Pne(), nmrnpr).Execute();
}
