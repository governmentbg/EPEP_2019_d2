DEFINE_RESPONSE_TABLE1(TOutRegSendBaseDialog, TTroubleDialog)
//{{TOutRegSendBaseDialogRSP_TBL_BEGIN}}
	EV_EN_UPDATE(IDC_OUTREG_SEND_DATE, DateENUpdate),
	EV_BN_CLICKED(IDC_GENERIC_SEND_NONE, PostalBNClicked),
	EV_BN_CLICKED(IDC_GENERIC_SEND_REGULAR, PostalBNClicked),
	EV_BN_CLICKED(IDC_GENERIC_SEND_R, PostalBNClicked),
	EV_BN_CLICKED(IDC_GENERIC_SEND_AR, PostalBNClicked),
	EV_BN_CLICKED(IDC_GENERIC_SEND_PACKAGE, PostalBNClicked),
//{{TOutRegSendBaseDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TOutRegSendBaseDialog Implementation}}

TOutRegSendBaseDialog::TOutRegSendBaseDialog(TWindow* parent, TOutReg *outReg, TInReg *tInReg, const char *kinds,
	TConnect **tConnects, bool edit, int resId)
:
	TTroubleDialog(parent, outReg, edit, TYPE_TEXT, resId), inReg(tInReg), connects(tConnects), initialized(false)
{
	kind = new TCharAliasFace(this, IDC_OUTREG_SEND_KIND, "F_KIND", Kind, kinds);
	no = new TLongFace(this, IDC_OUTREG_SEND_NO, "F_NO", SIZE_OF_NO);
	date = new TDateFace(this, IDC_OUTREG_SEND_DATE, "F_DATE");
	text = new TStringFace(this, IDC_OUTREG_SEND_TEXT, "F_TEXT", SIZE_OF_OUTREG_REMARK);
	lawsuitNo = new TLongFace(this, IDC_OUTREG_SEND_LAWSUIT_NO, "F_LAWSUIT_NO", SIZE_OF_NO);
	lawsuitYear = new TYearFace(this, IDC_OUTREG_SEND_LAWSUIT_YEAR, "F_LAWSUIT_YEAR");
	new TLongFace(this, IDC_OUTREG_SEND_IN_NO, "F_IN_NO", SIZE_OF_NO);
	new TYearFace(this, IDC_OUTREG_SEND_IN_YEAR, "F_IN_YEAR");
	add_postal_buttons(this, buttons);
	print = new TPushButton(this, IDPRINT);
	// 2009:173 +related
	vssIndexLabel = new TStatic(this, IDC_OUTREG_SEND_VSS_INDEX_LABEL);
	vssIndex = new TVSSIndexAliasFace(this, IDC_OUTREG_SEND_VSS_INDEX, "F_VSS_INDEX");
	deliveryNotice = new TCheckFace(this, IDC_OUTREG_SEND_DELIVERY_NOTICE, "F_DELIVERY_NOTICE");
}

bool TOutRegSendBaseDialog::IsValid()
{
	bool isValid = TTroubleDialog::IsValid();

	if (isValid)
	{
		// 2009:168 LPR/LRQ
		if (kind->C() == KIND_OTHER_OUT_DOCUMENT && !any(text))
			isValid = error("Полетата вид документ и текст не си съответстват.");
		// 2013:070 LPR/LRQ
		else if (kind->C() == KIND_OTHER_OUT_DOCUMENT && strlen(text->S()) >= SIZE_OF_ORIGIN_TEXT)
			isValid = error("Прекалено дълъг текст за този вид документ.");
	}

	return isValid;
}

void TOutRegSendBaseDialog::SetupWindow()
{
	TTroubleDialog::SetupWindow();

	initialized = true;
	DateENUpdate();
	vssIndex->SetS(OutReg()->vssIndex);
	PostalBNClicked();
}

void TOutRegSendBaseDialog::DateENUpdate()
{
	if (initialized && OutReg()->receiverType == UCN_INSTANCE_LOWER && date_valid(date->S(), true))
	{
		TConnect *connect = connects[connects[1] && !strcmp(receiver->S(), connects[1]->ucn)];
		char type = connectKind2Type(connect->connectKind);

		if (type == TYPE_REQUEST)
			type = OutReg()->college;

	#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
		const char *types = type == TYPE_PUNISHMENT_LAW ? C2S[TYPE_PUNISHMENT_LAW] : NULL;
	#endif  // APPEAL || AREAL || MILITARY || REGIONAL
	#if COURT_TYPE == COURT_ADMIN
		const char *types = type == TYPE_TRICKY_LAW ? C2S[TYPE_PUNISHMENT_LAW] : CONNECT_KIND_LAW_TYPES;
	#endif  // ADMIN

		Court->Seek(atoi(connect->ucn));	// == OutReg()->receiver
		vssIndex->SetCrit(Court->courtType, types, '\0', date->Date());
	}
}

void TOutRegSendBaseDialog::PostalBNClicked()
{
	if (buttons[IDC_POSTAL_INDEX_NONE]->GetCheck() == BF_CHECKED ||
		buttons[IDC_POSTAL_INDEX_REGULAR]->GetCheck() == BF_CHECKED)
	{
		deliveryNotice->EnableWindow(false);
		deliveryNotice->SetCheck(BF_UNCHECKED);
	}
	else
		deliveryNotice->EnableWindow(true);
}
