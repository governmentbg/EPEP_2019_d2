DEFINE_RESPONSE_TABLE1(TSelectSurroundsDialog, TQuickDialog)
//{{TSelectSurroundsDialogRSP_TBL_BEGIN}}
	EV_BN_CLICKED(IDOK, OKBNClicked),
//{{TSelectSurroundsDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TSelectSurroundsDialog Implementation}}

TSelectSurroundsDialog::TSelectSurroundsDialog(TWindow* parent, TGArray<TSurround> *tPossibles,
	TGArray<TSurround> *tSelecteds, bool tShowReloaded)
:
	TQuickDialog(parent, IDD_SELECT_SURROUNDS), possibles(tPossibles), selecteds(tSelecteds), showReloaded(tShowReloaded)
{
	surrounds = new TSelectSurroundsListBox(this, IDC_SELECT_SURROUNDS_SURROUNDS, possibles);
	reloaded = new TStatic(this, IDC_SELECT_SURROUNDS_RELOADED);
}

void TSelectSurroundsDialog::SetupWindow()
{
	TQuickDialog::SetupWindow();

	surrounds->SetMark(selecteds);
	reloaded->ShowWindow(showReloaded ? SW_SHOW : SW_HIDE);
}

void TSelectSurroundsDialog::OKBNClicked()
{
	surrounds->GetMark(selecteds);
	CmOk();
}
