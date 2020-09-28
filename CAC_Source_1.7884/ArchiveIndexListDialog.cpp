DEFINE_RESPONSE_TABLE1(TArchiveIndexListDialog, TGroupListDialog)
//{{TArchiveIndexListDialogRSP_TBL_BEGIN}}
	EV_BN_CLICKED(IDSUBJECT, SubjectBNClicked),
	EV_COMMAND_ENABLE(IDSUBJECT, SelectEnable),
	EV_BN_CLICKED(IDPRINT, PrintBNClicked),
	EV_BN_CLICKED(IDCANCEL, CancelBNClicked),
	EV_WM_CLOSE,
//{{TArchiveIndexListDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TArchiveIndexListDialog Implementation}}

TArchiveIndexListDialog::TArchiveIndexListDialog(TWindow* parent, int resId)
:
	TGroupListDialog(parent, resId)
{
	indexes = new TArchiveIndexListWindow(this, IDC_GROUP_LIST_WINDOW);
}

bool TArchiveIndexListDialog::CanClose()
{
	bool result = TGroupListDialog::CanClose();

	if (result)
	{
		mstr m;

		indexes->Empty(m);
		if (any(m))
			result = ask("Индекси без предмети: %s.\n\nЖелаете ли да продължите?", str(m));
	}

	return result;
}

void TArchiveIndexListDialog::SubjectBNClicked()
{
	indexes->Subject();
}

void TArchiveIndexListDialog::PrintBNClicked()
{
	indexes->Print();
}

void TArchiveIndexListDialog::CancelBNClicked()
{
	EvClose();
}

void TArchiveIndexListDialog::EvClose()
{
	if (CanClose())
		TGroupListDialog::EvClose();
}
