#include "cac.h"

static constant ActionNames[ACTION_COUNT] =
{
	"view",
	"edit",
	"open",
};

const long FILE_SIZE_MAX[TROUBLE_COUNT] = { 10485760, 20971520 };

TElectricType Electrics[TYPE_TOTAL] =
{
	{ "Текстови документи",   "текстов",   "текстовия" },
	{ "Графични документи",   "графичен",  "графичния" },
	{ "Обезличени документи", "обезличен", "обезличения" },
	{ "ЕИСПП пакети",         "ЕИСПП",     "ЕИСПП" },
	{ "Текстови файлове",     "текстов",   "текстовия" },
	{ "Графични документи",   "графичен",  "графичния" }
};

bool make_directory(const char *name)
{
	char *base;
	char tName[SIZE_OF_PATH];

	strcpy(tName, name);
	base = basename(tName);

	if (base > tName + 1)
	{
		base[-1] = '\0';

		if (exist(tName))		// exist as file ::= error
			return error("%s: съществува, но не е директория.", tName);

		if (errno != EISDIR && mkdir(tName))	// exist as dir ::= OK
			return error("%s: грешка %d при създаване на директория.", tName, errno);
	}

	return true;
}

bool name_shortened(const char *name, char *tName, bool force)
{
	if (!strcmpi(file_ext(name), "docx"))
	{
		// 2016:110 URQ/TRQ: also handle tempdir
		constant dirs[] = { tempdir, Electrics[TYPE_TEXT].docketPath, "" };
		const char *dir;

		for (int i = 0; *(dir = dirs[i]); i++)
		{
			size_t len = strlen(dir);

			if (!strncmpi(name, dir, len) && strchr("\\/", name[len]))
			{
				const TDoubleExt *ext = Electrics[TYPE_TEXT].ext;

				if (!force && !strcmpi(ext->type, "docx") && !strcmpi(ext->ltype, "doc"))
				{
					FILE *f = fopen(name, "rb");

					if (f)
					{
						char twocc[2] = { '\0', '\0' };

						if (fread(twocc, 1, sizeof twocc, f) == sizeof twocc &&
							(twocc[0] != 'P' || twocc[1] != 'K'))
						{
							force = true;
						}

						fclose(f);
					}
				}

				if (force)
				{
					DWORD result = GetShortPathName(name, tName, SIZE_OF_PATH);
					return result > 0 && result < SIZE_OF_PATH;
				}
			}
		}
	}

	return false;
}

#if TESTVER
struct TLaunchInfo
{
	HANDLE hProcess;
	DWORD processId;
	HWND hWindow;
};

static BOOL CALLBACK find_process_window(HWND hWnd, LPARAM lParam)
{
	TLaunchInfo *info = (TLaunchInfo *) lParam;
	DWORD procId = ~info->processId;

	GetWindowThreadProcessId(hWnd, &procId);

	if (procId == info->processId)
	{
		info->hWindow = hWnd;
		return FALSE;
	}

	return TRUE;
}

