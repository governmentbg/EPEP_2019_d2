DEFINE_RESPONSE_TABLE1(TFloatSubpoenaDialog, TFloatDialog)
//{{TFloatSubpoenaDialogRSP_TBL_BEGIN}}
	EV_BN_CLICKED(IDC_FLOAT_SUBPOENA_FIRM_DATA, FirmLawsuitBNClicked),
	EV_LBN_SELCHANGE(IDC_FLOAT_SUBPOENA_TYPES, TypesSELChange),
//{{TFloatSubpoenaDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TFloatSubpoenaDialog Implementation}}

TFloatSubpoenaDialog::TFloatSubpoenaDialog(TWindow* parent, TFloatSubpoenaGroup *group, int resId)
:
	TFloatDialog(parent, group, resId)
{
	types = new TTypeListFace(this, IDC_FLOAT_SUBPOENA_TYPES, "F_TYPES", TYPE_SUBPOENABLES, false);
	lawKinds = new TCharAutoListFace(this, IDC_FLOAT_SUBPOENA_LAW_KINDS, "F_LAW_KINDS", Kind, Default->xKinds,
		SIZE_OF_LAW_KINDS);
	minNo = new TLongFace(this, IDC_FLOAT_SUBPOENA_MIN_NO, "F_MIN_NO", SIZE_OF_NO);
	maxNo = new TLongFace(this, IDC_FLOAT_SUBPOENA_MAX_NO, "F_MAX_NO", SIZE_OF_NO);
	minYear = new TYearFace(this, IDC_FLOAT_SUBPOENA_MIN_YEAR, "F_MIN_YEAR");
	maxYear = new TYearFace(this, IDC_FLOAT_SUBPOENA_MAX_YEAR, "F_MAX_YEAR");
	kinds = new TCharListFace(this, IDC_FLOAT_SUBPOENA_KINDS, "F_KINDS", Kind, KIND_VISIBLE_SUBPOENABLES, SIZE_OF_KINDS);
	minDate = new TDateFace(this, IDC_FLOAT_SUBPOENA_MIN_SESSION_DATE, "F_MIN_SESSION_DATE", false);
	maxDate = new TDateFace(this, IDC_FLOAT_SUBPOENA_MAX_SESSION_DATE, "F_MAX_SESSION_DATE", false);
	minSubpoenaDate = new TDateFace(this, IDC_FLOAT_SUBPOENA_MIN_SUBPOENA_DATE, "F_MIN_SUBPOENA_DATE", false);
	maxSubpoenaDate = new TDateFace(this, IDC_FLOAT_SUBPOENA_MAX_SUBPOENA_DATE, "F_MAX_SUBPOENA_DATE", false);
	subpoenaKinds = new TCharListNameFace(this, IDC_FLOAT_SUBPOENA_SUBPOENA_KINDS, "F_SUBPOENA_KINDS", SubpoenaKind,
		SUBPOENA_KIND_VISIBLES, SIZE_OF_SUBPOENA_KINDS);
	receivedStatuses = new TCharListFace(this, IDC_FLOAT_SUBPOENA_RECEIVED_STATUSES, "F_RECEIVED_STATUSES", ReceivedStatus,
		SRS_SUBPOENAS, SIZE_OF_RECEIVED_STATUSES);
	deliverers = new TUserListBox(this, IDC_FLOAT_SUBPOENA_DELIVERERS, C2S[POST_DELIVERER], USER_EMPTY);
	firmLawsuit = new TCheckFace(this, IDC_FLOAT_SUBPOENA_FIRM_DATA, "F_FIRM_LAWSUIT");
	minReturnedDate = new TDateFace(this, IDC_FLOAT_SUBPOENA_MIN_RETURNED_DATE, "F_MIN_RETURNED_DATE", false);
	maxReturnedDate = new TDateFace(this, IDC_FLOAT_SUBPOENA_MAX_RETURNED_DATE, "F_MAX_RETURNED_DATE", false);
	add_postal_buttons(this);
	new TCheckFace(this, IDC_FLOAT_SUBPOENA_MULTY, "F_MULTY");
	new TCheckFace(this, IDC_FLOAT_SUBPOENA_LISTSIDE, "F_NAME_LIST_WIND");
	new TRadioFace(this, IDC_FLOAT_SUBPOENA_BY_DTYN, "F_BY_DTYN");
	new TRadioFace(this, IDC_FLOAT_SUBPOENA_BY_SDTYN, "F_BY_SDTYN");
	new TRadioFace(this, IDC_FLOAT_SUBPOENA_BY_NMTYN, "F_BY_NMTYN");
	new TRadioFace(this, IDC_FLOAT_SUBPOENA_BY_TYN, "F_BY_TYN");
	users = new TUserListBox(this, IDC_FLOAT_SUBPOENA_USERS, POST_PERSONELS);
	new TCheckFace(this, IDC_FLOAT_SUBPOENA_NOLETTER, "F_NOLETTER");
}

bool TFloatSubpoenaDialog::IsValid()
{
	bool isValid = TFloatDialog::IsValid();

	if (isValid)
	{
		if (invalid_range(minNo, maxNo))
			isValid = error("Началния номер дело е по-голям от крайния.");
		else if (invalid_range(minYear, maxYear))
			isValid = error("Началната година е по-голяма от крайната.");
		else if (invalid_range(minDate, maxDate))
			isValid = error("Началната дата на заседания/разпореждане по-голяма от крайната.");
		else if (invalid_range(minSubpoenaDate, maxSubpoenaDate))
			isValid = error("Началната дата на призовка/съобщения е по-голяма от крайната.");
		else if (invalid_range(minReturnedDate, maxReturnedDate))
			isValid = error("Началната дата на връщане е по-голяма от крайната.");
	}

	return isValid;
}

void TFloatSubpoenaDialog::Action(bool execute)
{
	msql m;
	TFloatSubpoenaGroup *group = (TFloatSubpoenaGroup *) Group();

	deliverers->GetMark(&group->deliverers);
	users->GetMark(&group->users);
	group->FormCriteria(m);

	if (execute)
		query_subpoena(this, str(m), group);
	else
	{
		msql w;

		if (group->Announces())
		{
			load_announces_criteria(w, group, group->deliverers);
			m.printf("\n\n%s", str(w));
		}

		Message(m);
	}
}

void TFloatSubpoenaDialog::SetupWindow()
{
	TFloatDialog::SetupWindow();

#if COURT_TYPE == COURT_AREAL
	firmLawsuit->EnableWindow(true);
	firmLawsuit->ShowWindow(SW_SHOW);
#endif  // AREAL
}

void TFloatSubpoenaDialog::FirmLawsuitBNClicked()
{
#if COURT_TYPE == COURT_AREAL
	if (firmLawsuit->GetCheck() == BF_CHECKED)
		types->SetS(TYPE_FIRM_LAWS);
	else
		types->SetS("");
#endif  // AREAL
}

void TFloatSubpoenaDialog::TypesSELChange()
{
#if COURT_TYPE == COURT_AREAL
	if (strcmp(types->S(), TYPE_FIRM_LAWS))
		firmLawsuit->SetCheck(BF_UNCHECKED);
	else
		firmLawsuit->SetCheck(BF_CHECKED);
#endif  // AREAL
}
