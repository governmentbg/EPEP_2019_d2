#include <stddef.h>

#include "vprinter.h"

static size_t _vsnf_count(const char *ptr, const size_t n, void *null)
{
	(void) ptr;
	(void) null;
	return n;
}

struct _vsnf_args
{
	char *buffer;
	int index;
	int length;
};

static size_t _vsnf_print(const char *ptr, const size_t n, struct _vsnf_args *const xa)
{
	size_t i;

	for (i = 0; i < n; i++)
		if (xa->index++ < xa->length)
			*xa->buffer++ = *ptr++;

	return n;
}

int vsnprintf(char *s, size_t n, const char *format, va_list ap)
{
	if (n > 0)
	{
		struct _vsnf_args xa;
		int result;

		xa.buffer = s;
		xa.index = 0;
		xa.length = (int) n - 1;
		result = __vprinter((putnF *) _vsnf_print, &xa, format, ap);
		s[result < 0 ? 0 : result < xa.length ? result : xa.length] = '\0';
		return result;
	}

	return __vprinter((putnF *) _vsnf_count, NULL, format, ap);
}

int snprintf(char *s, size_t n, const char *format, ...)
{
	va_list ap;
	int result;

	va_start(ap, format);
	result = vsnprintf(s, n, format, ap);
	va_end(ap);
	return result;
}
