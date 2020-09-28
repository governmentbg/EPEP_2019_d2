#include <dir.h>
#include <fcntl.h>
#include <io.h>
#include <malloc.h>
#include <process.h>
#include <ole2.h>
#include <objidl.h>
#include <shlobj.h>

#include "cac.h"

#ifdef __GNUC__
#ifndef strcmpi
#define strcmpi strcasecmp
#endif
#ifndef strncmpi
#define strncmpi strncasecmp
#endif
#endif

#ifdef PATH_MAX
#undef PATH_MAX	// MAX_PATH only
#endif

#define GETLASTERROR ((unsigned long) GetLastError())

constant AppNameName = "САС \"СЪДЕБНО ДЕЛОВОДСТВО\" - АКТУАЛИЗАЦИЯ";

static LPMALLOC ppMalloc = NULL;
static LPITEMIDLIST ppDrives = NULL;
static FILE *LogFile = NULL;
static char LogName[MAX_PATH];
static HANDLE HConsole = INVALID_HANDLE_VALUE;

static void cleanup()
{
	if (ppMalloc && ppDrives)
		ppMalloc->lpVtbl->Free(ppMalloc, ppDrives);

	if (LogFile)
	{
		if (fclose(LogFile))
		{
			LogFile = NULL;
			error("%s: грешка %d при затваряне.", LogName, errno);
		}
		else
			LogFile = NULL;
	}

	if (HConsole != INVALID_HANDLE_VALUE)
		CloseHandle(HConsole);
}

void failure()
{
	cleanup();
	exit(EXIT_FAILURE);
}

static void write_console(const void *data, int n_chars = -1)
{
	if (n_chars == -1)
		n_chars = strlen((const char *) data);

	DWORD unused;

	WriteConsole(HConsole, data, n_chars, &unused, NULL);
}

static void log(const char *en, const char *bg, const char *ext)
{
	if (en)
	{
		write_console(en, -1);

		if (ext)
		{
			write_console(" ", 1);
			write_console(ext, -1);
		}

		write_console("\r\n", 2);
	}

	if (LogFile)
	{
		fputs(bg, LogFile);

		if (ext)
		{
			fputc(' ', LogFile);
			fputs(ext, LogFile);
		}

		fputc('\n', LogFile);
		fflush(LogFile);

		if (ferror(LogFile))
		{
			fclose(LogFile);
			LogFile = NULL;
			error("%s: грешка при запис. Възможно е журнала да е непълен.", LogName);
		}
	}
}

#ifdef __GNUC__
static void lmessage(unsigned flags, const char *ident, const char *format, ...) __attribute__ ((format(printf, 3, 4)));
#endif

static void lmessage(unsigned flags, const char *ident, const char *format, ...)
{
	va_list ap;

	log("check", "проверка", ident);

	va_start(ap, format);
	bool result = vmessage(flags, format, ap);
	va_end(ap);

	if (!result)
		exit(EXIT_FAILURE);
}

#define W_RUNNING \
	"На компютъра има стартирана програма (програми) от САС. " \
	"Възможно е това да доведе до конфликти при актуализацията." \
	"\n\n" \
	"Препоръчва се да приключите работа с тези програми и да ги " \
	"спрете." \
	"\n\n" \
	"Желаете ли да продължите?"

#define W_FIND_IB \
	"В директорията на САС е налична база данни, " \
	"но на компютъра не е открита директория на InterBase/Firebird сървър"

#define W_FIND_PG \
	"В директорията на САС е наличен dump файл, " \
	"но на компютъра не е открита директория на PostgreSQL сървър"

#define W_FIND_DB \
	"%s. " \
	"Необходимо е да посочите такава изрично." \
	"\n\n" \
	"Ако този компютър не е сървър, прекъснете актуализацията, " \
	"преименувайте файла %s от директорията на САС " \
	"или го преместете в друга директория, " \
	"и стартирайте актуализацията отново." \
	"\n\n" \
	"Желаете ли да продължите?"

#define W_CLIENT \
	"%s, но актуализацията е към същата или " \
	"по-стара базова версия на САС. Ще бъде изпълнена само клиентска " \
	"актуализация." \
	"\n\n" \
	"Желаете ли да продължите?"

