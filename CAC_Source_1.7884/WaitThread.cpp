#include "cac.h"

DEFINE_RESPONSE_TABLE1(TWaitThreadDialog, TScaleDialog)
//{{TWaitThreadDialogRSP_TBL_BEGIN}}
	EV_BN_CLICKED(IDCANCEL, CancelBNClicked),
	EV_WM_CLOSE,
//{{TWaitThreadDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TWaitThreadDialog Implementation}}

TWaitThreadDialog::TWaitThreadDialog(int resId)
	: TScaleDialog(AppMainWindow, resId), initialized(false)
{
	exceptTerminate = true;
	header = new TStatic(this, IDC_WAIT_THREAD_HEADER);
	progress = new TGauge(this, IDC_WAIT_THREAD_PROGRESS);
	text = new TStatic(this, IDC_WAIT_THREAD_TEXT);
	cancel = new TPushButton(this, IDCANCEL, IDI_CANCEL);
	TWaitThread::SetWTDL(this);
}

TWaitThreadDialog::~TWaitThreadDialog()
{
	TWaitThread::SetWTDL(NULL);
}

void TWaitThreadDialog::StartStep()
{
	wantBreak = false;
	minPBVal = 0;
	maxPBVal = dfltMaxPBVal;
	lastText = 0;

	progress->SetRange(minPBVal, maxPBVal);
	progress->SetValue(0);
	header->SetText("");
	text->SetText(WAIT);
	cancel->EnableWindow(true);

	if (initialized)
	{
		PumpWaitingMessages();
		ShowWindow(SW_SHOW);
		SetActiveWindow();
	}
	else
	{
		Create();

		if (!CENTER_PROGRESS_BAR)
		{
			TRect rect = GetWindowRect();
			MoveWindow(AdjustedRect.left, AdjustedRect.top, rect.Width(), rect.Height(), true);
		}
	}
}

void TWaitThreadDialog::SetHeader(const char *tHeader)
{
	header->SetText(tHeader);
	PumpWaitingMessages();
}

void TWaitThreadDialog::SetProgressRange(int min, int max)
{
	minPBVal = min;
	maxPBVal = max;

	if (maxPBVal <= minPBVal)
		maxPBVal = minPBVal + 1;

	SetProgress(minPBVal + 1);
	SetProgress(minPBVal);
}

void TWaitThreadDialog::SetProgress(int value)
{
	if (minPBVal != maxPBVal)
	{
		value = ((value - minPBVal) * dfltMaxPBVal) / (maxPBVal - minPBVal);
		progress->SetValue(value);
		PumpWaitingMessages();
	}
}

void TWaitThreadDialog::SetText(const char *tText)
{
	DWORD now = GetTickCount();

	if (now - lastText >= WATTICK_TICK)
	{
		text->SetText(tText);
		PumpWaitingMessages();
		lastText = now;
	}
}

void TWaitThreadDialog::PleaseWait()
{
	text->SetText(PLEASE_WAIT);
	PumpWaitingMessages();
}

void TWaitThreadDialog::AlmostDone()
{
	text->SetText(ALMOST_DONE);
	PumpWaitingMessages();
}

void TWaitThreadDialog::CheckBreak()
{
#if RANGECHECK
	if (!initialized)
		fatal("wtdl not initialized");
#endif  // RANGECHECK
	PumpWaitingMessages();

	if (!wantBreak && !cancel->IsWindowEnabled())
	{
		if (ask("Наистина ли желаете да прекъснете\r\nизпълняваната операция/създаването\r\nна документа?"))
		{
			wantBreak = true;
			text->SetText("Моля, изчакайте завършването на започнатата стъпка...");
			lastText = UINT_MAX;
		}
		else
		{
			cancel->EnableWindow(true);
			SetActiveWindow();
		}

		PumpWaitingMessages();
	}
}

void TWaitThreadDialog::GetText(char *s, int maxChars)
{
	text->GetText(s, maxChars);
}

