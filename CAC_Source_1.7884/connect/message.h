// ----- MB_ -----------------------------------------------------------------
enum
{
	MB_NULL	= MB_OK,
	MB_ERROR	= MB_ICONERROR | MB_OK,
	MB_FATAL	= MB_ICONSTOP | MB_OK,
	MB_INFO	= MB_ICONINFORMATION | MB_OK,
	MB_VERIFY	= MB_ICONQUESTION | MB_YESNO,
	MB_WARN	= MB_ICONWARNING | MB_YESNO
};

enum
{
	MB_BUTTONS	= 0x0F,
	MB_ICONS	= 0xF0
};

// ----- messages ------------------------------------------------------------
bool message(const char *format, ...);
bool message(unsigned flags, const char *format, ...);
bool vmessage(unsigned flags, const char *format, va_list ap);

bool ask(const char *format, ...);
bool error(const char *format, ...);	// == false
void fatal(const char *format, ...);	// failure()
void info(const char *format, ...);
bool warn(const char *format, ...);
