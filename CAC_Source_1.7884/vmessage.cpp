#include "common.h"

bool vmessage(unsigned flags, const char *format, va_list ap)
{
	bool result = false;
	char *text = NULL;
	char cache[0x100];
	HWND active = GetActiveWindow();

	if (strchr(format, '%'))
	{
		size_t size = vsnprintf(NULL, 0, format, ap) + 1;
		bool entire = true;

		if (size <= sizeof cache)
			text = cache;
		else
		{
			if (size > MB_MAXSIZE)
			{
				size = MB_MAXSIZE;
				entire = false;
			}

			if ((text = (char *) malloc(size)) == NULL)
			{
				text = cache;
				size = sizeof cache;
				entire = false;
			}
		}

		if (entire)
			vsnprintf(text, size, format, ap);
		else
		{
			vsnprintf(text, size - 3, format, ap);
			strcpy(text + size - 4, "...");
		}
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

	// 2016:271 LPR: not needed?
//	if (active && active != GetActiveWindow())
//		SetActiveWindow(active);

	if (text != cache)
		free(text);

	return result;
}
