#ifndef __BORLANDC__
#error __BORLANDC__ not defined
#endif  // __BORLANDC__

#ifndef _CHAR_UNSIGNED
#error _CHAR_UNSIGNED not defined
#endif  // _CHAR_UNSIGNED

#if !_CHAR_UNSIGNED
#error !_CHAR_UNSIGNED
#endif  // !_CHAR_UNSIGNED

#ifdef __cplusplus
extern "C" {
#endif

// verify / declare
extern unsigned char _ctype[257];
extern unsigned char _lower[256];
extern unsigned char _upper[256];

#ifdef __cplusplus
}
#endif

#define tolower(c) _lower[c]
#define toupper(c) _upper[c]

#define NBSP '\xA0'
// c must be char, not int
#define islocal(c) (c & 0x80)