static bool launch(const char *path, const char *action, const char *args, const char *name, TLaunchInfo *info)
{
	char tName[SIZE_OF_PATH];

	if (name_shortened(name, tName, false))
		name = tName;

	if (!strchr(C2S[SETTING_PROGID_PREFIX], path[0]))
	{
		STARTUPINFO startInfo;
		PROCESS_INFORMATION procInfo;
		mstr m;

		memset(&startInfo, '\0', sizeof startInfo);
		startInfo.cb = sizeof startInfo;
		m.printf("\"%s\" %s \"%s\"", path, args, name);

		if (!CreateProcess(NULL, ncstr(m), NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS, NULL, NULL, &startInfo, &procInfo))
			return error("%s: грешка %lu при стартиране.", path, GETLASTERROR);

		info->hProcess = procInfo.hProcess;
		info->processId = procInfo.dwProcessId;
		CloseHandle(procInfo.hThread);
	}
	else
	{
		SHELLEXECUTEINFO execInfo;

		memset(&execInfo, '\0', sizeof execInfo);
		execInfo.cbSize = sizeof execInfo;
		execInfo.fMask = SEE_MASK_NOCLOSEPROCESS | SEE_MASK_FLAG_DDEWAIT | SEE_MASK_FLAG_NO_UI;
		execInfo.lpVerb = action;
		execInfo.lpFile = name;
		execInfo.nShow = SW_NORMAL;

		if (any(path))
		{
			execInfo.fMask |= SEE_MASK_CLASSNAME;
			execInfo.lpClass = path + 1;
		}

		bool result = ShellExecuteEx(&execInfo);

		if (!result && strcmpi(action, "open") && (execInfo.hInstApp == (HINSTANCE) SE_ERR_NOASSOC ||
			execInfo.hInstApp == (HINSTANCE) SE_ERR_ASSOCINCOMPLETE))
		{
			execInfo.lpVerb = "open";
			result = ShellExecuteEx(&execInfo);
		}

		if (!result)
			return error("%s: грешка %lu при отваряне.", name, (unsigned long) execInfo.hInstApp);

		info->hProcess = execInfo.hProcess;
		info->processId = GetProcessId ? GetProcessId(execInfo.hProcess) : 0;
	}

	return true;
}

static bool launch(const char *path, const char *action, const char *args, const char *name, TWindow *parent)
{
	TLaunchInfo info;

	if (!launch(path, action, args, name, &info))
		return false;

	if (parent && WAIT_FOR_ATTACH)
	{
		if (info.hProcess)
		{
			UINT mainSW = window_hide_permanents();
			TWaitWindow *wait = new TWaitWindow(NULL, WAIT_MORE);

			mstr m;
			m.printf("Затворете %s за да продължите...", basename(name));

			// unlikely to work for ShellExecute()
			while (WaitForSingleObject(info.hProcess, SLEEP_SYNC) == WAIT_TIMEOUT)
				wait->TickText(str(m));

			delete wait;				
			window_show_permanents(mainSW, parent->HWindow);
		}
	}
	else
	{
		// SEE_MASK_FLAG_DDEWAIT is more likely to work for ShellExecute()
		WaitForInputIdle(info.hProcess, CHROME_WAIT_FOR_IDLE * 1000);

		if (info.processId)
		{
			info.hWindow = NULL;
			EnumWindows(find_process_window, (LPARAM) &info);

			if (info.hWindow)
				SetForegroundWindow(info.hWindow);
		}
	}

	if (info.hProcess)
		CloseHandle(info.hProcess);

	sleep_sync();
	return true;
}
#else  // TESTVER
struct TFindProcWindow
{
	DWORD procId;
	HWND hWnd;
};

static BOOL CALLBACK find_proc_window(HWND hWnd, LPARAM lParam)
{
	TFindProcWindow *find = (TFindProcWindow *) lParam;
	DWORD procId = ~find->procId;

	GetWindowThreadProcessId(hWnd, &procId);

	if (procId == find->procId)
	{
		find->hWnd = hWnd;
		return FALSE;
	}

	return TRUE;
}

class CDtorSync
{
public:
	CDtorSync(TWindow *tParent);
	~CDtorSync();

	bool WaitFor(HANDLE hProcess, const char *name);
	bool Active() { return parent != NULL; }

protected:
	TWaitWindow *wait;
	TWindow *parent;
	UINT mainSW;
};

CDtorSync::CDtorSync(TWindow *tParent)
	: parent(WAIT_FOR_ATTACH ? tParent : NULL)
{
	if (Active())
	{
		mainSW = window_hide_permanents();
		wait = new TWaitWindow(NULL, WAIT_MORE);
	}
}

CDtorSync::~CDtorSync()
{
	if (Active())
	{
		delete wait;
		window_show_permanents(mainSW, parent->HWindow);
	}
}

