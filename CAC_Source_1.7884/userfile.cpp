#include "cac.h"

// ----- TUserFile -----------------------------------------------------------
TUserFile::TUserFile(const char *tWhat)
	: what(tWhat), rdwr(NULL), f(NULL)
{
#if RANGECHECK
	name = NULL;
#endif  // RANGECHECK
}

TUserFile::~TUserFile()
{
	if (f)
		Close();
}

FILE *TUserFile::Open(const char *name, const char *mode)
{
	if (!OpenFile(name, mode))
		Error("отваряне на");

	return f;
}

char *TUserFile::ReadAll(const char *name, long &size)
{
	char *buffer = NULL;

	if (Open(name, "rb"))
	{
		if (fseek(f, 0, SEEK_END) || (size = ftell(f)) < 0 || fseek(f, 0, SEEK_SET))
			Error("позициониране в");
		else
		{
			buffer = new char[size + 1];

			if (!size || fread(buffer, size, 1, f) == 1)
				buffer[size] = '\0';
			else
			{
				if (ferror(f))
					Error("четене на");
				else
					error("%s: грешка при четене на %s файл.", name, what);

				delete[] buffer;
				buffer = NULL;
			}
		}

		Close();
	}

	return buffer;
}

bool TUserFile::ReadAll(const char *name, memblock_t *m, bool text, long limit, long *mtime)
{
	bool result = false;

	if (Open(name, "rb"))
	{
		long size;

		if (fseek(f, 0, SEEK_END) || (size = ftell(f)) < 0 || fseek(f, 0, SEEK_SET))
			Error("позициониране в");
		else if (size > limit)
			error("%s: файлът е прекалено голям. Максималната големина е %ld мегабайта", name, limit >> 20);
		else
		{
			mbk_resize(m, size + text);

			if (!size || fread(m->data, size, 1, f) == 1)
			{
				if (text)
					m->data[size] = '\0';

				result = true;

				if (mtime)
				{
					struct stat statbuf;
					*mtime = fstat(fileno(f), &statbuf) == 0 ? stat2utime(statbuf.st_mtime) : 0;
				}
			}
			else
			{
				if (ferror(f))
					Error("четене на");
				else
					error("%s: грешка при четене на %s файл.", name, what);

				mbk_clear(m);
			}
		}

		Close();
	}

	return result;
}

bool TUserFile::WriteAll(const char *name, const void *data, long size, time_t mtime)
{
	return Open(name, "wb") && WriteAll(data, size, mtime);
}

const char *TUserFile::WriteTemp(const char *ext, const void *data, long size)
{
	OpenTemp("", ext, "wb", false);
	return WriteAll(data, size, 0) ? name : NULL;
}

bool TUserFile::Close()
{
	bool result = true;

	if (fflush(f))
		result = Error(rdwr);

	if (fclose(f))
		result = Error("затваряне на");

	f = NULL;
	return result;
}

bool TUserFile::Remove()
{
	return unlink(name) ? Error("изтриване на") : true;
}

bool TUserFile::Error(const char *func)
{
	mstr m;
	GetError(m, func);
	error("%s", str(m));
	return false;
}

bool TUserFile::Copy(const char *source, const char *target)
{
	return CopyFile(source, target, FALSE) ? true : error("%s: грешка %lu при копиране.", target, GETLASTERROR);
}

FILE *TUserFile::OpenTemp(const char *suffix, const char *ext, const char *mode, bool autoUnlink)
{
	static int pos = -1;

	if (pos == -1)
	{
		srand(time(NULL) + GetTickCount());
		pos = rand();
	}

	int i = pos;
	char base[SIZE_OF_BASE_NAME];

	what = "временен";

	do
	{
		i = ++i % HTML_TEMP_MAX;
		sprintf(base, "sa$tm%03d%s.%s", i, suffix, ext);
		build_fn(tempdir, base, temp);
		if (OpenFile(temp, mode))
			break;
	} while (i != pos);

	if (i == pos)
		fatal("%s: грешка %d при създаване на временен файл.", name, errno);

	if (autoUnlink)
		add_unlink_file(basename(name), false, false);

	pos = i;
	return f;
}

