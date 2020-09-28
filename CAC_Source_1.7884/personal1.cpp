#include <stddef.h>

#include "personal1.h"
#include "personal_t.h"

#if PERSONAL_UNICODE
#define L(s) L##s
#define LE(s, ws) (const wchar_t *) ws
#else  // PERSONAL_UNICODE
#define L(s) s
#define LE(s, ws) s
#endif  // PERSONAL_UNICODE

typedef const char_t *const constant_t;

// generic
static bool isquote_t(char_t c)
{
	return c && strchr_t(LE("'\"▒▓⌠■`└┌", "\x27\x00\x5C\x00\x22\x00\x18\x20\x19\x20\x1C\x20\x1D\x20\x60\x00\x1E\x20\x1A"
		"\x20\x00"), c);	// 2009:168 FIX: +down quote(s)
}

static bool isminus_t(char_t c) { return c && strchr_t(LE("-√≈╜", "\x2D\x00\x13\x20\x14\x20\xAD\x00\x00"), c); }
static void strmove_t(char_t *s1, const char_t *s2) { chrmove_t(s1, s2, strlen_t(s2) + 1); }
static bool isqorsp_t(char_t c) { return isspace_t(c) || c == '"'; }  // preprocessed text only
#define any_t(s) (*(s) != '\0')

static bool startsww_t(const char_t *text, const char_t *word)
{
	return startsw_t(text, word) && !isalpha_t(text[strlen_t(word)]);
}

static bool filter_prefix_t(char_t *name, const char_t *pref)
{
	size_t len = strlen_t(pref);
	char_t *s = NULL;

	if (!strncmp_t(name, pref, len) && !isalpha_t(name[len]))
	{
		for (s = name + len; isspace_t(*s) || *s == '.'; s++);
		strmove_t(name, s);
	}

	return s != NULL;
}

static void filter_prefixes_t(char_t *name, constant_t *prefixes)
{
	for (constant_t *prefix = prefixes; *prefix; prefix++)
		if (filter_prefix_t(name, *prefix))
			break;
}

#define _A '\1'	// address = ^A	2009:162 also dates, bank accounts
#define _B '\2'	// milbase = ^B	2013:322 star 1:1, not implemented
#define _C '\3'	// citizen = ^C	2009:232 also firm names
#define _D '\4'	// disable = ^D	^A..^C in original text

static char_t *globcmp_t(char_t *text, const char_t *patt)
{
	while (*patt)
	{
		bool match;

		switch (*patt)
		{
			case ' ' :
			{
				const char_t *beg;
				size_t n;

				for (n = 0; isspace_t(*patt); patt++)
					n += 3;
				for (beg = text; text - beg < n && isspace_t(*text); text++);
				if (text == beg)
					return NULL;

				continue;
			}
			case '&' : match = isalpha_t(*text); break;
			case '$' : match = isdigit_t(*text); break;
			case '?' : match = any_t(text); break;
			case '*' :
			{
				size_t n;
				char_t *end;

				for (n = 0; *patt == '*'; patt++)
					n += 10;

				do
				{
					if (!any_t(text))
						return NULL;
					if ((end = globcmp_t(++text, patt)) != NULL)
						return end;
				} while (--n);

				return NULL;
			}
			case '~' : match = any_t(text) && !isalnum_t(*text); break;
			case '\'' :
			case '"' : match = any_t(text) && isquote_t(*text); break;
			case '-' : match = any_t(text) && isminus_t(*text); break;
			case '#' :
			{
				static constant_t NOS[] = { LE("╧", "\x16\x21\x00"), L("NO."), L("NO"), L("#"),
					LE("мнлеп", "\x1D\x04\x1E\x04\x1C\x04\x15\x04\x20\x04\x00"), NULL };
				constant_t *nos;

				for (nos = NOS; *nos; nos++)
					if (startsw_t(text, *nos))
						break;

				if (*nos)
				{
					text += strlen_t(*nos);
					patt++;
					continue;
				}

				return NULL;
			}
			default : match = toupper_t(*text) == toupper_t(*patt);
		}

		if (!match)
			return NULL;

		text++;
		patt++;
	}

	return text;
}

static bool leftword_t(const char_t *found) { return !isalpha_t(found[-1]); }
static bool isword_t(const char_t *found, size_t len) { return leftword_t(found) && !isalpha_t(found[len]); }

static char_t *findword_t(char_t *text, const char_t *word)
{
	size_t len = strlen_t(word);

	if (len)
	{
		while ((text = strstr_t(text, word)) != NULL)
		{
			if (isword_t(text, len))
				return text;
			text += len;
		}
	}

	return NULL;
}

static const char_t *skip_spaces_t(const char_t *text)
{
	while (isspace_t(*text))
		text++;
	return text;
}

static const char_t *skip_digits_t(const char_t *text, size_t max)
{
	const char_t *s;
	for (s = text; isdigit_t(*s); s++);
	return s > text && s - text <= max ? s : NULL;
}

static char_t *find2end_t(char_t *text, const char_t *start, const char_t *space, bool word)
{
	if (space)
	{
		size_t len = strlen_t(space) - 1;

		while ((text = strchr_t(text, *start)) != NULL)
		{
			if (!strncmp_t(text, start, space - start))
			{
				const char_t *s = skip_spaces_t(text + (space - start));

				if (!strncmp_t(s, space + 1, len))
				{
					text = (char_t *) s + len;
					break;
				}
			}

			text++;
		}
	}
	else if ((text = word ? findword_t(text, start) : strstr_t(text, start)) != NULL)
	{
		if (leftword_t(text))
			text += strlen(start);
		else
			text = NULL;
	}

	return text;
}

static char_t *strchrnear_t(char_t *text, char_t c, size_t dist)
{
	for (int i = 0; *text && i <= dist && *text; i++, text++)
		if (*text == c)
			return text;

	return NULL;
}

static char_t *findnear_t(char_t *text, const char_t *word, size_t dist, bool leftonly = false)
{
	size_t len = strlen_t(word);

	if (len)
	{
		for (int i = 0; *text && i <= dist && *text; i++, text++)
			if (startsw_t(text, word))
				if (leftonly ? leftword_t(text) : isword_t(text, len))
					return text;
	}

	return NULL;
}

#define findleft_t(text, word, dist) findnear_t(text, word, dist, true)
#define FIND_CYCLES_MAX 10000

static int find_replace_valid_t(const char_t *find, const char_t *replace)
{
	int len = strlen_t(find);

	if (!len || strlen_t(replace) > len)
		return false;

	if (*find == '*' || find[len - 1] == '*' || find[len - 1] == '#')
		return false;

	const char_t *s;
	unsigned i;

	for (i = 0, s = find; *s; s++)
		if (!strchr_t(L("&$?*~ "), *s))
			i++;
	if (i < 2)
		return false;

	for (i = 1, s = find; (s = strchr_t(s, '*')) != NULL; s++)
	{
		unsigned n;

		for (n = 10; s[1] == '*'; n += 10, s++)
			if (n == 50)
				return false;

		if ((i *= n) > FIND_CYCLES_MAX)
			return false;
	}

	return true;
}

