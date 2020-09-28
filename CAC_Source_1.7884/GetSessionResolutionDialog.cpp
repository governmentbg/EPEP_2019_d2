DEFINE_RESPONSE_TABLE1(TGetSessionResolutionDialog, TGetGroupDialog)
//{{TGetSessionResolutionDialogRSP_TBL_BEGIN}}
	EV_CBN_SELCHANGE(IDC_GET_GROUP_TYPE, TypeCBNSelChange),
	EV_CBN_SELCHANGE(IDC_GET_SESSION_RESOLUTION_RESOLUTION_KIND, ResolutionKindCBNSelChange),
	EV_BN_CLICKED(IDOK, OKBNClicked),
//{{TGetSessionResolutionDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TGetSessionResolutionDialog Implementation}}

TGetSessionResolutionDialog::TGetSessionResolutionDialog(TWindow* parent, TSession *session, const char *statuses,
	TRCDKeyContainer **tResolutionPtr, const char *tResolutions, const char *tLabel, bool tSessionSelected)
:
	TGetGroupDialog(parent, session, NULL, TYPE_LAWSUITS, KIND_OPEN_SESSIONS, statuses, match_null, 0,
		IDD_GET_SESSION_RESOLUTION), resolutions(tResolutions), resolutionPtr(tResolutionPtr), label(tLabel),
		sessionSelected(tSessionSelected)
{
	resolutionKind = new TGetContainerAliasFace(this, IDC_GET_SESSION_RESOLUTION_RESOLUTION_KIND, "F_RESOLUTION_KIND",
		KIND_SECONDARY_SESSIONS_ENDOCS);
	resolutionDate = new TDateFace(this, IDC_GET_SESSION_RESOLUTION_RESOLUTION_DATE, "F_RESOLUTION_DATE");
	resolutionIndex = new TIndexFace(this, IDC_GET_SESSION_RESOLUTION_RESOLUTION_INDEX, "F_RESOLUTION_INDEX",
		KIND_CLOSED_SESSIONS, SIZE_OF_SESSION_INDEX);
}

void TGetSessionResolutionDialog::SetupWindow()
{
	TGetGroupDialog::SetupWindow();

	resolutionDate->SetDate(date->Date());
	TypeCBNSelChange();

	msql m;
	m.printf("Заседание за %s", label);
	kindLabel->SetCaption(str(m));

	if (sessionSelected)
	{
		TControl *controls[] = { type, no, year, kind, date, NULL };

		for (int i = 0; controls[i]; i++)
			controls[i]->EnableWindow(false);

		*Group() >> this;
	}
}

bool TGetSessionResolutionDialog::IsValid()
{
	// 2018-02-13 LPR: check resolutionDate/Index if sessionSelected
	bool isValid = sessionSelected ? resolutionDate->IsValid(true) && resolutionIndex->IsValid(true) :
		TGetGroupDialog::IsValid();

	if (isValid)
	{
		TRCDKeyContainer *tResolution = kind2RCDKeyContainer(resolutionKind->C());

		*Group() << this;

		tResolution->key = Group()->key;
		tResolution->kind = adjust_xxx_kind(resolutionKind->C(), resolutionIndex, KIND_CLOSED_SESSIONS);
		tResolution->date = resolutionDate->Date();

		if (!tResolution->Get())
			isValid = false;
		else if (strchr(KIND_SESSIONS, tResolution->kind))
		{
			TSession *tSession = (TSession *) tResolution;

			if (strchr(KIND_OPEN_SESSIONS, tSession->kind) ? strchr(RESULT_UNCARRIEDS, tSession->result) :
				(constant) !strchr(resolutions, tSession->result))
			{
				isValid = error("%s: статусът на разпореждането не съответства.", Result->Seek(tSession->result));
			}
		}

		if (isValid)
			*resolutionPtr = tResolution;
		else
			delete tResolution;
	}

	return isValid;
}

void TGetSessionResolutionDialog::TypeCBNSelChange()
{
	TGetGroupDialog::TypeCBNSelChange();

	resolutionKind->SetType(type->C());
	ResolutionKindCBNSelChange();
}

void TGetSessionResolutionDialog::ResolutionKindCBNSelChange()
{
	adjust_xxx_index(resolutionKind->C(), resolutionIndex, KIND_CLOSED_SESSIONS);
}

void TGetSessionResolutionDialog::OKBNClicked()
{
	if (sessionSelected)
		TGroupDialog::OKBNClicked();
	else
		TGetGroupDialog::OKBNClicked();
}