FILE *TUserFile::OpenFile(const char *tName, const char *mode)
{
	name = tName;
	f = fopen(tName, mode);

	if (f)
	{
		if (strchr(mode, 'r'))
			rdwr = "четене от";
		else if (strchr("FfVvWwВвФф", *what))
			rdwr = "запис във";
		else
			rdwr = "запис в";
	}

	return f;
}

void TUserFile::GetError(mstr &m, const char *func)
{
	m.printf("%s: грешка %d при %s %s файл.", name, errno, func, what);
}

bool TUserFile::WriteAll(const void *data, long size, time_t mtime)
{
	if (size == -1)
		size = strlen((const char *) data);

	if (fwrite(data, size, 1, f) != 1 || ferror(f))
	{
		Error("запис в");

		if (Close())
			remove_file(name, what, true);
	}
	else if (Close())
	{
		if (mtime)
		{
			struct utimbuf times = { mtime, mtime };
			utime(name, &times);
		}

		return true;
	}

	return false;
}

// ----- TTextFile -----------------------------------------------------------
TTextFile::TTextFile(const char *what)
	: TUserFile(what ? what : "текстов")
{
}

FILE *TTextFile::OpenTemp(const char *ext)
{
	return TUserFile::OpenTemp("", ext ? ext : "txt", "w", true);
}

FILE *TTextFile::ReopenTemp()
{
	f = fopen(name, "a");

	if (!f)
		fatal("%s: грешка %d при повторно отваряне на временен файл.", name, errno);

	return f;
}

void TTextFile::Save(const char *name, const char *text)
{
	if (Open(name, "wb"))
	{
		fwrite(text, 1, strlen(text), f);
		Close();
	}
}

bool TTextFile::Show(const char *name, TWaitWindow *wait)
{
	if (wait)
		wait->SetText("Стартиране на редактора...");

	return electric_action(name, TYPE_PLAIN, ACTION_EDIT);
}

bool TTextFile::Show(TWaitWindow *wait)
{
	if (f)
		Close();

	return Show(name, wait);
}

FILE *TTextFile::OpenRead(TWindow *parent, char *initialDir)
{
	if (!GetOpenFileName(parent, temp, "Текстови файлове", initialDir, "TXT", sizeof temp, NULL, false))
		return NULL;

	return Open(temp, "r");
}

FILE *TTextFile::OpenWrite(TWindow *parent, char *initialDir, const char *defaultName)
{
	if (!GetSaveFileName(parent, temp, "Текстови файлове", initialDir, "TXT", sizeof temp, defaultName, false))
		return NULL;

	return Open(temp, "w");
}

char *TTextFile::ReadAll()
{
	if (f)
		Close();

	long size;
	return TUserFile::ReadAll(name, size);
}

// ----- THTMLFile -----------------------------------------------------------
THTMLFile::THTMLFile(const char *what)
	: TUserFile(what ? what : "HTML"), header(false)
{
}

static constant html_header =
	"<!DOCTYPE html PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\">\r\n"
	"<html>\r\n"
	"<head>\r\n"
	"<meta http-equiv=Content-Type content=\"text/html; charset=windows-1251\">\r\n"
	"<meta name=ProgId content=Word.Document>\r\n"
	"<meta name=Generator content=\"Microsoft Word 9\">\r\n"
	"<meta name=Originator content=\"Microsoft Word 9\">\r\n"
	"<title>САС Съдебно Деловодство</title>\r\n"
	"</head>\r\n"
	"<body>\r\n"
	"<tt>\r\n";

const char *THTMLFile::OpenTemp()
{
	TUserFile::OpenTemp("", "htm", "wb", true);
	fputs(html_header, f);
	header = true;
	return name;
}

