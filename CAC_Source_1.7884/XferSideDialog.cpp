DEFINE_RESPONSE_TABLE1(TXferSideDialog, TQuickDialog)
//{{TXferSideDialogRSP_TBL_BEGIN}}
	EV_BN_CLICKED(IDOK, OKBNClicked),
	EV_COMMAND_ENABLE(IDOK, OKBNEnable),
//{{TXferSideDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TXferSideDialog Implementation}}

TXferSideDialog::TXferSideDialog(TWindow* parent, TXferDocument *document, TRCDKeyContainer *container,
	TGSArray<TXferSide> *tXferSides)
:
	TQuickDialog(parent, IDD_XFER_SIDE)
{
	container->Involvements(involvements);
	involvements.cach(INVOLVEMENT_INTERNAL);
	xferSides = new TXferSideListBox(this, IDC_XFER_SIDE_SIDES, document, tXferSides, str(involvements));
	involvement = new TCharAliasFace(this, IDC_XFER_SIDE_INVOLVEMENT, "F_INVOLVEMENT", Involvement, str(involvements));
}

void TXferSideDialog::OKBNClicked()
{
	if (IsValid())
	{
		xferSides->CutMark();
		xferSides->ApplyInvolvement(involvement->C());
		CmOk();
	}
}

void TXferSideDialog::OKBNEnable(TCommandEnabler &tce)
{
	tce.Enable(xferSides->GetSelCount() > 0);
}