// specific
enum
{
	ADDRESS_PART_AREA		= 0x0001,
	ADDRESS_PART_REGION	= 0x0002,
	ADDRESS_PART_UCLP		= 0x0004,
	ADDRESS_PART_DISTRICT	= 0x0008,
	ADDRESS_PART_QUARTER	= 0x0010,
	ADDRESS_PART_STREET	= 0x0020,
	ADDRESS_PART_ADR_NO	= 0x0040,
	ADDRESS_PART_SUB_NO	= 0x0080,
	ADDRESS_PART_BLOCK_NO	= 0x0100,
	ADDRESS_PART_ENTRANCE	= 0x0200,
	ADDRESS_PART_FLOOR	= 0x0400,
	ADDRESS_PART_APARTMENT	= 0x0800,
	ADDRESS_PART_NULL		= 0x0000
};

enum
{
	PREF2_DOT		= 0x01,
	PREF2_STRICT	= 0x02,
	PREF2_NO		= 0x04,
	PREF2_NULL		= 0x00
};

#if TESTVER
static bool poisoned(char_t *const start, const char_t *const text)
{
	static constant_t POISONS[] =
	{
		LE("онярюмнбкемхе", "\x1F\x04\x1E\x04\x21\x04\x22\x04\x10\x04\x1D\x04\x1E\x04\x12\x04\x1B\x04\x15"
			"\x04\x1D\x04\x18\x04\x15\x04\x00"),
		LE("мюпедаю", "\x1D\x04\x10\x04\x20\x04\x15\x04\x14\x04\x11\x04\x10\x04\x00"),
		LE("опюбхкмхй", "\x1F\x04\x20\x04\x10\x04\x12\x04\x18\x04\x1B\x04\x1D\x04\x18\x04\x1A\x04\x00"),
		LE("пеьемхе", "\x20\x04\x15\x04\x28\x04\x15\x04\x1D\x04\x18\x04\x15\x04\x00"),
		LE("гюонбед", "\x17\x04\x10\x04\x1F\x04\x1E\x04\x12\x04\x15\x04\x14\x04\x00"),
		LE("хмярпсйжхъ", "\x18\x04\x1D\x04\x21\x04\x22\x04\x20\x04\x23\x04\x1A\x04\x26\x04\x18\x04\x2F\x04"
			"\x00"),
		LE("пюгонпефдюме", "\x20\x04\x10\x04\x17\x04\x1F\x04\x1E\x04\x20\x04\x15\x04\x16\x04\x14\x04\x10"
			"\x04\x1D\x04\x15\x04\x00"),
		LE("нопедекемхе", "\x1E\x04\x1F\x04\x20\x04\x15\x04\x14\x04\x15\x04\x1B\x04\x15\x04\x1D\x04\x18\x04"
			"\x15\x04\x00"),
		LE("опхяздю", "\x1F\x04\x20\x04\x18\x04\x21\x04\x2A\x04\x14\x04\x10\x04\x00"),
		LE("яонпюгслемхе", "\x21\x04\x1F\x04\x1E\x04\x20\x04\x10\x04\x17\x04\x23\x04\x1C\x04\x15\x04\x1D"
			"\x04\x18\x04\x15\x04\x00"),
		LE("опнрнйнк", "\x1F\x04\x20\x04\x1E\x04\x22\x04\x1E\x04\x1A\x04\x1E\x04\x1B\x04\x00"),
		LE("декн", "\x14\x04\x15\x04\x1B\x04\x1E\x04\x00"),
		LE("днйслемр", "\x14\x04\x1E\x04\x1A\x04\x23\x04\x1C\x04\x15\x04\x1D\x04\x22\x04\x00"),
		LE("опеохяйю", "\x1F\x04\x20\x04\x15\x04\x1F\x04\x18\x04\x21\x04\x1A\x04\x10\x04\x00"),
		LE("лнкаю", "\x1C\x04\x1E\x04\x1B\x04\x11\x04\x10\x04\x00"),
		LE("фюкаю", "\x16\x04\x10\x04\x1B\x04\x11\x04\x10\x04\x00"),
		LE("опнреяр", "\x1F\x04\x20\x04\x1E\x04\x22\x04\x15\x04\x21\x04\x22\x04\x00"),
		LE("хяйюме", "\x18\x04\x21\x04\x1A\x04\x10\x04\x1D\x04\x15\x04\x00"),
		LE("гюъбкемхе", "\x17\x04\x10\x04\x2F\x04\x12\x04\x1B\x04\x15\x04\x1D\x04\x18\x04\x15\x04\x00"),
		LE("бзгпюфемхе", "\x12\x04\x2A\x04\x17\x04\x20\x04\x10\x04\x16\x04\x15\x04\x1D\x04\x18\x04\x15\x04"
			"\x00"),
		LE("деиярбхе", "\x14\x04\x15\x04\x19\x04\x21\x04\x22\x04\x12\x04\x18\x04\x15\x04\x00"),
		LE("охялн", "\x1F\x04\x18\x04\x21\x04\x1C\x04\x1E\x04\x00"),
		LE("бундъы", "\x12\x04\x25\x04\x1E\x04\x14\x04\x2F\x04\x29\x04\x00"),
		LE("хгундъы", "\x18\x04\x17\x04\x25\x04\x1E\x04\x14\x04\x2F\x04\x29\x04\x00"),
		LE("днйюгюрекярбн", "\x14\x04\x1E\x04\x1A\x04\x10\x04\x17\x04\x10\x04\x22\x04\x15\x04\x1B\x04\x21"
			"\x04\x22\x04\x12\x04\x1E\x04\x00"),
		LE("яслюрю", "\x21\x04\x23\x04\x1C\x04\x10\x04\x22\x04\x10\x04\x00"),
		LE("опнрхб", "\x1f\x04\x20\x04\x1e\x04\x22\x04\x18\x04\x12\x04\x00"),
		LE("япеыс", "\x21\x04\x20\x04\x15\x04\x29\x04\x23\x04\x00"),
		NULL };

	for (constant_t *poisons = POISONS; *poisons; poisons++)
		if (findnear_t(start, *poisons, text - start))
			return true;

	return false;
}
#endif  // TESTVER

