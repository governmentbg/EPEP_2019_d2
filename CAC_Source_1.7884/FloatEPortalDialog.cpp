DEFINE_RESPONSE_TABLE1(TFloatEPortalDialog, TFloatDialog)
//{{TFloatEPortalDialogRSP_TBL_BEGIN}}
	EV_BN_CLICKED(IDC_FLOAT_EPORTAL_LOG, InputBNClicked),
	EV_BN_CLICKED(IDC_FLOAT_EPORTAL_GUID, InputBNClicked),
//{{TFloatEPortalDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TFloatEPortalDialog Implementation}}

TFloatEPortalDialog::TFloatEPortalDialog(TWindow* parent, TFloatEPortalGroup *group)
:
	TFloatDialog(parent, group, IDD_FLOAT_EPORTAL)
{
	log = new TRadioFace(this, IDC_FLOAT_EPORTAL_LOG, "F_LOG");
	guid = new TRadioFace(this, IDC_FLOAT_EPORTAL_GUID, "F_GUID");
	logDateLabel = new TStatic(this, IDC_FLOAT_EPORTAL_LOG_DATE_LABEL);
	minLogDate = new TDateFace(this, IDC_FLOAT_EPORTAL_MIN_LOG_DATE, "F_MIN_LOG_DATE", false);
	maxLogDate = new TDateFace(this, IDC_FLOAT_EPORTAL_MAX_LOG_DATE, "F_MAX_LOG_DATE", false);
	minGuidDate = new TDateFace(this, IDC_FLOAT_EPORTAL_MIN_GUID_DATE, "F_MIN_GUID_DATE", false);
	maxGuidDate = new TDateFace(this, IDC_FLOAT_EPORTAL_MAX_GUID_DATE, "F_MAX_GUID_DATE", false);
	whats = new TWhatListBox(this, IDC_FLOAT_EPORTAL_WHATS);
	types = new TEPorTypeListFace(this, IDC_FLOAT_EPORTAL_TYPES, "F_TYPES");
	minNo = new TLongFace(this, IDC_FLOAT_EPORTAL_MIN_NO, "F_MIN_NO", SIZE_OF_NO);
	maxNo = new TLongFace(this, IDC_FLOAT_EPORTAL_MAX_NO, "F_MAX_NO", SIZE_OF_NO);
	minYear = new TYearFace(this, IDC_FLOAT_EPORTAL_MIN_YEAR, "F_MIN_YEAR");
	maxYear = new TYearFace(this, IDC_FLOAT_EPORTAL_MAX_YEAR, "F_MAX_YEAR");
	new TRadioFace(this, IDC_FLOAT_EPORTAL_ALL_RECORDS, "F_ALL_RECORDS");
	new TRadioFace(this, IDC_FLOAT_EPORTAL_SUM_ORIGINS, "F_SUM_ORIGINS");
	new TRadioFace(this, IDC_FLOAT_EPORTAL_TOTAL_WHATS, "F_TOTAL_WHATS");
}

bool TFloatEPortalDialog::IsValid()
{
	bool isValid = TFloatDialog::IsValid();

	if (isValid)
	{
		if (invalid_range(minLogDate, maxLogDate))
			isValid = error("Началната дата на изпращане е по-голяма от крайната.");
		else if (guid->GetCheck() == BF_CHECKED && invalid_range(minGuidDate, maxGuidDate))
			isValid = error("Началната дата на обект е по-голяма от крайната.");
		else if (invalid_range(minNo, maxNo))
			isValid = error("Началния номер е по-голям от крайния.");
		else if (invalid_range(minYear, maxYear))
			isValid = error("Началната година е по-голяма от крайната.");
	}

	return isValid;
}

void TFloatEPortalDialog::Action(bool execute)
{
	msql m;

	whats->GetMark(&FloatEPortalGroup()->whats);
	FloatEPortalGroup()->FormCriteria(m);

	if (execute)
		query_eportal(this, str(m), FloatEPortalGroup());
	else
		Message(m);
}

void TFloatEPortalDialog::SetupWindow()
{
	TFloatDialog::SetupWindow();

	InputBNClicked();
}

void TFloatEPortalDialog::InputBNClicked()
{
	bool tLog = log->GetCheck() == BF_CHECKED;

	logDateLabel->SetCaption(tLog ? "Дата на &изпращане" : "Дата на рег&истрация");
	minGuidDate->EnableWindow(!tLog);
	maxGuidDate->EnableWindow(!tLog);
	whats->SetLog(tLog);
}
