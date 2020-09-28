DEFINE_RESPONSE_TABLE1(TServeAnnounceDialog, TGroupDialog)
//{{TServeAnnounceDialogRSP_TBL_BEGIN}}
	EV_CBN_SELCHANGE(IDC_SERVE_ANNOUNCE_RECEIVED_STATUS, ReceivedStatusCBNSelChange),
	EV_BN_CLICKED(IDOK, OKBNClicked),
	EV_BN_CLICKED(IDC_SERVE_ANNOUNCE_OFFICIAL, OfficialBNClicked),
	EV_BN_CLICKED(IDATTACH, AttachBNClicked),
//{{TServeAnnounceDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TServeAnnounceDialog Implementation}}

TServeAnnounceDialog::TServeAnnounceDialog(TWindow* parent, TAnnounce *announce, TRCDKeyContainer *tContainer,
	const char *tScheds, const char *tStatuses, sign tAlready)
:
	TGroupDialog(parent, announce, IDD_SERVE_ANNOUNCE), subpoena(announce->subpoena), container(tContainer),
		scheds(tScheds), statuses(tStatuses), already(tAlready)
{
	header = new TStatic(this, IDC_SERVE_ANNOUNCE_HEADER);
	receivedStatus = new TCharAliasFace(this, IDC_SERVE_ANNOUNCE_RECEIVED_STATUS, "F_RECEIVED_STATUS", ReceivedStatus,
		statuses);
	received = new TDateFace(this, IDC_SERVE_ANNOUNCE_SUBPOENA_RECEIVED, "F_SUBPOENA_RECEIVED", false);
	receivedUpDn = new TDateFaceUpDown(this, IDC_SERVE_ANNOUNCE_SUBPOENA_RECEIVED_UPDN, received);
	receivedTime = new TTimeFace(this, IDC_SERVE_ANNOUNCE_RECEIVED_TIME, "F_RECEIVED_TIME", false);
	receivedTime->NoZeros();
	schedNo = new TRadioFace(this, IDC_SERVE_ANNOUNCE_SCHED_NO, "F_SCHED_NO");
	schedYes = new TRadioFace(this, IDC_SERVE_ANNOUNCE_SCHED_YES, "F_SCHED_YES");
	sched7 = new TRadioFace(this, IDC_SERVE_ANNOUNCE_SCHED_7, "F_SCHED_7");
	sched15 = new TRadioFace(this, IDC_SERVE_ANNOUNCE_SCHED_15, "F_SCHED_15");
	receivedText = new TStringFace(this, IDC_SERVE_ANNOUNCE_RECEIVED_TEXT, "F_RECEIVED_TEXT", SIZE_OF_RECEIVED_TEXT);
	returned = new TDateFace(this, IDC_SERVE_ANNOUNCE_RETURNED, "F_SUBPOENA_RETURNED", false);
	returnedUpDn = new TDateFaceUpDown(this, IDC_SERVE_ANNOUNCE_RETURNED_UPDN, returned);
	official = new TCheckFace(this, IDC_SERVE_ANNOUNCE_OFFICIAL, "F_OFFICIAL");
	imageName = new TStringFace(this, IDC_SERVE_ANNOUNCE_IMAGE_NAME, "F_IMAGE_NAME", SIZE_OF_PATH);
	attach = new TButton(this, IDATTACH);
	barcodeText = new TStringFace(this, IDC_SERVE_ANNOUNCE_BARCODE_TEXT, "F_BARCODE_TEXT", SIZE_OF_BARCODE_TEXT);
}

void TServeAnnounceDialog::SetupWindow()
{
	TGroupDialog::SetupWindow();

	mstr m;
	const TAnnounce *announce = Announce();

	SubpoenaKind->Seek(announce->subpoenaKind);
	ConnectKind->Seek(announce->key.kind);
	m.printf("%s по %s %ld/%ldг. на ", SubpoenaKind->alias, ConnectKind->alias, announce->key.no, announce->key.year);
	announce->GetSenderName(m);

	//m.printf("\n\n%s на %Dг.", strchr(statuses, SRS_INCOMING) ? "Получен(а,о)" : "Изготвен(а,о)", &announce->subpoenaDate);
	char tReceivedStatus = SRS_FOR_DELIVER;

	for (const char *s = statuses; *s; s++)
		if (strchr(SRS_INITIALS, *s))
			tReceivedStatus = *s;

	m.printf("\n\n%s на %Dг.", ReceivedStatus->Seek(tReceivedStatus), &announce->subpoenaDate);

	if (announce->street)
		m.printf(" за адрес %s %s", Street->Seek(announce->uclp, announce->street), announce->adrNo);

	if (atof(announce->delivererUcn))
	{
		m.cat("\n\nПризовкар: ");
		print_user(m, announce->delivererUcn);
	}

	header->SetCaption(str(m));

	// 2010:020 FIX: regression 3900
	if (subpoena)
	{
		receivedStatus->EnableWindow(subpoena->multi != SUBPOENA_MULTI && SubpoenaKind->MayServe());
		// 2014:028
		char tName[SIZE_OF_PATH];

		electric_name(subpoena, tName, TYPE_IMAGE);
		if (exist(tName))
			imageName->SetS(tName);

		imageName->EnableWindow(true);
		attach->EnableWindow(true);
	}

	ReceivedStatusCBNSelChange();
	get_barcode_text(Announce(), barcodeText);
}