const char *THTMLFile::OpenOffice(bool header)
{
	// Note: this is different from this->header
	TUserFile::OpenTemp(header ? "_header" : "", "htm", "wb", false);
	return name;
}

bool THTMLFile::Close()
{
	if (header)
		fputs("</tt>\r\n</body>\r\n</html>\r\n", f);

	return TUserFile::Close();
}

DWORD THTMLFile::lastTicks = 0;

bool THTMLFile::Show(const char *name, TWaitWindow *wait, bool editor, bool office)
{
	if (wait)
		wait->SetText(editor ? "Стартиране на редактора..." : "Стартиране на браузъра...");

	while (GetTickCount() - lastTicks < HTML_OPEN_DELTA * 100)
	{
		Sleep(100);
		GetApplicationObject()->PumpWaitingMessages();
	}

#if TESTVER
	bool result = electric_action(name, office ? TYPE_OFFICE : TYPE_HTML, editor ? ACTION_EDIT : ACTION_VIEW);
#else  // TESTVER
	bool result = electric_action(name, office ? TYPE_TEXT : TYPE_HTML, editor ? ACTION_EDIT : ACTION_VIEW);
#endif  // TESTVER
	lastTicks = GetTickCount();
	return result;
}

bool THTMLFile::Show(TWaitWindow *wait, bool editor, sign office)
{
	if (f)
		Close();

	return Show(name, wait, editor, office == -1 ? header : office);
}

bool THTMLFile::SaveBody(const char *target)
{
	if (f)
		Close();

	long size;
	char *const data = TUserFile::ReadAll(name, size);
	TCharPtr CP(data);

	if (data)
	{
		constant start = strstr(data, "<body");

		if (start)
		{
			const char *final = strstr(start, "</body");

			if (final && (final = strchr(final, '>')) != NULL)
				return TUserFile::WriteAll(target, start, final - start + 1);
		}
	}

	return error("%s: невалидно съдържание.");
}

void THTMLFile::puts(FILE *f, const char *s, char br)
{
	while (*s)
	{
		if (*s == br)
			fputs("<br>\r\n", f);
		else
		{
			switch (*s)
			{
				case '<' : fputs("&lt;", f); break;
				case '>' : fputs("&gt;", f); break;
				case '&' : fputs("&amp;", f); break;
				//case '"' : fputs("&quot;", f); break;	// optional
				case 0xA0 : fputc(' ', f); break;		// ms-specific
				case '\a' : fputc('<', f); break;		// 2010:189 CAC
				case '\b' : fputc('>', f); break;		// 2010:189 CAC
				case 0x06 : fputs("&nbsp;", f); break;	// 2012:095 CAC
				case '\n' : fputc('\r', f);			// KEEP last, fallback
				default : fputc(*s, f);				// N.B. 128..191 as-is
			}
		}
		s++;
	}
}

void THTMLFile::vprintf(const char *format, va_list ap)
{
	mstr m;
	m.vprintf(format, ap);
	puts(str(m));
}

void THTMLFile::printf(const char *format, ...)
{
	va_list ap;

	va_start(ap, format);
	vprintf(format, ap);
	va_end(ap);
}

void THTMLFile::putd(const char *format, const CDate &date)
{
	if (!date.Empty())
		printf(format, &date);
}

const char *THTMLFile::OpenEMail()
{
	TUserFile::OpenTemp("", "htm", "wb+", false);
	return name;
}

void THTMLFile::ReadBack(char *s, long offset, int len)
{
	if (fseek(f, offset, SEEK_SET))
		Error("позициониране в");
	else
	{
		int n = fread(s, 1, len, f);

		if (n >= 0)
			s[n] = '\0';

		if (ferror(f))
			Error("четене на");
	}
}

bool THTMLFile::OpenRetry(const char *name)
{
	what = "временен";
	return TUserFile::OpenFile(name, "wb");
}