bool CDtorSync::WaitFor(HANDLE hProcess, const char *name)
{
	if (parent)
	{
		mstr m;
		m.printf("Затворете %s за да продължите...", basename(name));

		while (WaitForSingleObject(hProcess, SLEEP_SYNC) == WAIT_TIMEOUT)
			wait->TickText(str(m));

		return true;
	}

	return false;
}

static bool launch(const char *path, const char *action, const char *args, const char *name, TWindow *parent)
{
	char tName[SIZE_OF_PATH];
	TFindProcWindow find;
	CDtorSync sync(parent);

	if (name_shortened(name, tName, false))
		name = tName;

	if (any(path))
	{
		STARTUPINFO startInfo;
		PROCESS_INFORMATION procInfo;
		mstr m;

		memset(&startInfo, '\0', sizeof startInfo);
		startInfo.cb = sizeof startInfo;
		m.printf("\"%s\" %s \"%s\"", path, args, name);

		if (!CreateProcess(NULL, ncstr(m), NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS, NULL, NULL, &startInfo, &procInfo))
			return error("%s: грешка %lu при стартиране.", path, GETLASTERROR);

		if (!sync.WaitFor(procInfo.hProcess, name))
		{
			find.procId = procInfo.dwProcessId;
			WaitForInputIdle(procInfo.hProcess, CHROME_WAIT_FOR_IDLE * 1000);
		}

		sleep_sync();
		CloseHandle(procInfo.hThread);
		CloseHandle(procInfo.hProcess);
	}
	else
	{
		SHELLEXECUTEINFO execInfo;

		memset(&execInfo, '\0', sizeof execInfo);
		execInfo.cbSize = sizeof execInfo;
		execInfo.fMask = SEE_MASK_NOCLOSEPROCESS | SEE_MASK_FLAG_DDEWAIT | SEE_MASK_FLAG_NO_UI;
		execInfo.lpVerb = action;
		execInfo.lpFile = name;
		execInfo.nShow = SW_NORMAL;

		if (!ShellExecuteEx(&execInfo))
		{
			if (strcmpi(action, "open") && (execInfo.hInstApp == (HINSTANCE) SE_ERR_NOASSOC ||
				execInfo.hInstApp == (HINSTANCE) SE_ERR_ASSOCINCOMPLETE))
			{
				execInfo.lpVerb = "open";
				if (!ShellExecuteEx(&execInfo))
					return error("%s: грешка %lu при отваряне.", name, (unsigned long) execInfo.hInstApp);
			}
			else
				return error("%s: грешка %lu при отваряне.", name, (unsigned long) execInfo.hInstApp);
		}

		// 2008:029 LPR: check for non-NULL hProcess
		if (!execInfo.hProcess)
			return true;

		if (sync.WaitFor(execInfo.hProcess, name))
			sleep_sync();
		else
		{
			WaitForInputIdle(execInfo.hProcess, CHROME_WAIT_FOR_IDLE * 1000);
			sleep_sync();

			find.procId = GetProcessId ? GetProcessId(execInfo.hProcess) : 0;
			CloseHandle(execInfo.hProcess);

			if (!find.procId)
				return true;
		}
	}

	if (!sync.Active())
	{
		find.hWnd = NULL;
		EnumWindows(find_proc_window, (LPARAM) &find);

		if (find.hWnd)
			SetForegroundWindow(find.hWnd);
	}

	return true;
}
#endif  // TESTVER

// 2010:158 LPR: storage date support; 2010:195 function
const CDate &electric_date(const TRCDKeyContainer *container)
{
	return container->key.type == TYPE_OUTREG && ((TOutReg *) container)->aret ? ((TOutReg *) container)->returned :
		container->date;
}

bool electric_matches(const char *name, int type, bool ltype, bool primary)
{
	TDoubleExt *ext = Electrics[type].ext;

	for (int i = 0; i < ELECTRIC_EXTS_MAX && any(ext[i].type); i++)
	{
		if (matches_exts(name, ext[i].type, ltype ? ext[i].ltype : NULL, NULL))
			return true;

		if (primary)
			break;
	}

	return false;
}

