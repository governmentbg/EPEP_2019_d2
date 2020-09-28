DEFINE_RESPONSE_TABLE1(TRequestSideListDialog, TOriginSideListDialog)
//{{TRequestSideListDialogRSP_TBL_BEGIN}}
	EV_BN_CLICKED(IDLOAD, LoadBNClicked),
	EV_BN_CLICKED(IDATTACH, AttachBNClicked),
//{{TRequestSideListDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TRequestSideListDialog Implementation}}

TRequestSideListDialog::TRequestSideListDialog(TWindow* parent, TRequest *request, long sideMode)
:
	TOriginSideListDialog(parent, request, NULL, sideMode | SIDE_LIST_KIND | SIDE_LIST_DATE, "документа", 
		IDD_REQUEST_SIDE_LIST)
{
	load = new TPushButton(this, IDLOAD);
}

void TRequestSideListDialog::Accept()
{
	eportal_send(Request(), LOG_WRITE);
	xfer_accept(Request());
	eispp_accept(this, Request());
}

void TRequestSideListDialog::Reject()
{
	xfer_reject(Request());
}

void TRequestSideListDialog::SetupWindow()
{
	TOriginSideListDialog::SetupWindow();
	load->EnableWindow(strchr(KIND_INDOCS, container->kind) && Request()->xfer);
}

void TRequestSideListDialog::LoadBNClicked()
{
	TGArray<TSideWind> tSideWinds;

	xfer_load(this, Request(), &tSideWinds);
	sideWinds->LoadSideWinds(&tSideWinds);
	tSideWinds.Clear();
}

void TRequestSideListDialog::AttachBNClicked()
{
	sideWinds->Attach();
}
