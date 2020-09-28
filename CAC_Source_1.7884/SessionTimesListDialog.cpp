DEFINE_RESPONSE_TABLE1(TSessionTimesListDialog, TGroupListDialog)
//{{TSessionTimesListDialogRSP_TBL_BEGIN}}
	EV_BN_CLICKED(IDCANCEL, CancelBNClicked),
	EV_WM_CLOSE,
//{{TSessionTimesListDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TSessionTimesListDialog Implementation}}

TSessionTimesListDialog::TSessionTimesListDialog(TWindow* parent, TGSArray<TSessionTimes> &tTimes, bool *edited)
:
	TGroupListDialog(parent, IDD_SESSION_TIMES_LIST)
{
	times = new TSessionTimesListWindow(this, IDC_GROUP_LIST_WINDOW, &tTimes, edited);
}

void TSessionTimesListDialog::CancelBNClicked()
{
	EvClose();
}

void TSessionTimesListDialog::EvClose()
{
	mstr m;

	if (times->IsValid(m) || ask("%s.\n\n%s", str(m), Q_CONTINUE))
		TGroupListDialog::EvClose();
}