void TServeAnnounceDialog::ReceivedStatusCBNSelChange()
{
	ReceivedStatus->Seek(receivedStatus->C());

	if (ReceivedStatus->AllowDateReceived())
	{
		received->SetDate(Announce()->subpoenaReceived.Empty() ? Today : Announce()->subpoenaReceived);
		received->EnableWindow(true);
		receivedUpDn->EnableWindow(true);
		receivedTime->SetTime(Announce()->receivedTime);
		receivedTime->EnableWindow(true);
	}
	else
	{
		received->EnableWindow(false);
		receivedUpDn->EnableWindow(false);
		received->SetS("");
		receivedTime->EnableWindow(false);
		receivedTime->SetS("");
	}

	if (ReceivedStatus->AllowDateReturned())
	{
		returned->SetDate(Announce()->subpoenaReturned.Empty() ? Today : Announce()->subpoenaReturned);
		// 2011:139 LRQ: true -> !LOCK_RETURN
		returned->EnableWindow(!LOCK_RETURN);
		returnedUpDn->EnableWindow(!LOCK_RETURN);
	}
	else
	{
		returned->EnableWindow(false);
		returnedUpDn->EnableWindow(false);
		returned->SetS("");
	}

	if (ReceivedStatus->AllowDateReceived() || ReceivedStatus->AllowDateReturned() || subpoena)
	{
		receivedText->SetS(Announce()->receivedText);
		receivedText->EnableWindow(true);
	}
	else
	{
		receivedText->EnableWindow(false);
		receivedText->SetS("");
	}

	// 2010:049 FIX: regression 4440
	SubpoenaKind->Seek(subpoena ? subpoena->subpoenaKind : Announce()->subpoenaKind);

	if (already == true || !subpoena || !SubpoenaKind->IsNotification())
	{
		official->EnableWindow(false);
		official->SetCheck(BF_UNCHECKED);
	}
	else if (already == -1)
	{
		official->EnableWindow(false);
		official->SetCheck(subpoena->OfficiallyNotified() ? BF_CHECKED : BF_UNCHECKED);
	}
	else
	{
		official->EnableWindow(true);
		official->SetCheck(strchr(SRS_INITIALS, receivedStatus->C()) ||
			receivedStatus->C() == SRS_NOT_RECEIVED ? BF_UNCHECKED : BF_CHECKED);
	}

	OfficialBNClicked();
}

void TServeAnnounceDialog::OKBNClicked()
{
	if (IsValid())
	{
		*Group() << this;

		if (subpoena)
			subpoena->officiallyNotified = official->GetCheck() == BF_CHECKED
				? SUBPOENA_OFFICIALLY_NOTIFIED : SUBPOENA_OFFICIALLY_NONNOTIFIED;

		if (subpoena || Group()->Update())
		{
			if (subpoena)
				electric_move(subpoena, imageName->S());

			CmOk();
		}
	}
}

bool TServeAnnounceDialog::IsValid()
{
	bool isValid = TGroupDialog::IsValid();

	if (isValid)
	{
		CDate minReturned;
		const char *badReturned;

		if (received->IsWindowEnabled())
		{
			if (received->Date().Empty())
				isValid = error("Датата на връчване е задължителна.");
			else if (received->Date() < Announce()->subpoenaDate)
				isValid = error("Датата на връчване не може да е преди датата на получаване/изготвяне.");
			else
			{
				minReturned = received->Date();
				badReturned = "Датата на връщане не може да е преди датата на връчване.";
			}
		}
		else
		{
			minReturned = Announce()->subpoenaDate;
			badReturned = "Датата на връщане не може да е преди датата на получаване/изготвяне.";
		}

		if (isValid && returned->IsWindowEnabled())
		{
			if (returned->Date().Empty())
				isValid = error("Датата на връщане е задължителна.");
			else if (returned->Date() > Today)
				isValid = error("Датата на връщане не може да е по-голяма от текущата дата.");
			else if (returned->Date() < minReturned)
				isValid = error(badReturned);
		}

		if (isValid && official->IsWindowEnabled() && official->GetCheck() == BF_CHECKED &&
			(receivedStatus->C() == SRS_SENT || receivedStatus->C() == SRS_NOT_RECEIVED))
		{
			isValid = ask("Статус \"%s\" не предполага официално уведомяване. Желаете ли да продължите?",
				ReceivedStatus->Seek(receivedStatus->C()));
		}
	}

	return isValid;
}

void TServeAnnounceDialog::OfficialBNClicked()
{
	SubpoenaKind->Seek(Announce()->subpoenaKind);
	ReceivedStatus->Seek(receivedStatus->C());

	bool sched = SCHED_ACTIVE && official->IsWindowEnabled() &&
		official->GetCheck() == BF_CHECKED &&
		SubpoenaKind->MaySchedule() && ReceivedStatus->AllowSchedule();

	schedNo->EnableWindow(sched);
	schedYes->EnableWindow(sched);
	sched7->EnableWindow(sched && strchr(scheds, SCHED_DECIDE_7));
	sched15->EnableWindow(sched && strchr(scheds, SCHED_SETTLE_15));

	if (!sched)
	{
		schedNo->SetCheck(BF_CHECKED);
		schedYes->EnableWindow(BF_UNCHECKED);
		sched7->EnableWindow(BF_UNCHECKED);
		sched15->EnableWindow(BF_UNCHECKED);
	}
}

void TServeAnnounceDialog::AttachBNClicked()
{
	electric_edit(this, imageName, true, TYPE_IMAGE);
}