static void electric_name(const TRCDKeyContainer *container, const CDate &date, const char *dir, char *name, int type,
	int extraNo)
{
	mstr m;
	int index = type - TYPE_SBE_DESC;
	char kind = container->kind;

#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
	if (index >= 0)
	{
		type = TYPE_EISPP;

		if (strchr(KIND_PUNISHMENT_INDOCS, container->kind))
			kind = *KIND_PUNISHMENT_INDOCS;
		else if (strchr(KIND_PUNISHMENT_LAWS, container->kind))
			kind = *KIND_PUNISHMENT_LAWS;
	}
#endif  // APPEAL || AREAL || MILITARY || REGIONAL

	m.printf("%02d%02x%02x%02d\\", (int) container->key.no / (container->key.no <= NO_MAX ? 1000 : 10000),
		type == TYPE_EISPP ? (int) date.year % 100 : container->key.type, kind, (int) container->key.year % 100);

	if (container->key.no <= NO_MAX)
		m.printf("%03d", (int) container->key.no % 1000);
	else
	{
		int no = container->key.no % 10000;
		m.printf("%c%c%c", 'a' + no / 22 / 22, 'a' + (no / 22) % 22, 'a' + no % 22);
	}

	m.printf("%01x%02d", date.month, date.day);

	if (type == TYPE_EISPP)
	{
		if (index)
			m.printf(index >= 0x100 ? "%06x" : "%02x", index);
		else
			m.cat("*");
	}
	else
	{
		m.printf("%02d", (int) date.year % 100);

		if (extraNo == -1 && !strcmpi(container->Name, "T_SURROUNDMENT"))
			extraNo = SUBPOENA_NO_MAX + EXTRA_NO_MAX + ((const TSurroundment *) container)->extraNo;

		if (extraNo >= 0)
		{
			if (extraNo)
				m.printf("%04x", extraNo);
			else
				m.cat("*");
		}
	}

	m.printf(".%s", type >= 0 ? Electrics[type].ext[0].type : "*");
	build_fn(dir, str(m), name);
}

void electric_name(const TRCDKeyContainer *container, const char *dir, char *name, int type, int extraNo)
{
	electric_name(container, electric_date(container), dir, name, type, extraNo);
}

void electric_name(const TRCDKeyContainer *container, const CDate &date, char *name, int type, int extraNo)
{
	electric_name(container, date, Electrics[type].docketPath, name, type, extraNo);
}

void electric_name(const TRCDKeyContainer *container, char *name, int type, int extraNo)
{
	electric_name(container, Electrics[type].docketPath, name, type, extraNo);
}

void electric_name(const TSubpoena *subpoena, char *name, int type)
{
	TRCDKeyContainer *const container = kind2RCDKeyContainer(subpoena->kind);
	TVirtPtr VP(container);

	*container << subpoena;
	electric_name(container, Electrics[type].docketPath, name, type, subpoena->subpoenaNo);
}

static bool electric_exist(const TRCDKeyContainer *container, char *name, int type)
{
	electric_name(container, name, type);
	return exist_document(name);
}

void electric_name(const TRCDKeyContainer *container, const CDate &date, TStringFace *name, int type, int extraNo)
{
	char tName[SIZE_OF_PATH];

	electric_name(container, date, tName, type, extraNo);

	if (exist(tName))
		name->SetS(tName);
}

void electric_name(const TRCDKeyContainer *container, TStringFace *name, int type, int extraNo)
{
	electric_name(container, electric_date(container), name, type, extraNo);
}

