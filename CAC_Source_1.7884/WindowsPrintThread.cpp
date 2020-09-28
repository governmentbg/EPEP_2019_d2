//{{TWindowsPrintThread Implementation}}

TWindowsPrintThread::TWindowsPrintThread(TCancelPrintDialog *tDialog)
:
	dialog(tDialog), Data(new TPrintDialog::TData), Error(1), prnDC(0)
{
	// from TPrinter::GetDefaultPrinter()
	Data->Flags |= PD_RETURNDC;
	TPrintDialog printDialog(NULL, *Data);
	if (!printDialog.GetDefaultPrinter())
		Data->Error = PDERR_NODEFAULTPRN;
}

TWindowsPrintThread::~TWindowsPrintThread()
{
	delete prnDC;
	delete Data;

	static constant text[5] =
	{
		"Грешка при печат",
		"Печатът е прекъснат",
		"Печатът е прекъснат в Print Manager",
		"Няма достатъчно дисково пространство",
		"Няма достатъчно памет"
	};

	if (Error & SP_NOTREPORTED)
	{
		if ((Error = -Error - 1) > 4)
			Error = 0;
		error("Документът \"%s\" не е отпечатан. %s.", printout->GetTitle(), text[Error]);//error("Документът не е отпечатан. %s.", text[Error]);
	}
}

void TWindowsPrintThread::CalcBandingFlags()
{
	if (UseBandInfo)
	{
		TBandInfo bandInfo;
		unsigned flags = 0;

		prnDC->BandInfo(bandInfo);
		if (bandInfo.HasGraphics)
			flags = pfGraphics;
		if (bandInfo.HasText)
			flags |= pfGraphics;
		Flags = (Flags & ~pfBoth) | flags;
	}
	else
	{
		if (
			FirstBand && !BandRect.left && !BandRect.top &&
			BandRect.right == PageSize.cx &&
			BandRect.bottom == PageSize.cy
		)
			Flags = pfText;
		else if ((Flags & pfBoth) == pfGraphics)
			Flags = (Flags & ~pfBoth) | pfGraphics;
		else
			Flags = Flags | pfBoth;
	}
	FirstBand = false;
}

bool TWindowsPrintThread::ExecPrintDialog(TWindow* parent)
{
	Data->Flags |= PD_USEDEVMODECOPIESANDCOLLATE;
	Data->Flags &= ~PD_COLLATE;

	return TPrintDialog(parent, *Data).Execute() == IDOK;
}

bool TWindowsPrintThread::Init(TWindow* parent, TPrintout *tPrintout, bool tPrompt)
{
	printout = tPrintout;
	prompt = tPrompt;

	// from TPrinter::Print()
	printout->GetDialogInfo(Data->MinPage, Data->MaxPage, selFromPage, selToPage);

	if (selFromPage)
	{
		//Data->Flags &= ~PD_NOSELECTION;
		Data->FromPage = selFromPage;
		Data->ToPage   = selToPage;
	}
	else
	{
		//Data->Flags |= PD_NOSELECTION;
		Data->FromPage = 0;
		Data->ToPage = 999;
	}

	if (Data->MinPage)
	{
		Data->Flags &= ~PD_NOPAGENUMS;
		if (Data->FromPage < Data->MinPage)
			Data->FromPage = Data->MinPage;
		else if (Data->FromPage > Data->MaxPage)
			Data->FromPage = Data->MaxPage;
		if (Data->ToPage < Data->MinPage)
			Data->ToPage = Data->MinPage;
		else if (Data->ToPage > Data->MaxPage)
			Data->ToPage = Data->MaxPage;
	}
	else
		Data->Flags |= PD_NOPAGENUMS;

	if (prompt)
	{
		Data->Flags |= PD_RETURNDC | PD_NOSELECTION;
		Data->Flags &= ~(PD_RETURNDEFAULT | PD_PRINTSETUP);

		bool ok = ExecPrintDialog(parent);

		if (!ok && Data->Error == PDERR_DEFAULTDIFFERENT)
		{
			Data->ClearDevMode();
			Data->ClearDevNames();
			ok = ExecPrintDialog(parent);
		}
		if (!ok)
			return false;

		prnDC = Data->TransferDC();
	}
	else
		prnDC = new TPrintDC(Data->GetDriverName(), Data->GetDeviceName(), Data->GetOutputName(), Data->GetDevMode());

	if (!prnDC)
	{
		Error = SP_ERROR;
		return false;
	}

	// from TPrinter::SetPageSizes()
	PageSize.cx = prnDC->GetDeviceCaps(HORZRES);
	PageSize.cy = prnDC->GetDeviceCaps(VERTRES);
	PageSizeInch.cx = prnDC->GetDeviceCaps(LOGPIXELSX);
	PageSizeInch.cy = prnDC->GetDeviceCaps(LOGPIXELSY);

	printout->SetPrintParams(prnDC, PageSize);

	return true;
}

