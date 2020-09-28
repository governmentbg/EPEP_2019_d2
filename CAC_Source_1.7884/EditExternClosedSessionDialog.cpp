DEFINE_RESPONSE_TABLE1(TEditExternClosedSessionDialog, TEditExternSessionDialog)
//{{TEditExternClosedSessionDialogRSP_TBL_BEGIN}}
	EV_BN_CLICKED(IDOK, OKBNClicked),
//{{TEditExternClosedSessionDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TEditExternClosedSessionDialog Implementation}}

TEditExternClosedSessionDialog::TEditExternClosedSessionDialog(TWindow* parent, TExternSession *session, bool edit)
:
	TEditExternSessionDialog(parent, session, edit, KIND_CLOSED_SESSIONS, "F_COMPOSITION, F_JUDGE",
		C2S[RESULT_EXTERN_CARRIED], IDD_EDIT_EXTERN_CLOSED_SESSION)
{
	index = new TIndexFace(this, IDC_EDIT_EXTERN_CLOSED_SESSION_INDEX, "F_INDEX", KIND_CLOSED_SESSIONS,
		SIZE_OF_SESSION_INDEX);
}

void TEditExternClosedSessionDialog::SetupWindow()
{
	TEditExternSessionDialog::SetupWindow();

	if (edit)
		index->SetL(kind2index(Session()->kind, KIND_CLOSED_SESSIONS));
	else
		adjust_xxx_index(Session()->kind, index, KIND_CLOSED_SESSIONS);

	SessionTimesChanged();
}

void TEditExternClosedSessionDialog::OKBNClicked()
{
	if (!edit)
		adjust_xxx_kind(Session(), index, KIND_CLOSED_SESSIONS);

	TEditExternSessionDialog::OKBNClicked();
}
