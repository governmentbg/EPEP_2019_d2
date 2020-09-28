#include "cac.h"

static void g_string_append_len(char *command, const char *text, size_t text_len)
{
	size_t command_len = strlen(command);

	if (command_len + text_len >= COMMAND_LINE_SIZE)
		fatal("resulting command line too long");

	memcpy(command + command_len, text, text_len);
	command[command_len + text_len] = '\0';
}

static void g_string_append_c(char *command, char c)
{
	g_string_append_len(command, &c, 1);
}

static void g_string_append(char *command, const char *text)
{
	g_string_append_len(command, text, strlen(text));
}

void command_record(char *command, const char *text)
{
	const char *s;

	if (*command)
		g_string_append_c(command, ' ');

	for (s = text; *s; s++)
		if (*s == '"' || *s <= ' ' || *s > '~')
			break;

	if (*text && !*s)
		g_string_append(command, text);
	else
	{
		g_string_append_c(command, '"');

		for (s = text; *s; s++)
		{
			const char *slash;

			for (slash = s; *slash == '\\'; slash++);

			if (slash > s)
			{
				g_string_append_len(command, s, slash - s);

				if (!*slash || *slash == '"')
				{
					g_string_append_len(command, s, slash - s);

					if (!*slash)
						break;
				}

				s = slash;
			}

			if (*s == '"')
				g_string_append_c(command, '\\');

			g_string_append_c(command, *s);
		}

		g_string_append_c(command, '"');
	}
}

void command_record2(char *command, const char *text1, const char *text2)
{
	command_record(command, text1);
	command_record(command, text2);
}

bool command_launch(char *command, PROCESS_INFORMATION &procInfo)
{
	STARTUPINFO startInfo;

	memset(&startInfo, '\0', sizeof startInfo);
	startInfo.cb = sizeof startInfo;

	if (CreateProcess(NULL, command, NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS, NULL, NULL, &startInfo, &procInfo))
	{
		CloseHandle(procInfo.hThread);
		return true;
	}

	return false;
}

int command_waitfor(HANDLE hProcess)
{
	DWORD exitCode = -1;

	WaitForSingleObject(hProcess, INFINITE);
	GetExitCodeProcess(hProcess, &exitCode);
	return exitCode;
}
