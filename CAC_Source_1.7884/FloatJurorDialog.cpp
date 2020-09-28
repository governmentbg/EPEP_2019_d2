DEFINE_RESPONSE_TABLE1(TFloatJurorDialog, TFloatDialog)
//{{TFloatJurorDialogRSP_TBL_BEGIN}}
	EV_BN_CLICKED(IDC_FLOAT_JUROR_LOAD, LoadBNClicked),
	EV_BN_CLICKED(IDC_FLOAT_JUROR_LOAD_FLAT, LoadBNClicked),
	EV_BN_CLICKED(IDC_FLOAT_JUROR_REDISTRIB, LoadBNClicked),
	EV_BN_CLICKED(IDC_FLOAT_JUROR_FLAT_EXTERNAL, ExternalBNClicked),
//{{TFloatJurorDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TFloatJurorDialog Implementation}}

TFloatJurorDialog::TFloatJurorDialog(TWindow* parent, TFloatJurorGroup *group, int resId)
:
	TFloatDialog(parent, group, resId)
{
	load = new TRadioFace(this, IDC_FLOAT_JUROR_LOAD, "F_LOAD");
	loadFlat = COURT_FLAT ? new TRadioFace(this, IDC_FLOAT_JUROR_LOAD_FLAT, "F_LOAD_FLAT") : NULL;	// 2015:140
	redistrib = new TRadioFace(this, IDC_FLOAT_JUROR_REDISTRIB, "F_REDISTRIB");
	manualRed = new TCheckFace(this, IDC_FLOAT_JUROR_MANUAL_RED, "F_MANUAL_RED");
	lawKinds = new TCharAutoListFace(this, IDC_FLOAT_JUROR_LAW_KINDS, "F_LAW_KINDS", Kind, Default->xKinds, SIZE_OF_KINDS);
	minNo = new TLongFace(this, IDC_FLOAT_JUROR_MIN_NO, "F_MIN_NO", SIZE_OF_NO);
	maxNo = new TLongFace(this, IDC_FLOAT_JUROR_MAX_NO, "F_MAX_NO", SIZE_OF_NO);
	minYear = new TYearFace(this, IDC_FLOAT_JUROR_MIN_YEAR, "F_MIN_YEAR");
	maxYear = new TYearFace(this, IDC_FLOAT_JUROR_MAX_YEAR, "F_MAX_YEAR");
	compositionsLabel = new TStatic(this, IDC_FLOAT_JUROR_COMPOSITIONS_LABEL);
	compositions = new TCharAutoListFace(this, IDC_FLOAT_JUROR_COMPOSITIONS, "F_COMPOSITIONS", CharComposition,
		QUERY_COMPOSITIONS, SIZE_OF_COMPOSITIONS);
	sesKinds = new TCharListFace(this, IDC_FLOAT_JUROR_SES_KINDS, "F_SES_KINDS", Kind, KIND_VISIBLE_SESSIONS,
		SIZE_OF_KINDS);
	dateLabel = new TStatic(this, IDC_FLOAT_JUROR_DATE_LABEL);
	minDate = new TDateFace(this, IDC_FLOAT_JUROR_MIN_DATE, "F_MIN_DATE", false);
	maxDate = new TDateFace(this, IDC_FLOAT_JUROR_MAX_DATE, "F_MAX_DATE", false);
	important = new TCheckFace(this, IDC_FLOAT_JUROR_IMPORTANT, "F_IMPORTANT");
	limited = new TCheckFace(this, IDC_FLOAT_JUROR_LIMITED, "F_LIMITED");
	sesJudges = new TUserListBox(this, IDC_FLOAT_JUROR_SES_JUDGES, POST_VISIBLE_JUDGES, USER_INTER);
	jurors = new TKnownPersonListBox(this, IDC_FLOAT_JUROR_JURORS, INVOLVEMENT_JUROR);
	if (can_extern_sess())
	{
		external = new TCheckFace(this, IDC_FLOAT_JUROR_FLAT_EXTERNAL, "F_EXTERNAL");
		types = new TCharListFace(this, IDC_FLOAT_JUROR_FLAT_TYPES, "F_TYPES", Type, Default->colleges, SIZE_OF_TYPES);
	}
	else
	{
		external = NULL;
		types = NULL;
	}
}

bool TFloatJurorDialog::Load()
{
	return load->GetCheck() == BF_CHECKED || (loadFlat && loadFlat->GetCheck() == BF_CHECKED);
}

bool TFloatJurorDialog::External()
{
	return external && external->GetCheck() == BF_CHECKED;
}

bool TFloatJurorDialog::IsValid()
{
	bool isValid = TFloatDialog::IsValid();

	if (isValid)
	{
		if (invalid_range(minNo, maxNo))
			isValid = error("Началния номер е по-голям от крайния.");
		else if (invalid_range(minYear, maxYear))
			isValid = error("Началната година е по-голяма от крайната.");
		else if (invalid_range(minDate, maxDate))
		{
			isValid = error("Началната дата на %s е по-голяма от крайната.", load->GetCheck() == BF_CHECKED ?
				"заседание" : "преразпределение");
		}
		else if (!Load() && External() && *compositions->S())
		{
			isValid = ask("При заседания по секретни дела, справката за преразпределение се извършва "
				"според съставите на заседанията. %s", Q_CONTINUE);
		}
	}

	return isValid;
}

void TFloatJurorDialog::Action(bool execute)
{
	msql m;
	TFloatJurorGroup *group = FloatJurorGroup();

	if (sesJudges->IsWindowEnabled())
		sesJudges->GetMark(&group->sesJudges);

	jurors->GetMark(&group->jurors);
	group->FormCriteria(m);

	if (execute)
	{
		if (group->load)
			float_juror_load(this, str(m), group);
		else
			float_juror_redistrib(this, str(m), group);
	}
	else
		MessageEx(m);
}

void TFloatJurorDialog::SetupWindow()
{
	TFloatDialog::SetupWindow();

	if (external)
	{
		external->EnableWindow(true);
		external->ShowWindow(SW_SHOW);
	}
}

void TFloatJurorDialog::LoadBNClicked()
{
	bool tLoad = Load();
	bool tExternal = External();

	compositionsLabel->SetCaption(tLoad || tExternal ? "&Състави по заседание" : "&Състави по дело");
	dateLabel->SetCaption(tLoad ? "&Дата на заседание" : "&Дата на преразпределение");
	sesKinds->EnableWindow(tLoad);
	sesJudges->EnableWindow(tLoad && !tExternal);
	jurors->EnableWindow(tLoad);
	manualRed->EnableWindow(!tLoad);
}

void TFloatJurorDialog::ExternalBNClicked()
{
	if (can_extern_sess())
	{
		bool internal = external->GetCheck() == BF_UNCHECKED;

		lawKinds->EnableWindow(internal);
		important->EnableWindow(internal);
		limited->EnableWindow(internal);
		LoadBNClicked();
		types->EnableWindow(!internal);
		lawKinds->ShowWindow(internal ? SW_SHOW : SW_HIDE);
		types->ShowWindow(internal ? SW_HIDE : SW_SHOW);
	}
}
