//{{TDirectPrintThread Implementation}}

TDirectPrintThread::TDirectPrintThread(const char *tText, const TPrinterGroup *tConfig, TCancelPrintDialog *tDialog)
	: text(tText), config(tConfig), dialog(tDialog), laser(false), hPrinter(INVALID_HANDLE_VALUE)
{
	sprintf(lpt, "LPT%d:", (config->port % 4) + 1);
	//strcpy(lpt, "a.txt");
}

TDirectPrintThread::~TDirectPrintThread()
{
	if (hPrinter != INVALID_HANDLE_VALUE && !CloseHandle(hPrinter))
		error("%s грешка %lu при затваряне.", lpt, GETLASTERROR);
}

bool TDirectPrintThread::Init()
{
	//laser = true;
	if (config->check)
	{
		PRINTER_INFO_2 printers[48];
		DWORD needed, returned;

		if (EnumPrinters(PRINTER_ENUM_LOCAL, NULL, 2, (BYTE *) printers, sizeof printers, &needed, &returned))
		{
			for (int i = 0; i < returned; i++)
			{
				if (strstr(printers[i].pPortName, lpt))
				{
					if (!ask("%s на този порт има WINDOWS принтер. Желаете ли да продължите?", lpt))
						return false;
					laser = true;
					break;
				}
			}
		}
	}

	hPrinter = OpenExisting(lpt, GENERIC_WRITE, 0, FILE_FLAG_WRITE_THROUGH);
	if (hPrinter == INVALID_HANDLE_VALUE)
		return error("%s грешка %lu при отваряне.", lpt, GETLASTERROR);

	return true;
}

static char pseudo_map[] = { 171, 151, 187, 166, 147, 145, 134, 146, 139, 132, 155, 000 };
static char pseudo_mik[] = { 218, 196, 207, 211, 194, 195, 197, 212, 192, 193, 217, 000 };
static char pseudo_asc[] = { '+', '-', '+', '|', '+', '+', '+', '+', '+', '+', '+', 000 };

int TDirectPrintThread::Run()
{
	char c;
	char *map;
	bool ignore_next = false;
	int page = 0;
	bool newPage = true;

	dialog->SetPrinterName(lpt);
	for (const char *s = text; (c = *s) != '\0'; s++)
	{
		// MAN-like directive corrections
		if (ignore_next && !strchr("\f\n\r", c))
			ignore_next = false;
		else if (laser && (c & 0xE0) == 0x00 && !strchr("\a\f\n\r\t\v", c))
			ignore_next = (c == '\b');
		else
		{
			if (newPage && (c & 0xE0))
			{
				dialog->SetPageNumber(++page);
				newPage = false;
			}
			// Pseudographic correction
			if ((map = strchr(pseudo_map, c)) != NULL)
				c = ((config->pseudo && !config->cp1251) ? pseudo_mik : pseudo_asc)[map - pseudo_map];
			// Cyrilic correction
			else if (!config->cp1251)
				if (c == 0x80 + ' ')
					c = ' ';
				else if ((c & 0x80) != 0)
					c ^= 0x40;

			bool failed = true;
			for (;;)
			{
				DWORD written;

				if (!WriteFile(hPrinter, &c, 1, &written, NULL))
				{
					const char *e;

					switch (GetLastError())
					{
						case 2 : e = " (липсващо устройство)"; break;
						case 5 : e = " (достъпът е отказан)"; break;
						case 21 : e = " (устройството не е готово)"; break;
						case 28 : e = " (няма хартия)"; break;
						case 123 : e = " (невалидно име на устройство)"; break;
						case 233 : e = " (липсващ изходен процес)"; break;
						case 1167 : e = " (липсващо устройство)"; break;
						default : e = "";
					}
					dialog->ShowWindow(SW_HIDE);
					if (ask("%s грешка %lu при печат%s. Желаете ли да опитате отново?", lpt, GETLASTERROR, e))
						dialog->ShowWindow(SW_SHOW);
					else
						break;
				}
				else if (!written)
				{
					dialog->ShowWindow(SW_HIDE);
					if (ask("%s грешка при печат. Желаете ли да опитате отново?", lpt))
						dialog->ShowWindow(SW_SHOW);
					else
						break;
				}
				else
				{
					failed = false;
					break;
				}
			}
			if (failed)
				break;
		}
		if (c == '\f')
			newPage = true;
	}
	dialog->CmOk();

	return true;
}
