DEFINE_RESPONSE_TABLE1(TStreetMapListDialog, TGroupListDialog)
//{{TStreetMapListDialogRSP_TBL_BEGIN}}
	EV_CBN_SELCHANGE(IDC_STREET_MAP_LIST_UCLP, UCLPCBNSelChange),
	EV_BN_CLICKED(IDPRINT, PrintBNClicked),
	EV_BN_CLICKED(IDCOPY, CopyBNClicked),
//{{TStreetMapListDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TStreetMapListDialog Implementation}}

TStreetMapListDialog::TStreetMapListDialog(TWindow* parent, TUser *tUser, int resId)
:
	TGroupListDialog(parent, resId), user(tUser)
{
	name = new TStringFace(this, IDC_STREET_MAP_LIST_NAME, "F_UCN", SIZE_OF_USER_NAME);
	uclp = new TUCLPAliasFace(this, IDC_STREET_MAP_LIST_UCLP, "F_UCLP", COURT_UCLP);
	streetMaps = new TStreetMapListWindow(this, user, IDC_GROUP_LIST_WINDOW);
}

void TStreetMapListDialog::SetupWindow()
{
	TGroupListDialog::SetupWindow();
	UCLPCBNSelChange();

	mstr m;
	print_user(m, user->ucn);
	name->SetS(str(m));
}

void TStreetMapListDialog::UCLPCBNSelChange()
{
	streetMaps->SetUCLP(uclp->L());
}

void TStreetMapListDialog::PrintBNClicked()
{
	streetMaps->Print();
}

void TStreetMapListDialog::CopyBNClicked()
{
	streetMaps->Copy();
}