static char_t *filter_part_t(const TTextPart *part, char_t *text, size_t dist, bool strict)
{
	char_t *const start = text;
	char_t *s;

	if (part->pref1[1] == '\0')
	{
		if ((s = strchrnear_t(text, *part->pref1, dist)) != NULL ||
			(s = strchrnear_t(text, *part->pref2, dist)) != NULL)
		{
			text = s + 1;
		}
	}
	else if ((s = findnear_t(text, part->pref1, dist)) != NULL)
		text = s + strlen_t(part->pref1);
	else if (!strict || !(part->pref2opt & PREF2_STRICT))
	{
		if (part->pref2opt & PREF2_DOT)
		{
			char_t p2dot[20];

			strcpy_t(p2dot, part->pref2);
			strcat_t(p2dot, L("."));

			if ((s = findleft_t(text, p2dot, dist)) != NULL)
				text = s + strlen_t(p2dot);
		}
		else
			s = NULL;

		if (!s)
		{
			s = strchr_t(part->pref2, '.') ? findleft_t(text, part->pref2, dist) : findnear_t(text, part->pref2, dist);
			if (s)
				text = s + strlen_t(part->pref2);
		}
	}

	// 2014:178 LPR: a single letter or digit, without prefix, followed by superfluous letter or digit, is not enough
	if (any_t(part->part) && (s = findnear_t(text, part->part, dist)) != NULL && (text > start ||
		strlen_t(part->part) > 1 || !isalnum_t(*s) || !(isalpha_t(*s) ? isalpha_t(s[1]) : isdigit_t(s[1]))))
	{
		text = s + strlen_t(part->part) + (s[strlen_t(part->part)] == '\"');
	}
	// N.B. false positives, mostly filtered by poisons
	else if (text > start)
	{
	#if TESTVER
	#else  // NEW_POISON
		if (part->pref2opt & PREF2_NO)
		{
			static constant_t POISONS[] =
			{
				LE("онярюмнбкемхе", "\x1F\x04\x1E\x04\x21\x04\x22\x04\x10\x04\x1D\x04\x1E\x04\x12\x04\x1B\x04\x15"
					"\x04\x1D\x04\x18\x04\x15\x04\x00"),
				LE("мюпедаю", "\x1D\x04\x10\x04\x20\x04\x15\x04\x14\x04\x11\x04\x10\x04\x00"),
				LE("опюбхкмхй", "\x1F\x04\x20\x04\x10\x04\x12\x04\x18\x04\x1B\x04\x1D\x04\x18\x04\x1A\x04\x00"),
				LE("пеьемхе", "\x20\x04\x15\x04\x28\x04\x15\x04\x1D\x04\x18\x04\x15\x04\x00"),
				LE("гюонбед", "\x17\x04\x10\x04\x1F\x04\x1E\x04\x12\x04\x15\x04\x14\x04\x00"),
				LE("хмярпсйжхъ", "\x18\x04\x1D\x04\x21\x04\x22\x04\x20\x04\x23\x04\x1A\x04\x26\x04\x18\x04\x2F\x04"
					"\x00"),
				LE("пюгонпефдюме", "\x20\x04\x10\x04\x17\x04\x1F\x04\x1E\x04\x20\x04\x15\x04\x16\x04\x14\x04\x10"
					"\x04\x1D\x04\x15\x04\x00"),
				LE("нопедекемхе", "\x1E\x04\x1F\x04\x20\x04\x15\x04\x14\x04\x15\x04\x1B\x04\x15\x04\x1D\x04\x18\x04"
					"\x15\x04\x00"),
				LE("опхяздю", "\x1F\x04\x20\x04\x18\x04\x21\x04\x2A\x04\x14\x04\x10\x04\x00"),
				LE("яонпюгслемхе", "\x21\x04\x1F\x04\x1E\x04\x20\x04\x10\x04\x17\x04\x23\x04\x1C\x04\x15\x04\x1D"
					"\x04\x18\x04\x15\x04\x00"),
				LE("опнрнйнк", "\x1F\x04\x20\x04\x1E\x04\x22\x04\x1E\x04\x1A\x04\x1E\x04\x1B\x04\x00"),
				LE("декн", "\x14\x04\x15\x04\x1B\x04\x1E\x04\x00"),
				LE("днйслемр", "\x14\x04\x1E\x04\x1A\x04\x23\x04\x1C\x04\x15\x04\x1D\x04\x22\x04\x00"),
				LE("опеохяйю", "\x1F\x04\x20\x04\x15\x04\x1F\x04\x18\x04\x21\x04\x1A\x04\x10\x04\x00"),
				LE("лнкаю", "\x1C\x04\x1E\x04\x1B\x04\x11\x04\x10\x04\x00"),
				LE("фюкаю", "\x16\x04\x10\x04\x1B\x04\x11\x04\x10\x04\x00"),
				LE("опнреяр", "\x1F\x04\x20\x04\x1E\x04\x22\x04\x15\x04\x21\x04\x22\x04\x00"),
				LE("хяйюме", "\x18\x04\x21\x04\x1A\x04\x10\x04\x1D\x04\x15\x04\x00"),
				LE("гюъбкемхе", "\x17\x04\x10\x04\x2F\x04\x12\x04\x1B\x04\x15\x04\x1D\x04\x18\x04\x15\x04\x00"),
				LE("бзгпюфемхе", "\x12\x04\x2A\x04\x17\x04\x20\x04\x10\x04\x16\x04\x15\x04\x1D\x04\x18\x04\x15\x04"
					"\x00"),
				LE("деиярбхе", "\x14\x04\x15\x04\x19\x04\x21\x04\x22\x04\x12\x04\x18\x04\x15\x04\x00"),
				LE("охялн", "\x1F\x04\x18\x04\x21\x04\x1C\x04\x1E\x04\x00"),
				LE("бундъы", "\x12\x04\x25\x04\x1E\x04\x14\x04\x2F\x04\x29\x04\x00"),
				LE("хгундъы", "\x18\x04\x17\x04\x25\x04\x1E\x04\x14\x04\x2F\x04\x29\x04\x00"),
				LE("днйюгюрекярбн", "\x14\x04\x1E\x04\x1A\x04\x10\x04\x17\x04\x10\x04\x22\x04\x15\x04\x1B\x04\x21"
					"\x04\x22\x04\x12\x04\x1E\x04\x00"),
				LE("яслюрю", "\x21\x04\x23\x04\x1C\x04\x10\x04\x22\x04\x10\x04\x00"),
				NULL };

			for (constant_t *poisons = POISONS; *poisons; poisons++)
				if ((s = findnear_t(start, *poisons, dist)) != NULL && s < text)	// or simply findnear_t(...)?
					return start;
		}
	#endif  // TESTVER

		while (isspace_t(*text))
			text++;

		if (*text == '\"')
		{
			text++;
			if ((s = strchr_t(text, '\"')) != NULL && s - text <= part->size + 4)
				text = s + 1;
			else
				s = NULL;
		}
		else
			s = NULL;

		if (!s)
		{
			s = text;
			while (isalnum_t(*text) && text - s <= part->size + 4)
				text++;
		}
	}

#if TESTVER
	if (text > start && poisoned(start, text))
		return start;
#endif  // TESTVER

	return text;
}

#define DIST_FAR 25
#define DIST_NEAR 12
#define DIST_AUTO 0

static char_t *filter_parts_t(const TTextPart *parts, char_t *text, size_t dist, int &flags, int stop)
{
	if (flags)
		dist = DIST_NEAR;
	else if (dist == DIST_AUTO)
		dist = DIST_FAR;

	for (const TTextPart *part = parts; part->size && part->flag != stop; )
	{
		char_t *s = text;

		if (!(flags & part->flag))
			text = filter_part_t(part, text, dist, stop);

		if (text > s)
		{
			flags |= part->flag;
			dist = DIST_NEAR;
			part = parts;
		}
		else
			part++;
	}

	return text;
}

static bool filter_addresses_t(char_t *text, size_t dist, const TTextPart *parts)
{
	char_t *const start = text;
	int flags = 0;

	text = filter_parts_t(parts, text, dist, flags, ADDRESS_PART_ADR_NO);

	if (text > start)
	{
		text = filter_parts_t(parts, text, dist, flags, ADDRESS_PART_NULL);
		strset_t(start, _A, text - start);
		return true;
	}

	return false;
}

#if PERSONAL_UNICODE
typedef struct
{
	char pref1[32];
	char pref2[16];
	int pref2opt;
	char part[64];
	size_t size;
	int flag;
} TTextPartA;

#if sizeof(TTextPart) != sizeof(TTextPartA)
#error TextPart/A size mismatch
#endif

