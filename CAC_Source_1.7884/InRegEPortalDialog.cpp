DEFINE_RESPONSE_TABLE1(TInRegEPortalDialog, TEditIncomingDialog)
//{{TInRegEPortalDialogRSP_TBL_BEGIN}}
	EV_BN_CLICKED(IDOK, OKBNClicked),
	EV_BN_CLICKED(IDC_INREG_EPORTAL_EDIT_SIDE, EditSideBNClicked),
	EV_COMMAND_ENABLE(IDC_INREG_EPORTAL_EDIT_SIDE, EditSideBNEnable),
	EV_BN_CLICKED(IDC_INREG_EPORTAL_REGISTER, EPortalModeBNClicked),
	EV_BN_CLICKED(IDC_INREG_EPORTAL_GRANT_ACCESS, EPortalModeBNClicked),
	EV_BN_CLICKED(IDC_INREG_EPORTAL_REVOKE_ACCESS, EPortalModeBNClicked),
//{{TInRegEPortalDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TInRegEPortalDialog Implementation}}

TInRegEPortalDialog::TInRegEPortalDialog(TWindow* parent, TInReg *inReg, bool edit, bool tHasRegistration, bool tHasSides,
	TSideWind &tSideWind)
:
	TEditIncomingDialog(parent, inReg, C2S[KIND_EPORTAL_REQUEST], C2S[COLLEGE_COMMON_ADMIN], edit, IDD_INREG_EPORTAL),
		hasRegistration(tHasRegistration), hasSides(tHasSides), sideWind(tSideWind)
{
	text = new TStringFace(this, IDC_INREG_EPORTAL_TEXT, "F_TEXT", SIZE_OF_ORIGIN_TEXT);
	editSide = new TButton(this, IDC_INREG_EPORTAL_EDIT_SIDE);
	eportalMail = new TStringFace(this, IDC_INREG_EPORTAL_EPORTAL_MAIL, "F_EPORTAL_MAIL", SIZE_OF_EMAIL);
	registration = new TRadioFace(this, IDC_INREG_EPORTAL_REGISTER, "F_EPORTAL_REGISTER");
	grantAccess = new TRadioFace(this, IDC_INREG_EPORTAL_GRANT_ACCESS, "F_EPORTAL_GRANT");
	revokeAccess = new TRadioFace(this, IDC_INREG_EPORTAL_REVOKE_ACCESS, "F_EPORTAL_REVOKE");
	sideWindNames = new TStatic(this, IDC_INREG_EPORTAL_SIDE_NAMES);
}

void TInRegEPortalDialog::SideNamesChanged()
{
	mstr m;

	sideWind.PrintNames(m);
	sideWindNames->SetCaption(str(m));
}

bool TInRegEPortalDialog::IsValid()
{
	bool isValid = TEditIncomingDialog::IsValid();

	if (isValid)
	{
		if (registration->GetCheck() == BF_CHECKED)
		{
			const char *email = eportalMail->S();

			if (!any(email))
				isValid = error("Електронната поща е задължителна.");
			else if (already_used(email))
				isValid = error("Електронната поща е дублирана.");
		}
	}

	return isValid;
}

void TInRegEPortalDialog::SetupWindow()
{
	TEditIncomingDialog::SetupWindow();

	if (hasRegistration)
	{
		registration->EnableWindow(false);
		//eportalMail->EnableWindow(false);  // 2018-10-25: EPortalModeBNClicked()
	}

	if (edit || !hasSides)
	{
		registration->EnableWindow(false);
		grantAccess->EnableWindow(false);
		revokeAccess->EnableWindow(false);
	}

	EPortalModeBNClicked();
	SideNamesChanged();
	editSide->SetCaption(strchr(INVOLVEMENT_LAWERS, sideWind.involvement) ? "Ад&вокат" : "Зая&вител");
}

void TInRegEPortalDialog::EditSideBNClicked()
{
	// 2018-10-25: LPR: no involvemens change, breaks initial checks
	if (!can_rename() || edit_ucn(this, &sideWind, InReg(), true, true))
	{
		SideNamesChanged();
		edit_addresses(this, &sideWind, ADDRESS_ALLOW_FOREIGN);
	}
}

void TInRegEPortalDialog::EditSideBNEnable(TCommandEnabler &tce)
{
	tce.Enable(edit);
}

void TInRegEPortalDialog::EPortalModeBNClicked()
{
	if (registration->GetCheck() == BF_CHECKED)
		eportalMail->EnableWindow(true);
	else
	{
		eportalMail->EnableWindow(false);
		eportalMail->SetS("");
	}
}