void TWaitThreadDialog::PumpWaitingMessages()
{
	if (initialized)
	{
		MSG msg;

		while (::PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
			GetApplicationObject()->ProcessMsg(msg);
	}
}

void TWaitThreadDialog::SetupWindow()
{
	TScaleDialog::SetupWindow();
	initialized = true;
}

void TWaitThreadDialog::CancelBNClicked()
{
	cancel->EnableWindow(false);
}

void TWaitThreadDialog::EvClose()
{
	CancelBNClicked();
}

// ----- TWaitThread ---------------------------------------------------------
TWaitThread::TWaitThread()
{
	wtdl->StartStep();
}

TWaitThreadDialog *TWaitThread::wtdl = NULL;

int TWaitThread::Run()
{
	Execute();
	return 0;
}

void TWaitThread::ExecWait()
{
	int i = 0;

	Start();

	while (GetStatus() == Running)
	{
		WaitForExit(70);

		if (!(i % 70))
			wtdl->CheckBreak();
	}
}

void TWaitThread::Tick(const bool completed)
{
	if (completed)
	{
		// Update the progress bar value
	}
	else
	{
		// Update current item texta
	}
}

#if RANGECHECK
void TWaitThread::SetWTDL(TWaitThreadDialog *tWTDL)
{
	if ((wtdl == NULL) == (tWTDL == NULL))
		fatal("replacing wtdl %lx with %lx", (unsigned long) wtdl, (unsigned long) tWTDL);

	wtdl = tWTDL;
}
#endif  // RANGECHECK

// ----- TCheckupThread ------------------------------------------------------
TCheckupThread::TCheckupThread(const int generalRowTicks, const int createOutDocTicks):
  barLowVal(0), barHiVal(100), generalRowSteps(generalRowTicks),
  createOutDocSteps(createOutDocTicks), currentTickRate(1),
  lastShownVal(0), startingVal(0), endingVal(0), totalPartSteps(0),
  currentPartStep(0), passeds(PASSEDS),
  lastGossipTime(CTime()),
  reportFinishedItems(true), generateRes(false)
{
}

bool TCheckupThread::ArgsOk()
{
	return barLowVal < barHiVal && currentTickRate > 0;
}

bool TCheckupThread::Generate()
{
	generateRes = false;

	if (ArgsOk())
	{
		generateRes = true;

		try
		{
			ExecWait();
		}
		CATCH_ANYTHING
	}

	return generateRes;
}

void TCheckupThread::Tick(const bool completed)
{
  if(wtdl->WantBreak())
  {
    return;
  }
  if(currentTickRate > 0)
  {  // -- 1 --
    if(completed == false)
    {  // -- 1 --
	if(totalPartSteps > 0)
	{  // -- 2 --
	  if(currentPartStep % currentTickRate == 0)
	  {  // -- 3 --
	    const int valInterval = endingVal - startingVal;
	    const double cPSDouble = currentPartStep;
	    int currentVal =
		(cPSDouble / totalPartSteps) * valInterval;

	    currentVal += startingVal;
//          info("currentVal: %d\nlastShownVal: %d\ncurrentPartStep: %d",
//          	currentVal, lastShownVal, currentPartStep);
	    if(currentVal > lastShownVal || currentPartStep == 0)
	    {  // -- 4 --
		const CTime thisMoment = Now();

		if(lastGossipTime.second != thisMoment.second ||
		   lastGossipTime.minute != thisMoment.minute ||
		   lastGossipTime.hour != thisMoment.hour ||
               currentPartStep == 0)
            {  // -- 5 --
              {
                char currentItemBuffer[64];
                const int cps =
                	currentPartStep + (reportFinishedItems ? 0 : 1);

                ostrstream(currentItemBuffer, sizeof(currentItemBuffer))
                  << (passeds ? passeds : "") << " "
                  << cps << " от "
                  << (totalPartSteps >= cps ? totalPartSteps : cps)
                  << " ..." << ends;
		    currentItemBuffer [sizeof(currentItemBuffer) - 1] = '\0';
		    wtdl->SetText(currentItemBuffer);
		  }
              if(currentVal > lastShownVal)
              {
		    lastShownVal = currentVal;
		    wtdl->SetProgress(lastShownVal);
		  }
              lastGossipTime = thisMoment;
            }  // -- 5 --
          }  // -- 4 --
        }  // -- 3 --
      }  // -- 2 --
	currentPartStep ++;
    }  // -- 1 --
    else
    {
	lastShownVal = endingVal;
	wtdl->SetProgress(lastShownVal);
	startingVal = lastShownVal;
    }
  }  // -- 0 --
}

void TCheckupThread::MakeGeneralRow()
{
}

void TCheckupThread::CreateOutDoc()
{
}