bool electric_move(TWindow *parent, const TRCDKeyContainer *container, const CDate &date, const char *name, int type,
	const TRegisterAct *act, int extraNo)
{
#if RANGECHECK
	if (act && extraNo != -1)
		fatal("electric_move: both act and extraNo");
#endif  // RANGECHECK

	if (!any(name))
		return true;

	char tName[SIZE_OF_PATH];
	electric_name(container, date, tName, type, extraNo);

	long filter = 0;
	bool copied = false;

	// 2016:008; 2016:013 moved before strcmpi()
	if (TElectric::ExtraUser(extraNo))
		strcpy(file_ext(tName), file_ext(name));
	// 2016:015 FIX: no filter for user extra only
	else if (type == TYPE_TEXT)
	{
		// 2008:141 LPR/URQ: motives by DECISIONS not OTHERS
		if (strchr(KIND_ENDOCS, container->kind) || strchr(KIND_ENDOC_RIDERS, container->kind))
			filter = FILTER_DECISIONS;
		else if (strchr(KIND_SESSIONS, container->kind))
			filter = FILTER_SESSIONS;
		// 2016:117
		else if (TElectric::ExtraSurrouend(extraNo))
			filter = FILTER_DECISIONS;
		else
			filter = FILTER_OTHERS;
	}

	// 2015:295 no copy attempt -> success
	bool result = !strcmpi(name, tName) || (exist(tName) && !TElectric::ExtraUser(extraNo) &&
		!ask("Документът %s вече съществува. Ще го презапишете ли?", tName));

	// 2008:136 LPR: renumber support
	if (!result)
	{
		if (type == TYPE_TEXT && (FILTER_NUMBER | FILTER_RUN_MACRO) && extraNo == -1)
			return filter_number(parent, container, date, name, tName, filter, act);

		if (copy_overead_file(name, tName, false, true))
		{
			clear_archive_attr(name);
			electric_log(container, tName, act);
			copied = result = true;
		}
	}

	if (filter)
		filter_office(parent, container, date, tName, copied);

	return result;
}

void electric_move(const TSubpoena *subpoena, const char *name)
{
	if (any(name))
	{
		// 2014:028 from electric
		char tName[SIZE_OF_PATH];
		electric_name(subpoena, tName, TYPE_IMAGE);

		if (strcmpi(name, tName) &&
			(!exist(tName) || ask("Документът %s вече съществува. Ще го презапишете ли?", tName)) &&
			copy_overead_file(name, tName, false, true))
		{
			TDummyContainer container;

			clear_archive_attr(name);
			container << subpoena;
			electric_log(LOG_ATTACH, &container, tName, subpoena->subpoenaNo);
		}
	}
}

bool electric_action(const char *name, int type, int action, TWindow *parent)
{
	// 2016:004 LPR: ignore the default HTML editor (notepad), better open for VIEW
	if (type == TYPE_HTML && action == ACTION_EDIT && !any(Electrics[type].Actions[action].program))
		action = ACTION_VIEW;

#if TESTVER
	bool matches;
	
	if (type == TYPE_OFFICE)
	{
		type = TYPE_TEXT;
		matches = true;
	}
	else	// 2014:035 LPR/PRQ: matches the 1st type only; 2015:037 force -> TYPE_PLAIN
		matches = (type == TYPE_PLAIN) || electric_matches(name, type, true, true);

	return launch(matches ? Electrics[type].Actions[action].program : "",
		ActionNames[action], Electrics[type].Actions[action].arguments, name, parent);
#else  // TESTVER
	// 2014:035 LPR/PRQ: program for the 1st type only; 2015:037 force -> TYPE_PLAIN
	return launch(type == TYPE_PLAIN || electric_matches(name, type, true, true) ?
		Electrics[type].Actions[action].program : "",
		ActionNames[action], Electrics[type].Actions[action].arguments, name, parent);
#endif  // TESTVER
}

void electric_action(const TRCDKeyContainer *container, int type, int action, TWindow *parent)
{
	char name[SIZE_OF_PATH];

	if (electric_exist(container, name, type))
		electric_action(name, type, action, parent);
}

