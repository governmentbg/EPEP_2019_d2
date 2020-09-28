#include "cac.h"

void SetWaitCursor(bool wait)
{
	::SetCursor(::LoadCursor(NULL, wait ? IDC_WAIT : IDC_ARROW));
}

bool atob(const char *ucn)
{
	return atof(ucn) != 0;
}

long atobc(constant ucns[], size_t count)
{
	long result = 0;

	for (int i = 0; i < count; i++)
		if (atob(ucns[i]))
			result++;

	return result;
}

long atobc(const char *ucn1, const char *ucn2, ...)
{
	long result = 0;

	if (atob(ucn1))
		result++;

	if (ucn2)
	{
		if (atob(ucn2))
			result++;

		va_list ap;
		const char *ucn;

		va_start(ap, ucn2);

		while ((ucn = va_arg(ap, const char *)) != NULL)
			if (atob(ucn))
				result++;

		va_end(ap);
	}

	return result;
}

void catset(char *s, const char *set)
{
	// from criteria
	if (strlen(set) < 2)
		scprintf(s, "= '%c'", *set);
	else
	{
		strcat(s, "IN (");

		while (*set)
		{
			strcat(s, "'");
			strcach(s, *set);
			strcat(s, "'");
			if (*++set)
				strcat(s, ", ");
		}

		strcat(s, ")");
	}
}

// BC++ 5.01 RTL: converts GetLastError() to errno and returns -1
extern "C" int __NTerror(void);

bool exist(const char *path, bool file)
{
	struct stat statbuf;

	if (stat(path, &statbuf))
	{
		// LPR: stat() fails on network volumes/resources, try again
		DWORD dwAttributes;

		if ((dwAttributes = GetFileAttributes(path)) == 0xFFFFFFFF)
		{
			__NTerror();
			return false;
		}

		if (!(dwAttributes & FILE_ATTRIBUTE_DIRECTORY) != file)
		{
			errno = file ? EISDIR : ENOTDIR;
			return false;
		}

		return true;
	}

	if (!S_ISDIR(statbuf.st_mode) != file)
	{
		errno = file ? EISDIR : ENOTDIR;
		return false;
	}

	return true;
}

bool exist_document(const char *name)
{
	return exist(name) ? true : error("Документът %s липсва или е недостъпен (грешка %d).", name, errno);
}

char *globcmpi(char *text, const char *patt)
{
	while (*patt)
	{
		bool match;

		switch (*patt)
		{
			case ' ' :
			{
				const char *beg;
				int n;

				for (n = 0; isspace(*patt); patt++)
					n += 3;
				for (beg = text; text - beg < n && isspace(*text); text++);
				if (text == beg)
					return NULL;

				continue;
			}
			case '&' : match = isalpha(*text); break;
			case '$' : match = isdigit(*text); break;
			case '?' : match = any(text); break;
			case '*' :
			{
				int n;
				char *end;

				for (n = 0; *patt == '*'; patt++)
					n += 10;

				do
				{
					if (!any(text))
						return NULL;
					if ((end = globcmpi(++text, patt)) != NULL)
						return end;
				} while (--n > 0);

				return NULL;
			}
			case '~' : match = any(text) && !isalnum(*text); break;
			case '\'' :
			case '"' : match = any(text) && strchr(FILTER_QUOTES, *text); break;
			case '-' : match = any(text) && strchr(FILTER_MINUSES, *text); break;
			case '#' :
			{
				static constant Nos[] = { "№", "No.", "No", "#", "номер", NULL };
				constant *no;

				for (no = Nos; *no; no++)
					if (!strncmpi(text, *no, strlen(*no)))
						break;

				if (*no)
				{
					text += strlen(*no);
					patt++;
					continue;
				}

				return NULL;
			}
			default : match = toupper(*text) == toupper(*patt);
		}

		if (!match)
			return NULL;

		text++;
		patt++;
	}

	return text;
}

bool intersect(const char *s1, const char *s2)
{
	for (; *s1; s1++)
		if (strchr(s2, *s1))
			break;
			
	return any(s1);
}

