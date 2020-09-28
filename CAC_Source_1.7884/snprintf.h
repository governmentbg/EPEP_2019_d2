#include <stddef.h>
#include <stdarg.h>

#ifdef __cplusplus
extern "C"
{
#endif

int vsnprintf(char *s, size_t n, const char *format, va_list ap);
int snprintf(char *s, size_t n, const char *format, ...);

#ifdef __cplusplus
}
#endif
