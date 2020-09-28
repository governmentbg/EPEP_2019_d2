DEFINE_RESPONSE_TABLE1(TFloatOutRegDialog, TFloatSubjectDialog)
//{{TFloatOutRegDialogRSP_TBL_BEGIN}}
	EV_EN_UPDATE(IDC_FLOAT_OUTREG_MIN_RETURNED, ReturnedENUpdate),
	EV_EN_UPDATE(IDC_FLOAT_OUTREG_MAX_RETURNED, ReturnedENUpdate),
//{{TFloatOutRegDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TFloatOutRegDialog Implementation}}

TFloatOutRegDialog::TFloatOutRegDialog(TWindow* parent, TFloatOutRegGroup *group)
:
	TFloatSubjectDialog(parent, group, IDD_FLOAT_OUTREG)
{
	lawKindsRT = new TLawKindsRTFace(this, IDC_FLOAT_OUTREG_LAW_TYPES, "F_LAW_KINDS_RT");
	inRegKinds = new TCharListFace(this, IDC_FLOAT_OUTREG_IN_REG_KINDS, "F_IN_REG_KINDS", Kind, KIND_IN_RESOLUTABLES,
		SIZE_OF_KINDS);
	minDate = new TDateFace(this, IDC_FLOAT_OUTREG_MIN_DATE, "F_MIN_DATE", false);
	maxDate = new TDateFace(this, IDC_FLOAT_OUTREG_MAX_DATE, "F_MAX_DATE", false);
	minReturned = new TDateFace(this, IDC_FLOAT_OUTREG_MIN_RETURNED, "F_MIN_RETURNED", false);
	maxReturned = new TDateFace(this, IDC_FLOAT_OUTREG_MAX_RETURNED, "F_MAX_RETURNED", false);
	receivers = new TSenderListBox(this, IDC_FLOAT_OUTREG_RECEIVERS, C2S[UCN_INSTANCE_HIGHER]);
	judges = new TUserListBox(this, IDC_FLOAT_OUTREG_JUDGES, POST_VISIBLE_JUDGES);
	resultsIII = new TCharListFace(this, IDC_FLOAT_OUTREG_RESULTS_III, "F_RESULTS_III", AppealResolution, NULL,
		SIZE_OF_RESOLUTIONS);
	resultsKS = new TCharListFace(this, IDC_FLOAT_OUTREG_RESULTS_KS, "F_RESULTS_KS", AppealResolution, NULL,
		SIZE_OF_RESOLUTIONS);
	vssIndexes = new TVSSIndexListBox(this, IDC_FLOAT_OUTREG_VSS_INDEXES);
	decKinds = new TCharListFace(this, IDC_FLOAT_OUTREG_DEC_KINDS, "F_DEC_KINDS", Kind, KIND_VISIBLE_ENDOCS, SIZE_OF_KINDS);
	new TRadioFace(this, IDC_FLOAT_OUTREG_BY_DTYN, "F_BY_DTYN");
	new TRadioFace(this, IDC_FLOAT_OUTREG_BY_RTYN, "F_BY_RTYN");
	text = new TStringFace(this, IDC_FLOAT_OUTREG_TEXT, "F_TEXT", SIZE_OF_ORIGIN_TEXT);
	compositions = new TCharAutoListFace(this, IDC_FLOAT_OUTREG_COMPOSITIONS, "F_COMPOSITIONS", CharComposition,
		QUERY_COMPOSITIONS, SIZE_OF_COMPOSITIONS);
	new TCheckFace(this, IDC_FLOAT_OUTREG_FINAL_DECS, "F_FINAL_DECS");
}

bool TFloatOutRegDialog::IsValid()
{
	bool isValid = TFloatSubjectDialog::IsValid();

	if (isValid)
	{
		if (invalid_range(minDate, maxDate))
			isValid = error("Началната дата на изпращане е по-голяма от крайната.");
		else if (invalid_range(minReturned, maxReturned))
			isValid = error("Началната дата на връщане е по-голяма от крайната.");
	}

	return isValid;
}

void TFloatOutRegDialog::Action(bool execute)
{
	msql m;

	TFloatSubjectDialog::GetSubjects(FloatOutRegGroup()->newSubjects, FloatOutRegGroup()->oldSubjects);
	receivers->GetMark(&FloatOutRegGroup()->receivers);
	judges->GetMark(&FloatOutRegGroup()->judges);
	vssIndexes->GetMark(&FloatOutRegGroup()->vssIndexes);
	FloatOutRegGroup()->FormCriteria(m);

	if (execute)
		query_outreg(this, str(m));
	else
		Message(m);
}

void TFloatOutRegDialog::ReturnedENUpdate()
{
	if (date_valid(minReturned->S(), false) && date_valid(maxReturned->S(), false))
		vssIndexes->SetCrit(minReturned->Date(), maxReturned->Date());
}
