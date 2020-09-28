DEFINE_RESPONSE_TABLE1(TInformedListDialog, TGroupListDialog)
//{{TInformedListDialogRSP_TBL_BEGIN}}
	EV_CBN_SELCHANGE(IDC_INFORMED_LIST_INFORMED, InformedCBNSelChange),
	EV_BN_CLICKED(IDIMPORT, ImportBNClicked),
	EV_BN_CLICKED(IDPRINT, PrintBNClicked),
//{{TInformedListDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TInformedListDialog Implementation}}

TInformedListDialog::TInformedListDialog(TWindow* parent, int resId)
:
	TGroupListDialog(parent, resId)
{
	informed = new TSenderAliasFace(this, IDC_INFORMED_LIST_INFORMED, "F_INFORMED", UCN_INFORMED);
	informeds = new TInformedListWindow(this, IDC_GROUP_LIST_WINDOW);
}

void TInformedListDialog::SetupWindow()
{
	TGroupListDialog::SetupWindow();
	InformedCBNSelChange();
}

void TInformedListDialog::InformedCBNSelChange()
{
	informeds->SetInformed(informed->S());
}

void TInformedListDialog::ImportBNClicked()
{
	informeds->Import();
}

void TInformedListDialog::PrintBNClicked()
{
	informeds->Print();
}
