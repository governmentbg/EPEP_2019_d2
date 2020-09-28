DEFINE_RESPONSE_TABLE1(TGetRekindTargetDialog, TScaleDialog)
//{{TGetRekindTargetDialogRSP_TBL_BEGIN}}
	EV_BN_CLICKED(IDOK, OKBNClicked),
	EV_COMMAND_ENABLE(IDOK, OKBNEnable),
	EV_CBN_SELCHANGE(IDC_GET_REKIND_TARGET_REQUEST_KIND, RequestKindCBNSelChange),
	EV_CBN_SELCHANGE(IDC_GET_REKIND_TARGET_LAWSUIT_KIND, LawsuitKindCBNSelChange),
	EV_CBN_SELCHANGE(IDC_GET_REKIND_TARGET_EVENT_KIND, EventKindCBNSelChange),
//{{TGetRekindTargetDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TGetRekindTargetDialog Implementation}}

TGetRekindTargetDialog::TGetRekindTargetDialog(TWindow* parent, TRequest *tRequest, TLawsuit *tLawsuit)
:
	TScaleDialog(parent, IDD_GET_REKIND_TARGET), request(tRequest), lawsuit(tLawsuit), types(C2S[lawsuit->key.type])
{
	header = new TStatic(this, IDC_GET_REKIND_TARGET_HEADER);

	for (const TReKindTypeDocs *docs = ReKindTypeDocs; docs->type; docs++)
	{
		if (docs->type == lawsuit->key.type && strchr(docs->kinds, request->kind))
		{
			for (const char *kind = docs->kinds; *kind; kind++)
				if (!strchr(KIND_QUICKONLYS, *kind))
					requestKinds.cach(*kind);
			break;
		}
	}

	requestKind = new TCharAliasFace(this, IDC_GET_REKIND_TARGET_REQUEST_KIND, "F_REQUEST_KIND", Kind, str(requestKinds));
	requestNo = new TLongFace(this, IDC_GET_REKIND_TARGET_REQUEST_NO, "F_REQUEST_NO", SIZE_OF_NO);
	requestYear = new TYearFace(this, IDC_GET_REKIND_TARGET_REQUEST_YEAR, "F_REQUEST_YEAR");
	kind2LawKinds(request->kind, types, lawKinds);
	lawsuitKind = new TCharAliasFace(this, IDC_GET_REKIND_TARGET_LAWSUIT_KIND, "F_LAWSUIT_KIND", Kind, str(lawKinds));
	lawsuitNo = new TLongFace(this, IDC_GET_REKIND_TARGET_LAWSUIT_NO, "F_LAWSUIT_NO", SIZE_OF_NO);
	lawsuitYear = new TYearFace(this, IDC_GET_REKIND_TARGET_LAWSUIT_YEAR, "F_LAWSUIT_YEAR");
	subject = new TSubjectAliasFace(this, IDC_GET_REKIND_TARGET_SUBJECT, "F_SUBJECT", lawsuit->kind);
	eventKind = new TGetContainerAliasFace(this, IDC_GET_REKIND_TARGET_EVENT_KIND, "F_EVENT_KIND",
		lawsuit->finished.Empty() ? KIND_SECONDARY_SESSIONS_ENDOCS : KIND_ENDOCS);
	eventDate = new TDateFace(this, IDC_GET_REKIND_TARGET_EVENT_DATE, "F_EVENT_DATE", true);
	eventIndex = new TIndexFace(this, IDC_GET_REKIND_TARGET_EVENT_INDEX, "F_EVENT_INDEX", KIND_CLOSED_SESSIONS,
		SIZE_OF_SESSION_INDEX);
}

bool TGetRekindTargetDialog::IsValid()
{
	bool isValid = TScaleDialog::IsValid();

	if (isValid)
	{
		TRCDKeyContainer *const event = kind2RCDKeyContainer(eventKind->C());
		TVirtPtr VP(event);

		event->key = lawsuit->key;
		event->date = eventDate->Date();
		event->kind = adjust_xxx_kind(eventKind->C(), eventIndex, KIND_CLOSED_SESSIONS);

		if (!event->Get())
			isValid = false;
		else if (strchr(KIND_OPEN_SESSIONS, eventKind->C()) && strchr(RESULT_UNCARRIEDS, event->Status()))
			isValid = error("Разпореждането не отговаря на необходимите критерии.");
		else if (!subject->L())
			isValid = error("Предметът е задължителен.");
	}

	return isValid;
}

void TGetRekindTargetDialog::SetupWindow()
{
	mstr m;

	TScaleDialog::SetupWindow();

	m.printf("%s %ld/%ldг. образувано по ", Kind->Seek(lawsuit->kind), lawsuit->key.no, lawsuit->key.year);
	m.cat(Kind->Seek(request->kind));
	header->SetCaption(str(m));

	requestKind->SetC(request->kind);
	requestNo->SetL(request->key.no);
	requestYear->SetL(request->key.year);
	lawsuitKind->SetC(lawsuit->kind);
	lawsuitNo->SetL(lawsuit->key.no);
	lawsuitYear->SetL(lawsuit->key.year);
	eventDate->SetS("");
	RequestKindCBNSelChange();
}

void TGetRekindTargetDialog::OKBNClicked()
{
	if (IsValid())
	{
		request->kind = lawsuit->sourceKind = requestKind->C();
		lawsuit->kind = lawsuitKind->C();
		request->subject = lawsuit->subject = subject->L();

		Kind->Seek(lawsuit->kind);

		if (EISPP_ACTIVE && Kind->eispp)
		{
			info("Новия вид дело допуска номер по ЕИСПП. Ако делото предполага изпращане на данни към ЕИСПП, "
				"попълнете или генерирайте номера, и редактирайте всички събития, свързани с делото, за да "
				"изпратите данни към ЕИСПП.");
		}

		CmOk();
	}
}

void TGetRekindTargetDialog::OKBNEnable(TCommandEnabler &tce)
{
	tce.Enable(requestKind->C() != request->oldKind);
}

void TGetRekindTargetDialog::RequestKindCBNSelChange()
{
	kind2LawKinds(requestKind->C(), types, lawKinds);
	lawsuitKind->SetXCrit(str(lawKinds));
	LawsuitKindCBNSelChange();
}

void TGetRekindTargetDialog::LawsuitKindCBNSelChange()
{
	char kind = lawsuitKind->C();

	subject->SetKind(kind);

	if (!subject->L() && kind == lawsuit->kind)
		subject->SetL(lawsuit->subject);

	eventKind->SetType(kind2type(kind));
	EventKindCBNSelChange();
}

void TGetRekindTargetDialog::EventKindCBNSelChange()
{
	adjust_xxx_index(eventKind->C(), eventIndex, KIND_CLOSED_SESSIONS);
}
