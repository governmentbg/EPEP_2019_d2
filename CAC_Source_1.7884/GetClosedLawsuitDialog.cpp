DEFINE_RESPONSE_TABLE1(TGetClosedLawsuitDialog, TGetExternContainerDialog)
//{{TGetClosedLawsuitDialogRSP_TBL_BEGIN}}
	EV_BN_CLICKED(IDOK, OKBNClicked),
//{{TGetClosedLawsuitDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TGetClosedLawsuitDialog Implementation}}

TGetClosedLawsuitDialog::TGetClosedLawsuitDialog(TWindow* parent, TRCDKeyGroup **grouptr, CDate *tSessionDate,
	const TExternSession *suggest, bool forceExcess)
:
	TGetExternContainerDialog(parent, grouptr, match_null, suggest, forceExcess, 0, IDD_GET_CLOSED_LAWSUIT),
		sessionDate(tSessionDate)
{
	sesKind = new TCharAliasFace(this, IDC_GET_GROUP_KIND, "F_SES_KIND", Kind, C2S[KIND_CLOSED_SESSION]);
	sesDate = new TDateFace(this, IDC_GET_GROUP_DATE, "F_SES_DATE", true);
}

void TGetClosedLawsuitDialog::SetupWindow()
{
	TGetExternContainerDialog::SetupWindow();

	if (forceExcess)
		external->ShowWindow(SW_SHOW);
}

void TGetClosedLawsuitDialog::OKBNClicked()
{
	*sessionDate = sesDate->Date();
	TGetExternContainerDialog::OKBNClicked();
}
