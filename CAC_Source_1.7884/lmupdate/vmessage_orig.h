enum
{
	MB_ERROR = MB_ICONERROR | MB_OK,
	MB_FATAL = MB_ICONSTOP | MB_OK,
	MB_INFO  = MB_ICONINFORMATION | MB_OK,
	MB_ASK   = MB_ICONQUESTION | MB_YESNO,
	MB_WARN  = MB_ICONWARNING | MB_YESNO,
	
	MB_BUTTONS = 0x000F,
	MB_ICONS   = 0x00F0,
	MB_MAXSIZE = 0x1400
};

bool vmessage(unsigned flags, const char *format, va_list ap);