static constant ms_word_page_break =
	//"<p class=MsoNormal><span lang=BG><![if !supportEmptyParas]>&nbsp;<![endif]><o:p></o:p>\r\n"
	//"<p class=MsoNormal><span lang=BG>&nbsp;<o:p></o:p>\r\n"
	"<p class=MsoNormal><span lang=BG><o:p></o:p>\r\n"
	"<br clear=all style='mso-special-character:line-break;page-break-before:always'>\r\n"
	"</span></p>\r\n"
;

void THTMLFile::MsPageBr()
{
	fputs(ms_word_page_break, f);
}

// querydoc
void THTMLFile::qutn(const char *s)
{
	bool dirty = false;

	puts(s);
	while (*s)
	{
		if (*s == '\n')
			dirty = false;
		else if (!strchr(" \r\t", *s))
			dirty = true;
		s++;
	}

	if (dirty)
		putc('\n');
}

void THTMLFile::qutp(const char *s, size_t size)
{
	if (any(s))
	{
		if (size >= SIZE_OF_HTML_PARAGRAPH)
			fputs("</tt>", f);
		qutn(s);
		if (size >= SIZE_OF_HTML_PARAGRAPH)
			fputs("<tt>", f);
	}
}

void THTMLFile::qutt(int n)
{
	while (--n >= 0)
		fputs("&nbsp;", f);
}

void THTMLFile::font(bool small)
{
	fputs(small ? "</tt><hr><font size=\"-1\">" : "</font><tt>", f);
}

// ----- TXMLError -----------------------------------------------------------
TXMLError::TXMLError(const char *lead, const char *format, va_list ap)
	: silent(*format == '-')
{
	m.catn(lead, strlen(lead) > 10 ? 10 : strlen(lead));
	m.cat(": ");
	m.vprintf(format + silent, ap);
}

void TXMLError::Handle()
{
	log(LOG_EISPP, 0, str(m));
	error("%s", str(m));
}

void TXMLError::Handle(TRCDKeyContainer *container)
{
	log(LOG_EISPP, 0, container, str(m));

	if (!silent)
		error("%s", str(m));
}

void xml_error(const char *lead, const char *format, ...)
{
	va_list ap;
	TXMLError *x;

	va_start(ap, format);
	x = new TXMLError(lead, format, ap);
	va_end(ap);
	throw x;
}

void xml_error_now(TRCDKeyContainer *container, const char *lead, const char *format, ...)
{
	va_list ap;

	va_start(ap, format);
	TXMLError x(lead, format, ap);
	va_end(ap);

	x.Handle(container);
}

// ----- TXMLError2 ----------------------------------------------------------
TXMLError2::TXMLError2(const char *lead, const char *format, va_list ap)
	: TXMLError(lead, format, ap)
{
}

static void xml_error_2(const char *lead, const char *format, ...)
{
	va_list ap;
	TXMLError2 *x;

	va_start(ap, format);
	x = new TXMLError2(lead, format, ap);
	va_end(ap);
	throw x;
}

// ----- TXMLFile ------------------------------------------------------------
TXMLFile::TXMLFile(const char *what)
	: TUserFile(what ? what : "XML"), level(0)
{
}

constant TXMLFile::BOM = "\xEF\xBB\xBF";
constant TXMLFile::HEADER = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";

