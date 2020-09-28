//{{TPrepareNIAProtocolDialog Implementation}}

DEFINE_RESPONSE_TABLE1(TPrepareNIAProtocolDialog, TFloatDialog)
//{{TPrepareNIAProtocolDialogRSP_TBL_BEGIN}}
	EV_BN_CLICKED(IDC_PREPARE_NIA_PROTOCOL_BY_EXEC_DATE, ByBNClicked),
	EV_BN_CLICKED(IDC_PREPARE_NIA_PROTOCOL_BY_OUTREG_DATE, ByBNClicked),
	EV_BN_CLICKED(IDOK, OKBNClicked),
//{{TPrepareNIAProtocolDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

TPrepareNIAProtocolDialog::TPrepareNIAProtocolDialog(TWindow* parent, TPrepareNIAProtocolGroup *group, int resId)
:
	TFloatDialog(parent, group, resId)
{
	niaTDep = new TNIATDepAliasFace(this, IDC_PREPARE_NIA_PROTOCOL_NIA_TDEP, "F_NIA_TDEP");
	typesLabel = new TStatic(this, IDC_PREPARE_NIA_PROTOCOL_TYPES_LABEL);
	types = new TCharListFace(this, NIA_PROT_NUMBERING ? IDC_PREPARE_NIA_PROTOCOL_TYPE : IDC_PREPARE_NIA_PROTOCOL_TYPES,
		"F_TYPES", Type, Default->colleges, SIZE_OF_TYPES);
	byExecDate = new TRadioFace(this, IDC_PREPARE_NIA_PROTOCOL_BY_EXEC_DATE, "F_BY_EXEC_DATE");
	byOutRegDate = new TRadioFace(this, IDC_PREPARE_NIA_PROTOCOL_BY_OUTREG_DATE, "F_BY_OUTREG_DATE");
	minExecDate = new TDateFace(this, IDC_PREPARE_NIA_PROTOCOL_MIN_EXEC_DATE, "F_MIN_EXEC_DATE", false);
	minExecDateUpDn = new TDateFaceUpDown(this, IDC_PREPARE_NIA_PROTOCOL_MIN_EXEC_DATE_UPDN, minExecDate);
	maxExecDate = new TDateFace(this, IDC_PREPARE_NIA_PROTOCOL_MAX_EXEC_DATE, "F_MAX_EXEC_DATE", false);
	maxExecDateUpDn = new TDateFaceUpDown(this, IDC_PREPARE_NIA_PROTOCOL_MAX_EXEC_DATE_UPDN, maxExecDate);
	minOutRegDate = new TDateFace(this, IDC_PREPARE_NIA_PROTOCOL_MIN_OUTREG_DATE, "F_MIN_OUTREG_DATE", false);
	minOutRegDateUpDn = new TDateFaceUpDown(this, IDC_PREPARE_NIA_PROTOCOL_MIN_OUTREG_DATE_UPDN, minOutRegDate);
	maxOutRegDate = new TDateFace(this, IDC_PREPARE_NIA_PROTOCOL_MAX_OUTREG_DATE, "F_MAX_OUTREG_DATE", false);
	maxOutRegDateUpDn = new TDateFaceUpDown(this, IDC_PREPARE_NIA_PROTOCOL_MAX_OUTREG_DATE_UPDN, maxOutRegDate);
}

bool TPrepareNIAProtocolDialog::IsValid()
{
	bool isValid = TFloatDialog::IsValid();

	if (isValid)
	{
		if (!*types->S())
			isValid = error("Не е избран %sвид дело.", NIA_PROT_NUMBERING ? "" : "нито един ");
		else if (byExecDate->GetCheck() == BF_CHECKED && minExecDate->Date().Empty())
			isValid = error("Началната дата на издаване на изпълнителен лист е задължителна.");
		else if (byExecDate->GetCheck() == BF_CHECKED && invalid_range(minExecDate, maxExecDate))
			isValid = error("Началната дата на издаване на изпълнителен лист е по-голяма от крайната.");
		else if (byOutRegDate->GetCheck() == BF_CHECKED && minOutRegDate->Date().Empty())
			isValid = error("Началната дата на изходящ документ на придружително писмо е задължителна.");
		else if (byOutRegDate->GetCheck() == BF_CHECKED && invalid_range(minOutRegDate, maxOutRegDate))
			isValid = error("Началната дата на изходящ документ на придружително писмо е по-голяма от крайната.");
	}

	return isValid;
}

void TPrepareNIAProtocolDialog::SetupWindow()
{
	TFloatDialog::SetupWindow();

	niaTDep->SelectElement(NULL, '\0');
	types->EnableWindow(true);
	types->ShowWindow(SW_SHOW);
	ByBNClicked();

	if (NIA_PROT_NUMBERING)
		typesLabel->SetCaption("&Вид дело");
}

void TPrepareNIAProtocolDialog::ByBNClicked()
{
	bool tByExecDate = byExecDate->GetCheck() == BF_CHECKED;
	bool tByOutRegDate = byOutRegDate->GetCheck() == BF_CHECKED;

	minExecDate->EnableWindow(tByExecDate);
	minExecDateUpDn->EnableWindow(tByExecDate);
	maxExecDate->EnableWindow(tByExecDate);
	maxExecDateUpDn->EnableWindow(tByExecDate);
	minOutRegDate->EnableWindow(tByOutRegDate);
	minOutRegDateUpDn->EnableWindow(tByOutRegDate);
	maxOutRegDate->EnableWindow(tByOutRegDate);
	maxOutRegDateUpDn->EnableWindow(tByOutRegDate);
}

void TPrepareNIAProtocolDialog::OKBNClicked()
{
	if (IsValid())
	{
		*Group() << this;

		if (nia_protocol(this, PrepareGroup()))
			CmOk();
	}
}