#define W_SERVER \
	"В директорията на САС %s. " \
	"Ще бъде изпълнена сървърска актуализация за %s." \
	"\n\n" \
	"Проверете дали всички потребители на САС " \
	"са прекратили работа със системата и са излезли от нея, " \
	"след което задължително архивирайте базата данни." \
	"\n\n" \
	"Ако този компютър не е сървър, прекъснете актуализацията, " \
	"преименувайте файла %s от директорията на САС " \
	"или го преместете в друга директория, " \
	"и стартирайте актуализацията отново." \
	"\n\n" \
	"SQL командите, които се изпълняват при сървърска актуализация, " \
	"се извеждат и на екрана, като кирилицата може да се показва " \
	"грешно. " \
	"\n\n" \
	"Желаете ли да продължите?"

#define V_VERSION \
	"Актуализацията е към същата или по стара версия. " \
	"Желаете ли да продължите?"

#define W_PORT_PG \
	"Указан е нестандартен порт на базата данни. " \
	"Желаете ли да продължите?"

static constant appname_exe = "AppName.exe";

static void sbc(char *target, const char *s1, const char *s2, const char *s3 = NULL)
{
	if (s3 != NULL)
	{
		if (strlen(s1) + strlen(s2) + strlen(s3) + 2 >= MAX_PATH)
			fatal("%s\\%s\\%s: името е прекалено дълго.", s1, s2, s3);

		sprintf(target, "%s\\%s\\%s", s1, s2, s3);
	}
	else
	{
		if (strlen(s1) + strlen(s2) + 1 >= MAX_PATH)
			fatal("%s\\%s: името е прекалено дълго.", s1, s2);

		sprintf(target, "%s\\%s", s1, s2);
	}
}

static char lmpath[MAX_PATH];

static void copy_file(const char *file, const char *subdir)
{
	char target[MAX_PATH];

	if (subdir && *subdir)
		sbc(target, lmpath, subdir, file);
	else
		sbc(target, lmpath, file);

	if (!CopyFile(file, target, FALSE))
		fatal("%s: грешка %lu при копиране.", target, GetLastError());
}

static void end_find(HANDLE hFind, const char *mask)
{
	DWORD dwError = GetLastError();

	if (dwError != ERROR_FILE_NOT_FOUND && dwError != ERROR_NO_MORE_FILES)
		fatal("%s: грешка %lu при търсене.", mask, dwError);

	if (hFind != INVALID_HANDLE_VALUE && !FindClose(hFind))
		error("%s: грешка %lu при търсене.", mask, dwError);
}