bool not_inset(const char *test, const char *full)
{
	const char *s;

	for (s = test; *s; s++)
		if (!strchr(full, *s))
			break;

	return any(s);
}

int scprintf(char *buffer, const char *format, ...)
{
	va_list ap;
	int result;

	va_start(ap, format);
	result = vsprintf(buffer + strlen(buffer), format, ap);
	va_end(ap);
	return result;
}

char *split_half(char *s)
{
	s += strlen(s) / 2;

	while (*s && !isspace(*s))
		s++;

	if (*s)
		*s++ = '\0';

	return s;
}

char *split_at(char *s, int len)
{
	int _len = strlen(s);

	if (_len > len)
	{
		if (_len  >= len * 2)
			return split_half(s);

		for (char *p = s + len; p > s; p--)
		{
			if (isspace(*p))
			{
				*p = '\0';
				return ++p;
			}
		}
	}

	return "";
}

void strcach(char *s, char c)
{
	char ts[2] = { c, '\0' };
	strcat(s, ts);
}

void strcach_nx(char *s, char c)
{
	if (!strchr(s, c))
		strcach(s, c);
}

void strcat_nx(char *s, const char *chars)
{
	while (*chars)
		strcach_nx(s, *chars++);
}

bool strcatell(char *buffer, const char *s, const char *sep, int size)
{
	if (*s)
	{
		if (*buffer && *sep && !strcatell(buffer, sep, "", size))
			return false;

		int len = strlen(buffer);

		if (strlen(s) + len < size)
			strcat(buffer, s);
		else
		{
			memcpy(buffer + len, s, size - len - 1);
			strcpy(buffer + size - 3 - 1, "...");
			return false;
		}
	}

	return true;
}

void stremove(char *from, const char *what)
{
	for (char *s = from; *s; )
	{
		if (strchr(what, *s))
			strmove(s, s + 1);
		else
			s++;
	}
}

void stremove(char *from, char what)
{
	stremove(from, C2S[what]);
}

void strimall(char *buffer)
{
	char *s;

	for (s = buffer; *s && isspace(*s); s++);
	strmove(buffer, s);
	strimtrail(buffer);
}

void strimtrail(char *buffer)
{
	char *end = buffer;

	for (char *s = buffer; *s; s++)
		if (!isspace(*s))
			end = s + 1;

	*end = '\0';
}

bool strinex(const char *inex, char c)
{
	return strcmp(inex, "!") && (!any(inex) || (strchr(inex, c) != NULL) == (*inex != '!'));
}

bool strinex2(const char *inex, char c1, char c2)
{
	return strcmp(inex, "!") && (!any(inex) || (strchr(inex, c1) || strchr(inex, c2)) == (*inex != '!'));
}

bool strmatch(const char xCrit, const char *xData)
{
	// 2003:302 BRW: should do the same as the old strmatch()
	return !xData || (xCrit && strchr(xData, xCrit));
}

bool strmatch(const char *xCrit, const char *xData)
{
	// N.B. don't touch, and especially, don't "optimize"
	if (!xCrit)
	{
		if (xData)
			fatal("Unable to apply xData %s to NULL", xData);
		return true;
	}

	return strmatch(*xCrit, xData);
}

char *strpad(char *s, int len)
{
	while (strlen(s) < len)
		strcat(s, " ");

	return s;
}

char *strsetlen(char *s, int len)
{
	if (strlen(s) > len)
		s[len] = '\0';
	else if (strlen(s) < len)
		strpad(s, len);

	return s;
}

char *strzcpy(char *s, const char *ts, size_t size)
{
	strncpy(s, ts, size - 1);
	s[size - 1] = '\0';
	return s;
}