#define LC(s, ws) ws
static const TTextPartA EMPTY_PARTS_A[] =
#define EMPTY_PARTS (const TTextPart *const) EMPTY_PARTS_A
#else  // PERSONAL_UNICODE
#define LC(s, ws) s
static const TTextPart EMPTY_PARTS[] =
#endif  // PERSONAL_UNICODE
{
	{ LC("накюяр", "\x1E\x04\x11\x04\x1B\x04\x10\x04\x21\x04\x22\x04\x00"), LC("нак", "\x1E\x04\x11\x04\x1B\x04\x00"),
		PREF2_DOT, LC("", "\x00"), 26, ADDRESS_PART_AREA },
	{ LC("наыхмю", "\x1E\x04\x11\x04\x29\x04\x18\x04\x1D\x04\x10\x04\x00"), LC("наы.", "\x1E\x04\x11\x04\x29\x04\x2E\x00"
		"\x00"), PREF2_NULL, LC("", "\x00"), 26, ADDRESS_PART_REGION },
	{ LC("цпюд", "\x13\x04\x20\x04\x10\x04\x14\x04\x00"), LC("цп", "\x13\x04\x20\x04\x00"), PREF2_DOT | PREF2_STRICT,
		LC("", "\x00"), 26, ADDRESS_PART_UCLP },
	{ LC("яекн", "\x21\x04\x15\x04\x1B\x04\x1E\x04\x00"), LC("я.", "\x21\x04\x2E\x00\x00"), PREF2_STRICT, LC("", "\x00"),
		26, ADDRESS_PART_UCLP },
	{ LC("леярмняр", "\x1C\x04\x15\x04\x21\x04\x22\x04\x1D\x04\x1E\x04\x21\x04\x22\x04\x00"),
		LC("л.", "\x1C\x04\x2E\x00\x00"), PREF2_STRICT, LC("", "\x00"), 26, ADDRESS_PART_UCLP },
	{ LC("йбюпрюк", "\x1A\x04\x12\x04\x10\x04\x20\x04\x22\x04\x10\x04\x1B\x04\x00"), LC("йб", "\x1A\x04\x12\x04\x00"),
		PREF2_DOT | PREF2_STRICT, LC("", "\x00"), 31, ADDRESS_PART_QUARTER },
	{ LC("ф.й.", "\x16\x04\x2E\x00\x1A\x04\x2E\x00\x00"), LC("фй", "\x16\x04\x1A\x04\x00"), PREF2_DOT, LC("", "\x00"), 31,
		ADDRESS_PART_QUARTER },
	{ LC("скхжю", "\x23\x04\x1B\x04\x18\x04\x26\x04\x10\x04\x00"), LC("ск", "\x23\x04\x1B\x04\x00"), PREF2_DOT,
		LC("", "\x00"), 31, ADDRESS_PART_STREET },
	{ LC("аскебюпд", "\x11\x04\x23\x04\x1B\x04\x15\x04\x12\x04\x10\x04\x20\x04\x14\x04\x00"),
		LC("аск", "\x11\x04\x23\x04\x1B\x04\x00"), PREF2_DOT, LC("", "\x00"), 31, ADDRESS_PART_STREET },
	{ LC("мнлеп", "\x1D\x04\x1E\x04\x1C\x04\x15\x04\x20\x04\x00"), LC("No", "No\x00"), PREF2_DOT | PREF2_NO,
		LC("", "\x00"), 8, ADDRESS_PART_ADR_NO },
	{ LC("╧", "\x16\x21\x00"), LC("#", "#\x00"), PREF2_NO, LC("", "\x00"), 8, ADDRESS_PART_ADR_NO },
	{ LC("ондмнлеп", "\x1F\x04\x1E\x04\x14\x04\x1D\x04\x1E\x04\x1C\x04\x15\x04\x20\x04\x00"), LC("ондм",
		"\x1F\x04\x1E\x04\x14\x04\x1D\x04\x00"), PREF2_DOT | PREF2_NO, LC("", "\x00"), 2, ADDRESS_PART_SUB_NO },
	{ LC("акнй", "\x11\x04\x1B\x04\x1E\x04\x1A\x04\x00"), LC("ак", "\x11\x04\x1B\x04\x00"), PREF2_DOT, LC("", "\x00"), 6,
		ADDRESS_PART_BLOCK_NO },
	{ LC("бунд", "\x12\x04\x25\x04\x1E\x04\x14\x04\x00"), LC("бу", "\x12\x04\x25\x04\x00"), PREF2_DOT, LC("", "\x00"), 3,
		ADDRESS_PART_ENTRANCE },
	{ LC("ерюф", "\x15\x04\x22\x04\x10\x04\x16\x04\x00"), LC("ер", "\x15\x04\x22\x04\x00"), PREF2_DOT, LC("", "\x00"), 3,
		ADDRESS_PART_FLOOR },
	{ LC("юоюпрюлемр", "\x10\x04\x1F\x04\x10\x04\x20\x04\x22\x04\x10\x04\x1C\x04\x15\x04\x1D\x04\x22\x04\x00"),
		LC("юо", "\x10\x04\x1F\x04\x00"), PREF2_DOT, LC("", "\x00"), 4, ADDRESS_PART_APARTMENT },
	{ LC("", "\x00"), LC("", "\x00"), 0, LC("", "\x00"), 0, 0 }
};

// 2008:131 LPR: separate function, was for names only, now also ucns
static void filter_addresses_t(char_t *text, const TTextPart *parts, bool jumpy = true)
{
	// 2008:108 LPR: strenght 1 helper: jumpy ADDRESS
	char_t *s;

	if (!parts->size)
		parts = EMPTY_PARTS;

	if (jumpy && (s = findnear_t(text, LE("юдпея", "\x10\x04\x14\x04\x20\x04\x15\x04\x21\x04\x00"), DIST_NEAR)) != NULL)
	{
		text = s + 5 + (s[5] == ':');
		jumpy = false;
	}

	if (!filter_addresses_t(text + (*text != '\0'), DIST_AUTO, parts) &&
		jumpy && (s = findnear_t(text, LE("юдпея", "\x10\x04\x14\x04\x20\x04\x15\x04\x21\x04\x00"), DIST_FAR)) != NULL)
	{
		text = s + 5 + (s[5] == ':');
		filter_addresses_t(text + (*text != '\0'), DIST_NEAR, parts);
	}
}

#define ap1 ((const TTextPart *) (p1))
#define ap2 ((const TTextPart *) (p2))
static int compare_parts_t(const void *p1, const void *p2)
{
	//return A(p1)->flag != A(p2)->flag ? A(p1)->flag - A(p2)->flag : any(A(p2)->part) - any(A(p1)->part);
	return ap1->flag != ap2->flag ? ap1->flag - ap2->flag :
		strlen_t(ap1->part) != strlen_t(ap2->part) ? strlen_t(ap2->part) - strlen_t(ap1->part) :	// longer parts first
		strcmp_t(ap1->part, ap2->part) ? strcmp_t(ap1->part, ap2->part) :
		strcmp_t(ap1->pref1, ap2->pref1);
}
#undef ap2
#undef ap1

