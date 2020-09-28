DEFINE_RESPONSE_TABLE1(TPerformNIAProtocolDialog, TGroupDialog)
//{{TPerformNIAProtocolDialogRSP_TBL_BEGIN}}
	EV_COMMAND_ENABLE(IDPRINT, PrintBNEnable),
	EV_BN_CLICKED(IDPRINT, PrintBNClicked),
	EV_COMMAND_ENABLE(IDENTER, EnterBNEnable),
	EV_COMMAND(IDENTER, EnterBNClicked),
//{{TPerformNIAProtocolDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TPerformNIAProtocolDialog Implementation}}

TPerformNIAProtocolDialog::TPerformNIAProtocolDialog(TWindow* parent, TNIAProtocol *protocol, int resId)
:
	TGroupDialog(parent, protocol, resId)
{
	niaNo = new TLongFace(this, IDC_PERFORM_NIA_PROTOCOL_NIA_NO, "F_NIA_NO", SIZE_OF_NO);
	execLists = new TSelectExecListListBox(this, IDC_PERFORM_NIA_PROTOCOL_EXEC_LISTS, &protocol->execLists);
}

void TPerformNIAProtocolDialog::SetupWindow()
{
	TGroupDialog::SetupWindow();
	niaNo->EnableWindow(Default->post == POST_ADMIN);
}

void TPerformNIAProtocolDialog::PrintBNEnable(TCommandEnabler &tce)
{
	tce.Enable(execLists->GetSelCount());
}

void TPerformNIAProtocolDialog::PrintBNClicked()
{
	if (execLists->CheckCollects())
	{
		execLists->CutMark();
		CloseWindow(IDPRINT);
	}
}

void TPerformNIAProtocolDialog::EnterBNEnable(TCommandEnabler &tce)
{
	tce.Enable(execLists->GetSelCount());
}

void TPerformNIAProtocolDialog::EnterBNClicked()
{
	NIAProtocol()->nia.no = niaNo->L();

	if (niaNo->L() && NIAProtocol()->Try("F_NIA_NO"))
		error("Вече има протокол с този номер.");
	else if (execLists->CheckCollects())
	{
		execLists->CutMark();
		CloseWindow(IDENTER);
	}
}