// ----- sum_in_words --------------------------------------------------------
static void inWords1(unsigned int i1, unsigned int i2, unsigned int i3, unsigned int x, long male, mstr &text)
{
	static constant text0[] = { "", "едно ", "две ", "три ", "четири ", "пет ", "шест ", "седем ", "осем ", "девет ",
		"десет "};
	static constant text1[] = { "", "", "двадесет ", "тридесет ", "четиридесет ", "петдесет ", "шестдесет ", "седемдесет ",
		"осемдесет ", "деветдесет " };
	static constant text2[] = { "", "сто ", "двеста ", "триста ", "четиристотин ", "петстотин ", "шестстотин ",
		"седемстотин ", "осемстотин ", "деветстотин " };
	static constant text3[] = { "", "хиляди ", "милиона ", "милиарда ", "трилиона " };
	static constant text4[] = { "десет ", "единадесет ", "дванадесет ", "тринадесет ", "четиринадесет ", "петнадесет ",
		"шестнадесет ", "седемнадесет ", "осемнадесет ", "деветнадесет " };
	static constant text5[] = { "", "хилядa ", "милион ", "милиард ", "трилион " };

	char s1[32], s2[32], s3[32], s4[32];
	unsigned int val;

	s3[0] = s4[0] = '\0';
	s1[0] = s2[0] = '\0';

	strcpy(s1, text2[i1]);

	if (i2 == 1)
	{
		strcpy(s2, text4[i3]);
		i3 = 0;
	}
	else
	{
		strcpy(s2, text1[i2]);
		strcpy(s3, text0[i3]);
	}

	val = i1 * 100 + i2 * 10 + i3;

	if (i3 == 1)
	{
		if (x == 1)
		{
			if (val == 1)
			{
				s3[0] = '\0';
				strcpy(s4, text5[1]);
			}
			else
			{
				strcpy(s3, "една ");
				strcpy(s4, text3[1]);
			}
		}
		else
		{
			if (val == 1)
				strcpy(s4, text5[x]);
			else
				strcpy(s4, text3[x]);

			if (x)
				strcpy(s3, "един ");
			else
				strcpy(s3, male == MFO_MALE ? "един " : male == MFO_FEMALE ? "една " : "едно ");
		}
	}
	else if (i3 == 2)
	{
		if (x == 1)
		{
			strcpy(s3, "две ");
			strcpy(s4, text3[1]);
		}
		else
		{
			if (x)
				strcpy(s3, "два ");
			else
				strcpy(s3, male == MFO_MALE ? "два " : "две ");

			strcpy(s4, text3[x]);
		}
	}
	else if (val)
		strcpy(s4, text3[x]);

	if (i3)
	{
		if (val > 10)
			strcat(s2, "и ");
	}
	else if (i2)
	{
		if(val > 100)
			strcat(s1, "и ");
	}

	text.cat(s1);
	text.cat(s2);
	text.cat(s3);
	text.cat(s4);
}

static void inWords(const char *num, mstr &text, long male)
{
	mstr temp;
	unsigned int x, y, z, i, i1, i2, i3;

	z = strlen(num);
	i = 0;

	while (z)
	{
		i1 = i2 = i3 = 0;
		temp.clear();
		x = (z - 1) / 3;
		y = (z - 1) % 3;

		switch (y)
		{
			case 2 : i1 = num[i++] - '0';
			case 1 : i2 = num[i++] - '0';
			case 0 : i3 = num[i++] - '0';
		}

		inWords1(i1, i2, i3, x, male, temp);

		if (x == 0 && text[0] && temp[0] && temp.index(" и ") == INT_MAX)
			text.cat("и ");

		text.cat(temp);
		z -= (y + 1);
	}

	if (text[0] == '\0')
		text.cat("нула ");
}

void sum_in_words(const double sum, mstr &m, const char *currency, bool zerofrac)
{
	mstr numTxt;
	mstr buff;

	Currency->Seek(currency ? currency : CURRENCY_BGN);

	numTxt.printf("%.2lf", sum);
	char *decimals = strchr(ncstr(numTxt), '.');

	*decimals++ = '\0';

	inWords(str(numTxt), buff, Currency->integerMfo);
	m.cat(buff);
	m.cat(Currency->Integer(numTxt));

	if (zerofrac || atol(decimals))
	{
		m.cat(" и ");
		buff.clear();
		inWords(decimals, buff, Currency->fractionMfo);
		m.cat(buff);
		m.cat(Currency->Fraction(decimals));
	}
}