static unsigned short cp1251high[0x81] =
{
	0x2302,
	0x0402, 0x0403, 0x201A, 0x0453, 0x201E, 0x2026, 0x2020, 0x2021, 0x20AC, 0x2030, 0x0409, 0x2039, 0x040A, 0x040C, 0x040B,
	0x040F,
	0x0452, 0x2018, 0x2019, 0x201C, 0x201D, 0x2022, 0x2013, 0x2014, 0xFFFD, 0x2122, 0x0459, 0x203A, 0x045A, 0x045C, 0x045B,
	0x045F,
	0x00A0, 0x040E, 0x045E, 0x0408, 0x00A4, 0x0490, 0x00A6, 0x00A7, 0x0401, 0x00A9, 0x0404, 0x00AB, 0x00AC, 0x00AD, 0x00AE,
	0x0407,
	0x00B0, 0x00B1, 0x0406, 0x0456, 0x0491, 0x00B5, 0x00B6, 0x00B7, 0x0451, 0x2116, 0x0454, 0x00BB, 0x0458, 0x0405, 0x0455,
	0x0457,
	0x0410, 0x0411, 0x0412, 0x0413, 0x0414, 0x0415, 0x0416, 0x0417, 0x0418, 0x0419, 0x041A, 0x041B, 0x041C, 0x041D, 0x041E,
	0x041F,
	0x0420, 0x0421, 0x0422, 0x0423, 0x0424, 0x0425, 0x0426, 0x0427, 0x0428, 0x0429, 0x042A, 0x042B, 0x042C, 0x042D, 0x042E,
	0x042F,
	0x0430, 0x0431, 0x0432, 0x0433, 0x0434, 0x0435, 0x0436, 0x0437, 0x0438, 0x0439, 0x043A, 0x043B, 0x043C, 0x043D, 0x043E,
	0x043F,
	0x0440, 0x0441, 0x0442, 0x0443, 0x0444, 0x0445, 0x0446, 0x0447, 0x0448, 0x0449, 0x044A, 0x044B, 0x044C, 0x044D, 0x044E,
	0x044F
};

bool TXMLFile::OpenLL(mstr &e, const char *what, TRCDKeyContainer *container)
{
	if (!TUserFile::OpenFile(temp, "wb"))
	{
		mstr m;

		GetError(e, "отваряне на");
		m.printf("%s: грешка %d при отваряне на файл", what, errno);

		if (container)
			log(LOG_TRANSFER, 0, container, str(m));

		return false;
	}

	//fputs(BOM, f);
	fputs(HEADER, f);
	return true;
}

bool TXMLFile::OpenBank(mstr &e, const char *suffix, TRCDKeyContainer *container)
{
	static CTime last;	// avoid overwriting
	CTime now;
	mstr m;

	if (!any(XFER_BANKRUPT_PATH))
	{
		log(LOG_TRANSFER, 0, "bank: липсва BANKRUPT_PATH");
		e.cat("Не е зададена директория за трансфер на данни към ИСПН.");
		return false;
	}

	if ((now = Now()) == last)
		Sleep(100);
	last = now;

	m.printf("%04ld-%02d-%02d-%02d.%02d.%02d-%s-%s.xml", Today.year, Today.month, Today.day, now.hour, now.minute,
		now.second, CONFIG_NAME_LOCAL, suffix);
	build_fn(XFER_BANKRUPT_PATH, str(m), temp);

	return OpenLL(e, "bank", container);
}

bool TXMLFile::OpenLegal(mstr &e, const char *name, TRCDKeyContainer *container)
{
	strcpy(temp, name);
	return OpenLL(e, "legalaid", container);
}

bool TXMLFile::OpenSms(mstr &e, const char *name)
{
	strcpy(temp, name);
	return OpenLL(e, "sms", NULL);
}

void TXMLFile::Emit(const char *tag, const char *s)
{
	if (tag)
	{
		OpenTag(tag, false);
		Put(s);
		CloseTag(tag, false);
	}
	else
		Put(s);
}

void TXMLFile::Emit(const char *tag, long l)
{
	PrintF(tag, "%ld", l);
}

void TXMLFile::EmitM(const char *tag, double d)
{
	PrintF(tag, "%.2lf", d);
}

void TXMLFile::Emit(const char *tag, const CDate &date)
{
	PrintF(tag, "%04ld-%02d-%02d", date.year, date.month, date.day);
}

void TXMLFile::Emit(const char *tag, const CTime &time)
{
	if (!time.Empty())
		PrintF(tag, "%0lT", &time);
}

void TXMLFile::Emit(const char *tag, const CDate &date, const CTime &time)
{
	PrintF(tag, "%04ld-%02d-%02dT%0lT", date.year, date.month, date.day, &time);
}