void electric_edit(TWindow *parent, TStringFace *name, bool update, int type, bool primary)
{
	if (update && any(name))
		electric_action(name->S(), type, ACTION_EDIT, parent);
	else
	{
		char fileName[SIZE_OF_PATH];

		if (GetOpenFileName(parent, fileName, type, primary, sizeof fileName, NULL, false) &&
			electric_action(fileName, type, ACTION_EDIT, parent))
		{
			name->SetS(fileName);
		}
	}
}

bool electric_exist(const TRCDKeyContainer *container, int type)
{
	char name[SIZE_OF_PATH];
	electric_name(container, name, type);
	return exist(name);
}

// 2008:107 (html) group file and helper functions

bool lock_unlock_file(const char *name, bool lock)
{
	DWORD attrib = GetFileAttributes(name);

	if (attrib == 0xFFFFFFFF)
		error("%s: грешка %lu при достъп до атрибутите на файла.", name, GETLASTERROR);

	if ((attrib == 0xFFFFFFFF || !(attrib & FILE_ATTRIBUTE_READONLY) != !lock) &&
		!SetFileAttributes(name, (lock ? FILE_ATTRIBUTE_READONLY : 0) | (attrib & FILE_ATTRIBUTE_ARCHIVE)))
	{
		return error("%s: грешка %lu при смяна на правата за достъп.", name, GETLASTERROR);
	}

	return true;
}

void lock_unlock_file(const char *name, const void *lock)
{
	lock_unlock_file(name, (bool) lock);
}

void clear_archive_attr(const char *name, const void *)
{
	DWORD attrib = GetFileAttributes(name);

	if (attrib != 0xFFFFFFFF && (attrib & (FILE_ATTRIBUTE_ARCHIVE | FILE_ATTRIBUTE_SYSTEM)) == FILE_ATTRIBUTE_ARCHIVE)
		SetFileAttributes(name, attrib & ~FILE_ATTRIBUTE_ARCHIVE);
}

bool unlock_delete_file(const char *name, bool unlock)
{
	if (!unlock || lock_unlock_file(name, false))
	{
		if (!unlink(name))
			return true;

		error("%s: грешка %d при опит за изтриване.", name, errno);

		if (unlock)
			lock_unlock_file(name, true);
	}

	return false;
}

void unlock_delete_file(const char *name, const void *unlock)
{
	unlock_delete_file(name, (bool) unlock);
}

bool copy_overead_file(const char *source, const char *where, int build, bool makedir)
{
	char target[SIZE_OF_PATH];

	if (build == BUILD_REDIR)
	{
		strcpy(target, where);
		*basename(target) = '\0';
		where = target;
	}

	if (build)
		where = build_fn(where, basename(source), target);
	else if (makedir && !make_directory(where))
		return false;

	if (strcmpi(source, where))
	{
		bool exists = exist(where);

		if (!exists || lock_unlock_file(where, false))
		{
			// 2008:137 LPR: (re)lock even on failed copy
			bool copied = CopyFile(source, where, FALSE);
			long lastError = GETLASTERROR;

			// LPR: don't touch, must be != true
			if ((copied || exists) && build != true)
				lock_unlock_file(where, true);

			if (copied)
				return true;

			error("%s: грешка %lu при копиране в %s.", source, lastError, where);
		}
	}

	return false;
}

void copy_overead_file(const char *name, const void *tName)
{
	copy_overead_file(name, (const char *) tName, BUILD_REDIR, false);
}

bool finish_find(HANDLE hFind, const char *name, bool is_html)
{
	unsigned long dwError = GETLASTERROR;

	if (dwError != ERROR_FILE_NOT_FOUND && dwError != ERROR_NO_MORE_FILES && (is_html || dwError != ERROR_PATH_NOT_FOUND))
		return error("%s: грешка %lu при търсене на свързаните файлове.", name, dwError);

	if (hFind != INVALID_HANDLE_VALUE && !FindClose(hFind))
		return error("%s: грешка %lu при търсене на свързаните файлове.", name, dwError);

	return true;
}

