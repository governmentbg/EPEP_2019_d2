#include "cac.h"

// ----- message -------------------------------------------------------------
bool message(const char *format, ...)
{
	va_list ap;

	va_start(ap, format);
	vmessage(MB_NULL, format, ap);
	va_end(ap);

	return false;
}

bool message(unsigned flags, const char *format, ...)
{
	bool result;
	va_list ap;

	va_start(ap, format);
	result = vmessage(flags, format, ap);
	va_end(ap);

	return result;
}

#ifdef __BORLANDC__
static unsigned memblock_printf(const char *ptr, unsigned n, memblock_t *m)
{
	mbk_insert(m, ptr, m->size - 1, n);
	return n;
}
#endif

bool vmessage(unsigned flags, const char *format, va_list ap)
{
	bool result = false;
	HWND active = GetActiveWindow();
	char *text = NULL;
#ifdef __BORLANDC__
	memblock_t m;
#endif

	if (strchr(format, '%'))
	{
	#ifdef __BORLANDC__
		mbk_init(&m);
		mbk_insert(&m, "", 0, 1);
		__vprinter((putnF *) memblock_printf, &m, format, ap);
		text = (char *) m.data;
	#else
		size_t size = vsnprintf(NULL, 0, format, ap) + 1;

		if ((text = (char *) malloc(size)) == NULL)
			out_of_memory();

		vsnprintf(buffer, size, format, ap);
	#endif
	}

	switch (MessageBox(active, text ? text : format, MessageBoxTitle, flags | MB_TASKMODAL))
	{
		case IDOK :
		{
			result = ((flags & MB_BUTTONS) != MB_OK) ||
				(((flags & MB_ICONS) != MB_ICONERROR) && ((flags & MB_ICONS) != MB_ICONSTOP));
			break;
		}
		case IDYES :
		{
			result = true;
			break;
		}
		case IDCANCEL :
		case IDNO :
		{
			result = false;
			break;
		}
	}

	if (active && active != GetActiveWindow())
		SetActiveWindow(active);

#ifdef __BORLANDC__
	if (text)
		mbk_clear(&m);
#else
	free(text);
#endif

	return result;
}

bool ask(const char *format, ...)
{
	va_list ap;
	bool result;

	va_start(ap, format);
	result = vmessage(MB_VERIFY, format, ap);
	va_end(ap);

	return result;
}

bool error(const char *format, ...)
{
	va_list ap;

	va_start(ap, format);
	vmessage(MB_ERROR, format, ap);
	va_end(ap);

	return false;
}

void fatal(const char *format, ...)
{
	va_list ap;

	va_start(ap, format);
	vmessage(MB_FATAL, format, ap);
	va_end(ap);
	failure();
}

void info(const char *format, ...)
{
	va_list ap;

	va_start(ap, format);
	vmessage(MB_INFO, format, ap);
	va_end(ap);
}

bool warn(const char *format, ...)
{
	va_list ap;
	bool result;

	va_start(ap, format);
	result = vmessage(MB_WARN, format, ap);
	va_end(ap);

	return result;
}
