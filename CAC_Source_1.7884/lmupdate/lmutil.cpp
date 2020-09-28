#include "cac.h"

const char *basename(const char *name)
{
	const char *base = NULL;

	for (const char *s = name; *s; s++)
		if (strchr(":/\\", *s))
			base = s;

	return base ? base + 1 : name;
}

bool exists(const char *name, bool file)
{
	struct stat statbuf;

	if (stat(name, &statbuf))
		return false;

	if (!S_ISDIR(statbuf.st_mode) != file)
		fatal("%s съществува, но е %s.", name, file ? "директория" : "файл.");

	return true;
}

#define MAX_PORT 6

const char *invalid_port(const char *port)
{
	if (strlen(port) >= MAX_PORT)
		return "Прекалено дълъг номер на порт";

	for (const char *s = port; *s; s++)
		if (!isdigit(*s))
			return "Невалиден номер на порт - изискват се само цифри";

	return atol(port) > 65535 ? "Прекалено голям номер на порт" : NULL;
}

constant ibase_sql_dirs[] =
{
	"Program Files\\Borland\\IntrBase", "Program Files (x86)\\Borland\\IntrBase",
	"Program Files\\Borland\\InterBase", "Program Files (x86)\\Borland\\InterBase",
	"Program Files\\InterBase\\InterBase", "Program Files (x86)\\InterBase\\InterBase",
	"Program Files\\Firebird\\Firebird_1_5", "Program Files (x86)\\Firebird\\Firebird_1_5",
	"Program Files\\Firebird\\Firebird_2_5", "Program Files (x86)\\Firebird\\Firebird_2_5",
	NULL
};

constant pgres_sql_dirs[] =
{
	"Program Files\\PostgreSQL\\9.4", "Program Files (x86)\\PostgreSQL\\9.4",
	"Program Files\\PostgreSQL\\9.2", "Program Files (x86)\\PostgreSQL\\9.2",
	NULL
};

void shorten_and_check(const char *fullname, char *shortname)
{
	DWORD result = GetShortPathName(fullname, shortname, MAX_PATH);

	if (!result || result >= MAX_PATH)
		fatal("%s: грешка при съкращаване на името.", fullname);

	if (strchr(shortname, ' '))
		fatal("%s: краткото име съдържа интервал(и).", shortname);
}

bool find_sql_executable(const char *basename, constant *search_dirs, char *fullname)
{
	for (const char *drive = "CD"; *drive; drive++)
	{
		if (*drive == 'D' && GetDriveType("D:\\") != DRIVE_FIXED)
			continue;

		for (constant *dir = search_dirs; *dir; dir++)
		{
			sprintf(fullname, "%c:\\%s\\bin\\%s", *drive, *dir, basename);

			if (exists(fullname))
				return true;
		}
	}

	return false;
}

static bool sql_isspace(char c)
{
	return c == ' ' || c == '\t';
}

static bool sql_isword(const char *buffer, const char *s, size_t len, const char *endword = "")
{
	return (s == buffer || sql_isspace(s[-1])) && (sql_isspace(s[len]) || strchr(endword, s[len]));
}

#define interbase_mode (interbase_gdb != NULL)

// xsutil: much simpler, sql files most not include heading lines
void lm_combine_scripts(const char *output_name, constant *script_names, const char *interbase_gdb)
{
	char *const line_buffer = (char *) malloc(0x10180);
	constant ignore = interbase_mode ? "GRANT" : "COMMIT";
	const size_t ignore_len = strlen(ignore);

	if (!line_buffer)
		out_of_memory();

	FILE *output = fopen(output_name, "wb");

	if (!output)
		fatal("%s: грешка %d при отваряне.", output_name, errno);

	const char *input_name;
	bool first_script = true;

	while ((input_name = *script_names++) != NULL)
	{
		FILE *input = fopen(input_name, "rb");
		bool inside_quotes = false;
		unsigned line_number = 0;

		if (!input)
			fatal("%s: грешка %d при отваряне.", input_name, errno);

		if (first_script)
		{
			if (interbase_mode)
			{
				fputs("SET NAMES WIN1251;\r\n", output);
				fprintf(output, "CONNECT '127.0.0.1:%s' USER '????????' PASSWORD '????????';\r\n", interbase_gdb);
			}

			first_script = false;
		}

		*line_buffer = '\0';
		fprintf(output, "/* %s */\r\n", input_name);

		while (fgets(line_buffer, 0x10000, input) != NULL)
		{
			size_t line_len = strlen(line_buffer);
			char *s = line_buffer;

			line_number++;

			if (interbase_mode && strchr(line_buffer, 26))
				fatal("%s: ред %u съдържа ^Z.", input_name, line_number);

			if (!line_len || line_buffer[line_len - 1] != '\n')
			{
				if (!feof(input) || line_buffer[line_len - 1] != ';')
					fatal("%s: ред %u е прекалено дълъг или непълен.", input_name, line_number);

				strcpy(line_buffer + line_len, "\n");
			}

			while (*s == ' ' || *s == '\t')
				s++;

			if (!strnicmp(s, ignore, ignore_len) && strchr(" \t", s[ignore_len]))
				continue;

			if (!interbase_mode)
			{
				while (*s)
				{
					if (*s == '\'')
					{
						// simplest case only
						inside_quotes ^= true;
					}
					else if (!inside_quotes && !strnicmp(s, "CHAR", 4) && sql_isword(line_buffer, s, 4, "("))
					{
						if (strlen(line_buffer) >= 0x10170)
							fatal("%s: ред %u е прекалено дълъг.", input_name, line_number);

						memmove(s + 3, s, strlen(s) + 1);
						memcpy(s, "VAR", 3);
						s += 6;
					}

					s++;
				}
			}

			fputs(line_buffer, output);
		}

		if (ferror(input))
			fatal("%s: грешка при четене.", input_name);

		if (fclose(input))
			fatal("%s: грешка %d при затваряне", input_name, errno);

		if (interbase_mode)
			fputs("COMMIT WORK;\r\n", output);
	}

	if (ferror(output))
		fatal("%s: грешка при запис.", output_name);

	if (fclose(output))
		fatal("%s: грешка %d при затваряне", output_name, errno);

	free(line_buffer);
}
