DEFINE_RESPONSE_TABLE1(TFloatExecListDialog, TFloatDialog)
//{{TFloatExecListDialogRSP_TBL_BEGIN}}
	EV_BN_CLICKED(IDC_FLOAT_EXEC_LIST_EXEC_LIST_ALLS, ExecListKindBNClicked),
	EV_BN_CLICKED(IDC_FLOAT_EXEC_LIST_EXEC_LIST_JUDICIALS, ExecListKindBNClicked),
	EV_BN_CLICKED(IDC_FLOAT_EXEC_LIST_EXEC_LIST_COUNTRY, ExecListKindBNClicked),
	EV_BN_CLICKED(IDC_FLOAT_EXEC_LIST_EXEC_LIST_PRIVATE, ExecListKindBNClicked),
//{{TFloatExecListDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TFloatExecListDialog Implementation}}

TFloatExecListDialog::TFloatExecListDialog(TWindow* parent, TFloatExecListGroup *group, int resId)
:
	TFloatDialog(parent, group, resId)
{
	lawKinds = new TCharAutoListFace(this, IDC_FLOAT_EXEC_LIST_LAW_KINDS, "F_LAW_KINDS", Kind, Default->xKinds,
		SIZE_OF_KINDS);
	minNo = new TLongFace(this, IDC_FLOAT_EXEC_LIST_MIN_NO, "F_MIN_NO", SIZE_OF_NO);
	maxNo = new TLongFace(this, IDC_FLOAT_EXEC_LIST_MAX_NO, "F_MAX_NO", SIZE_OF_NO);
	minYear = new TYearFace(this, IDC_FLOAT_EXEC_LIST_MIN_YEAR, "F_MIN_YEAR");
	maxYear = new TYearFace(this, IDC_FLOAT_EXEC_LIST_MAX_YEAR, "F_MAX_YEAR");
	compositions = new TCharAutoListFace(this, IDC_FLOAT_EXEC_LIST_COMPOSITIONS, "F_COMPOSITIONS", CharComposition,
		QUERY_COMPOSITIONS, SIZE_OF_COMPOSITIONS);
	execListAlls = new TRadioFace(this, IDC_FLOAT_EXEC_LIST_EXEC_LIST_ALLS, "F_EXEC_LIST_ALLS");
	execListJudicials = new TRadioFace(this, IDC_FLOAT_EXEC_LIST_EXEC_LIST_JUDICIALS, "F_EXEC_LIST_JUDICIALS");
	execListCountry = new TRadioFace(this, IDC_FLOAT_EXEC_LIST_EXEC_LIST_COUNTRY, "F_EXEC_LIST_COUNTRY");
	execListPrivate = new TRadioFace(this, IDC_FLOAT_EXEC_LIST_EXEC_LIST_PRIVATE, "F_EXEC_LIST_PRIVATE");
	minExecListNo = new TLongFace(this, IDC_FLOAT_EXEC_LIST_MIN_EXEC_LIST_NO, "F_MIN_EXEC_LIST_NO", SIZE_OF_NO);
	maxExecListNo = new TLongFace(this, IDC_FLOAT_EXEC_LIST_MAX_EXEC_LIST_NO, "F_MAX_EXEC_LIST_NO", SIZE_OF_NO);
	minExecListDate = new TDateFace(this, IDC_FLOAT_EXEC_LIST_MIN_EXEC_LIST_DATE, "F_MIN_EXEC_LIST_DATE", false);
	maxExecListDate = new TDateFace(this, IDC_FLOAT_EXEC_LIST_MAX_EXEC_LIST_DATE, "F_MAX_EXEC_LIST_DATE", false);
	sumTypes = new TCharListFace(this, IDC_FLOAT_EXEC_LIST_SUM_TYPES, "F_SUM_TYPES", SumType, SUM_TYPE_EXEC_LISTS,
		SIZE_OF_SUM_TYPES);
	senders = new TAccountListBox(this, IDC_FLOAT_EXEC_LIST_SENDERS);
	minCollectDate = new TDateFace(this, IDC_FLOAT_EXEC_LIST_MIN_COLLECT_DATE, "F_MIN_COLLECT_DATE", false);
	maxCollectDate = new TDateFace(this, IDC_FLOAT_EXEC_LIST_MAX_COLLECT_DATE, "F_MAX_COLLECT_DATE", false);
	voluntary = new TCheckFace(this, IDC_FLOAT_EXEC_LIST_VOLUNTARY, "F_VOLUNTARY");
	receivers = new TSenderListBox(this, IDC_FLOAT_EXEC_LIST_RECEIVERS, UCN_EXEC_RECEIVERS);
	minSendDate = new TDateFace(this, IDC_FLOAT_EXEC_LIST_MIN_SEND_DATE, "F_MIN_SEND_DATE", false);
	maxSendDate = new TDateFace(this, IDC_FLOAT_EXEC_LIST_MAX_SEND_DATE, "F_MAX_SEND_DATE", false);
	new TCheckFace(this, IDC_FLOAT_EXEC_LIST_LETTER_OUT_DATE, "F_LETTER_OUT_DATE"); 
}

bool TFloatExecListDialog::IsValid()
{
	bool isValid = TFloatDialog::IsValid();

	if (isValid)
	{
		if (invalid_range(minNo, maxNo))
			isValid = error("Началния номер на дело е по-голям от крайния.");
		else if (invalid_range(minYear, maxYear))
			isValid = error("Началната година е по-голяма от крайната.");
		else if (invalid_range(minExecListNo, maxExecListNo))
			isValid = error("Началния номер на изпълнителен лист е по-голям от крайния.");
		else if (invalid_range(minExecListDate, maxExecListDate))
			isValid = error("Началната дата на изпълнителен лист е по-голяма от крайната.");
		else if (minCollectDate->IsWindowEnabled() && invalid_range(minCollectDate, maxCollectDate))
			isValid = error("Началната дата на внасяне на суми е по-голяма от крайната.");
	}

	return isValid;
}

void TFloatExecListDialog::Action(bool execute)
{
	msql m;

	senders->GetMark(&FloatExecListGroup()->senders);
	receivers->GetMark(&FloatExecListGroup()->receivers);
	FloatExecListGroup()->FormCriteria(m);

	if (execute)
		float_exec_list(this, str(m), FloatExecListGroup());
	else
		Message(m);
}

void TFloatExecListDialog::ExecListKindBNClicked()
{
	bool country = execListPrivate->GetCheck() != BF_CHECKED;

	senders->EnableWindow(country);
	minCollectDate->EnableWindow(country);
	maxCollectDate->EnableWindow(country);
}
