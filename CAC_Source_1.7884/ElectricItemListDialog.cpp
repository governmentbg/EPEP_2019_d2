DEFINE_RESPONSE_TABLE1(TElectricItemListDialog, TGroupListDialog)
//{{TElectricItemListDialogRSP_TBL_BEGIN}}
	EV_COMMAND_ENABLE(IDVIEW, ViewBNEnable),
	EV_BN_CLICKED(IDVIEW, ViewBNClicked),
	EV_COMMAND_ENABLE(IDADD, AddBNEnable),
	EV_COMMAND_ENABLE(IDEDIT, EditBNEnable),
	EV_BN_CLICKED(IDEDIT, EditBNClicked),
	EV_COMMAND_ENABLE(IDDELETE, DeleteBNEnable),
//{{TElectricItemListDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TElectricItemListDialog Implementation}}

TElectricItemListDialog::TElectricItemListDialog(TWindow* parent, TRCDKeyContainer *container, TGSArray<TElectric> *tItems,
	bool tAttach, const TLawsuit *lawsuit)
:
	TGroupListDialog(parent, IDD_ELECTRIC_ITEM_LIST), attach(tAttach)
{
	items = new TElectricItemListWindow(this, IDC_GROUP_LIST_WINDOW, container, tItems, lawsuit);
	new TPushButton(this, IDVIEW, IDQUERY);
	add = new TPushButton(this, IDADD);
	edit = new TPushButton(this, IDEDIT);
}

void TElectricItemListDialog::SetupWindow()
{
	TGroupListDialog::SetupWindow();

	add->ShowWindow(attach);
	edit->ShowWindow(attach);
}

void TElectricItemListDialog::ViewBNEnable(TCommandEnabler &tce)
{
	tce.Enable(items->Group());
}

void TElectricItemListDialog::ViewBNClicked()
{
	items->ViewGroup();
}

void TElectricItemListDialog::AddBNEnable(TCommandEnabler &tce)
{
	tce.Enable(attach);
}

void TElectricItemListDialog::EditBNEnable(TCommandEnabler &tce)
{
	TElectric *electric = items->Electric();
	tce.Enable(attach && electric && electric->ExtraUser());
}

void TElectricItemListDialog::EditBNClicked()
{
	items->Edit();
}

void TElectricItemListDialog::DeleteBNEnable(TCommandEnabler &tce)
{
	// 2012:016 can_admin_post() -> can_func()
	tce.Enable(can_func(RIGHT_DELETE) && items->Group() && !COURT_MINI);
}
