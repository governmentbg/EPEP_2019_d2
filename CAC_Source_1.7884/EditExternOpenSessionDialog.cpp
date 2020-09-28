DEFINE_RESPONSE_TABLE1(TEditExternOpenSessionDialog, TEditExternSessionDialog)
//{{TEditExternOpenSessionDialogRSP_TBL_BEGIN}}
	EV_CBN_SELCHANGE(IDC_EDIT_EXTERN_SESSION_KIND, KindCBNSelChange),
	EV_CBN_SELCHANGE(IDC_EDIT_EXTERN_SESSION_TYPE, TypeCBNSelChange),
//{{TEditExternOpenSessionDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TEditExternOpenSessionDialog Implementation}}

TEditExternOpenSessionDialog::TEditExternOpenSessionDialog(TWindow* parent, TExternSession *session, bool edit)
:
	TEditExternSessionDialog(parent, session, edit, type2ses_kinds(session->key.type),
		JUROR_NAMES_UNQUOTED ", F_COMPOSITION, F_JUDGE, F_SECRETAR", RESULT_EXTERN_SESSIONS,
		IDD_EDIT_EXTERN_OPEN_SESSION)
{
	time = new TTimeFace(this, IDC_EDIT_EXTERN_OPEN_SESSION_TIME, "F_TIME", true);
}

bool TEditExternOpenSessionDialog::IsValid()
{
	bool isValid = TEditExternSessionDialog::IsValid();

	if (isValid)
	{
		if (!startime->Time().Empty() && startime->Time() < time->Time())
			isValid = error("Началния час е преди часа на насрочване.");
	}

	return isValid;
}

void TEditExternOpenSessionDialog::SetupWindow()
{
	TEditExternSessionDialog::SetupWindow();

	if (!edit)
	{
		TControl *controls[] = { no, year, type, kind, date, NULL };

		for (int i = 0; controls[i]; i++)
			controls[i]->EnableWindow(true);
	}
	
	TypeCBNSelChange();
	secretar->EnableWindow(true);
	time->EnableWindow(true);
}

void TEditExternOpenSessionDialog::KindCBNSelChange()
{
	const TBaseSession *session = Session();

	if (session->key.type == TYPE_PUNISHMENT_LAW)
	{
		end->SetDate(session->end);
		end->EnableWindow(true);
	}
	else
	{
		end->EnableWindow(false);
		end->SetS("");
	}

	SessionTimesChanged();
}

void TEditExternOpenSessionDialog::TypeCBNSelChange()
{
	kind->SetXCrit(type2ses_kinds(Session()->key.type));
	KindCBNSelChange();
}