static void initialize_parts_t(TTextPart *parts)
{
	static constant_t
		FILTER_UCLP_PREFS_T[] = { LE("цпюд", "\x13\x04\x20\x04\x10\x04\x14\x04\x00"), LE("цп", "\x13\x04\x20\x04\x00"),
			LE("яекн", "\x21\x04\x15\x04\x1B\x04\x1E\x04\x00"), LE("я", "\x21\x04\x00"), LE("леярмняр",
			"\x1C\x04\x15\x04\x21\x04\x22\x04\x1D\x04\x1E\x04\x21\x04\x22\x04\x00"), LE("л", "\x1C\x04\x00"), NULL },
		FILTER_STREET_PREFS_T[] = { LE("скхжю", "\x23\x04\x1B\x04\x18\x04\x26\x04\x10\x04\x00"), LE("ск",
			"\x23\x04\x1B\x04\x00"), LE("аскебюпд", "\x11\x04\x23\x04\x1B\x04\x15\x04\x12\x04\x10\x04\x20\x04\x14"
			"\x04\x00"), LE("аск", "\x11\x04\x23\x04\x1B\x04\x00"), LE("йбюпрюк", "\x1A\x04\x12\x04\x10\x04\x20\x04"
			"\x22\x04\x10\x04\x1B\x04\x00"), LE("йб", "\x1A\x04\x12\x04\x00"), LE("ф.й", "\x16\x04\x2E\x00\x1A\x04"
			"\x00"), LE("фй", "\x16\x04\x1A\x04\x00"), NULL };

	TTextPart *part;

	for (part = parts; part->size; part++)
	{
		if (part->flag & ADDRESS_PART_UCLP)
			filter_prefixes_t(part->part, FILTER_UCLP_PREFS_T);
		else if (part->flag & (ADDRESS_PART_QUARTER | ADDRESS_PART_STREET))
			filter_prefixes_t(part->part, FILTER_STREET_PREFS_T);
	}

	qsort(parts, part - parts, sizeof(TTextPart), compare_parts_t);
}

static void filter_addresses_t(char_t *text, TTextSide *side, TTextContext *context, int index, bool jumpy = true)
{
	if (!side->parts)
	{
		side->parts = context->get_parts_t(context->data, index);
		initialize_parts_t(side->parts);
	}

	filter_addresses_t(text, side->parts, jumpy);
}

static void filter_keyword_t(char_t *text, const char_t *word, bool name, TTextSide *side, TTextContext *context, int index,
	bool jumpy)
{
	size_t len = strlen_t(word);

	while ((text = findword_t(text, word)) != NULL)
	{
		if (name)
			strset_t(text, _C, len);

		text += len;
		filter_addresses_t(text, side, context, index, jumpy);
	}
}

static char_t *filter_replace_t(TTextContext *context, char_t *text, char_t *buffer, char_t *pos, const char_t *end,
	const char_t *rep)
{
	size_t len;

	if (rep)
		len = strlen_t(rep);
	else
	{
		rep = pos;
		len = end - pos;
	}

	if (context->filter_replace_t && !context->filter_replace_t(context->data, text, pos - buffer, end - pos, rep, len))
		return L("");

	chrcpy_t(text + (pos - buffer), rep, len);
	pos += len;

	if (end > pos)
	{
		size_t size = strlen_t(end) + 1;
		chrmove_t(text + (pos - buffer), text + (end - buffer), size);
		chrmove_t(pos, end, size);	// <=> strmove(text, end)
	}

	return pos;
}

static void find_replace_t(const char_t *find, const char_t *replace, char_t *text, TTextContext *context)
{
	char_t *s = text;
	size_t rep_len = strlen_t(replace);

	// glob case-insensitive search
	while (*s)
	{
		char_t *end = globcmp_t(s, find);

		if (!end)
		{
			s++;
			continue;
		}

		if (context->filter_replace_t && !context->filter_replace_t(context->data, text, s - text, end - s, replace,
			rep_len))
		{
			break;
		}

		strmove_t(s, end - rep_len);
		chrcpy_t(s, replace, rep_len);
		s += rep_len;
	}
}

static bool company_name_t(const char_t *name)
{
	static constant_t COMPANY_NAMES[] =
	{
		LE("ер", "\x15\x04\x22\x04\x00"), LE("яд", "\x21\x04\x14\x04\x00"), LE("йд", "\x1A\x04\x14\x04\x00"),
		LE("ннд", "\x1E\x04\x1E\x04\x14\x04\x00"), LE("юд", "\x10\x04\x14\x04\x00"),
		LE("йдю", "\x1A\x04\x14\x04\x10\x04\x00"), LE("еннд", "\x15\x04\x1E\x04\x1E\x04\x14\x04\x00"),
		LE("еюд", "\x15\x04\x10\x04\x14\x04\x00"), LE("йкнм", "\x1A\x04\x1B\x04\x1E\x04\x1D\x04\x00"),
		LE("йнно", "\x1A\x04\x1E\x04\x1E\x04\x1F\x04\x00"), LE("лйо", "\x1C\x04\x1A\x04\x1F\x04\x00"),
		LE("йо", "\x1A\x04\x1F\x04\x00"), LE("йя", "\x1A\x04\x21\x04\x00"),
		LE("ядпмж", "\x21\x04\x14\x04\x20\x04\x1D\x04\x26\x04\x00"), LE("тнмд", "\x24\x04\x1E\x04\x1D\x04\x14\x04\x00"),
		LE("лоб", "\x1C\x04\x1F\x04\x12\x04\x00"), LE("юдбд", "\x10\x04\x14\x04\x12\x04\x14\x04\x00"),
		LE("йннон", "\x1A\x04\x1E\x04\x1E\x04\x1F\x04\x1E\x04\x00"), LE("йгяо", "\x1A\x04\x17\x04\x21\x04\x1F\x04\x00"),
		LE("хгя", "\x18\x04\x17\x04\x21\x04\x00"), LE("до", "\x14\x04\x1F\x04\x00"), LE("но", "\x1E\x04\x1F\x04\x00"),
		LE("онан", "\x1F\x04\x1E\x04\x11\x04\x1E\x04\x00"), LE("дют", "\x14\x04\x10\x04\x24\x04\x00"),
		LE("нндт", "\x1E\x04\x1E\x04\x14\x04\x24\x04\x00"), LE("мндт", "\x1D\x04\x1E\x04\x14\x04\x24\x04\x00"),
		LE("ялют", "\x21\x04\x1C\x04\x10\x04\x24\x04\x00"), LE("ннялт", "\x1E\x04\x1E\x04\x21\x04\x1C\x04\x24\x04\x00"),
		LE("етц", "\x15\x04\x24\x04\x13\x04\x00"), LE("йтц", "\x1A\x04\x24\x04\x13\x04\x00"),
		LE("дтц", "\x14\x04\x24\x04\x13\x04\x00"), LE("вттхк", "\x27\x04\x24\x04\x24\x04\x18\x04\x1B\x04\x00"), NULL
	};

	for (constant_t *company = COMPANY_NAMES; *company; company++)
		if (startsww_t(name, *company))
			return true;

	return false;
}

#define YEAR_MIN 1871
#define YEAR_MAX 2037

struct DMY
{
	int day, month, year;
};