int TWindowsPrintThread::Run()
{
	dialog->SetPrinterName(Data->GetDeviceName());

	bool banding = printout->WantBanding() && (prnDC->GetDeviceCaps(RASTERCAPS) & RC_BANDING);

	if (banding)
		UseBandInfo = ToBool(prnDC->QueryEscSupport(BANDINFO));
	else
		BandRect.Set(0, 0, PageSize.cx, PageSize.cy);

	int copiesPerPass = 1;
	int tempCopiesPerPass = copiesPerPass;
	//int Copies = (Data->GetDevMode()->dmFields & DM_COPIES) ? Data->GetDevMode()->dmCopies : Data->Copies;
	int Copies = Data->Copies;

	if (!(Data->Flags & PD_COLLATE))
		prnDC->SetCopyCount(Copies, copiesPerPass);

	int fromPage;
	int toPage;

	if (prompt && (Data->Flags & PD_SELECTION) || selFromPage)
	{
		fromPage = selFromPage;
		toPage = selToPage;
	}
	else if (prompt && (Data->Flags & PD_PAGENUMS))
	{
		fromPage = Data->FromPage;
		toPage = Data->ToPage;
	}
	else if (Data->MinPage)
	{
		fromPage = Data->MinPage;
		toPage = Data->MaxPage;
	}
	else
	{
		fromPage = 1;
		toPage = INT_MAX;
	}

	dialog->SetPageCount(toPage - fromPage + 1);

	// Copies loop, one pass per block of document copies.
	//
	printout->BeginPrinting();
	for (int copies = Copies; copies > 0 && Error > 0; copies -= tempCopiesPerPass)
	{
		// On last multi-copy pass, may need to adjust copy count
		//
		if (copiesPerPass > 1 && copies < copiesPerPass)
			prnDC->SetCopyCount(copies, copiesPerPass);

		// Whole document loop, one pass per page
		//
		Flags = pfBoth;
		Error = prnDC->StartDoc(printout->GetTitle(), 0);
		printout->BeginDocument(fromPage, toPage, Flags);

		for (int pageNum = fromPage; Error > 0 && pageNum <= toPage && printout->HasPage(pageNum); pageNum++)
		{
			dialog->SetPageNumber(pageNum);

			// Begin the page by getting the first band or calling StartPage()
			//
			if (banding)
			{
				FirstBand = true;
				Error = prnDC->NextBand(BandRect);
			}
			else
				Error = prnDC->StartPage();

			// Whole page loop, one pass per band (once when not banding)
			//
			while (Error > 0 && !BandRect.IsEmpty())
			{
				GetApplicationObject()->PumpWaitingMessages();

				if (ShouldTerminate())
					if ((Error = prnDC->AbortDoc()) > 0)
						Error = 0;

				if (banding)
				{
					CalcBandingFlags();
					if (printout->WantForceAllBands() && (Flags & pfBoth) == pfGraphics)
						prnDC->SetPixel(TPoint(0, 0), 0);  // Some old drivers need this
					prnDC->DPtoLP(BandRect, 2);
				}

				printout->PrintPage(pageNum, BandRect, Flags);

				if (banding)
					Error = prnDC->NextBand(BandRect);
				else
					break;
			}

			GetApplicationObject()->PumpWaitingMessages();

			if (ShouldTerminate())
				if ((Error = prnDC->AbortDoc()) > 0)
					Error = 0;

			// EndPage (NEWFRAME) need only called if not banding
			//
			if (Error > 0 && !banding)
			{
				Error = prnDC->EndPage();
				if (Error == 0)    // a zero return here is OK for this call
					Error = 1;
			}
		}  // End of Whole Document-loop

		// Tell GDI the document is finished
		//
		if (Error > 0)
			prnDC->EndDoc();

		printout->EndDocument();
	} // End of Copy-loop

	printout->EndPrinting();

	if (copiesPerPass > 1)
		prnDC->SetCopyCount(1, copiesPerPass);

	dialog->CloseDialog(IDOK);

	return Error > 0;
}