void html_group_action(const char *name, void (*action)(const char *name, const void *data), const void *data)
{
	char tName[SIZE_OF_PATH];
	char *base, *ext;
	size_t ext_pos;
	bool is_html;

	strcpy(tName, name);
	base = basename(tName);
#if RANGECHECK
	if (base == tName)
		fatal("group action: %s: no directory.", name);
#endif  // RANGECHECK
	ext = file_ext(base);
	ext_pos = ext - base;

	if (!strcmpi(ext, "xml"))
		is_html = false;
	else
	{
	#if RANGECHECK
		if (!electric_matches(base, TYPE_TEXT, true, true) && !html_name(base))
			fatal("group action: %s: unsupported ext", name);
	#endif  // RANGECHECK
		is_html = true;
		strcpy(ext - 1, "_*.*");
	}

	WIN32_FIND_DATA find;
	HANDLE hFind = FindFirstFile(tName, &find);

	if (hFind != INVALID_HANDLE_VALUE)
	{
		do
		{
			// ignore foo_.ext or foo__x.ext
			if (!is_html || !strchr("_.", find.cFileName[ext_pos]))
			{
				*base = '\0';
				build_fn(tName, find.cFileName, tName);
				action(tName, data);
			}
		} while (FindNextFile(hFind, &find));
	}

	finish_find(hFind, name, is_html);
}

void electric_log(char gop, const TRCDKeyContainer *container, const char *name, int extraNo)
{
	mstr m;

	m.printf("на %s файл", file_ext(name));

	if (extraNo)
		m.printf(" доп # %ld", extraNo);

	log(gop, CM_ELECTRIC_BLUE, container, str(m));
}

void electric_log(const TRCDKeyContainer *container, const char *tName, const TRegisterAct *act)
{
	if (act)
	{
		char text[SIZE_OF_LOG_TEXT];
		sprintf(text, FORMAT_FROM_ACT, act->actNo, act->kind);
		log(LOG_ATTACH, CM_ELECTRIC_BLUE, container, text);
	}
	else
		electric_log(LOG_ATTACH, container, tName, 0);
}

static void electric_rename(const char *name, const void *tName)
{
	if (copy_overead_file(name, (const char *) tName, BUILD_REDIR, false))
		unlock_delete_file(name, true);
}

void electric_rename(const char *name, TRCDKeyContainer *target, int type, bool html_group)
{
	char tName[SIZE_OF_PATH];
	electric_name(target, tName, type);

	if (copy_overead_file(name, tName, false, true))
	{
		unlock_delete_file(name, true);

		if (html_group)
			html_group_action(name, electric_rename, tName);
	}
}

void electric_rename(TRCDKeyContainer *source, TRCDKeyContainer *target)
{
	for (int type = 0; type < TYPE_COUNT; type++)
	{
		char name[SIZE_OF_PATH];
		electric_name(source, name, type);

		if (exist(name))
			electric_rename(name, target, type, type == TYPE_HTML);
	}
}

void electric_rename(TOriginContainer *container)
{
	TOriginContainer *const tContainer = (TOriginContainer *) type2RCDKeyContainer(container->key.type);
	TVirtPtr VP(tContainer);

	*tContainer << container;
	tContainer->kind = tContainer->oldKind;
	electric_rename(tContainer, container);
}

void electric_copy(const char *source, const char *target, sign group_action)
{
	if (copy_overead_file(source, target, false, true))
	{
		if (group_action == true || (group_action == -1 && html_name(source)))
		{
			html_group_action(target, unlock_delete_file, VOID_TRUE);
			html_group_action(source, copy_overead_file, target);
		}
	}
}

void electric_temp(const TRCDKeyContainer *container, char *name, int type)
{
	electric_name(container, tempdir, name, type);
	*strrchr(name, '\\') = '_';

	char *base = basename(name);

	strmove(base + 6, base);
	memcpy(base, "ca$tm_", 6);
}