static bool valid_date(DMY *d)
{
	static const int dom[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
	bool leap = (d->year % 400 == 0) || ((d->year % 4 == 0) && (d->year % 100 != 0));

	return d->year >= YEAR_MIN && d->year <= YEAR_MAX && d->month >= 1 && d->month <= 12 && d->day >= 1 &&
		d->day <= dom[d->month - 1] + (d->month == 2 && leap);
}

static void ucn_to_date(const char_t *ucn, DMY *d)
{
	char_t value[7];

	chrcpy_t(value, ucn, 6);
	value[6] = '\0';
	d->day = atoi_t(value + 4);
	value[4] = '\0';
	d->month = atoi_t(value + 2);
	value[2] = '\0';
	d->year = atoi_t(value);

	if (d->month >= 40)
	{
		d->month -= 40;
		d->year += 2000;
	}
	else if (d->month >= 20)
	{
		d->month -= 20;
		d->year += 1800;
	}
	else
		d->year += 1900;
}

static bool equal_date(const char_t *ucn, DMY *d)
{
	DMY ud;
	ucn_to_date(ucn, &ud);
	return ud.day == d->day && ud.month == d->month && ud.year == d->year;
}

static int scan_date(const char_t *text, DMY *d)
{
	constant_t start = text;
	const char_t *s;

	text = skip_spaces_t(text);
	if ((s = skip_digits_t(text, 2)) == NULL)
		return 0;
	d->day = atoi_t(text);

	text = skip_spaces_t(s);
	if (*text != '.')
		return 0;
	text = skip_spaces_t(text + 1);
	if ((s = skip_digits_t(text, 2)) == NULL)
		return 0;
	d->month = atoi_t(text);

	text = skip_spaces_t(s);
	if (*text != '.')
		return 0;
	text = skip_spaces_t(text + 1);
	if ((s = skip_digits_t(text, 4)) == NULL)
		return 0;
	d->year = atoi_t(text);

	return s - start;
}

#define UCN_CITIZEN_UCN 'c'
#define UCN_CITIZEN_CODE 'C'
#define UCN_CITIZENS L("cC")

#define UCN_FIRMS L("fF")
#define FIRM_NAMES_MAX 50

#define side_arguments (TTextSide *) side, context, side - sides

static void filter_personal_t(char_t *text, const TTextSide *sides, int strenght, bool military, int judicial,
	TTextContext *context, const TTextPair *pairs)
{
	char_t *const start = text;
	char_t *const buffer = (char_t *) chralloc_t(strlen_t(text) + 2) + 1;
	bool strong = strenght > 0;

	if (buffer == (char_t *) NULL + 1)
	{
		*text = '\0';
		return;
	}

	{
		// pass 0: pre-process
		char_t *s = text, *t = buffer;
		buffer[-1] = ' ';

		do
		{
			if (*s >= _A && *s <= _C)
				*t++ = _D;
		#if PERSONAL_UNICODE
			else if (*s == 0xA0 || (*s >= 0x2000 && *s <= 0x200A))
		#else  // PERSONAL_UNICODE
			else if (*s == 0xA0)
		#endif  // PERSONAL_UNICODE
				*t++ = ' ';
			else if (isquote_t(*s))
				*t++ = '\"';
			else if (isminus_t(*s))
				*t++ = '-';
			else
				*t++ = toupper_t(*s);
		} while (*s++);
	}

	if (judicial > 0)
	{
		// pass 1: firms
		for (const TTextSide *side = sides; side->ucnType; side++)
		{
			if (!strchr_t(UCN_FIRMS, (char_t) side->ucnType))
				continue;

			size_t count;
			size_t fullen = 0;
			const char_t (*names)[64] = side->names;

			for (count = 0; any_t(names[count]); count++)
				fullen += strlen_t(names[count]);

			if (judicial == 1)
			{
				size_t i;

				for (i = 0; i < count; i++)
					if (company_name_t(names[i]))
						break;

				if (i == count)
					continue;
			}

			text = buffer;
			while ((text = findword_t(text, names[0])) != NULL)
			{
				// TODO: nalloc_t NULL check
				char_t **words = (char_t **) nalloc_t(FIRM_NAMES_MAX * count, sizeof(char_t *));
				size_t *sizes = (size_t *) nalloc_t(FIRM_NAMES_MAX * count, sizeof(size_t));
				size_t found = 0;
				size_t foulen = 0;
				char_t *s = text;
				char_t *spc;

				for (;;)
				{
					for (spc = s; spc > buffer && text - spc < DIST_FAR && isqorsp_t(spc[-1]); spc--)
					{
						if (s - spc > 4)
						{
							spc = s;
							break;
						}
					}

					if (spc == s)
						break;

					for (s = spc; s > buffer && text - s < DIST_FAR && !isqorsp_t(s[-1]); s--);
					if (s == spc)
						break;

					size_t i;

					for (i = 0; i < count; i++)
						// 2013:317 FIX: was !strncmp_t(s, names[i], spc - s)
						if (startsww_t(s, names[i]))
							break;

					if (i == count)
						break;

					words[found] = s;
					foulen += sizes[found++] = spc - s;		// should be strlen_t(names[i])
				}

				words[found] = text;
				foulen += sizes[found] = strlen_t(names[0]);
				text += sizes[found++];

				spc = text;
				for (;;)
				{
					for (s = spc; isqorsp_t(*s); s++)
					{
						if (s - spc > 4)
						{
							s = spc;
							break;
						}
					}

					if (s == spc)
						break;

					for (spc = s; *spc && !isqorsp_t(*spc); spc++);
					if (spc == s)
						break;

					size_t i;

					for (i = 0; i < count; i++)
						// 2013:317 FIX: was !strncmp_t(s, names[i], spc - s)
						if (startsww_t(s, names[i]))
							break;

					if (i == count)
						break;

					words[found] = s;
					foulen += sizes[found++] = spc - s;		// should be strlen_t(names[i])
					text = spc;
				}

				if (foulen * 5 >= fullen * 4 && found * 4 >= count * 3)
				{
					for (size_t i = 0; i < found; i++)
						if (!company_name_t(words[i]))
							strset_t(words[i], _C, sizes[i]);

					if (strong)
						filter_addresses_t(text, side_arguments);
				}

				free_t(words);
				free_t(sizes);
			}

			// 2014:177 FIX: strong only
			if (strong)
			{
				filter_keyword_t(buffer, LE("сопюбкемхе", "\x23\x04\x1F\x04\x20\x04\x10\x04\x12\x04\x1B\x04\x15\x04"
					"\x1D\x04\x18\x04\x15\x04\x00"), false, side_arguments, false);

				filter_keyword_t(buffer, LE("яедюкхые", "\x21\x04\x15\x04\x14\x04\x10\x04\x1B\x04\x18\x04\x29\x04"
					"\x15\x04\x00"), false, side_arguments, true);
			}
		}
	}

	{
		// pass 2: valid UCNs
		text = buffer;
		for (int len = strlen_t(text); len >= 10; text++, len--)
		{
			size_t i;
			DMY dmy;

			if (!isdigit_t(*text) || isdigit_t(text[-1]) || isdigit_t(text[10]))
				continue;

			// all digits?
			for (i = 1; i < 10; i++)
				if (!isdigit_t(text[i]))
					break;
			if (i < 10)
				continue;

			// valid checksum?
			static const int multi[9] = { 2, 4, 8, 5, 10, 9, 7, 3, 6 };
			int sum = 0;

			for (i = 0; i < 9; i++)
				sum += multi[i] * (text[i] - '0');

			if ((sum % 11 % 10) != (text[9] - '0'))
				continue;

			ucn_to_date(text, &dmy);
			if (!valid_date(&dmy))
				continue;

			// full valid UCN - star it
			// but try to find the side first
			const TTextSide *side = sides;

			if (strong)
			{
				for (; side->ucnType; side++)
					if (side->ucnType == UCN_CITIZEN_UCN && !strncmp_t(side->ucn, text, 10))
						break;
			}

			strset_t(text, _B, 10);
			text += 10;
			len -= 10;

			if (strong)
			{
				if (side->ucnType)
					filter_addresses_t(text, side_arguments);
				else
					filter_addresses_t(text, EMPTY_PARTS);
			}
			// next char can't be digit, so let for() skip it
		}

		// pass 3: broken UCNs; 2013:071 PRQ: +LNCs
		static constant_t UCNS[] = { LE("ецм", "\x15\x04\x13\x04\x1D\x04\x00"),
			LE("кмв", "\x1B\x04\x1D\x04\x27\x04\x00"), NULL };

		for (constant_t *ucns = UCNS; *ucns; ucns++)
		{
			text = buffer;
			while ((text = findword_t(text, *ucns)) != NULL)
			{
				text += strlen_t(*ucns);

				// skip optional leading space and punctuation
				while (isspace_t(*text) || ispunct_t(*text))
					text++;

				// and star any digit sequence
				char_t *mark = text;
				while (isdigit_t(*text))
					*text++ = _B;
				if (strong && text > mark)
					filter_addresses_t(text, EMPTY_PARTS);
			}
		}
	}

	// 2009:334 moved before 4/5/6
	if (strong)
	{
		// pass 7: birth date texts
		static constant_t BIRTHS[] = { LE("пндем", "\x20\x04\x1E\x04\x14\x04\x15\x04\x1D\x04\x00"),
			LE("пюфд", "\x20\x04\x10\x04\x16\x04\x14\x04\x00"),
			//LE("пюфд.", "\x20\x04\x10\x04\x16\x04\x14\x04\x2E\x00\x00"),	// captured by пюфд
			LE("пнд.", "\x20\x04\x1E\x04\x14\x04\x2E\x00\x00"), NULL };

		for (constant_t *births = BIRTHS; *births; births++)
		{
			text = buffer;
			while ((text = strstr_t(text, *births)) != NULL)
			{
				char_t *s = text;

				text += strlen_t(*births);
				while (isalpha_t(*text))
					text++;

				if (!leftword_t(s))
					continue;

				for (s = text; *s && !isdigit_t(*s) && s - text < DIST_NEAR; s++);

				if (!isdigit_t(*s))
					continue;

				DMY dmy;
				int n;

				if ((n = scan_date(s, &dmy)) == 0 || n > 20)
					continue;

				if (!valid_date(&dmy))
					continue;

				const TTextSide *side = sides;

				// 2009:331 LPR: find which address if possible
				for (; side->ucnType; side++)
					if (side->ucnType == UCN_CITIZEN_UCN && equal_date(side->ucn, &dmy))
						break;

				strset_t(s, _A, n);
				text = s + n;

				// 2009:330
				if (side->ucnType)
					filter_addresses_t(text, side_arguments);
				else
					filter_addresses_t(text, EMPTY_PARTS);
			}
		}

		// pass 8: birth date ucns
		for (char_t *text = buffer; *text; )
		{
			DMY dmy;
			int n;
			char_t *s = text++;

			if (!isdigit_t(*s) || isdigit_t(s[-1]))
				continue;

			if ((n = scan_date(s, &dmy)) == 0 || n > 20)
				continue;

			for (const TTextSide *side = sides; side->ucnType; side++)
			{
				if (side->ucnType == UCN_CITIZEN_UCN && equal_date(side->ucn, &dmy))
				{
					strset_t(s, _A, n);
					text = s + n;
					filter_addresses_t(text, side_arguments);	// 2009:334
					break;
				}
			}
		}
	}

	// 2009:334 moved before 4/5/6
	if (military)
	// pass M: military bases
	{
		static constant_t BASES[] = { LE("онд.", "\x1F\x04\x1E\x04\x14\x04\x2E\x00\x00"),
			LE("ондекемхе", "\x1F\x04\x1E\x04\x14\x04\x15\x04\x1B\x04\x15\x04\x1D\x04\x18\x04\x15\x04\x00"), NULL };

		for (constant_t *bases = BASES; *bases; bases++)
		{
			text = buffer;
			while ((text = strstr_t(text, *bases)) != NULL)
			{
				char_t *s = text;
				text += strlen_t(*bases);

				if (!leftword_t(s))
					continue;

				for (s = text; ispunct_t(*text) || isspace_t(*text); text++);
				if (text - s > 5)
					continue;

				char_t *const p = text;
				size_t ndi = 0;

				while (isdigit_t(*text) || isspace_t(*text))
				{
					if (isdigit_t(*text))
						ndi++;
					if (++text == p + 6)
						break;
				}

				if (ndi != 5)
					continue;

				s = text;
				bool minus = false;

				while (*text)
				{
					if (*text == '-')
						minus = true;
					else if (!isspace_t(*text))
						break;
					text++;
				}

				if (minus && text - s <= 5 && isdigit_t(*text) && isdigit_t(text[1]) && !isdigit_t(text[2]))
					text += 2;
				else
					text = s;	// roll back after 5-digit number

				strset_t(p, _A, text - p);
				if (strong)		// 2014:177 FIX: strong only
					filter_addresses_t(text, EMPTY_PARTS);	// 2009:330
			}
		}
	}

	for (const TTextSide *side = sides; side->ucnType; side++)
	{
		// pass 4: broken sideWind ucn; 2013:071 PRQ: or LNC
		if (side->ucnType == UCN_CITIZEN_UCN)
		{
			char_t value[10];

			chrcpy_t(value, side->ucn, 9);
			value[9] = '\0';

			for (text = buffer; (text = strstr_t(text, value)) != NULL; text += 9)
			{
				size_t ext = 0;

				// number boundaries
				if (isdigit_t(text[-1]))
					continue;
				else if (isdigit_t(text[9]))
				{
					if (isdigit_t(text[10]))
						continue;
					// 10-digit value & 1st 9 digits match
					ext++;
				}

				strset_t(text, _B, 9 + ext);
				if (strong)
					filter_addresses_t(text + 9 + ext, side_arguments);
			}
		}
		else if (side->ucnType != UCN_CITIZEN_CODE)
			continue;

		// pass L: LNC
		{
			if (any_t(side->lnc))
			{
				for (text = buffer; (text = strstr_t(text, side->lnc)) != NULL; text += 10)
				{
					// number boundaries
					if (isdigit_t(text[-1]) || isdigit_t(text[10]))
						continue;

					strset_t(text, _B, 10);
					if (strong)
						filter_addresses_t(text + 10, side_arguments);
				}
			}
		}

		if (strong)
		{
			// pass 5: names
			for (const char_t (*names)[64] = side->names; any_t(*names); names++)
				filter_keyword_t(buffer, *names, true, side_arguments, true);

			// pass 6: addresses
			static constant_t ADDRS[] =
			{
				LE("юдпея", "\x10\x04\x14\x04\x20\x04\x15\x04\x21\x04\x00"),
				LE("фхбсы", "\x16\x04\x18\x04\x12\x04\x23\x04\x29\x04\x00"),
				LE("фхбсыю", "\x16\x04\x18\x04\x12\x04\x23\x04\x29\x04\x10\x04\x00"),
				LE("фхрекярбн", "\x16\x04\x18\x04\x22\x04\x15\x04\x1B\x04\x21\x04\x22\x04\x12\x04\x1E\x04\x00"),
				LE("фхрек", "\x16\x04\x18\x04\x22\x04\x15\x04\x1B\x04\x00"),
				LE("леярнфхрекярбн", "\x1C\x04\x15\x04\x21\x04\x22\x04\x1E\x04\x16\x04\x18\x04\x22\x04\x15\x04"
					"\x1B\x04\x21\x04\x22\x04\x12\x04\x1E\x04\x00"),
				LE("леярнфхбееме", "\x1C\x04\x15\x04\x21\x04\x22\x04\x1E\x04\x16\x04\x18\x04\x12\x04\x15\x04"
					"\x15\x04\x1D\x04\x15\x04\x00"),
				//LE("хлнр", "\x18\x04\x1C\x04\x1E\x04\x22\x04\x00"),
				//LE("фхкхые", "\x16\x04\x18\x04\x1B\x04\x18\x04\x29\x04\x15\x04\x00"),
				//LE("лъярн", "\x1C\x04\x2F\x04\x21\x04\x22\x04\x1E\x04\x00"),
				//LE("юоюпрюлемр", "\x10\x04\x1F\x04\x10\x04\x20\x04\x22\x04\x10\x04\x1C\x04\x15\x04\x1D\x04\x22"
				//	"\x04\x00"),
				LE("онярнъмем юдпея", "\x1F\x04\x1E\x04\x21\x04\x22\x04\x1E\x04\x2F\x04\x1D\x04\x15\x04\x1D\x04"
					"\x20\x00\x10\x04\x14\x04\x20\x04\x15\x04\x21\x04\x00"),
				LE("мюярнъы юдпея", "\x1D\x04\x10\x04\x21\x04\x22\x04\x1E\x04\x2F\x04\x29\x04\x20\x00\x10\x04"
					"\x14\x04\x20\x04\x15\x04\x21\x04\x00"),
				LE("яздеаем юдпея", "\x21\x04\x2A\x04\x14\x04\x15\x04\x11\x04\x15\x04\x1D\x04\x20\x00\x10\x04"
					"\x14\x04\x20\x04\x15\x04\x21\x04\x00"),
				LE("язд. юдпея", "\x21\x04\x2A\x04\x14\x04\x2E\x00\x20\x00\x10\x04\x14\x04\x20\x04\x15\x04"
					"\x21\x04\x00"),
				NULL
			};

			for (constant_t *addrs = ADDRS + (strenght <= 1); *addrs; addrs++)
			{
				const char_t *space = strchr_t(*addrs, ' ');
				text = buffer;

				if (strenght > 1 && space && !strcmp_t(space + 1, *ADDRS))
					break;

				while ((text = find2end_t(text, *addrs, space, true)) != NULL)
					filter_addresses_t(text, side_arguments, false);
			}
		}
	}

	// pass 9: bank accounts
	{
		static constant_t ACCOUNTS[] = { L("IBAN"),
			LE("аюмйнбю ялерйю", "\x11\x04\x10\x04\x1D\x04\x1A\x04\x1E\x04\x12\x04\x10\x04\x20\x00\x21\x04\x1C\x04"
				"\x15\x04\x22\x04\x1A\x04\x10\x04\x00"),
			L("BIC"), NULL };

		for (constant_t *accounts = ACCOUNTS; *accounts; accounts++)
		{
			const char_t *space = strchr_t(*accounts, ' ');

			text = buffer;
			// "IBANBG123", "BIC99..." -> not findword()
			while ((text = find2end_t(text, *accounts, space, false)) != NULL)
			{
				char_t *s = text;

				while (ispunct_t(*text) || isspace_t(*text))
					text++;

				if (text - s > 5)
					continue;

				size_t n = 0;
				char_t *p = text;

				for (s = text; (isalnum_t(*text) || isspace_t(*text)) && text - s <= 44 && n <= 34; text++)
				{
					if (isalnum_t(*text))
					{
						p = text + 1;
						n++;
					}
				}

				strset_t(s, _A, p - s);
			}
		}
	}

	// pass N: xfer/apply changes
	{
		char_t *s;

		text = buffer;
		while ((text = strchr_t(text, _B)) != NULL)
		{
			for (s = text; *s == _B; s++)
			{
				if (start[s - buffer] == '*')
					break;

				*s = '*';
			}

			if (s > text)
				text = filter_replace_t(context, start, buffer, text, s, NULL);
			else
				text++;
		}

		text = buffer;
		while ((text = strchr_t(text, _C)) != NULL)
		{
			for (s = text + 1; *s == _C; s++);

			// 2013:317 LPR: FIX: was >, but AX rules don't apply here
			if (s - text >= 2)
			{
				char_t rep[3] = { start[text - buffer], '.', '\0' };

				if (isspace_t(*s) && s[1] == _C)		// X. Y. -> X.Y.
					s++;
				// sometimes Ab. -> A.. but mostly A.. at end of sentence
				//else if (*s == '.')
				//	rep[1] = '\0';

				text = filter_replace_t(context, start, buffer, text, s, rep);
			}
			else
				text++;
		}

		text = buffer;
		while ((text = strchr_t(text, _A)) != NULL)
		{
			constant_t rep = L(" ***") + (text == buffer || text[-1] != '*');

			for (s = text + 1; *s == _A; s++);
			if (s > text)
				text = filter_replace_t(context, start, buffer, text, s, rep);
			else
				text++;
		}
	}

	free_t(buffer - 1);

	// pass 10: user texts
	while (any_t(pairs->find))
	{
		if (find_replace_valid_t(pairs->find, pairs->replace))
			find_replace_t(pairs->find, pairs->replace, start, context);
		pairs++;
	}
}

#include <stdio.h>
#include <stdlib.h>
#if PERSONAL_UNICODE
#include <windows.h>
#endif  // PERSONAL_UNICODE

static void write(FILE *f, const char_t *text)
{
	fwrite(text, sizeof(char_t), strlen_t(text), f);
}

static void write(FILE *f, const char_t *text1, const char_t *text2)
{
	write(f, text1);
	write(f, L(" "));
	write(f, text2);
	write(f, L("\n"));
}

static void write(FILE *f, const char_t *text, int n)
{
	char_t s[20];
#if PERSONAL_UNICODE
	wsprintfW(s, L"%d", n);
#else  // PERSONAL_UNICODE
	sprintf(s, "%d", n);
#endif  // PERSONAL_UNICODE
	write(f, text, s);
}

extern "C"
{

void _export initialize_sides_t(TTextSide *sides)
{
	while (sides->ucnType)
	{
		if (sides->parts)
			initialize_parts_t(sides->parts);

		sides++;
	}
}

void filter_t(const TTextSide *sides, char_t *text, int strenght, int military, int judicial, const TTextPair *pairs)
{
	TTextContext context = { NULL, NULL, NULL };
	filter_personal_t(text, sides, strenght, military, judicial, &context, pairs);
}

void filter_ex_t(TTextSide *sides, char_t *text, int strenght, int military, int judicial, TTextContext *context,
	const TTextPair *pairs)
{
	filter_personal_t(text, sides, strenght, military, judicial, context, pairs);
}

void _export dump_t(const TTextSide *sides, char_t *text, const TTextPair *pairs)
{
#if PERSONAL_UNICODE
	FILE *f = fopen("__dump.A", "ab");
#else  // PERSONAL_UNICODE
	FILE *f = fopen("__dump.W", "ab");
#endif  // PERSONAL_UNICODE

	for (const TTextSide *side = sides; side->ucnType; side++)
	{
		write(f, L("side.ucn"), side->ucn);
		write(f, L("side.ucnType"), side->ucnType);
		write(f, L("side.lnc"), side->lnc);

		for (const char_t (*names)[64] = side->names; any_t(*names); names++)
			write(f, L("side.name"), *names);

		for (const TTextPart *part = side->parts; part->size; part++)
		{
			write(f, L("part.pref1"), part->pref1);
			write(f, L("part.pref2"), part->pref2);
			write(f, L("part.pref2opt"), part->pref2opt);
			write(f, L("part.part"), part->part);
			write(f, L("part.size"), part->size);
			write(f, L("part.flag"), part->flag);
		}
	}

	write(f, L("text.data"), text);

	for (const TTextPair *pair = pairs; any_t(pair->find); pair++)
	{
		write(f, L("pair.find"), pair->find);
		write(f, L("pair.replace"), pair->replace);
	}

	fclose(f);
}

}  // extern "C"
