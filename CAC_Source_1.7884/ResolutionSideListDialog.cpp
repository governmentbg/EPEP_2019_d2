DEFINE_RESPONSE_TABLE1(TResolutionSideListDialog, TSideWindListDialog)
//{{TResolutionSideListDialogRSP_TBL_BEGIN}}
	EV_BN_CLICKED(IDMESSAGES, MessagesBNClicked),
	EV_BN_CLICKED(IDLIST, ListBNClicked),
	EV_BN_CLICKED(IDRIGHT, RightBNClicked),
	EV_COMMAND_ENABLE(IDCHILD, ChildBNEnable),
	EV_BN_CLICKED(IDCHILD, ChildBNClicked),
//{{TResolutionSideListDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TResolutionSideListDialog Implementation}}

TResolutionSideListDialog::TResolutionSideListDialog(TWindow* parent, TRequestResolution *resolution, TRequest *tRequest,
	long sideMode)
:
	TSideWindListDialog(parent, resolution, NULL, sideMode | SIDE_LIST_KIND, "разпореждането", IDD_RESOLUTION_SIDE_LIST),
		request(tRequest)
{
	new TTwinButton(this, IDLIST, IDRIGHT);
	child = new TPushButton(this, IDCHILD);
}

void TResolutionSideListDialog::Header1(mstr &m)
{
	m.printf("%s %ld/%ldг.", Kind->Seek(request->kind), request->key.no, request->key.year);
}

void TResolutionSideListDialog::Header2(mstr &m)
{
	m.cat("Разпореждане ");

	if (PRINT_JUDGEMENTS && Resolution()->eventNo)
		m.printf("%ld/%ldг.", Resolution()->eventNo, Resolution()->date.year);
	else
		Resolution()->date.Print(m, "от %Dг.");
}

void TResolutionSideListDialog::MessagesBNClicked()
{
	sideWinds->Messages();
}

void TResolutionSideListDialog::ListBNClicked()
{
	sideWinds->Roll();
}

void TResolutionSideListDialog::RightBNClicked()
{
	// 2004:229 LPR: or something else?
	sideWinds->List(true);
}
