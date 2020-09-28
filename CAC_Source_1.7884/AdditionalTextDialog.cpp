DEFINE_RESPONSE_TABLE1(TAdditionalTextDialog, TGroupDialog)
//{{TAdditionalTextDialogRSP_TBL_BEGIN}}
	EV_CBN_SELCHANGE(IDC_ADDITIONAL_TEXT_APPEAL, AppealCBNSelChange),
	EV_BN_CLICKED(IDOK, OKBNClicked),
	EV_BN_CLICKED(IDEDIT, EditBNClicked),
//{{TAdditionalTextDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TAdditionalTextDialog Implementation}}

TAdditionalTextDialog::TAdditionalTextDialog(TWindow* parent, TPrintSubpoena *printsub, TRCDKeyContainer *tContainer, int resId)
:
	TGroupDialog(parent, printsub, resId), container(tContainer)
{
	if (SubpoenaKind->HasAccomply())
	{
		topLabel = new TStatic(this, IDC_ADDITIONAL_TEXT_ACCOMPLY_LABEL);
		transcript = NULL;
		// 2015:166 special PUNI set for area; 2015:190 special set for MEET-s
		accomply = new TCharAliasFace(this, IDC_ADDITIONAL_TEXT_TOP_COMBO, "F_ACCOMPLY", Accomply,
		#if COURT_TYPE == COURT_AREAL
			strchr(SUBPOENA_KIND_MEET_ACCOMPLYS, SubpoenaKind->subpoenaKind) ? ACCOMPLY_CREDITORS :
		#endif  // AREAL
			container->key.type == TYPE_PUNISHMENT_LAW ? ACCOMPLY_SUBPUNIABLES : ACCOMPLY_SUBPOENABLES);
	}
	else
	{
		topLabel = new TStatic(this, IDC_ADDITIONAL_TEXT_TRANSCRIPT_LABEL);
		transcript = new TTranscriptComboBox(this, IDC_ADDITIONAL_TEXT_TOP_COMBO, printsub);
		accomply = NULL;
	}

	additionalText = new TStringFace(this, IDC_ADDITIONAL_TEXT_TEXT, "F_ADDITIONAL_TEXT", SIZE_OF_LARGE_TEXT);
	name = new TStringFace(this, IDC_ADDITIONAL_TEXT_NAME, "F_NAME", SIZE_OF_PATH);
	appeal = new TComboBox(this, IDC_ADDITIONAL_TEXT_APPEAL);
	interval = new TComboBox(this, IDC_ADDITIONAL_TEXT_INTERVAL);
	before = new TComboBox(this, IDC_ADDITIONAL_TEXT_BEFORE);
	from = new TComboBox(this, IDC_ADDITIONAL_TEXT_FROM);
}

void TAdditionalTextDialog::SetupWindow()
{
	TGroupDialog::SetupWindow();

	electric_name(container, name, TYPE_TEXT);

	appeal->AddString("");
#if COURT_TYPE == COURT_APPEAL
	appeal->AddString("може да се обжалва с касац. жалба и протест пред");
#endif  // APPEAL
	appeal->AddString("може да се обжалва пред");
	appeal->AddString("може да се обжалва с касационна жалба пред");
	noappeal = appeal->AddString("не подлежи на обжалване.");
	appeal->AddString("подлежи на проверка пред");
	nocheck = appeal->AddString("не подлежи на касационна проверка.");

	interval->AddString("в 7-дневен срок");
	interval->AddString("в 14-дневен срок");
	interval->AddString("в 15-дневен срок");
	interval->AddString("в 30-дневен срок");
	interval->AddString("в ...-дневен срок");

	TSender sender;
	msql m;

	sender.ucnType = UCN_INSTANCE_HIGHER;
	sender.FloatCriteria(m);

	TQuery q(str(m));
	while (q.Read())
	{
		sender << q;
		// 20070226 _ да не се извежда "Върховен КАСАЦИОНЕН съд"
#if COURT_TYPE == COURT_ADMIN
		if (strcmp(sender.ucn, INSTANCE_SUPER))
#endif  // ADMIN
			before->AddString(sender.name);
	}

	from->AddString("от получаване на настоящото.");

	SubpoenaKind->Seek(PrintSub()->subpoenaKind);
	appeal->EnableWindow(SubpoenaKind->AppealAllowed());
	appeal->SetSelIndex(0);
	AppealCBNSelChange();
	// 2010:176 LPR: may be appeal or surroundKinds only
	additionalText->EnableWindow(SubpoenaKind->AdditionalText());
	topLabel->ShowWindow(SW_SHOW);	// 2010:330
}

bool TAdditionalTextDialog::AppealActive()
{
	int sel = appeal->GetSelIndex();
	return sel > 0 && sel != noappeal && sel != nocheck;
}

void TAdditionalTextDialog::AppealCBNSelChange()
{
	if (AppealActive())
	{
		interval->SetSelIndex(0);
		interval->EnableWindow(true);
		before->SetSelIndex(0);
		before->EnableWindow(true);
		from->SetSelIndex(0);
		from->EnableWindow(true);
	}
	else
	{
		interval->EnableWindow(false);
		interval->SetText("");
		before->EnableWindow(false);
		before->SetText("");
		from->EnableWindow(false);
		from->SetText("");
	}
}

void TAdditionalTextDialog::OKBNClicked()
{
	if (IsValid())
	{
		*PrintSub() << this;
		char *s = PrintSub()->appeal;

		appeal->GetText(s, SIZE_OF_APPEAL_FIELD);

		if (AppealActive())
		{
			strcat(s, " ");
			before->GetText(s + strlen(s), SIZE_OF_APPEAL_FIELD);
			strcat(s, " ");
			interval->GetText(s + strlen(s), SIZE_OF_APPEAL_FIELD);
			strcat(s, " ");
			from->GetText(s + strlen(s), SIZE_OF_APPEAL_FIELD);
		}

		if (transcript)
			PrintSub()->transcript = transcript->GetContainer();

		CmOk();
	}
}

void TAdditionalTextDialog::EditBNClicked()
{
	electric_edit(this, name, true, TYPE_TEXT);
}
