DEFINE_RESPONSE_TABLE1(TEditDangerDialog, TGroupDialog)
//{{TEditDangerDialogRSP_TBL_BEGIN}}
	EV_BN_CLICKED(IDOK, OKBNClicked),
//{{TEditDangerDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TEditDangerDialog Implementation}}

TEditDangerDialog::TEditDangerDialog(TWindow* parent, TDanger *tDanger)
:
	TGroupDialog(parent, tDanger, IDD_EDIT_DANGER)
{
	danger = new TLongFace(this, IDC_EDIT_DANGER_DANGER, "F_DANGER_LONG", SIZE_OF_DANGER);
	active = new TCheckFace(this, IDC_EDIT_DANGER_ACTIVE, "F_FLAGS_ACTIVE");
	new TStringFace(this, IDC_EDIT_DANGER_NAME, "F_NAME", SIZE_OF_DANGER_NAME);
}

void TEditDangerDialog::OKBNClicked()
{
	if (IsValid())
	{
		*Group() << this;
		if (Group()->Update())
			CmOk();
	}
}

void TEditDangerDialog::SetupWindow()
{
	TGroupDialog::SetupWindow();
	active->EnableWindow(danger->L() % DANGER_MASK_ARTICLE);
}
