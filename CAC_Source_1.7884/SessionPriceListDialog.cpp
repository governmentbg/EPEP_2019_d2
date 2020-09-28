DEFINE_RESPONSE_TABLE1(TSessionPriceListDialog, TGroupListDialog)
//{{TSessionPriceListDialogRSP_TBL_BEGIN}}
	EV_BN_CLICKED(IDPRINT, PrintBNClicked),
//{{TSessionPriceListDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TSessionPriceListDialog Implementation}}

TSessionPriceListDialog::TSessionPriceListDialog(TWindow* parent, int resId)
:
	TGroupListDialog(parent, resId)
{
	prices = new TSessionPriceListWindow(this, IDC_GROUP_LIST_WINDOW);
}

void TSessionPriceListDialog::PrintBNClicked()
{
	prices->Print();
}
