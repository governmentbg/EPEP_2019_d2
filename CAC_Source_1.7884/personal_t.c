#include <windows.h>  /* for GetCharType(), C1_ */

#include "personal1.h"
#include "personal_t.h"

#ifdef __cplusplus
extern "C"
{
#endif

#if PERSONAL_UNICODE

#if PERSONAL_OUTLINE
int strcmp_t(const wchar_t *s1, const wchar_t *s2) { return lstrcmpW(s1, s2); }
size_t strlen_t(const wchar_t *s) { return lstrlenW(s); }
wchar_t *strcat_t(wchar_t *dest, const wchar_t *src) { return lstrcatW(dest, src); }
wchar_t *strcpy_t(wchar_t *dest, const wchar_t *src) { return lstrcpyW(dest, src); }
wchar_t toupper_t(wchar_t c) { return (wchar_t) CharUpperW((LPWSTR) c); }
#endif  // PERSONAL_OUTLINE

int strncmp_t(const wchar_t *s1, const wchar_t *s2, size_t len)
{
	while (len--)
	{
		if (*s1 != *s2++)
			return 1;

		if (!*s1++)
			break;
	}

	return 0;
}

wchar_t *strchr_t(const wchar_t *text, wchar_t c)
{
	const wchar_t *s;
	for (s = text; *s && *s != c; s++);
	return *s == c ? (wchar_t *) s : NULL;
}

int atoi_t(const wchar_t *s)
{
	int i = 0;

	while (isdigit_t(*s))
		i = i * 10 + (*s++ - '0');

	return i;
}

wchar_t *strstr_t(wchar_t *text, const wchar_t *s)
{
	while ((text = strchr_t(text, s[0])) != NULL)
	{
		if (startsw_t(text, s))
			break;
		text++;
	}

	return text;
}

wchar_t *strset_t(wchar_t *s, wchar_t c, size_t n)
{
	wchar_t *p = s;
	while (n-- > 0) *p++ = c;
	return s;
}

static WORD CharType_t(wchar_t c)
{
	WORD result;
	return GetStringTypeW(CT_CTYPE1, &c, 1, &result) ? result : (WORD) 0;
}
#else  /* PERSONAL_UNICODE */

#include <string.h>

#if PERSONAL_OUTLINE
int strcmp_t(const char *s1, const char *s2) { return strcmp(s1, s2); }
size_t strlen_t(const char *s) { return strlen(s); }
char *strcat_t(char *dest, const char *src) { return strcat(dest, src); }
char *strcpy_t(char *dest, const char *src) { return strcpy(dest, src); }
char toupper_t(char c) { return (char) CharUpperA((char *) c); }
int strncmp_t(const char *s1, const char *s2, size_t len) { return strncmp(s1, s2, len); }
char *strchr_t(const char *text, char c) { return (char *) strchr(text, c); }
int atoi_t(const char *s) { return atoi(s); }
char *strstr_t(char *text, const char *s) { return strstr(text, s); }
char *strset_t(char *s, char c, size_t n) { return (char *) memset(s, c, n); }
#endif  // PERSONAL_OUTLINE

static WORD CharType_t(char c)
{
	WORD result;
	return GetStringTypeA(LOCALE_USER_DEFAULT, CT_CTYPE1, &c, 1, &result) ? result : (WORD) 0;
}
#endif  /* PERSONAL_UNICODE */

int startsw_t(const char_t *s1, const char_t *s2) { return !strncmp_t(s1, s2, strlen_t(s2)); }
int isalpha_t(char_t c) { return CharType_t(c) & C1_ALPHA; }
int isdigit_t(char_t c) { return c >= '0' && c <= '9'; }  /* ignore other digits for now */
int isalnum_t(char_t c) { return isalpha_t(c) || isdigit_t(c); }
int isspace_t(char_t c) { return CharType_t(c) & C1_SPACE; }
int ispunct_t(char_t c) { return CharType_t(c) & C1_PUNCT; }

#include <stdlib.h>

char_t *chrcpy_t(char_t *dest, const char_t *src, size_t n) { return (char_t *) memcpy(dest, src, n * sizeof(char_t)); }
char_t *chrmove_t(char_t *dest, const char_t *src, size_t n) { return (char_t *) memmove(dest, src, n * sizeof(char_t)); }
char_t *chralloc_t(size_t n) { return (char_t *) malloc(n * sizeof(char_t)); }
void *nalloc_t(size_t n, size_t size) { return malloc(n * size); }
void free_t(void *dest) { free(dest); }

#ifdef __cplusplus
}
#endif
