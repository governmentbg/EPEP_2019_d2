DEFINE_RESPONSE_TABLE1(TOutRegReceiveBaseDialog, TTroubleDialog)
//{{TOutRegReceiveBaseDialogRSP_TBL_BEGIN}}
	EV_BN_CLICKED(IDOK, OKBNClicked),
	EV_BN_CLICKED(IDPRINT, PrintBNClicked),
	EV_BN_CLICKED(IDRIGHT, RightBNClicked),
//{{TOutRegReceiveBaseDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TOutRegReceiveBaseDialog Implementation}}

TOutRegReceiveBaseDialog::TOutRegReceiveBaseDialog(TWindow* parent, TOutReg *outReg, const char *tResults, int resId)
:
	TTroubleDialog(parent, outReg, !outReg->returned.Empty(), TYPE_IMAGE, resId), results(tResults),
		higher(outReg->receiverType == UCN_INSTANCE_HIGHER), oldRestart(outReg->flagsX & OUTREG_X_RESTART)
{
	electricDate = &outReg->returned;
	header1 = new TStatic(this, IDC_OUTREG_RECEIVE_HEADER_1);
	header2 = new TStatic(this, IDC_OUTREG_RECEIVE_HEADER_2);
	returned = new TDateFace(this, IDC_OUTREG_RECEIVE_RETURNED, "F_RETURNED");
	result = new TCharAliasFace(this, IDC_OUTREG_RECEIVE_RESULT, "F_RESULT", Result, results);
	remark = new TStringFace(this	, IDC_OUTREG_RECEIVE_REMARK_VZ, higher ? "F_REMARK_VZ" : "F_REMARK", SIZE_OF_OUTREG_REMARK);
	registry = new TCheckFace(this, IDC_OUTREG_RECEIVE_REGISTRY, "F_REGISTRY");
	restart = new TCheckFace(this, IDC_OUTREG_RECEIVE_RESTART, "F_FLAGS_RESTART");
	new TTwinButton(this, IDPRINT, IDRIGHT);
}

void TOutRegReceiveBaseDialog::OKBNClicked()
{
	if (IsValid())
	{
		*OutReg() << this;

		if (OutReg()->Update())
		{
			// 2003:171 date->Date() -> returned->Date()
			trouble.Move(OutReg(), returned->Date());
			CloseDialog(registry->GetCheck() == BF_CHECKED ? IDREGISTRY : IDOK);
		}
	}
}

void TOutRegReceiveBaseDialog::Print(long flags)
{
	if (IsValid())
	{
		*OutReg() << this;
		//print_one_outreg(this, OutReg());
		print_outreg(this, OutReg(), flags);
	}
}

void TOutRegReceiveBaseDialog::PrintBNClicked()
{
	Print(BOOK_NEW_FORMAT);
}

void TOutRegReceiveBaseDialog::RightBNClicked()
{
	Print(BOOK_OLD_FORMAT);
}

bool TOutRegReceiveBaseDialog::IsValid()
{
	bool isValid = TTroubleDialog::IsValid();

	if (isValid)
	{
		CDate tReturned = returned->Date();

		if (returned->Date() < OutReg()->date)
			isValid = error("Датата на връщане/отговор е по-малка от датата на изпращане.");
		else if (returned->Date() > Today)
			isValid = error("Датата на връщане/отговор е по-голяма от текущата дата.");
		// 2008:298 LRQ; last check; 2009:044 LPR: interferes with print, moved to OK
		//else if (vssIndex->IsWindowEnabled() && !*vssIndex->S())
		//	isValid = ask("Не е попълнен резултат - индекс за справки за ВСС. Желаете ли да продължите?"));
		// 2017:037 LPR: generic restart check function
		else if (already_restarted(OutReg()->lawsuit, oldRestart, restart->GetCheck() == BF_CHECKED, tReturned))
			isValid = error("Делото вече започва отначало на %Dг.", &tReturned);
	}

	return isValid;
}

void TOutRegReceiveBaseDialog::SetupWindow()
{
	TTroubleDialog::SetupWindow();

#if TESTVER  // 2017:111
	mstr m;

	if (OutReg()->lawsuit.type)
		m.printf("%s ", Type->Seek(OutReg()->lawsuit.type));

	if (OutReg()->lawsuit.no)
		m.printf("%ld", OutReg()->lawsuit.no);

	if (OutReg()->lawsuit.year)
	{
		if (OutReg()->lawsuit.no)
			m.cat("/");

		m.printf("%ldг.", OutReg()->lawsuit.year);
	}

	header1->SetCaption(str(m));
	m.clear();
	m.printf("%s Изх. № %ld/%ldг., изпратен на %Dг.", Kind->Seek(OutReg()->kind), OutReg()->key.no, OutReg()->key.year,
		&OutReg()->date);
	header2->SetCaption(str(m));
#else  // TESTVER
	char s[0x100];

	if (OutReg()->lawsuit.type)
		sprintf(s, "%s ", Type->Seek(OutReg()->lawsuit.type));
	else
		*s = '\0';

	if (OutReg()->lawsuit.no)
		scprintf(s, "%ld", OutReg()->lawsuit.no);

	if (OutReg()->lawsuit.year)
	{
		if (OutReg()->lawsuit.no)
			strcat(s, "/");
		scprintf(s, "%ldг.", OutReg()->lawsuit.year);
	}
	header1->SetCaption(s);

	sprintf(s, "%s Изх. № %ld/%ldг., изпратен на %Dг.", Kind->Seek(OutReg()->kind), OutReg()->key.no, OutReg()->key.year,
		&OutReg()->date);
	header2->SetCaption(s);
#endif  // TESTVER

	// 2012:045 FIX: was disabled in the new instance scheme; LRQ: always enabled
	//registry->EnableWindow(resultI->IsWindowEnabled() || resultKS->IsWindowEnabled());
	// 2016:277 IRQ/TRQ/FIX: allow lawsuits and '\0', reject requests
	registry->EnableWindow(strchr(TYPE_LAWSUITS, OutReg()->lawsuit.type));
	returned->EnableWindow(!edit);
	// 2009:286 LPR: HIGHER only; 2010:334 LPR: LAWSUITS only; 2011:175 RESTARTS
	if (strchr(UCN_INSTANCE_RESTARTS, OutReg()->receiverType) && strchr(TYPE_LAWSUITS, OutReg()->lawsuit.type))
		restart->EnableWindow(true);
}
