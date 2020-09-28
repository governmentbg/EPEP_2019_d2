DEFINE_RESPONSE_TABLE1(TSurrouendocListDialog, TGroupListDialog)
//{{TSurrouendocListDialogRSP_TBL_BEGIN}}
	EV_COMMAND_ENABLE(IDDELETE, DeleteBNEnable),
	EV_BN_CLICKED(IDDELETE, DeleteBNClicked),
	EV_COMMAND_ENABLE(IDAPPEAL, SelectEnable),
	EV_BN_CLICKED(IDAPPEAL, AppealBNClicked),
	EV_LVN_ITEMCHANGING(IDC_GROUP_LIST_WINDOW, ListLVNItemChanging),
//{{TSurrouendocListDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TSurrouendocListDialog Implementation}}

TSurrouendocListDialog::TSurrouendocListDialog(TWindow* parent, TSession *tSession, TGArray<TSurroundment> *tSurrouendocs,
	TLawsuit *tLawsuit, bool tAppealing)
:
	TGroupListDialog(parent, IDD_SURROUENDOC_LIST), session(tSession), lawsuit(tLawsuit), appealing(tAppealing)
{
	new TCharAliasFace(this, IDC_SURROUENDOC_LIST_TYPE, "F_TYPE", Type, NULL);
	new TLongFace(this, IDC_SURROUENDOC_LIST_NO, "F_NO", SIZE_OF_NO);
	new TYearFace(this, IDC_SURROUENDOC_LIST_YEAR, "F_YEAR");
	new TDateFace(this, IDC_SURROUENDOC_LIST_DATE, "F_DATE");
	surrouendocs = new TSurrouendocListWindow(this, IDC_GROUP_LIST_WINDOW, session, tSurrouendocs, lawsuit);
	edit = new TPushButton(this, IDEDIT);
	erase = new TPushButton(this, IDDELETE);
	appeal = new TPushButton(this, IDAPPEAL);
}

void TSurrouendocListDialog::SetupWindow()
{
	TGroupListDialog::SetupWindow();
	*session >> this;

	if (appealing)
	{
		edit->EnableWindow(false);
		edit->ShowWindow(SW_HIDE);
		erase->EnableWindow(false);
		erase->ShowWindow(SW_HIDE);
		appeal->ShowWindow(SW_SHOW);
		appeal->EnableWindow(true);
	}
}

void TSurrouendocListDialog::DeleteBNEnable(TCommandEnabler &tce)
{
	TSurroundment *surrouendoc = (TSurroundment *) surrouendocs->Group();
	tce.Enable(surrouendoc && surrouendoc->decisionKind);
}

void TSurrouendocListDialog::DeleteBNClicked()
{
	surrouendocs->Clear();
}

void TSurrouendocListDialog::AppealBNClicked()
{
	surrouendocs->Appeal();
}

bool TSurrouendocListDialog::ListLVNItemChanging(TLwNotify &lwn)
{
	bool result = TGroupListDialog::ListLVNItemChanging(lwn);
	TSurroundment *surrouendoc = (TSurroundment *) surrouendocs->Group();

	edit->SetCaption(surrouendoc && surrouendoc->decisionKind ? "&Редактиране" : "&Вписване");
	return result;
}