long stat2utime(long st_time)
{
	return mktime(gmtime(&st_time));
}

static bool semipad(char *buffer)
{
	size_t len = strlen(buffer);

	if (*buffer != ';')
	{
		if (strlen(buffer) >= LENGTH_OF_KEYWORDS)
			return false;

		strmove(buffer + 1, buffer);
		*buffer = ';';
		len++;
	}

	if (buffer[len - 1] != ';')
	{
		if (len >= LENGTH_OF_KEYWORDS)
			return false;

		strcpy(buffer + len, ";");
	}

	return strlen(buffer) <= LENGTH_OF_KEYWORDS;
}

bool alnumdif(char c1, char c2)
{
	return isalnum(c1) && isalnum(c2) && isalpha(c1) != isalpha(c2);
}

static void linkalnum(char *buffer)
{
	for (char *s = buffer + 1; *s; )
	{
		if (isspace(*s) && alnumdif(s[-1], s[1]))
			strmove(s, s + 1);
		else
			s++;
	}
}

bool kowpress(char *buffer)
{
	if (any(buffer))
	{
		for (char *s = buffer + 1; *s; s++)
			if (*s == ';' && s[-1] == ';')
				s[-1] = ' ';

		squeeze(buffer);
		linkalnum(buffer);
		compress(buffer);
		strimall(buffer);
		return semipad(buffer);
	}

	return true;
}

bool absolute(const char *path)
{
	if (PathIsRelativeA)
		return !PathIsRelativeA(path);

	if (strlen(path) < 3)
		return false;

	if (!memcmp(path, "\\\\", 2))
		return !strchr(":\\/", path[2]);

	return isalpha(*path) && path[1] == ':' && strchr("\\/", path[2]);
}

char *tocp1251(const wchar_t *ws)
{
	size_t size = lstrlenW(ws) + 1;
	char *s = new char[size];
	int result = WideCharToMultiByte(1251, 0, ws, size, s, size, "?", NULL);

	if (result == FALSE)
	{
		delete[] s;
		s = NULL;
	}

	for (int i = 0; i < result - 1; i++)
	{
		if (CHROME_UNICODE_CONVERT == 2 && strchr("?", s[i]) && i < size - 1)
		{
			struct TUniConv
			{
				wchar_t w;
				char c;
			};

			static const TUniConv UniConvs[] =
			{
				{ 0x0405, 'S' },
				{ 0x0406, 'I' },
				{ 0x0408, 'J' },
				{ 0x0455, 's' },
				{ 0x0456, 'i' },
				{ 0x0458, 'j' },
				{ 0x040D, 'Й' },
				{ 0x045D, 'й' },
				{ 0x04C0, 'I' },
				{ 0x04CF, 'l' },
				{ 0, '\0' }
			};

			const TUniConv *conv;

			for (conv = UniConvs; conv->w; conv++)
			{
				if (conv->w == ws[i])
				{
					s[i] = conv->c;
					break;
				}
			}

			if (conv->w)
				continue;
		}

		if (!s[i])
			s[i] = '?';
	}

	return s;
}

char *cliptext(TWindow *parent)
{
	TClipboard clipboard(parent->Handle, false);

	if (!clipboard || !clipboard.IsClipboardFormatAvailable(CF_UNICODETEXT))
		return NULL;

	HANDLE handle = clipboard.GetClipboardData(CF_UNICODETEXT);
	const wchar_t *address;

	if (!handle || (address = (const wchar_t *) GlobalLock(handle)) == NULL)
		return NULL;

	char *s = tocp1251(address);

	GlobalUnlock(handle);
	return s;
}

// ----- TStringArray --------------------------------------------------------
static bool strlesseqli(const long value1, const long value2)
{
	return strcmpi((const char *) value1, (const char *) value2) <= 0;
}

TStringArray::TStringArray(bool sorted)
	: TPointerArray(DeleteString)
{
	if (sorted)
		lesseql = strlesseqli;
}

void TStringArray::DeleteString(void *s)
{
	delete[] (char *) s;
}