void TXMLFile::EmitBase64(const char *tag, char *buffer, long size)
{
	OpenTag(tag);
	{
		int div = 0;

		for (char *ptr = buffer; size > 0; size -= 3, ptr += size > 0 ? 3 : 0)	// :0 for CG
		{
			static constant base64 = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

			fputc(base64[ptr[0] >> 2], f);
			fputc(base64[((ptr[0] & 0x03) << 4) | ((ptr[1] & 0xF0) >> 4)], f);
			fputc(size > 1 ? base64[((ptr[1] & 0x0F) << 2) | ((ptr[2] & 0xC0) >> 6)] : '=', f);
			fputc(size > 2 ? base64[ptr[2] & 0x3F] : '=', f);

			if ((++div % 25) == 0)
				fputc('\n', f);
		}

		if (div % 25)
			fputc('\n', f);
	}
	CloseTag(tag);
}

bool TXMLFile::EmitFile(const char *tag, const char *name, const char *what)
{
	TUserFile uf(what);
	long size;
	char *buffer = uf.ReadAll(name, size);

	if (buffer)
	{
		EmitBase64(tag, buffer, size);
		delete[] buffer;
		return true;
	}

	return false;
}

void TXMLFile::PrintF(const char *tag, const char *format, ...)
{
	va_list ap;
	mstr m;

	va_start(ap, format);
	m.vprintf(format, ap);
	va_end(ap);
	Emit(tag, str(m));
}

void TXMLFile::Remove()
{
	if (f)
		TUserFile::Close();

	if (rdwr)
		TUserFile::Remove();
}

#if RANGECHECK
bool TXMLFile::Close()
{
	if (level)
		error("%s: level %d at close", name, level);

	return TUserFile::Close();
}
#endif  // RANGECHECK

void TXMLFile::Indent()
{
	for (int i = 0; i < level; i++)
		fputc('\t', f);
}

void TXMLFile::OpenTag(const char *tag, bool user)
{
	Indent();
	level++;
	fprintf(f, "<%s>", tag);
	if (user)
		fputc('\n', f);
}

void TXMLFile::CloseTag(const char *tag, bool user)
{
	level--;
#if RANGECHECK
	if (level < 0)
		error("%s: negative level at </%s>", name, tag);
#endif  // RANGECHECK
	if (tag)
	{
		if (user)
			Indent();
			
		fprintf(f, "</%s>\n", tag);
	}
	else
		fputs("/>\n", f);
}

void TXMLFile::PutRaw(const char *s, bool indent)
{
	if (indent)
		Indent();

	fputs(s, f);
}

static constant x_esc[] = { "apos", "quot", "amp", "lt", "gt", NULL };
static constant x_rep = "'\"&<>";
static const char undef_1251 = 0x98;

void utf8_write(const char c, mstr &m)
{
	if (c <= 0x7E)
		m.cach(c);
	else
	{
		unsigned short u = cp1251high[c - 0x7F];

		if (u <= 0x7FF)
			m.cach((char) (0xC0 + (u >> 6)));
		else
		{
			m.cach((char) (0xE0 + (u >> 12)));
			m.cach((char) (0x80 + ((u >> 6) & 0x3F)));
		}

		m.cach((char) (0x80 + (u & 0x3F)));
	}
}

void TXMLFile::Put(const char *s)
{
	mstr m;

	while (*s)
	{
		char c = *s;
		int i = strindex(x_rep, c);

		if (i != INT_MAX)
		{
			m.printf("&%s;", x_esc[i]);
		}
		else if (c < ' ')
		{
			static constant cntrl = "\x09\x0A\x0D";

			if (strchr(cntrl, c))
				m.printf("&#%d;", c);
		}
		else
		{
			utf8_write(c, m);
		}

		s++;
	}

	fputs(str(m), f);
}

