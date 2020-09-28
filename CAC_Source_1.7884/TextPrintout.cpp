//{{TTextPrintout Implementation}}

TTextPrintout::TTextPrintout(const char* title, const char *tText, const TPrinterGroup *tConfig, int tPages, int part, const TWindowsPrintThread *tPrinter, TCancelPrintDialog *tDialog)
:
	TPrintout(title), pages(tPages), start(NULL),
	points(12), maxcols(0), config(tConfig), text(tText),
	left(part != PRINT_RIGHT), right(part != PRINT_LEFT),
	split(part != PRINT_BOTH),
	printer(tPrinter), dialog(tDialog), brush(TColor(0, 0, 0)),
	warned(!tDialog),
	standard(NULL), bold(NULL), underline(NULL), large(NULL)
{
	// 2007:081 from show text - we have alignment here, so adjust it
	if (part == PRINT_LEFT)
		alignment = RIGHT_ALIGNMENT;
	else if (part == PRINT_RIGHT)
		alignment = LEFT_ALIGNMENT;
	// 2007:337 FIX: backward compatibility
	else if (config->alignto == FLOAT_ALIGNMENT)
		alignment = DEFAULT_ALIGNMENT;
	else
		alignment = config->alignto;

	const char *s = text;
	start = new const char *[pages + 1];
	for (int i = 0; i < pages; i++, s++)
	{
		start[i] = s;
		s = strchr(s, '\f');
	}

	start[pages] = text + strlen(text);
}

TTextPrintout::~TTextPrintout()
{
	delete large;
	delete underline;
	delete bold;
	delete standard;
	delete[] start;
}

static const char *get_line(const char *text, char *buffer, size_t size)
{
	const char *eoln = strchr(text, '\n');

	if (!eoln)
		eoln = strchr(text, '\f');

	if (!eoln)
		eoln = text + strlen(text);

	int len = eoln - text;

	if (len >= size)
	{
		len = size;
		error("Документът съдържа ред по-дълъг от %d символа.", --len);
	}

	memcpy(buffer, text, len);
	buffer[len] = '\0';

	char *scan;

	for (char c = 0x01; c < 0x20; c++)
	{
		if (!strchr("\a\b\t\v", c))
			while ((scan = strchr(buffer, c)) != NULL)
				memmove(scan, scan + 1, strlen(scan));
	}

	return eoln;
}

static size_t less_len(const char *s)
{
	const char *backspace = s;
	size_t len = strlen(s);

	while ((backspace = strchr(backspace, '\b')) != NULL)
	{
		len--;
		if (*++backspace)
			len--;
	}

	return len;
}

static TFont *new_font(TDC *DC, int points, int weight, bool underline)
{
	char *family = "Courier New";

	if (weight == FW_AUTOMATIC)
		weight = points >= 12 ? FW_MEDIUM : FW_NORMAL;

	TFont *font = new TFont
	(
		family,
		MulDiv(-points, DC->GetDeviceCaps(LOGPIXELSY), 72),
		0,
		0,
		0,
		weight,
		DEFAULT_PITCH | FF_DONTCARE,
		false,				// italic
		underline,
		false,				// strikeout
		RUSSIAN_CHARSET//,
		//OUT_DEFAULT_PRECIS,
		//CLIP_DEFAULT_PRECIS,
		//(uint8) (quality ? PROOF_QUALITY : DEFAULT_QUALITY)
	);

	return font;
}

enum
{
	_LT = 0x01,
	_RT = 0x02,
	_UP = 0x04,
	_DN = 0x08,

	_LU = _DN + _RT,
	_HZ = _LT + _RT,
	_RU = _DN + _LT,
	_VT = _UP + _DN,
	_HD = _HZ + _DN,
	_VR = _VT + _RT,
	_CT = _HZ + _VT,
	_VL = _VT + _LT,
	_LD = _UP + _RT,
	_HU = _HZ + _UP,
	_RD = _UP + _LT,

	_NW = 0x00
};

static char pseudo_dir[] = { _LU, _HZ, _RU, _VT, _HD, _VR, _CT, _VL, _LD, _HU, _RD, _NW };

void TTextPrintout::TextOut(TRect *band, int x, int y, const char *s, int len)
{
	int rex = band->left + x + push - pull;

	if (x >= skip && rex < band->right)
		DC->TextOut(rex, band->top + y, s, len);
}

void TTextPrintout::FillRect(TRect *band, int x1, int y1, int x2, int y2)
{
	int rex1 = band->left + x1 + push - pull;
	int rex2 = band->left + x2 + push - pull;

	if (x1 >= skip && rex1 < band->right)
		DC->FillRect(rex1, band->top + y1, rex2, band->top + y2, brush);
}

