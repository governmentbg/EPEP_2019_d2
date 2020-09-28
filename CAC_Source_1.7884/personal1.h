#ifndef PERSONAL1_H

#define PERSONAL_UNICODE 1

#if PERSONAL_UNICODE
#define char_t wchar_t
#else  // PERSONAL_UNICODE
#define char_t char
#endif  // PERSONAL_UNICODE

typedef struct TTextPart
{
	char_t pref1[16];
	char_t pref2[8];
	int pref2opt;
	char_t part[32];
	size_t size;
	int flag;
} TTextPart;

typedef struct TTextSide
{
	char_t ucn[16];
	int ucnType;
	char_t lnc[16];
	char_t names[102][64];
	TTextPart *parts;
} TTextSide;

typedef struct TTextPair
{
	char_t find[128];
	char_t replace[128];
} TTextPair;

typedef struct TTextContext
{
	TTextPart *(*get_parts_t)(void *data, int index);
	int (*filter_replace_t)(void *data, const char_t *text, int start, int len, const char_t *rep, int replen);
	void *data;
} TTextContext;

#ifdef __cplusplus
extern "C"
{
#endif
void initialize_sides_t(TTextSide *side);

void filter_t(const TTextSide *sides, char_t *text, int strenght, int military, int judicial, const TTextPair *pairs);
void filter_ex_t(TTextSide *sides, char_t *text, int strenght, int military, int judicial, TTextContext *context,
	const TTextPair *pairs);

void dump_t(const TTextSide *sides, char_t *text, const TTextPair *pairs);
#ifdef __cplusplus
}
#endif

#define PERSONAL1_H
#endif
