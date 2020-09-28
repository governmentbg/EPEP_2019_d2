#include "common.h"

#ifdef __BORLANDC__
#include "snprintf.h"
#endif  // __BORLANDC__

// ----- rtl extras ----------------------------------------------------------
int strindex(const char *s, char c)
{
	const char *ts = strchr(s, c);
	return ts ? ts - s : INT_MAX;
}

int strindex(const char *s, const char *t)
{
	const char *ts = strstr(s, t);
	return ts ? ts - s : INT_MAX;
}

char *strnew(const char *s)
{
	int len = strlen(s) + 1;
	char *ts = new char[len];

	memcpy(ts, s, len);
	return ts;
}

int strrindex(const char *s, char c)
{
	const char *ts = strrchr(s, c);
	return ts ? ts - s : INT_MAX;
}

// ----- mstr ----------------------------------------------------------------
mstr::mstr()
{
	mbk_init(this);
	clear();
}

mstr::mstr(const char *s)
{
	mbk_init(this);
	cpy(s);
}

mstr::mstr(const mstr &m)
{
	mbk_init(this);
	cpy(m.str());
}

void mstr::cach_nx(char c)
{
	if (!chr(c))
		cach(c);
}

void mstr::catn(const char *s, size_t len)
{
	mbk_insert(this, s, size - 1, len);
}

void mstr::cat(const char *s)
{
	catn(s, strlen(s));
}

void mstr::cat_nx(const char *chars)
{
	while (*chars)
		cach_nx(*chars++);
}

void mstr::cat_quot(const char *s)
{
	size_t len = size - 1;
	cat(s);
	quote(len);
}

void mstr::cat_quoted(const char *prefix, const char *s)
{
	cat(prefix);
	cach('\'');
	cat_quot(s);
	cach('\'');
}

void mstr::catset(const char *set)
{
	if (strlen(set) == 1)
		printf("= '%c'", *set);
	else
	{
		cat("IN (");

		for (int i = 0; set[i]; i++)
		{
			if (i)
				cat(", ");

			printf("'%c'", set[i]);
		}

		cat(")");
	}
}

void mstr::clear()
{
	cpy("");
}

void mstr::compress(size_t pos, const char *spaces)
{
	::compress(ncstr() + pos, spaces);
	fixize();
}

void mstr::cpy(const char *s)
{
	mbk_clear(this);
	mbk_insert(this, s, 0, strlen(s) + 1);
}

void mstr::cut(size_t len)
{
	if (size > len + 1)
		mbk_remove(this, len, size - len - 1);
}

void mstr::cutell(size_t len)
{
	if (size > len + 1)
	{
		cut(len - 3);
		cat("...");
	}
}

void mstr::fixize()
{
	mbk_resize(this, strlen(str()) + 1);
}

void mstr::pad(size_t len)
{
	while (size <= len)
		cach(' ');
}

void mstr::printf(const char *format, ...)
{
	va_list ap;

	va_start(ap, format);
	vprintf(format, ap);
	va_end(ap);
}

void mstr::quote(size_t len)
{
	const char *s;

	while ((s = strchr(str() + len, '\'')) != NULL)
	{
		mbk_insert(this, "'", s - str(), 1);
		len = s - str() + 2;
	}
}

void mstr::sep_cat(const char *s, const char *t)
{
	if (*t)
	{
		sep(s);
		cat(t);
	}
}

void mstr::sep(const char *s)
{
	if (*str())
		cat(s);
}

void mstr::setlen(size_t len)
{
	pad(len);
	cut(len);
}

void mstr::trim_at(int pos)
{
	const char *s;

	for (s = str() + pos; *s && isspace(*s); s++);
	mbk_remove(this, pos, s - str() - pos);
}

void mstr::trim_trail()
{
	const char *s;

	for (s = str() + len() - 1; s >= str(); s--)
		if (!isspace(*s))
			break;
	cut(++s);
}

void mstr::vprintf(const char *format, va_list ap)
{
	size_t oldLen = len();
	size_t printLen = vsnprintf(NULL, 0, format, ap);

	mbk_insert(this, NULL, oldLen, printLen);
	vsnprintf((char *) data + oldLen, printLen + 1, format, ap);
}

// ----- functions -----------------------------------------------------------
void itom(int i, mstr &m)
{
	if (i)
		m.printf("%d", i);
}

bool strequal(const char *s1, const char *s2)
{
	if (strlen(s1) != strlen(s2))
		return false;

	while (*s1)
		if (!strchr(s2, *s1++))
			return false;

	return true;
}

char *compress(char *const s, const char *spaces)
{
	char *beg = s;

	if (!spaces)
		spaces = "\xA0\n";

	while (*beg)
	{
		if (isspace(*beg) || strchr(spaces, *beg))
		{
			char *end;

			*beg++ = ' ';
			for (end = beg; isspace(*end) || (*end && strchr(spaces, *end)); end++);
			if (end > beg)
				strmove(beg, end);
		}
		else
			beg++;
	}

	return s;
}

void squeeze(char *s)
{
	const char *start = s;

	while (*s)
	{
		bool punct2 = s > start && ispunct(s[-1]);

		if (isspace(*s) && (ispunct(s[1]) || punct2))
			strmove(s, s + 1);
		else
			s++;
	}
}

const char *skiplead(const char *s)
{
	while (isspace(*s))
		s++;

	return s;
}

char *strrep(char *s, char source, char target)
{
	if (source != target)
		while (strchr(s, source) != NULL)
			*strchr(s, source) = target;

	return s;
}
