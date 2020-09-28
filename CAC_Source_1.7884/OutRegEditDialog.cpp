DEFINE_RESPONSE_TABLE1(TOutRegEditDialog, TOutRegSendBaseDialog)
//{{TOutRegEditDialogRSP_TBL_BEGIN}}
	EV_BN_CLICKED(IDOK, OKBNClicked),
	EV_BN_CLICKED(IDPRINT, PrintBNClicked),
//{{TOutRegEditDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TOutRegEditDialog Implementation}}

TOutRegEditDialog::TOutRegEditDialog(TWindow* parent, TOutReg *outReg, TInReg *inReg, TConnect **connects)
:
	TOutRegSendBaseDialog(parent, outReg, inReg, NULL, connects, true, IDD_OUTREG_EDIT) 
{
	lawsuitType = new TCharAliasFace(this, IDC_OUTREG_SEND_LAWSUIT_TYPE, "F_LAWSUIT_TYPE", Type, NULL, ALIAS_EMPTY);

	if (outReg->receiverType == UCN_INSTANCE_RESERVED)
		receiver = NULL;
	else
		receiver = new TSenderAliasFace(this, IDC_OUTREG_SEND_FIXEIVER, "F_RECEIVER", outReg->receiverType);

	college = new TCharAliasFace(this, IDC_OUTREG_SEND_COLLEGE, "F_COLLEGE", College, Default->xColleges);
	ok = new TPushButton(this, IDOK);
	resend = new TCheckFace(this, IDC_OUTREG_SEND_RESEND, "F_RESEND");
}

void TOutRegEditDialog::OKBNClicked()
{
	if (IsValid())
	{
		TOutReg *outReg = OutReg();

		*outReg << this;
		outReg->resend = resend->GetCheck() == BF_CHECKED;

		if (OutReg()->Update())
		{
			trouble.Move(OutReg());
			CmOk();
		}
	}
}

void TOutRegEditDialog::PrintBNClicked()
{
	if (IsValid())
	{
		*OutReg() << this;
		print_outreg(this, OutReg(), inReg, NULL, true, NULL, connects ? connects[0] : NULL);
	}
}

void TOutRegEditDialog::SetupWindow()
{
	TOutRegSendBaseDialog::SetupWindow();

	TOutReg *outReg = OutReg();

	print->EnableWindow(outreg_subpoena_kind(outReg) && outReg->receiverType != UCN_INSTANCE_HIGHER);
	college->EnableWindow(!outReg->lawsuit.type);
	resend->EnableWindow(xfer_resendable(outReg, inReg));

	// N.B. not automatic since receiver is not mandatory
	if (outReg->receiverType == UCN_INSTANCE_LOWER)
	{
		vssIndexLabel->ShowWindow(SW_SHOW);
		vssIndex->ShowWindow(SW_SHOW);
		receiver->EnableWindow(false);
	}

	if (!print->IsWindowEnabled())
		SetDefault(print, ok);

	if (receiver)
		receiver->ShowWindow(SW_SHOW);
}
