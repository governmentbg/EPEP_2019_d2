DEFINE_RESPONSE_TABLE1(TEditExternSessionDialog, TSessionTimesDialog)
//{{TEditExternSessionDialogRSP_TBL_BEGIN}}
	EV_BN_CLICKED(IDOK, OKBNClicked),
	EV_EN_UPDATE(IDC_EDIT_EXTERN_SESSION_DATE, DateENUpdate),
	EV_CBN_SELCHANGE(IDC_EDIT_EXTERN_SESSION_RESULT, ResultCBNSelChange),
//{{TEditExternSessionDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TEditExternSessionDialog Implementation}}

TEditExternSessionDialog::TEditExternSessionDialog(TWindow* parent, TExternSession *session, bool edit, const char *kinds,
	const char *getFields, const char *results, int resId)
:
	TSessionTimesDialog(parent, session, NULL, edit, resId)
{
	type = new TCharAliasFace(this, IDC_EDIT_EXTERN_SESSION_TYPE, "F_TYPE", Type, Default->colleges);
	no = new TLongFace(this, IDC_EDIT_EXTERN_SESSION_NO, "F_NO", SIZE_OF_NO);
	year = new TYearFace(this, IDC_EDIT_EXTERN_SESSION_YEAR, "F_YEAR");
	kind = new TCharAliasFace(this, IDC_EDIT_EXTERN_SESSION_KIND, "F_KIND", Kind, kinds);
	date = new TDateFace(this, IDC_EDIT_EXTERN_SESSION_DATE, "F_DATE");
	if (!edit)
		session->GetLast(getFields);
	result = new TCharAliasFace(this, IDC_EDIT_EXTERN_SESSION_RESULT, "F_RESULT", Result, results);
	times = new TPushButton(this, IDC_SESSION_TIMES_TIMES);
}

bool TEditExternSessionDialog::SingleDate() const
{
	return !date->IsValid(false) || !end->IsValid(false) || end->Date() <= date->Date();
}

bool TEditExternSessionDialog::IsValid()
{
	bool isValid = TSessionTimesDialog::IsValid();

	if (isValid)
	{
		bool tEndate = !end->Date().Empty() && end->Date() != date->Date();
		bool tEntime = !entime->Time().Empty();

		if (!no->L() || !year->L())
			isValid = error("Номера и годината са задължителни");
		else if (tEndate && end->Date() < date->Date())
			isValid = error("Крайната дата е преди датата на насрочване.");
		// 2008:060 FIX: was !entime->Time().Empty()
		else if (tEndate && !tEntime)
			isValid = error("Крайния час е задължителен.");
		// 2008:060 FIX: was missing
		else if (!tEndate && tEntime && entime->Time() < startime->Time())
			isValid = error("Крайния час е преди началния.");
		else if (!atob(judge))
			isValid = error("Докладчикът е задължителен.");
		// not really, we don't know the subject
		//else if (secretar->IsWindowEnabled() && !atob(secretar))
		//	isValid = error("Секретарят е задължителен.");
	}

	return isValid;
}

void TEditExternSessionDialog::SetupWindow()
{
	TTroubleDialog::SetupWindow();

	initialized = true;
}

void TEditExternSessionDialog::OKBNClicked()
{
	if (IsValid() && CheckSessionTimes())
	{
		*Group() << this;

		if (Group()->Save(edit))
		{
			CommitTimes();
			CmOk();
		}
	}
}

void TEditExternSessionDialog::DateENUpdate()
{
	if (initialized && date->IsValid(false))
		SingleDateChanged();
}

void TEditExternSessionDialog::ResultCBNSelChange()
{
	SessionTimesChanged();
}
