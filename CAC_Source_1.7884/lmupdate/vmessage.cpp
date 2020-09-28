bool quiet = false;

#define vmessage vmessage_orig
#include "vmessage_orig.cpp"
#undef vmessage

bool vmessage(unsigned flags, const char *format, va_list ap)
{
	return quiet ? false : vmessage_orig(flags, format, ap);
}
