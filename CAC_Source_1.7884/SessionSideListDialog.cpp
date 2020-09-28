//{{TSessionSideListDialog Implementation}}

TSessionSideListDialog::TSessionSideListDialog(TWindow* parent, TSession *session, TLawsuit *lawsuit, long sideMode,
	int resId)
:
	TDecisionSideListDialog(parent, session, session, lawsuit, sideMode, resId)
{
	what = "заседанието";
	messages = new TButton(this, IDMESSAGES);
}

void TSessionSideListDialog::SetupWindow()
{
	if (session->flags & SESSION_EXPERT)
	{
		TWaitWindow wait(this, WAIT_CC);
		xfer_resign(session, sideWinds->Lawsuit()->kind);
		xfer_assign(session, sideWinds->Lawsuit()->kind);
	}

	TDecisionSideListDialog::SetupWindow();

	SetCaption("Страни по заседание");

	if (Session()->Status() == RESULT_FIXED)
		messages->SetCaption("&Призовки");
}
