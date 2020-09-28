DEFINE_RESPONSE_TABLE1(TEPortalAssignListDialog, TGroupListDialog)
//{{TEPortalAssignListDialogRSP_TBL_BEGIN}}
	EV_COMMAND_ENABLE(IDDELETE, DeleteBNEnable),
	EV_BN_CLICKED(IDCANCEL, CancelBNClicked),
	EV_COMMAND_ENABLE(IDC_EPORTAL_ASSIGN_LIST_EPORTAL_SUBPOENA, EPortalSubpoenaBNEnable),
	EV_BN_CLICKED(IDC_EPORTAL_ASSIGN_LIST_EPORTAL_SUBPOENA, EPortalSubpoenaBNClicked),
	EV_WM_CLOSE,
//{{TEPortalAssignListDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TEPortalAssignListDialog Implementation}}

TEPortalAssignListDialog::TEPortalAssignListDialog(TWindow* parent, TInReg *tInReg, const TSideWind *tSideWind, bool tEdit)
:
	TGroupListDialog(parent, IDD_EPORTAL_ASSIGN_LIST), inReg(tInReg), edit(tEdit)
{
	header1 = new TStatic(this, IDC_EPORTAL_ASSIGN_LIST_HEADER_1);
	header2 = new TStatic(this, IDC_EPORTAL_ASSIGN_LIST_HEADER_2);
	assigns = new TEPortalAssignListWindow(this, IDC_GROUP_LIST_WINDOW, tSideWind, inReg->eportalMode);
	eportalSubpoena = new TPushButton(this, IDC_EPORTAL_ASSIGN_LIST_EPORTAL_SUBPOENA);
}

bool TEPortalAssignListDialog::CanClose()
{
	bool result = TGroupListDialog::CanClose();

	if (result)
	{
		if (edit && !assigns->Count())
			result = error("Няма дела.");
	}

	return result;
}

void TEPortalAssignListDialog::SetupWindow()
{
	TGroupListDialog::SetupWindow();

	mstr m;

	inReg->Print(m);
	header1->SetCaption(str(m));
	header2->SetCaption(inReg->EPortalModeText());
}

void TEPortalAssignListDialog::DeleteBNEnable(TCommandEnabler &tce)
{
	tce.Enable(assigns->Group() && assigns->Count() >= 2 || !edit);
}

void TEPortalAssignListDialog::CancelBNClicked()
{
	EvClose();
}

void TEPortalAssignListDialog::EvClose()
{
	if (CanClose())
	{
		if (assigns->Count())
			TGroupListDialog::EvClose();
		else if (warn("Няма дела.\n\nДа изтрия ли документа?"))
		{
			if (delete_incomplete(inReg))
				TGroupListDialog::EvClose();
			else
				error("Грешка при изтриване на документа.");
		}
	}
}

void TEPortalAssignListDialog::EPortalSubpoenaBNEnable(TCommandEnabler &tce)
{
	tce.Enable(inReg->eportalMode == EPORTAL_GRANT && assigns->Group());
}

void TEPortalAssignListDialog::EPortalSubpoenaBNClicked()
{
	assigns->Active();
}
