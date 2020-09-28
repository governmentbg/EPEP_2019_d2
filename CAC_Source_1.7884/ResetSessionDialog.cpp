DEFINE_RESPONSE_TABLE1(TResetSessionDialog, TSetSessionBaseDialog)
//{{TResetSessionDialogRSP_TBL_BEGIN}}
	EV_BN_CLICKED(IDOK, OKBNClicked),
//{{TResetSessionDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TResetSessionDialog Implementation}}

TResetSessionDialog::TResetSessionDialog(TWindow* parent, TSession *session, TLawsuit *lawsuit, const char *kinds)
:
	TSetSessionBaseDialog(parent, session, lawsuit, kinds, true, IDD_RESET_SESSION)
{
	time = new TTimeFace(this, IDC_SET_SESSION_TIME, "F_TIME");
	new TLongAliasFace(this, IDC_SET_SESSION_HALL, "F_HALL", Hall, NULL, FLAG_ACTIVE | ALIAS_EMPTY);
}

void TResetSessionDialog::SetupWindow()
{
	TSetSessionBaseDialog::SetupWindow();

	if (Session()->end.Empty())
		end->SetS("");
	if (Session()->entime.Empty())
		entime->SetS("");
}

void TResetSessionDialog::OKBNClicked()
{
	if (IsValid())
	{
		// 2004:217 LRQ: check for non-work days
		*Session() << this;

		if (Session()->date.Work() || ask("%Dг. е почивен ден. Желаете ли да продължите?", &Session()->date))
			CmOk();
	}
}
