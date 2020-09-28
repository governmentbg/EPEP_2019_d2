DEFINE_RESPONSE_TABLE1(TGetBarCodeDialog, TQuickDialog)
//{{TGetBarCodeDialogRSP_TBL_BEGIN}}
	EV_BN_CLICKED(IDOK, OKBNClicked),
	EV_WM_TIMER,
//{{TGetBarCodeDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TGetBarCodeDialog Implementation}}

TGetBarCodeDialog::TGetBarCodeDialog(TWindow* parent, long *tCourt, long *tRequestId, char *tCES, int resId)
:
	TQuickDialog(parent, resId), court(tCourt), requestId(tRequestId), ces(tCES)
{
	barcode = new TBarCodeFace(this, IDC_GET_BARCODE_BARCODE, "F_BARCODE");
}

bool TGetBarCodeDialog::IsValid()
{
	bool isValid = TQuickDialog::IsValid();

	if (isValid)
	{
		if (!*barcode->S())
			isValid = error("Баркода е задължителен.");
		else
			isValid = barcode_scan(barcode->S(), court, requestId, ces);
	}

	return isValid;
}

void TGetBarCodeDialog::SetupWindow()
{
	TQuickDialog::SetupWindow();

	timerId = SetTimer(0, 50, NULL);
}

void TGetBarCodeDialog::OKBNClicked()
{
	if (IsValid())
		CmOk();
}

void TGetBarCodeDialog::EvTimer(uint timerId)
{
#if TESTVER
	// 2017:097 switch it instead of displaying a warning
	if (GetFocus() == barcode->HWindow && get_layout() != LAYOUT_EN_US)
		set_layout_ll(LAYOUT_EN_US, false);
#endif  // TESTVER
	TQuickDialog::EvTimer(timerId);
}
