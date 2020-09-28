DEFINE_RESPONSE_TABLE1(TXferWebListDialog, TGroupListDialog)
//{{TXferWebListDialogRSP_TBL_BEGIN}}
	EV_BN_CLICKED(IDSEARCH, SearchBNClicked),
	EV_BN_CLICKED(IDQUERY, QueryBNClicked),
	EV_BN_CLICKED(IDVIEW, ViewBNClicked),
	EV_BN_CLICKED(IDDELETE, DeleteBNClicked),
	EV_BN_CLICKED(IDPRINT, PrintBNClicked),
	EV_CBN_SETFOCUS(IDC_XFER_WEB_LIST_LAW_KINDS, SetSearch),
	EV_CBN_SETFOCUS(IDC_XFER_WEB_LIST_DEC_KINDS, SetSearch),
	EV_EN_SETFOCUS(IDC_XFER_WEB_LIST_MIN_XFER_DATE, SetSearch),
	EV_EN_SETFOCUS(IDC_XFER_WEB_LIST_MAX_XFER_DATE, SetSearch),
	EV_COMMAND_ENABLE(IDQUERY, SelectEnable),
	EV_COMMAND_ENABLE(IDVIEW, SelectEnable),
	EV_COMMAND_ENABLE(IDDELETE, DeleteBNEnable),
	EV_MESSAGE(WM_FOCUS_FACE, EvFocusFace),
//{{TXferWebListDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TXferWebListDialog Implementation}}

TXferWebListDialog::TXferWebListDialog(TWindow* parent, TXferWebGroup *webGroup)
:
	TGroupListDialog(parent, IDD_XFER_WEB_LIST)
{
	new TCharAutoListFace(this, IDC_XFER_WEB_LIST_LAW_KINDS, "F_LAW_KINDS", Kind, Default->xKinds, SIZE_OF_KINDS);
	new TCharAutoListFace(this, IDC_XFER_WEB_LIST_DEC_KINDS, "F_DEC_KINDS", Kind, KIND_ENDOCS, SIZE_OF_KINDS);
	minXferDate = new TDateFace(this, IDC_XFER_WEB_LIST_MIN_XFER_DATE, "F_MIN_XFER_DATE", false);
	maxXferDate = new TDateFace(this, IDC_XFER_WEB_LIST_MAX_XFER_DATE, "F_MAX_XFER_DATE", false);
	xferWebs = new TXferWebListWindow(this, IDC_GROUP_LIST_WINDOW, webGroup);
	search = new TPushButton(this, IDSEARCH);
	query = new TPushButton(this, IDQUERY);
}

void TXferWebListDialog::SetSearch()
{
	SetDefault(query, search);
}

void TXferWebListDialog::SetupWindow()
{
	TGroupListDialog::SetupWindow();
	minXferDate->SetS("");
	maxXferDate->SetS("");
}

void TXferWebListDialog::SearchBNClicked()
{
	xferWebs->Search();
}

void TXferWebListDialog::QueryBNClicked()
{
	xferWebs->Query();
}

void TXferWebListDialog::ViewBNClicked()
{
	xferWebs->View();
}

void TXferWebListDialog::PrintBNClicked()
{
	xferWebs->Print();
}

void TXferWebListDialog::DeleteBNEnable(TCommandEnabler &tce)
{
	TXferWebPrint *xferWeb = xferWebs->XferWeb();
	tce.Enable(xferWeb && strchr(Default->colleges, xferWebs->XferWebType()) && can_admin(RIGHT_DELETE));
}

long TXferWebListDialog::EvFocusFace(TParam1 wParam, TParam2)
{
	if (wParam)
		SetDefault(search, query);

	return MW_FACE_FOCUSED;
}
