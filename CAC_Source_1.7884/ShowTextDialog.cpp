DEFINE_RESPONSE_TABLE1(TShowTextDialog, TShowDialog)
//{{TShowTextDialogRSP_TBL_BEGIN}}
	EV_BN_CLICKED(IDPRINT, PrintBNClicked),
	EV_BN_CLICKED(IDLEFT, LeftBNClicked),
	EV_BN_CLICKED(IDRIGHT, RightBNClicked),
	EV_BN_CLICKED(IDSAVE, SaveBNClicked),
	EV_EN_SETFOCUS(IDC_SHOW_TEXT_EDIT, EditSetFocus),
	EV_BN_CLICKED(IDCANCEL, CancelBNClicked),
	EV_WM_CLOSE,
//{{TShowTextDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TShowTextDialog Implementation}}

TShowTextDialog::TShowTextDialog(TWindow* parent, char *tText, TPrinterGroup *tConfig, int resId)
:
	TShowDialog(parent, CHROME_TEXT_MAXIMIZED, resId), config(tConfig), maxLen(strlen(tText) + 0x1000),
		text(new char[maxLen + 1]), printed(false)
{
	strcpy(text, tText);
	delete[] tText;

	if (!config)
	{
		config = new TPrinterGroup;
		config->Load();
	}

	// 2007:085 LPR: copied from showtext
	{
		const char *s;
		char *end;

		for (s = text; *s; s++)
			if (isupper(*s) && islocal(*s))
				break;
		if (*s)
		{
			end = title;
			do
			{
				// 2007:032 LPR: to ignore (un)signed chars
				// 2009:269 LPR: do not touch or printers will fall
				if ((*s >= 32 && *s <= 126) || islocal(*s))
				{
					*end++ = *s;
					*end = '\0';
				}
				if (*++s == '\b')
					s++;
			} while (end - title < sizeof title - 1 && *s && *s != '\n');
		}
		else
			sprintf(title, "Документ #%d", random(32720));
	}

	autoSize = edit = new TEdit(this, IDC_SHOW_TEXT_EDIT, maxLen);
	autoDown[0] = new TButton(this, IDPRINT);
	autoDown[1] = new TButton(this, IDLEFT);
	autoDown[2] = new TButton(this, IDRIGHT);
	autoDown[3] = new TButton(this, IDSAVE);
	autoDown[4] = new TButton(this, IDCANCEL);
}

TShowTextDialog::~TShowTextDialog()
{
	delete config;
	delete[] text;
}

void TShowTextDialog::SetupWindow()
{
	TShowDialog::SetupWindow();

	mstr m;
	m.printf("%s - %s", title, AppNameName);
	SetCaption(str(m));

	edit->Transfer(text, tdSetData);
}

int TShowTextDialog::CountPages()
{
	const char *s = text;
	int pages = 0;

	while ((s = strchr(s, '\f')) != NULL)
	{
		pages++;
		s++;
	}

	return pages;
}

void TShowTextDialog::Print(int part)
{
	edit->Transfer(text, tdGetData);

	if (config->windows)
	{
		TCancelPrintDialog dialog(this, title);
		TWindowsPrintThread *thread = new TWindowsPrintThread(&dialog);
		TTextPrintout *printout = new TTextPrintout(title, text, config, CountPages(), part, thread, &dialog);

		if (thread->Init(this, printout, config->setupex))
		{
			dialog.Create();
			dialog.ShowWindow(SW_SHOW);
			GetApplicationObject()->PumpWaitingMessages();
			thread->Start();
		}

		// delete thread first, it's destructor uses printout title
		delete thread;
		delete printout;
		// 2007:241 LRQ: always set printed
		printed = true;
	}
	else if (part == PRINT_BOTH)
	{
		TCancelPrintDialog dialog(this, title);
		TDirectPrintThread *thread = new TDirectPrintThread(text, config, &dialog);

		dialog.SetPageCount(CountPages());
		if (thread->Init())
		{
			TThread::THandle hThread = thread->Start();

			if (hThread)
			{
				int iter;

				// 2007:241 LRQ: always set printed
				//printed = dialog.Execute() == IDOK;
				dialog.Execute();
				for (iter = 10; iter > 0; iter--)
					if (!thread->WaitForExit(100))
						break;

				if (!iter)
				{
					TerminateThread(hThread, printed);
					thread->WaitForExit();
				}
			}
			else
				error("Грешка %lu при стартиране на задача.", GETLASTERROR);
		}
		delete thread;
		// 2007:241 LRQ: always set printed
		printed = true;
	}
	else
		error("Директният печат не поддържа леви и десни страници.");

	// 2016:271 LPR: prevent lose focus
	if (!GetActiveWindow)
		SetActiveWindow();
}

void TShowTextDialog::Save(const char *name)
{
	TTextFile tf;

	edit->Transfer(text, tdGetData);
	tf.Save(name, text);
}

void TShowTextDialog::PrintBNClicked()
{
	Print(PRINT_BOTH);
}

void TShowTextDialog::LeftBNClicked()
{
	Print(PRINT_LEFT);
}

void TShowTextDialog::RightBNClicked()
{
	Print(PRINT_RIGHT);
}

void TShowTextDialog::SaveBNClicked()
{
	char name[SIZE_OF_PATH];

	if (GetSaveFileName(this, name, "Текстови файлове", OUT_PATH, "TXT", sizeof name, NULL, false))
		Save(name);
}

void TShowTextDialog::EditSetFocus()
{
	edit->SetSelection(0, 0);
}

void TShowTextDialog::CancelBNClicked()
{
	EvClose();
}

void TShowTextDialog::EvClose()
{
	CloseDialog(printed ? IDOK : IDCANCEL);
}