class TElectricCleanup
{
public:
	TElectricCleanup(char *name, char *tName);
	~TElectricCleanup();

private:
	char *names[2];
};

TElectricCleanup::TElectricCleanup(char *name, char *tName)
{
	names[0] = name;
	names[1] = tName;
}

TElectricCleanup::~TElectricCleanup()
{
	for (int i = 0; i < 2; i++)
	{
		char *name = names[i];

		SetFileAttributes(name, FILE_ATTRIBUTE_NORMAL);
		DeleteFile(name);
		*basename(name) = '\0';
		RemoveDirectory(name);
	}
}

bool electric_access(const char *docketPath)
{
	TUserFile uf("временен");
	char name[SIZE_OF_PATH];
	char tName[SIZE_OF_PATH];

	build_fn(docketPath, "__test0\\__test0.tmp", name);
	build_fn(docketPath, "__test1\\__test1.tmp", tName);

	TElectricCleanup cleanup(name, tName);

	if (!make_directory(name))
		return false;

	if (exist(name) && !unlock_delete_file(name, true))
		return false;

	if (!uf.WriteAll(name, "test\r\n", -1, 0))
		return false;

	if (!lock_unlock_file(name, true))
		return false;

	if (!copy_overead_file(name, tName, false, true))
		return false;

	if (!unlock_delete_file(tName, true))
		return false;

	if (!unlock_delete_file(name, true))
		return false;

	return true;
}

bool electric_rekind_access_check()
{
	for (int type = 0; type < TYPE_COUNT; type++)
	{
		constant docketPath = Electrics[type].docketPath;
		bool isValid = true;

		if (!any(docketPath) && !ask("Не е указана директория за %s. %s", Electrics[type].whats, Q_CONTINUE))
			isValid = false;
		else if (!exist(docketPath, false))
			isValid = error("Директорията за %s липсва или е недостъпна.", Electrics[type].whats);
		else if (!electric_access(docketPath))
			isValid = error("Няма пълен достъп до директорията за %s.", Electrics[type].whats);

		if (!isValid)
			return false;
	}

	return true;
}

bool electric_rekind(const TRCDKeyContainer *container, char targetKind, const CDate &targetDate)
{
	// ignore eispp, it uses the 1st punishment kind for origins
	for (int type = 0; type < TYPE_COUNT; type++)
	{
		constant docketPath = Electrics[type].docketPath;

		if (!any(docketPath))
			continue;

		if (!exist(docketPath, false))
		{
			return error("Няма достъп до директорията за %s. Възстановете достъпа, и изпълнете операцията отново.",
				Electrics[type].whats);
		}

		char name[SIZE_OF_PATH];
		WIN32_FIND_DATA find;

		electric_name(container, container->date, docketPath, name, -1, 0);

		HANDLE hFind = FindFirstFile(name, &find);

		if (hFind != INVALID_HANDLE_VALUE)
		{
			bool makedir = true;

			do
			{
				if (strlen(find.cFileName) < SIZE_OF_TWICE_BASE)
				{
					strcpy(basename(name), find.cFileName);
					// 2017:096 IRQ: ENOENT on copy
					if (exist(name))
					{
						char tName[SIZE_OF_PATH];
						char *baseName;
						mstr temp;

						strcpy(tName, name);
						baseName = basename(tName);
						temp.printf("%02x", targetKind);
						memcpy(baseName - 5, str(temp), 2);

						temp.clear();
						temp.printf("%01x%02d%02ld", targetDate.month, targetDate.day, targetDate.year % 100);
						memcpy(baseName + 3, str(temp), 5);

						if (copy_overead_file(name, tName, false, makedir))
						{
							makedir = false;
							unlock_delete_file(name, true);
						}
					}
				}
			} while (FindNextFile(hFind, &find));
		}

		if (!finish_find(hFind, name, false))
		{
			return error("Грешка при работа с директорията за %s. Извършете необходимите корекции, и изпълнете "
				"операцията отново.", Electrics[type].whats);
		}
	}

	return true;
}
