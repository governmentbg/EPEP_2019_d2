#include "cac.h"

#include <owl/opensave.h>

static UINT CALLBACK OFNHookProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == WM_INITDIALOG)
	{
		RECT desktop, window;
		POINT point;

		GetWindowRect(GetDesktopWindow(), &desktop);
		GetWindowRect(GetParent(hWnd), &window);
		point.x = (desktop.right - desktop.left) / 2 - (window.right - window.left) / 2;
		point.y = (desktop.bottom - desktop.top) / 2 - (window.bottom - window.top) / 2;
		SetWindowPos(GetParent(hWnd), HWND_TOP, point.x, point.y, 0, 0, SWP_NOSIZE);
	}

	return TOpenSaveDialog::StdDlgProc(hWnd, uMsg, wParam, lParam);
}

#include "TNTOpenSaveDialog.h"
#include "TNTOpenSaveDialog.cpp"

#define OFN_ENABLESIZING 0x00800000

static bool GetOpenSaveName(TWindow *parent, char *name, char *initdir, mstr &filter, char *defext, bool open, size_t size,
	const char *defname, bool limitdir)
{
	char *dotext = strchr(defext, '.');

	TOpenSaveDialog::TData data((open ? OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST : OFN_OVERWRITEPROMPT) | OFN_NOCHANGEDIR |
		OFN_EXPLORER | OFN_ENABLESIZING, ncstr(filter), NULL, initdir, dotext ? dotext + 1 : defext, size);

	if (defname)
		strcpy(data.FileName, defname);

	if (TNTOpenSaveDialog(parent, data, open).Execute() == IDOK)
	{
		if (!limitdir || basename(data.FileName) - data.FileName <= LENGTH_OF_DIR)
		{
			strcpy(name, data.FileName);
			return true;
		}

		error("Прекалено дълго име на директория.", name);
	}
	else
	{
		switch (data.Error)
		{
			case FNERR_BUFFERTOOSMALL : error("Прекалено дълго име на файл."); break;
			case FNERR_INVALIDFILENAME : error("Невалидно име на файл."); break;
			case 0 : break;
			default : error("Грешка %lu при избор на файл", data.Error);
		}
	}

	return false;
}

static bool GetOpenSaveName(TWindow *parent, char *name, const char *whats, char *initdir, const char **exts, bool open,
	size_t size, const char *defname, bool limitdir)
{
	mstr m(whats);
	char type[SIZE_OF_BASE_NAME];

	for (int i = 0; exts[i]; i++)
	{
		strcpy(type, exts[i]);
		strlwr(type);
		m.cat(i ? ", " : " (");
		m.printf(strchr(type, '.') ? "%s" : "*.%s", type);
	}

	m.cat(")");
	mbk_append(&m, "", 1);

	for (int i = 0; exts[i]; i++)
	{
		const char *type = exts[i];

		if (i)
			m.sep(";");

		m.printf(strchr(type, '.') ? "%s" : "*.%s", type);
	}

	mbk_append(&m, "", 1);
	strcpy(type, exts[0]);
	strlwr(type);
	return GetOpenSaveName(parent, name, initdir, m, type, open, size, defname, limitdir);
}

bool GetOpenFileName(TWindow *parent, char *name, int type, bool primary, size_t size, const char *defname, bool limitdir)
{
	const char *exts[ELECTRIC_EXTS_MAX * 2 + 1];
	TDoubleExt *ext = Electrics[type].ext;
	size_t n = 0;

	for (int i = 0; i < (primary ? 1 : ELECTRIC_EXTS_MAX) && any(ext->type); i++, ext++)
	{
		exts[n++] = ext->type;

		if (any(ext->ltype))
			exts[n++] = ext->ltype;
	}

	exts[n] = NULL;

	if (!GetOpenSaveName(parent, name, Electrics[type].whats, Electrics[type].sourcePath, exts, true, size, defname,
		limitdir))
	{
		return false;
	}

	if (!primary && !electric_matches(name, type, true, false))
		return error("Разширението на файла не е от разрешените типове.");

	return true;
}

static bool GetOpenSaveName(TWindow *parent, char *name, const char *whats, char *initdir, const char *ext, bool open,
	size_t size, const char *defname, bool limitdir)
{
	const char *exts[2] = { ext, NULL };
	return GetOpenSaveName(parent, name, whats, initdir, exts, open, size, defname, limitdir);
}

bool GetOpenFileName(TWindow *parent, char *name, const char *whats, char *initdir, const char *ext, size_t size,
	const char *defname, bool limitdir)
{
	return GetOpenSaveName(parent, name, whats, initdir, ext, true, size, defname, limitdir);
}

bool GetSaveFileName(TWindow *parent, char *name, const char *whats, char *initdir, const char *ext, size_t size,
	const char *defname, bool limitdir)
{
	return GetOpenSaveName(parent, name, whats, initdir, ext, false, size, defname, limitdir);
}
