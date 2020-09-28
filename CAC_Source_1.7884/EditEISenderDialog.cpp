DEFINE_RESPONSE_TABLE1(TEditEISenderDialog, TScaleDialog)
//{{TEditEISenderDialogRSP_TBL_BEGIN}}
	EV_BN_CLICKED(IDOK, OKBNClicked),
	EV_EN_KILLFOCUS(IDC_EDIT_EISENDER_EISPP, EisppKillFocus),
//{{TEditEISenderDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TEditEISenderDialog Implementation}}

TEditEISenderDialog::TEditEISenderDialog(TWindow* parent, TSender *sender, bool edit, bool full, int resId)
:
	TEditSenderDialog(parent, sender, edit, full, resId)
{
	eispp = new TLongFace(this, IDC_EDIT_EISENDER_EISPP, "F_EISPP", SIZE_OF_EISPP_DEPART);
	depart = new TStringFace(this, IDC_EDIT_EISENDER_DEPART, "F_DEPART", SIZE_OF_EISPP_NAME);
}

bool TEditEISenderDialog::IsValid()
{
	bool isValid = TEditSenderDialog::IsValid();

	if (isValid)
	{
		EisDepart->code = eispp->L();

		if (EisDepart->code && !EisDepart->Try())
			isValid = error("В номенклатурата на ЕИСПП липсва елемент с посочения код.");
		else if (EisDepart->code && !eispp_ucn_match(Sender()->ucnType, EisDepart->nom))
			isValid = error("Посоченият ЕИСПП код не отговаря на вида адресант.");
	}

	return isValid;
}

void TEditEISenderDialog::SetupWindow()
{
	TEditSenderDialog::SetupWindow();
	EisppKillFocus();
	eispp->EnableWindow(edit);
}

void TEditEISenderDialog::OKBNClicked()
{
	EisppKillFocus();
	TEditSenderDialog::OKBNClicked();
}

void TEditEISenderDialog::EisppKillFocus()
{
	EisDepart->code = eispp->L();
	depart->SetS(EisDepart->code && EisDepart->Try() ? EisDepart->name : "");
}