static void copy_all(const char *mask, const char *subdir, size_t maxlen)
{
	HANDLE hFind;
	WIN32_FIND_DATA find;

	log("copy", "копиране на", mask);

	if ((hFind = FindFirstFile(mask, &find)) != INVALID_HANDLE_VALUE)
	{
		do
		{
			if (find.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				fatal("%s: намерена е директория.", find.cFileName);

			if (strlen(find.cFileName) > maxlen)
				fatal("%s: прекалено дълго име на файл.", find.cFileName);

			copy_file(find.cFileName, subdir);

		} while (FindNextFile(hFind, &find));
	}

	end_find(hFind, mask);
}

static void fresh_all(const char *mask, const char *subdir, size_t maxlen)
{
	char target_mask[MAX_PATH];
	HANDLE hFind;
	WIN32_FIND_DATA find;

	sbc(target_mask, lmpath, subdir, mask);
	log("delete", "изтриване на", target_mask);

	if ((hFind = FindFirstFile(target_mask, &find)) != INVALID_HANDLE_VALUE)
	{
		do
		{
			char target_file[MAX_PATH];
			sbc(target_file, lmpath, subdir, find.cFileName);

			if (find.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				fatal("%s: намерена е директория.", target_file);

			if (strlen(find.cFileName) > maxlen)
				fatal("%s\\%s\\%s: прекалено дълго име на файл.", lmpath, subdir, find.cFileName);

			if (unlink(target_file))
				fatal("%s: грешка %d при изтриване.", target_file, errno);

		} while (FindNextFile(hFind, &find));
	}

	end_find(hFind, target_mask);
	copy_all(mask, subdir, maxlen);
}

static void mk_subdir(const char *dir1, const char *dir2 = NULL)
{
	char target[MAX_PATH];

	sbc(target, lmpath, dir1, dir2);
	log("mkdir", "създаване на", target);

	if (!exists(target, false) && mkdir(target))
		fatal("%s: грешка %d при създаване.", target, errno);
}

static void do_client()
{
	// 2009:266 LPR: copy connect.exe only
	static constant connect_exe = "connect.exe";

	if (exists(connect_exe))
	{
		log("copy", "копиране на", connect_exe);
		copy_file(connect_exe, NULL);
	}

	fresh_all("*.fm?", "text", 12);
	mk_subdir("htm");
	fresh_all("*.htm", "htm", 21);
	fresh_all("*.xml", "htm", 23);
	fresh_all("*.png", "htm", 23);
	mk_subdir("image");
	copy_all("*.bmp", "image", 12);
	mk_subdir("image", "court");
	copy_all("logo.*", "image\\court", 12);

	// 2012:177 ARQ: moved last for cancel resistence
	log("copy", "копиране на", appname_exe);
	copy_file(appname_exe, NULL);
}

static constant reserver_sql = "reserver.sql";

static int exec_failed(const char **command, int *pipes, bool interbase_mode, const char *reserver_log)
{
	int log_fd = open(reserver_log, O_CREAT | O_TRUNC | O_WRONLY | O_BINARY, S_IWRITE);

	if (log_fd == -1)
		fatal("%s: грешка %d при отваряне.", reserver_log, errno);

	const char *xsql = command[0];
	int result = spawnv(P_NOWAIT, xsql, (char **) command);

	if (result == -1)
		fatal("%s %s: грешка %d при стартиране.", xsql, reserver_sql, errno);

	// read pipes
	HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | SYNCHRONIZE, FALSE, result);
	DWORD wait4;

	if (!hProcess)
		fatal("OpenProcess: грешка %lu!", GETLASTERROR);

	bool log_write_failed = false;
	int xsql_likely_failed = false;	// -1 == failed, retry exec
	char roll_buffer[256 + 1];
	#define HALF_SIZE (sizeof roll_buffer / 2)

	memset(roll_buffer, ' ', HALF_SIZE);

	do
	{
		#define read_buffer (roll_buffer + HALF_SIZE)
		int n_read;

		wait4 = WaitForSingleObject(hProcess, 50);

		while ((n_read = read(pipes[0], read_buffer, HALF_SIZE)) > 0)
		{
			write_console(read_buffer, n_read);

			if (write(log_fd, read_buffer, n_read) < n_read)
				log_write_failed = true;

			if (!xsql_likely_failed)
			{
				read_buffer[n_read] = '\0';

				if (interbase_mode)
				{
					if (strstr(roll_buffer, "Statement failed") || strstr(roll_buffer, "encountered EOF"))
						xsql_likely_failed = 1;
				}
				else
				{
					static constant fail_texts[] = { "fe_sendauth", "authentication failed", "FATAL", "ERROR",
						NULL };
					const char *fail_text;

					for (int i = 0; (fail_text = fail_texts[i]) != NULL; i++)
					{
						if (strstr(roll_buffer, fail_text))
						{
							xsql_likely_failed = strstr(fail_text, "auth") ? -1 : 1;
							break;
						}
					}
				}

				memmove(roll_buffer, roll_buffer + n_read, HALF_SIZE + 1);
			}
		}
	} while (wait4 == WAIT_TIMEOUT);

	// close log asap
	if (close(log_fd) == -1)
	{
		if (log_write_failed)
			error("%s: грешки при запис и затваряне. Възможно е журнала да е непълен.", reserver_log);
		else
			error("%s: грешка %d при затваряне. Възможно е журнала да е непълен.", reserver_log, errno);
	}
	else if (log_write_failed)
		error("%s: грешка при запис. Възможно е журнала да е непълен.", reserver_log);

	// check result
	int failed = 1;

	switch (wait4)
	{
		case WAIT_FAILED : error("WaitForSingleObject: грешка %lu.", GETLASTERROR); break;
		case WAIT_OBJECT_0 :
		{
			DWORD exitCode;

			if (!GetExitCodeProcess(hProcess, &exitCode))
				error("GetExitCodeProcess: грешка %lu.", GETLASTERROR);
			else if (exitCode == 0)
				failed = xsql_likely_failed != 0;
			else if (xsql_likely_failed != -1)
				error("%s %s: грешка %lu при изпълнение.", xsql, reserver_sql, (unsigned long) exitCode);
			else if (warn("%s %s: грешка %lu при изпълнение.\n\nМоже би е въведена неправилна парола. "
				"Желаете ли да опитате отново?", xsql, reserver_sql, (unsigned long) exitCode))
			{
				failed = -1;
			}

			break;
		}
		default : error("WaitForSingleObject: резултат %lu.", (unsigned long) wait4);
	}

	CloseHandle(hProcess);
	return failed;
}

static void do_server(const char *xsql, const char *court_sql, const char *interbase_gdb, const char *postgres_port)
{
	constant sql_scripts[] = { "common~.sql", "update.sql", "common.sql", court_sql, NULL };
	char lmpath_reserver_sql[MAX_PATH];
	char lmpath_sql[MAX_PATH];
	char reserver_log[MAX_PATH];

	sbc(reserver_log, lmpath, "reserver.log");

	mk_subdir("_sql");
	sbc(lmpath_sql, lmpath, "_sql");

	sbc(lmpath_reserver_sql, lmpath_sql, reserver_sql);
	lm_combine_scripts(lmpath_reserver_sql, sql_scripts, interbase_gdb);

	if (chdir(lmpath_sql))
		fatal("%s: грешка %d при смяна на текущата директория.", lmpath_sql, errno);

	// setup pipes
	int pipes[2];

	if (_pipe(pipes, 512, O_BINARY))
		fatal("_pipe: грешка %d.", errno);

	{
		HANDLE h = (HANDLE) _get_osfhandle(pipes[0]);
		DWORD state;

		if (h == INVALID_HANDLE_VALUE)
			fatal("_get_osfhandle: грешка %d.", errno);

		if (!GetNamedPipeHandleState(h, &state, NULL, NULL, NULL, NULL, 0))
			fatal("GetNamedPipeHandleState: грешка %lu.", GETLASTERROR);

		state |= PIPE_NOWAIT;

		if (!SetNamedPipeHandleState(h, &state, NULL, NULL))
			fatal("SetNamedPipeHandleState: грешка %lu.", GETLASTERROR);
	}

	if (dup2(pipes[1], 2) || dup2(pipes[1], 1))
		fatal("dup2: грешка %d.", errno);

	close(pipes[1]);

	// exec/retry
	const char *command[20];
	size_t command_count = 0;
	#define record(s) (command[command_count++] = (s))
	#define record2(s1, s2) do { record(s1); record(s2); } while (0)

	record(xsql);

	if (interbase_gdb != NULL)
	{
		record("-e");
		record("-m");
		record("-i");
	}
	else if (postgres_port != NULL)
	{
		record("-e");
		record("-q");
		record2("-h", "127.0.0.1");
		record2("-p", postgres_port);
		record2("-d", "CAC_REGCOURT");
		record2("-U", "CAC_REGADMIN");
		record("-X");
		record("-f");
	}
	else
		fatal("do_server: невалидни параметри!");

	record(reserver_sql);
	command[command_count] = NULL;

	// exec / retry
	int failed;
	const char *xsql_base = basename(xsql);

	log(xsql_base, xsql_base, reserver_sql);

	while ((failed = exec_failed(command, pipes, interbase_gdb, reserver_log)) == -1)
		if (!ask("Може би е въведена неправилна парола. Желаете ли да опитате отново?"))
			break;

	close(pipes[0]);
	close(1);
	close(2);

	spawnlp(P_NOWAIT, "notepad.exe", "notepad", reserver_log, NULL);

	if (failed)
		message(MB_ICONWARNING | MB_OK, "Актуализацията вероятно е неуспешна. Прегледайте %s за грешки.", reserver_log);
	else
		message(MB_OK, "Актуализацията приключи. Прегледайте %s за грешки.", reserver_log);
}

static void fatal_ver_lack(const char *name)
{
	fatal("%s: информацията за версията на файла липсва или е недостъпна (грешка %lu).", name, GetLastError());
}

static void fatal_ver_bad(const char *name)
{
	fatal("%s: информацията за версията на файла е невалидна.", name);
}

static DWORD ver_buff[0x200];

static char *file_version(const char *name)
{
	DWORD size, zero;
	UINT len;
	char *file;

	if ((size = GetFileVersionInfoSize((char *) name, &zero)) == 0)
		fatal_ver_lack(name);

	if (size > sizeof ver_buff)
		fatal("%s: информацията за версията на файла е прекалено дълга.", name);

	if (!GetFileVersionInfo((char *) name, zero, sizeof ver_buff, ver_buff))
		fatal_ver_lack(name);

	if (!VerQueryValue(ver_buff,  "\\StringFileInfo\\040904E4\\FileVersion", (void **) &file, &len) || len == 0)
		fatal_ver_lack(name);

	return file;
}

struct lm_version
{
	int base;
	int extra;
	char type[9];
};

static void get_version(const char *name, lm_version *version)
{
	char *product, *file = file_version(name), *type;
	UINT len;

	if (!VerQueryValue(ver_buff, "\\StringFileInfo\\040904E4\\ProductVersion", (void **) &product, &len) || len == 0)
		fatal_ver_lack(name);
	if (strncmp(product, "1.", 2) || strncmp(file, "1.", 2))
		fatal_ver_bad(name);
	if ((version->base = atoi(product + 2)) <= 0)
		fatal_ver_bad(name);
	if ((file = strchr(file, '+')) == NULL)
		version->extra = 0;
	else if ((version->extra = atoi(file + 1)) < 0 || (version->extra == 0 && file[1] != '0'))
		fatal_ver_bad(name);

	if (!VerQueryValue(ver_buff,  "\\StringFileInfo\\040904E4\\SpecialBuild", (void **) &type, &len) || len == 0)
		fatal_ver_lack(name);

	if (!strcmp(type, "Appealative"))
		strcpy(version->type, "appeal");
	else if (!strcmp(type, "Administrative"))
		strcpy(version->type, "admin");
	else if (!strcmp(type, "Areal/District"))
		strcpy(version->type, "area");
	else if (!strcmp(type, "Special"))
		strcpy(version->type, "special");
	else if (!strcmp(type, "Military"))
		strcpy(version->type, "military");
	else if (!strcmp(type, "Regional"))
		strcpy(version->type, "region");
	else
		fatal_ver_bad(name);
}

#define E_FIND_LM "%s: грешка %lu при търсене на директорията на САС. Укажете явно директорията за актуализация."

static int find_lmpath(const char *dir, lm_version *version, int sofar)
{
	char target_mask[MAX_PATH];
	char target[MAX_PATH];
	HANDLE hFind;
	WIN32_FIND_DATA find;

	sbc(target_mask, dir, "*.*");

	if ((hFind = FindFirstFile(target_mask, &find)) == INVALID_HANDLE_VALUE)
	{
		switch (GetLastError())
		{
			case ERROR_FILE_NOT_FOUND :
			case ERROR_PATH_NOT_FOUND :
			case ERROR_INVALID_DRIVE :
			case ERROR_NO_MORE_FILES :
			case ERROR_BAD_UNIT :
			case ERROR_NOT_READY :
			case ERROR_NOT_SUPPORTED :
			case ERROR_DEV_NOT_EXIST :
			case ERROR_UNEXP_NET_ERR :
			case ERROR_NETNAME_DELETED :
			case ERROR_BAD_DEV_TYPE :
			case ERROR_BAD_NET_NAME :
			case ERROR_REQ_NOT_ACCEP :
			case ERROR_CALL_NOT_IMPLEMENTED :
			case ERROR_UNRECOGNIZED_VOLUME : return sofar;	// to avoid big else block
			default : fatal(E_FIND_LM, target_mask, GetLastError());
		}
	}

	do
	{
		if (!(find.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			continue;

		if (!strcmp(find.cFileName, ".") || !strcmp(find.cFileName, ".."))
			continue;

		sbc(target, dir, find.cFileName, appname_exe);

		if (exists(target))
		{
			get_version(target, version);
			sbc(lmpath, dir, find.cFileName);
			log("found LM dir", NULL, lmpath);

			if (++sofar > 1)
			{
				SetLastError(ERROR_NO_MORE_FILES);
				break;
			}
		}

	} while (FindNextFile(hFind, &find));

	end_find(hFind, target_mask);
	return sofar;
}

#define E_MANY_LM "%s: директорията съдържа файлове за актуализация на два или повече вида съдилища."
#define E_MANY_THIS "Актуализацоинната директория съдържа файлове за актуализация на два или повече вида съдилища."

static const char *find_court_sql(const char *dir)
{
	const char *court_sql = NULL;
	static constant sqls[] = { "appeal.sql", "admin.sql", "area.sql", "special.sql", "military.sql", "region.sql", NULL };

	for (constant *sqlp = sqls; *sqlp; sqlp++)
	{
		char target[MAX_PATH];

		if (dir)
			sbc(target, dir, *sqlp);
		else
			strcpy(target, *sqlp);

		if (exists(target))
		{
			if (court_sql != NULL)
			{
				if (dir)
					fatal(E_MANY_LM, dir);
				else
					fatal(E_MANY_THIS);
			}

			court_sql = *sqlp;
		}
	}

	return court_sql;
}

static void check_sqls(const char *dir)
{
	static constant sqls[] = { "common.sql", "common~.sql", "update.sql", NULL };

	for (constant *sqlp = sqls; *sqlp; sqlp++)
	{
		char target[MAX_PATH];

		if (dir)
			sbc(target, dir, *sqlp);
		else
			strcpy(target, *sqlp);

		if (!exists(target))
		{
			if (dir)
				fatal("%s: директорията не съдържа необходимите SQL файлове.", dir);
			else
				fatal("Актуализационната директория не съдържа необходимите SQL файлове.");
		}
	}
}

static HWND HMessageWindow = NULL;

static void select_dir(char *target, const char *prompt)
{
	// no error checks here
	if (!ppMalloc)
		CoGetMalloc(1, &ppMalloc);

	if (!ppDrives)
		SHGetSpecialFolderLocation(NULL, CSIDL_DRIVES, &ppDrives);

	BROWSEINFO bi =
	{
		HMessageWindow,
		ppDrives,
		target,
		prompt,
		BIF_RETURNONLYFSDIRS,
		NULL,
		0,
		0
	};

	LPITEMIDLIST list;

	if ((list = SHBrowseForFolder(&bi)) == NULL)
		exit(0);

	if (!SHGetPathFromIDList(list, target) || !*target)
		fatal("Грешка при конверсия на идентификатор към директория.");

	if (ppMalloc && list)
		ppMalloc->lpVtbl->Free(ppMalloc, list);	// no error check

	if (strlen(target) >= MAX_PATH)
		fatal("%s: името на директорията е прекалено дълго.", target);

	if (target[strlen(target) - 1] == '\\')
		target[strlen(target) - 1] = '\0';
}

static HWND HLMWindow = NULL;

static BOOL CALLBACK WindowEnumerator(HWND hwnd, LPARAM)
{
	DWORD pid;
	char buffer[0x100];

	if (GetWindowText(hwnd, buffer, sizeof buffer) &&
		strstr(buffer, "САС") && strstr(buffer, "ДЕЛОВОДСТВО") &&
		GetClassName(hwnd, buffer, sizeof buffer) &&
		(strstr(buffer, "OWL") || *buffer == '#'))
	{
		HLMWindow = hwnd;
	}

	GetWindowThreadProcessId(hwnd, &pid);

	if (pid == GetCurrentProcessId())
		HMessageWindow = hwnd;

	return HMessageWindow == NULL || HLMWindow == NULL;
}

void set_title(const char *s)
{
	char oem[0x100];

	CharToOem(s, oem);
	SetConsoleTitle(oem);
}

BOOL WINAPI ConsoleCtrlHandler(DWORD dwCtrlType)
{
	return dwCtrlType == CTRL_C_EVENT || dwCtrlType == CTRL_CLOSE_EVENT;
}

#define M_COMPAT "Този синтаксис е само за съвместимост. Използвайте C: или буквата на съответното устройство."
#define V_OLD_SAME "Актуализацията е към същата или по стара версия. Желаете ли да продължите?"
#define V_AUTORUN "Актуализацията приключи. Да стартирам ли САС?"

int main(int argc, char **argv)
{
	set_title(AppNameName);

	EnumWindows(WindowEnumerator, 0);

	if (HLMWindow != NULL)
		message(MB_WARN, W_RUNNING);

	bool start = false;
	int init_argc = argc;

	while (argc >= 2)
	{
		const char *last_arg = argv[argc - 1];

		if (!strcmpi(last_arg, "/xcopy"))
		{
			// ignore
			argc--;
		}
		else if (!strcmpi(last_arg, "/chdir"))
		{
			size_t size = basename(last_arg) - last_arg;

			if (size > 0 && last_arg[size] != '\0')
			{
				char current[MAX_PATH];
				char workdir[MAX_PATH];

				memcpy(workdir, last_arg, size);
				workdir[size] = '\0';

				if (!getcwd(current, sizeof current) || strcmpi(current, workdir))
					if (chdir(workdir))
						fatal("%s: грешка %d при смяна на текущата директория.", workdir, errno);
			}
		}
		else if (!strcmpi(last_arg, "/quiet"))
		{
			quiet = true;
			argc--;
		}
		else if (!strncmpi(last_arg, "/start:", 7))
		{
			int seconds = atoi(last_arg + 7);

			if (seconds < 1 || seconds > 10)
				fatal("Невалиден брой секунди за изчакване.");

			start = true;
			Sleep(seconds * 1000);
			argc--;
		}
		else
			break;
	}

	bool lm_spec = argc >= 2;
	bool xs_spec = argc >= 3;
	lm_version ver_this, ver_lmpath;

	if (argc >= 4)
		fatal("Невалиден брой параметри.");

	const char *court_sql_this = find_court_sql(NULL);

	if (!court_sql_this || !exists(appname_exe))
		fatal("Актуализационната директория не съдържа необходимите файлове.");

	{
		char appname_exe_this[MAX_PATH];

		sbc(appname_exe_this, ".", appname_exe);
		get_version(appname_exe_this, &ver_this);
	}

	// this type is not empty, we do ncmp to cut sql to the first word
	if (strncmp(ver_this.type, court_sql_this, strlen(ver_this.type)))
		fatal("Актуализацията е некомплектна: %s за %s версия!", court_sql_this, ver_this.type);

	bool lm_checked = false;

	if (lm_spec)
	{
		// check lmpath
		if (strlen(argv[1]) == 2 && isalpha(argv[1][0]) && argv[1][1] == ':')
		{
			sbc(lmpath, argv[1], "Program Files", "IS - Varna\\Lawsuit Management");
			lmpath[0] = argv[1][0];
		}
		else
		{
			if (strlen(argv[1]) >= sizeof lmpath)
				fatal("%s: името на директорията е прекалено дълго.", argv[1]);

			strcpy(lmpath, argv[1]);
		}
	}
	else
	{
		// find lmpath
		int count = find_lmpath("C:\\Program Files\\IS - Varna", &ver_lmpath, 0);

		if (count < 2)
			count = find_lmpath("C:\\Program Files (x86)\\IS - Varna", &ver_lmpath, count);

		if (count < 2 && GetDriveType("D:\\") == DRIVE_FIXED)
		{
			count = find_lmpath("D:\\Program Files\\IS - Varna", &ver_lmpath, count);

			if (count < 2)
				count = find_lmpath("D:\\Program Files (x86)\\IS - Varna", &ver_lmpath, count);
		}

		if (count == 1)
			lm_checked = true;
		else
		{
			select_dir(lmpath, count ? "Открити са две или повече САС директории.\nИзберете директория за "
				"актуализация." : "Не е открита директория на САС.\nИзберете директория за актуализация.");
		}
	}

	if (!lm_checked)
	{
		char appname_exe_lmpath[MAX_PATH];
		sbc(appname_exe_lmpath, lmpath, appname_exe);

		if (!exists(appname_exe_lmpath))
			fatal("%s: директорията не съдържа необходимите файлове.", lmpath);

		get_version(appname_exe_lmpath, &ver_lmpath);
	}

	// lmpath type may be empty, we do ncmp to skip the test or cut sql
	if (strncmp(ver_lmpath.type, court_sql_this, strlen(ver_lmpath.type)))
		fatal("Актуализацията е за друг вид съд.");

	// lm path found, open log & console
	sbc(LogName, lmpath, "reclient.log");
	if ((LogFile = fopen(LogName, "w")) == NULL)
		fatal("%s: грешка %d при отваряне.", LogName, errno);

	HConsole = CreateFile("CONOUT$", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_WRITE | FILE_SHARE_READ, NULL,
		OPEN_EXISTING, 0, NULL);

	if (HConsole == INVALID_HANDLE_VALUE)
		fatal("CONOUT: грешка %lu при отваряне.", GETLASTERROR);

	// start update		
	log("update", "актуализация на", lmpath);
	for (int arg = 1; arg < init_argc; arg++)
		log("argument", "параметър", argv[arg]);

	bool server = false;
	char xsql_short[MAX_PATH];
	char appname_gdb[MAX_PATH];
	char appname_dump[MAX_PATH];
	char *appname_port = NULL;

	sbc(appname_gdb, lmpath, "appname.gdb");
	bool gdb_exist = exists(appname_gdb);
	bool dump_exist = false;

	{
		char appname_dump_mask[MAX_PATH];
		HANDLE hFind;
		WIN32_FIND_DATA find;

		sbc(appname_dump_mask, lmpath, "appname.dump*");

		if ((hFind = FindFirstFile(appname_dump_mask, &find)) != INVALID_HANDLE_VALUE)
		{
			sbc(appname_dump, lmpath, find.cFileName);

			if (find.cFileName[12] != '-')
				fatal("%s: невалидно име на dump файл.", appname_dump);

			appname_port = strrchr(appname_dump, '-') + 1;
			dump_exist = true;

			const char *invalid = invalid_port(appname_port);

			if (invalid)
				fatal("%s: %s.", invalid, appname_dump);

			if (strcmp(appname_port, "5432"))
				lmessage(MB_WARN, "W_PORT_PG", W_PORT_PG);

			if (FindNextFile(hFind, &find))	// must be exactly before end_find()
				fatal("%s: открити са два или повече dump файла.", appname_dump_mask);
		}

		end_find(hFind, appname_dump_mask);
	}

	if (!start && !quiet && (gdb_exist || dump_exist))
	{
		// we're server
		if (gdb_exist && dump_exist)
			fatal("Намерени са едновременно база данни за InterBase/Firebird и dump файл за PostgreSQL.");

		if (gdb_exist)
			log("found IB database", "намерена е InterBase/Firebird БД", appname_gdb);
		else
			log("found dump file", "намерен е dump файл", appname_dump);

		check_sqls(NULL);

		// check _sql\\*.sql if any
		const char *court_sql_lmpath = find_court_sql(lmpath);

		if (court_sql_lmpath)
		{
			if (!strcmp(court_sql_this, court_sql_lmpath))
				fatal("%s: директорията съдържа SQL файлове за друг вид съд.", lmpath);

			check_sqls(lmpath);
		}

		if (ver_this.base > ver_lmpath.base)
		{
			const char *xsql_base = gdb_exist ? "isql.exe" : "psql.exe";
			char bin_xsql_exe[MAX_PATH];
			char xsql[MAX_PATH];

			sbc(bin_xsql_exe, "bin", xsql_base);
			log("server", "сървърска актуализация", NULL);
			// doing server, so find ibspec
			if (xs_spec)
			{
				sbc(xsql, argv[2], bin_xsql_exe);

				if (!exists(xsql))
					fatal("%s: директорията не съдържа необходимите файлове.", argv[2]);
			}
			else
			{
				sbc(xsql, lmpath, "_sql", xsql_base);

				if (!exists(xsql) && !find_sql_executable(xsql_base, gdb_exist ? ibase_sql_dirs : pgres_sql_dirs,
					xsql))
				{
					lmessage(MB_WARN, "W_FIND_DB", W_FIND_DB, gdb_exist ? W_FIND_IB : W_FIND_PG,
						basename(gdb_exist ? appname_gdb : appname_dump));

					char server_path[MAX_PATH];

					select_dir(server_path, gdb_exist ? "Посочете директорията на InterBase/Firebird:" :
						"Посочете директорията на PostgreSQL:");
					sbc(xsql, server_path, bin_xsql_exe);

					if (!exists(xsql))
						fatal("%s: директорията не съдържа необходимите файлове.", server_path);
				}
			}

			shorten_and_check(xsql, xsql_short);
			lmessage(MB_WARN, "W_SERVER", W_SERVER, gdb_exist ? "е открита база данни" : "е открит dump файл",
				gdb_exist ? "InterBase/Firebird" : "PostgreSQL",
				basename(gdb_exist ? appname_gdb : appname_dump));
			server = true;
		}
		else
			lmessage(MB_WARN, "W_CLIENT", W_CLIENT, gdb_exist ? "Открита е база данни" : "Открит е dump файл");
	}
	else	// we're client
	{
		if (xs_spec)
			fatal("Указана е сървърска директория, но липсва база данни или dump файл.");

		// no XS, check version and do client update
		if (ver_this.base < ver_lmpath.base || (ver_this.base == ver_lmpath.base && ver_this.extra <= ver_lmpath.extra))
		{
			if (quiet)
			{
				cleanup();
				return 0;	// nothing to do
			}

			lmessage(MB_ICONQUESTION | MB_YESNO, "V_OLD_SAME", V_OLD_SAME);
		}
	}

	SetConsoleCtrlHandler(ConsoleCtrlHandler, TRUE);

	set_title("Актуализация на САС - моля изчакайте...");

	// 2015:300 inspired by postgres wrong password
	if (server)
	{
		char appname_exe_lmpath[MAX_PATH];
		char appname_backup_lmpath[MAX_PATH];

		sbc(appname_exe_lmpath, lmpath, appname_exe);
		sbc(appname_backup_lmpath, lmpath, "AppName.exe.bak");

		log("backup", "съхраняване", "AppName.exe.bak");

		if (!CopyFile(appname_exe_lmpath, appname_backup_lmpath, FALSE))
			fatal("%s: грешка %lu при копиране.", appname_backup_lmpath, GetLastError());
	}

	do_client();

	if (!quiet)
	{
		if (start)
		{
			SetConsoleTitle("Рестартиране на САС - моля изчакайте...");
			Sleep(1000);

			if (chdir(lmpath))
				fatal("%s: грешка %d при смяна на текущата директория.", lmpath, errno);

			cleanup();
			execl(appname_exe, "appname", NULL);
			fatal("Грешка %d при стартиране на САС.", errno);
		}
		else if (server)
		{
			SetConsoleCP(1251);
			do_server(xsql_short, court_sql_this, gdb_exist ? appname_gdb : NULL, appname_port);
		}
		else
		{
			set_title(AppNameName);
			message(MB_INFO, "Актуализацията приключи успешно.");
		}
	}

	cleanup();
	return 0;
}
