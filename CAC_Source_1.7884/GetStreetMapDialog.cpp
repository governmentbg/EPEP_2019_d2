DEFINE_RESPONSE_TABLE1(TGetStreetMapDialog, TQuickDialog)
//{{TGetStreetMapDialogRSP_TBL_BEGIN}}
	EV_COMMAND_ENABLE(IDOK, OKBNEnable),
	EV_BN_CLICKED(IDOK, OKBNClicked),
	EV_CBN_SELCHANGE(IDC_GET_STREET_MAP_USER, UserCBNSelChange),
	EV_COMMAND_ENABLE(IDC_GET_STREET_MAP_SELECT_ALL, SelectAllBNEnable),
	EV_BN_CLICKED(IDC_GET_STREET_MAP_SELECT_ALL, SelectAllBNClicked),
//{{TGetStreetMapDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TGetStreetMapDialog Implementation}}

TGetStreetMapDialog::TGetStreetMapDialog(TWindow* parent, TGArray<TStreetMap> *tSelect, const char *exclude)
:
	TQuickDialog(parent, IDD_GET_STREET_MAP), select(tSelect)
{
	user = new TUserAliasFace(this, IDC_GET_STREET_MAP_USER, "F_USER", C2S[POST_DELIVERER], FLAG_ACTIVE | ALIAS_OPTION,
		exclude);
	streetMap = new TStreetMapListBox(this, IDC_GET_STREET_MAP_STREETS);
}

void TGetStreetMapDialog::SetupWindow()
{
	TQuickDialog::SetupWindow();

	UserCBNSelChange();
}

void TGetStreetMapDialog::OKBNEnable(TCommandEnabler &tce)
{
	tce.Enable(atob(user->S()) && streetMap->GetSelCount() > 0);
}

void TGetStreetMapDialog::OKBNClicked()
{
	streetMap->GetMark(select);
	CmOk();
}

void TGetStreetMapDialog::UserCBNSelChange()
{
	streetMap->SetUser(user->S());
}

void TGetStreetMapDialog::SelectAllBNEnable(TCommandEnabler &tce)
{
	tce.Enable(streetMap->GetCount() > 0);
}

void TGetStreetMapDialog::SelectAllBNClicked()
{
	streetMap->SelectAll(!streetMap->GetSel(0));
}
