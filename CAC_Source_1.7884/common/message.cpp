#include "common.h"

bool message(unsigned flags, const char *format, ...)
{
	bool result;
	va_list ap;

	va_start(ap, format);
	result = vmessage(flags, format, ap);
	va_end(ap);
	return result;
}

bool message(const char *format, ...)
{
	va_list ap;

	va_start(ap, format);
	vmessage(MB_OK, format, ap);
	va_end(ap);
	return false;
}

bool ask(const char *format, ...)
{
	va_list ap;
	bool result;

	va_start(ap, format);
	result = vmessage(MB_ASK, format, ap);
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

void fatal(const char *format, ...)
{
	va_list ap;

	va_start(ap, format);
	vmessage(MB_FATAL, format, ap);
	va_end(ap);
	failure();
}
