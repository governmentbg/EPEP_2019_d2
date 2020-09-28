DEFINE_RESPONSE_TABLE1(TInRegEditDialog, TEditIncomingDialog)
//{{TInRegEditDialogRSP_TBL_BEGIN}}
	EV_BN_CLICKED(IDOK, OKBNClicked),
//{{TInRegEditDialogRSP_TBL_END}}
END_RESPONSE_TABLE;
//{{TInRegEditDialog Implementation}}

TInRegEditDialog::TInRegEditDialog(TWindow* parent, TInReg *inReg)
:
	TEditIncomingDialog(parent, inReg, NULL, Default->xColleges, true, IDD_INREG_EDIT)
{
	text = new TStringFace(this, IDC_INREG_EDIT_TEXT, "F_TEXT", SIZE_OF_ORIGIN_TEXT);
	receiver = new TSenderAliasFace(this, IDC_INREG_EDIT_RECEIVER, "F_RECEIVER", inReg->receiverType, ALIAS_OPTION);
}

void TInRegEditDialog::OKBNClicked()
{
	if (IsValid() && CheckDirection())
	{
		if (strchr(KIND_TEXTUAL_IN_DOCS, kind->C()) == NULL)
			text->SetS("");

		TEditIncomingDialog::OKBNClicked();
	}
}

void TInRegEditDialog::SetupWindow()
{
	TEditIncomingDialog::SetupWindow();

	TInReg *inReg = (TInReg *) Group();

	text->EnableWindow(strchr(KIND_TEXTUAL_IN_DOCS, inReg->kind));
	college->EnableWindow(!inReg->lawsuit.type);
	set_direction(inReg->lawsuit, direction);

	// inReg->receiverType == UCN_INSTANCE_HIGHER -> lock even empty if sent
	if (inReg->LastReceiver(NULL, NULL))
		receiver->EnableWindow(false);

	if (inReg->kind == KIND_EPORTAL_REQUEST)
		college->EnableWindow(false);
}
