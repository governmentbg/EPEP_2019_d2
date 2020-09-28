DEFINE_RESPONSE_TABLE1(TEditSetSessionDialog, TResetSessionDialog)
//{{TEditSetSessionDialogRSP_TBL_BEGIN}}
	EV_BN_CLICKED(IDOK, OKBNClicked),
//{{TEditSetSessionDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TEditSetSessionDialog Implementation}}

TEditSetSessionDialog::TEditSetSessionDialog(TWindow* parent, TSession *session, TLawsuit *lawsuit, const char *kinds)
:
	TResetSessionDialog(parent, session, lawsuit, kinds)
{
}

void TEditSetSessionDialog::SetupWindow()
{
	TResetSessionDialog::SetupWindow();

	SetCaption("Редактиране на насрочване");
	date->EnableWindow(false);
	time->EnableWindow(false);
}

void TEditSetSessionDialog::OKBNClicked()
{
	if (IsValid())
	{
		*Session() << this;

		if (Session()->Update())
			CmOk();
	}
}