char *TXMLFile::ReadAll(const char *name)
{
	long size;
	char *tText = TUserFile::ReadAll(name, size);

	if (tText)
	{
		char *text;

		try
		{
			text = Parse(strncmp(tText, BOM, 3) ? tText : tText + 3);
			delete[] tText;
			return text;
		}
		catch (...)
		{
			delete[] tText;
			throw;
		}
	}

	if (errno == 2)
		xml_error_2(basename(name), "-read error %d", 2);
	else
		xml_error(basename(name), "-read error %d", errno);

	return NULL;
}

bool TXMLFile::OpenWrite(const char *name)
{
	if (TUserFile::Open(name, "wb"))
	{
		fputs(BOM, f);
		fputs(HEADER, f);
		return true;
	}

	return false;
}

void TXMLFile::OpenObject(const char *name)
{
	Indent();
	level++;
	fprintf(f, "<%s", name);
}

char *TXMLFile::Parse(const char *tText)
{
	const char *lead = tText;
	char last = '?';
	int level = 0;
	char *text = new char[strlen(tText) + 1];
	char *p = text;

	for (const char *s = tText; *s; s++)
	{
		switch (*s)
		{
			case '\'' :
			case '"' :
			{
				char quot = *s;

				do
				{
					*p++ = *s++;

					if (*s == '\0')
						xml_error(lead, "parse %c expected", quot);

				} while (*s != quot);

				break;
			}
			case '<' :
			{
				const char *suffix = NULL;
				lead = s;

				if (!strncmp(s, "<!--", 4))
					suffix = "-->";
				else if (s[1] == '?')	// or header only?
					suffix = "?>";

				if (suffix)
				{
					if ((s = strstr(s, suffix)) == NULL)
						xml_error(lead, "parse %s expected", suffix);

					s += strlen(suffix) - 1;	// for(++)
					last = '>';
					continue;
				}
				else
					level++;

				break;
			}
			case '>' :
			{
				if (--level < 0)
					xml_error(lead, "level -1");
				break;
			}
		}

		if (!isspace(*s))
			*p++ = *s;
		else if (isalnum(last) && isalnum(s[1]))
			*p++ = ' ';

		last = *s;
	}

	if (level)
		xml_error(basename(name), "EOF level %d", level);

	*p = '\0';

	return text;
}

static char to_cp1251(int u, const char *)
{
	if (u > 0)
	{
		if (u <= 0x7E)
			return (char) u;

		for (int i = 0; i < 0x81; i++)
			if (cp1251high[i] == u)
				return (char) (i + 0x7F);
	}

	// ex(message);
	return undef_1251;
}

void utf8_parse(char *text)
{
	for (char *s = text; *s; s++)
	{
		char *end = NULL;

		if (*s == '&')
		{
			end = strchr(s, ';');

			if (!end)
				xml_error(s, "amp ; expected");

			*end = '\0';

			if (s[1] == '#')
			{
				int u, n;

				if (sscanf(s + 2, "%d%n", &u, &n) != 1 || n != strlen(s + 2))
					xml_error(s, "bad amp code");

				*s = to_cp1251(u, "amp code outside 1251");
			}
			else
			{
				int i;

				for (i = 0; x_esc[i]; i++)
				{
					if (!strcmp(x_esc[i], s + 1))
					{
						*s = x_rep[i];
						break;
					}
				}

				if (!x_esc[i])
					xml_error(s, "unknown amp");
			}
		}
		else	// utf-8, 16-bit unicode range
		{
			if ((s[0] & 0xC0) == 0xC0 && (s[1] & 0xC0) == 0x80)
			{
				int u;

				if ((s[0] & 0x20) && (s[2] & 0xC0) == 0x80)
				{
					u = ((s[0] & 0x0F) << 12) + ((s[1] & 0x3F) << 6) + (s[2] & 0x3F);
					end = s + 2;
				}
				else
				{
					u = ((s[0] & 0x1F) << 6) + (s[1] & 0x3F);
					end = s + 1;
				}

				*s = to_cp1251(u, "unichar outside 1251");
			}
		}

		if (end)
			strmove(s + 1, end + 1);
	}
}
