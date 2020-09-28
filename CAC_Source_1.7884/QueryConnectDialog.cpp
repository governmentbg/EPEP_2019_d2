DEFINE_RESPONSE_TABLE1(TQueryConnectDialog, TGroupDialog)
//{{TQueryConnectDialogRSP_TBL_BEGIN}}
	EV_BN_CLICKED(IDOK, OKBNClicked),
	EV_CBN_SELCHANGE(IDC_QUERY_CONNECT_CONNECT_TYPE, ConnectTypeCBNSelChange),
	EV_CBN_SELCHANGE(IDC_QUERY_CONNECT_CONNECT_KIND, ConnectKindCBNSelChange),
//{{TQueryConnectDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TQueryConnectDialog Implementation}}

TQueryConnectDialog::TQueryConnectDialog(TWindow* parent, TConnect *connect, int resId)
:
	TGroupDialog(parent, connect, resId)
{
	connectType = new TCharAliasFace(this, IDC_QUERY_CONNECT_CONNECT_TYPE, "F_CONNECT_TYPE", ConnectType, CONNECT_TYPE_VISIBLES);
	ConnectType->Seek(*CONNECT_TYPE_VISIBLES);
	senderTypes[0] = ConnectType->senderType;
	senderTypes[1] = '\0';
	sender = new TReceiverAliasFace(this, IDC_QUERY_CONNECT_UCN, "F_UCN", Receiver, senderTypes);
	connectNo = new TConnectNoFace(this, IDC_QUERY_CONNECT_CONNECT_NO, "F_CONNECT_NO", connectType);
	connectYear = new TYearFace(this, IDC_QUERY_CONNECT_CONNECT_YEAR, "F_CONNECT_YEAR");
	connectKind = new TCharAliasFace(this, IDC_QUERY_CONNECT_CONNECT_KIND, "F_CONNECT_KIND", ConnectKind, "", ALIAS_EMPTY);
}

void TQueryConnectDialog::OKBNClicked()
{
	if (IsValid())
	{
		Connect()->ucnType = sender->UCNType();
		*Group() << this;
		CmOk();
	}
}

void TQueryConnectDialog::ConnectTypeCBNSelChange()
{
	ConnectType->Seek(connectType->C());

	// 2009:203 LPR: copied from edit connect
	*connectKinds = '\0';
	for (const char *s = ConnectType->connectKinds; *s; s++)
		if (strchr(strchr(CONNECT_TYPE_SELFS, connectType->C()) ? CONNECT_KIND_SELFS : CONNECT_KIND_ALLS, *s))
			strcach(connectKinds, *s);

	connectKind->SetXCrit(connectKinds);
	// 2013:199 LRQ: auto select 1 element
	if (strlen(connectKinds) >= 2)
	{
		connectKind->EnableWindow(true);
		connectKind->SetC(Connect()->connectKind);
	}
	else
	{
		connectKind->EnableWindow(false);
		connectKind->SetC(*connectKinds);
	}

	ConnectKindCBNSelChange();
}

void TQueryConnectDialog::SetupWindow()
{
	TGroupDialog::SetupWindow();

	ConnectTypeCBNSelChange();
	// 2007:023 LPR: reset after initial/destructive initial type change
	sender->SetS(Connect()->ucn);
}

bool TQueryConnectDialog::IsValid()
{
	bool isValid = TGroupDialog::IsValid();

	if (isValid)
	{
		if (!any(connectNo) || !connectYear->L())
			isValid = error("Полетата номер и година са задължителни.");
	}

	return isValid;
}

void TQueryConnectDialog::ConnectKindCBNSelChange()
{
	// 2007:319 LPR: reset only senders that became invalid: does the
	// same job as in edit connect, only the implementation is different
	char xData = *sender->XCrit();

	if (connectKind->C() == CONNECT_KIND_PRIVATE_EXECUTOR)
		sender->SetXCrit(UCN_EXECUTORS);
	else
	{
		senderTypes[0] = ConnectType->senderType;
		sender->SetXCrit(senderTypes);
	}

	if (xData != *sender->XCrit())
	{
		// 2007:023 LPR: clear now incompatible or non-first sender
		sender->SetS(EMPTY);
	}
}