void TTextPrintout::PrintPage(int page, TRect& rect, uint)
{
	const char *text;
	const char *eoln;
	const char *final;
	int lines = 0;
	int width, height;
	int page_width, page_height;
	char buffer[SIZE_OF_LINE];
	char *scan;
	TEXTMETRIC tm;
	int old_points = points;
	TRect band = rect;

	text = start[--page];
	final = strchr(text, '\f');

	if (alignment == FLOAT_ALIGNMENT)
	{
		const DEVMODE *devmode = printer->GetData()->GetDevMode();
		alignment = ((devmode->dmFields & DM_ORIENTATION) && devmode->dmOrientation == DMORIENT_LANDSCAPE) ? TOP_ALIGNMENT : LEFT_ALIGNMENT;
	}

	if (alignment == LEFT_ALIGNMENT)
		band.left += config->justify;
	else if (alignment == RIGHT_ALIGNMENT)
		band.right -= config->justify;
	else if (alignment == TOP_ALIGNMENT)
		band.top += config->justify;
	else if (alignment == BOTTOM_ALIGNMENT)
		band.bottom -= config->justify;

	do
	{
		if (++lines > config->pagelen)
		{
			error("Страница %d съдържа повече от %d реда.", page + 1, lines = config->pagelen);
			break;
		}

		eoln = get_line(text, buffer, sizeof buffer);

		if (less_len(buffer) > maxcols)
			maxcols = less_len(buffer);

	} while ((text = ++eoln) < final);

	if (!standard)
		standard = new_font(DC, points, FW_AUTOMATIC, false);

	for (;;)
	{
		// 2007:256 FIX: we RestoreFont() at end, so select always 
		DC->SelectObject(*standard);
		DC->GetTextMetrics(tm);
		width = tm.tmAveCharWidth;
		height = tm.tmHeight + tm.tmExternalLeading;

		page_width = (split ? (maxcols + 1) / 2 : maxcols) * width;
		page_height = config->pagelen * height;

		if (page_width <= band.Width() && page_height <= band.Height())
		{
			if (alignment == CENTER_ALIGNMENT && band.Width() - page_width >= config->justify / 10)
				band.left += (band.Width() - page_width) / 2;
			break;
		}

		if (!--points)
		{
			if (!warned)
			{
				message(MB_ICONWARNING | MB_OK, "Няма шрифт с който документа да се отпечата коректно.");
				warned = true;
			}
			points++;
			break;
		}

		DC->RestoreFont();
		delete standard;

		standard = new_font(DC, points, FW_AUTOMATIC, false);
	}

	// 2007:116
	if (points != old_points || !bold)
	{
		delete bold;
		bold = new_font(DC, points, FW_BOLD, false);
		delete underline;
		underline = new_font(DC, points, FW_AUTOMATIC, true);
		delete large;
		large = new_font(DC, points * 2, FW_AUTOMATIC, false);
	}

	if (split)
	{
		if (left)
		{
			skip = 0;
			push = (band.Width() - page_width);
			pull = 0;
		}
		else	// right
		{
			skip = page_width;
			push = 0;
			pull = page_width;
		}
	}
	else
	{
		skip = 0;
		push = 0;
		pull = 0;
	}

	text = start[page];

	for (int line = 0; line < lines; line++)
	{
		GetApplicationObject()->PumpWaitingMessages();
		eoln = get_line(text, buffer, sizeof buffer);

		// pass1: directives
		scan = buffer;
		while ((scan = strchr(scan, '\b')) != NULL)
		{
			if (scan == buffer)
				error("Ред %d започва с директива.", line + 1);
			else
			{
				TFont *font = standard;

				switch (scan[1])
				{
					case '_' : font = underline; break;
					case ' ' : font = large; break;
					case '\0' :
					{
						error("Ред %d завършва с директива.", line + 1);
						break;
					}
					default :
					{
						if (scan[1] == scan[-1])
							font = bold;
						else
							error("Ред %d съдържа неизвестна директива.", line + 1);
					}
				}

				DC->SelectObject(*font);
				TextOut(&band, (scan - 1 - buffer) * width, line * height, scan - 1, 1);
				scan[-1] = ' ';
			}
			if (scan[1])
				memmove(scan, scan + 2, strlen(scan) - 1);
			else
				*scan = '\0';
		}

		// pass2: pseudographics
		for (scan = buffer; *scan; scan++)
		{
			const char *psd = strchr(pseudo_map, *scan);

			if (psd != NULL)
			{
				int dir = pseudo_dir[psd - pseudo_map];
				int x1 = (scan - buffer) * width;
				int y1 = line * height;
				int xc = x1 + width / 2;
				int yc = y1 + height / 2;
				int x2 = x1 + width - 1;
				int y2 = y1 + height - 1;
				int rw = (config->linewidth ? config->linewidth : width / 16) / 2;

				if (dir & _LT)
					FillRect(&band, x1, yc - rw, xc + 1, yc + rw + 1);
				if (dir & _RT)
					FillRect(&band, xc, yc - rw, x2 + 1, yc + rw + 1);
				if (dir & _UP)
					FillRect(&band, xc - rw, y1, xc + rw + 1, yc + 1);
				if (dir & _DN)
					FillRect(&band, xc - rw, yc, xc + rw + 1, y2 + 1);

				*scan = ' ';
			}
		}


		// pass3: normal text
		DC->SelectObject(*standard);

		for (scan = buffer; *scan; scan++)
			if (*scan != ' ')
				TextOut(&band, (scan - buffer) * width, line * height, scan, 1);

		text = ++eoln;
	}

	DC->RestoreFont();
}

bool TTextPrintout::HasPage(int pageNumber)
{
	return pageNumber > 0 && pageNumber <= pages;
}

void TTextPrintout::GetDialogInfo(int& minPage, int& maxPage, int& selFromPage, int& selToPage)
{
	minPage = selFromPage = pages ? 1 : 0;
	maxPage = selToPage = pages;
}

void TTextPrintout::error(const char *format, ...)
{
	va_list ap;

	if (dialog)
		dialog->ShowWindow(SW_HIDE);
	va_start(ap, format);
	vmessage(MB_ERROR, format, ap);
	va_end(ap);
	if (dialog)
		dialog->ShowWindow(SW_SHOW);
}
