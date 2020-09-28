DEFINE_RESPONSE_TABLE1(TEditNIATDepDialog, TEditNamedDialog)
//{{TEditNIATDepDialogRSP_TBL_BEGIN}}
	EV_BN_CLICKED(IDOK, OKBNClicked),
//{{TEditNIATDepDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TEditNIATDepDialog Implementation}}

TEditNIATDepDialog::TEditNIATDepDialog(TWindow* parent, TSender *sender, bool edit, int resId)
:
	TEditNamedDialog(parent, sender, edit, true, resId)
{
	area = new TAreaAliasFace(this, IDC_EDIT_NIA_TDEP_AREA, "F_AREA");
	name = new TSenderNameFace(this, IDC_EDIT_NIA_TDEP_NAME, "F_NAME", SIZE_OF_NAME);
}

void TEditNIATDepDialog::SetupWindow()
{
	TEditNamedDialog::SetupWindow();

	if (edit)
	{
		area->SetL(atol(Sender()->ucn) / SENDER_NIA_TDEP_MULT);
		area->EnableWindow(false);
	}
}

void TEditNIATDepDialog::OKBNClicked()
{
	if (!edit)
		sprintf(Sender()->ucn, "%10ld", area->L() * SENDER_NIA_TDEP_MULT);

	TEditDialog::OKBNClicked();
}
