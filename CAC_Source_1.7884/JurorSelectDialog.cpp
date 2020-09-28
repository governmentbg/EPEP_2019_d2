DEFINE_RESPONSE_TABLE1(TJurorSelectDialog, TJurorMotivesDialog)
//{{TJurorSelectDialogRSP_TBL_BEGIN}}
	EV_COMMAND_ENABLE(IDOK, OKBNEnable),
	EV_BN_CLICKED(IDOK, OKBNClicked),
//{{TJurorSelectDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TJurorSelectDialog Implementation}}

TJurorSelectDialog::TJurorSelectDialog(TWindow* parent, char *motives, const char *tName, char *tUCN,
	TGArray<TDistribJuror> *tJurors, int resId)
:
	TJurorMotivesDialog(parent, "Избор на заседател", motives, resId), name(tName), ucn(tUCN)
{
	label = new TStatic(this, IDC_JUROR_SELECT_LABEL);
	jurors = new TSelectJurorListBox(this, IDC_JUROR_SELECT_JURORS, tJurors);
}

void TJurorSelectDialog::SetupWindow()
{
	TJurorMotivesDialog::SetupWindow();
	label->SetText(name);
}

void TJurorSelectDialog::OKBNEnable(TCommandEnabler &tce)
{
	tce.Enable(jurors->Group());
}

void TJurorSelectDialog::OKBNClicked()
{
	strcpy(ucn, TYPECHECK_PTR_CAST(TDistribJuror, jurors->Group())->ucn);
	TJurorMotivesDialog::OKBNClicked();
}
