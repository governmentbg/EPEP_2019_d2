#ifndef PERSONAL_T_H

#ifdef __cplusplus
extern "C"
{
#endif

#if PERSONAL_UNICODE
#define strcmp_t(s1, s2) lstrcmpW((s1), (s2))
#define strlen_t(s) ((size_t) lstrlenW(s))
#define strcat_t(dest, src) lstrcatW((dest), (src))
#define strcpy_t(dest, src) lstrcpyW((dest), (src))
#define toupper_t(c) ((wchar_t) CharUpperW((LPWSTR) (c)))
int strncmp_t(const char_t *s1, const char_t *s2, size_t len);
char_t *strchr_t(const char_t *text, char_t c);
int atoi_t(const char_t *s);
char_t *strstr_t(char_t *text, const char_t *s);
char_t *strset_t(char_t *s, char_t c, size_t n);
#else  // PERSONAL_UNICODE
#define strcmp_t(s1, s2) strcmp((s1), (s2))
#define strlen_t(s) strlen(s)
#define strcat_t(dest, src) strcat((dest), (src))
#define strcpy_t(dest, src) strcpy((dest), (src))
#define toupper_t(c) ((char) CharUpperA((char *) (c)))
#define strncmp_t(s1, s2, len) strncmp((s1), (s2), (len))
#define strchr_t(text, c) strchr((text), (c))
#define atoi_t(s) atoi(s)
#define strstr_t(text, s) strstr((text), (s))
#define strset_t(s, c, n) ((char *) memset((s), (c), (n)))
#endif  // PERSONAL_UNICODE

int startsw_t(const char_t *s1, const char_t *s2);
int isalpha_t(char_t c);
int isdigit_t(char_t c);
int isalnum_t(char_t c);
int isspace_t(char_t c);
int ispunct_t(char_t c);

char_t *chrcpy_t(char_t *dest, const char_t *src, size_t n);
char_t *chrmove_t(char_t *dest, const char_t *src, size_t n);
char_t *chralloc_t(size_t n);
void *nalloc_t(size_t n, size_t size);
void free_t(void *dest);

#ifdef __cplusplus
}
#endif

#define PERSONAL_T_H 1
#endif
