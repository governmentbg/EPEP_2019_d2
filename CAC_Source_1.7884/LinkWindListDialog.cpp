DEFINE_RESPONSE_TABLE1(TLinkWindListDialog, TGroupListDialog)
//{{TLinkWindListDialogRSP_TBL_BEGIN}}
	EV_BN_CLICKED(IDADD, AddBNClicked),
	EV_BN_CLICKED(IDEDIT, EditBNClicked),
	EV_BN_CLICKED(IDDELETE, DeleteBNClicked),
//{{TLinkWindListDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TLinkWindListDialog Implementation}}

TLinkWindListDialog::TLinkWindListDialog(TWindow* parent, TLinkWindArray *tLinkWinds, TGArray<TSideWind> *ordinary,
	TGArray<TSideWind> *proxy, TGArray<TSideWind> *redirect, int resId)
:
	TGroupListDialog(parent, resId)
{
	linkWinds = new TLinkWindListWindow(this, IDC_GROUP_LIST_WINDOW, tLinkWinds, ordinary, proxy, redirect);
	counter = new TStatic(this, IDC_LINK_WIND_LIST_COUNTER);
}

void TLinkWindListDialog::UpdateCounter()
{
	mstr m;
	m.printf("Брой: %d", linkWinds->Count());
	counter->SetCaption(str(m));
}

void TLinkWindListDialog::SetupWindow()
{
	TGroupListDialog::SetupWindow();
	UpdateCounter();
}

void TLinkWindListDialog::AddBNClicked()
{
	TGroupListDialog::AddBNClicked();
	UpdateCounter();
}

void TLinkWindListDialog::EditBNClicked()
{
	linkWinds->Edit();
}

void TLinkWindListDialog::DeleteBNClicked()
{
	TGroupListDialog::DeleteBNClicked();
	UpdateCounter();
}
