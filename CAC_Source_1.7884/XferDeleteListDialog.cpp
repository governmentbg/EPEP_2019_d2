DEFINE_RESPONSE_TABLE1(TXferDeleteListDialog, TGroupListDialog)
//{{TXferDeleteListDialogRSP_TBL_BEGIN}}
	EV_CBN_SELCHANGE(IDC_XFER_DELETE_LIST_COURT, CourtCBNSelChange),
	EV_BN_CLICKED(IDPRINT, PrintBNClicked),
//{{TXferDeleteListDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TXferDeleteListDialog Implementation}}

TXferDeleteListDialog::TXferDeleteListDialog(TWindow* parent, const char *xData)
:
	TGroupListDialog(parent, IDD_XFER_DELETE_LIST)
{
	court = new TLongAliasFace(this, IDC_XFER_DELETE_LIST_COURT, "F_COURT", Court, xData, FLAG_NULL);
	documents = new TXferDeleteListWindow(this, IDC_GROUP_LIST_WINDOW);
}

TXferDeleteListDialog::~TXferDeleteListDialog()
{
}

void TXferDeleteListDialog::SetupWindow()
{
	TGroupListDialog::SetupWindow();
	CourtCBNSelChange();
}

void TXferDeleteListDialog::CourtCBNSelChange()
{
	documents->SetCourt(court->L());
}

void TXferDeleteListDialog::PrintBNClicked()
{
	xfer_print_list(this, court->L(), documents->Array());
}
