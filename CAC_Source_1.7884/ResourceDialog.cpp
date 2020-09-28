#include "cac.h"

//{{TResourceDialog Implementation}}

TResourceDialog::TResourceDialog(TWindow* parent, sign scale, int resId)
:
	TNTDialog(parent, resId), dlgTempl(NULL), dlgMsbmp(true), dlgId(resId)
{
	static BYTE qs_res[] =
	{
		0x01, 0x08, 0x00, 0x40, 0x00, 0x00, 0x02, 0x00, 0x08, 0x00, 0x06, 0x00, 0x0A, 0x00, 0x0A, 0x00,
		0xDA, 0x02, 0xFF, 0xFF, 0x81, 0x00, 0xAB, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x40,
		0x00, 0x00, 0x02, 0x00, 0x12, 0x00, 0x06, 0x00, 0x44, 0x00, 0x0A, 0x00, 0xDB, 0x02, 0xFF, 0xFF,
		0x81, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x08, 0x00, 0x40, 0x00, 0x00, 0x02, 0x00,
		0x56, 0x00, 0x06, 0x00, 0x0A, 0x00, 0x0A, 0x00, 0xDC, 0x02, 0xFF, 0xFF, 0x81, 0x00, 0xBB, 0x00,
		0x00, 0x00, 0x00, 0x00
	};

	HRSRC hr;
	HGLOBAL hg;
	DLGTEMPLATE *dt;
	DWORD size;

	if ((hr = FindResource(NULL, DialogAttr.Name, RT_DIALOG)) == NULL || (hg = ::LoadResource(NULL, hr)) == NULL ||
		(dt = (DLGTEMPLATE *) LockResource(hg)) == NULL || ((size = SizeofResource(NULL, hr)) == 0))
	{
		fatal("Грешка при четене на данни за прозорец %d", dlgId);
	}

#if RANGECHECK
	if ((dt->style & (DS_SETFONT | WS_MAXIMIZE)) != DS_SETFONT)
		fatal("Невалидни флагове на прозорец %d", dlgId);
#endif  // RANGECHECK		

	WORD *ptr = (WORD *) (dt + 1);

	if (*ptr == 0xFFFF)
		ptr += 2;
	else
		while (*ptr++);
	if (*ptr == 0xFFFF)
		ptr += 2;
	else
		while (*ptr++);
	while (*ptr++);

#if RANGECHECK
	if (size % 2)
		fatal("%s: odd size %u, needs full rounding", dlgId, size);
		//sizeR1 += (sizeof(DWORD) - sizeR1 % sizeof(DWORD)) % sizeof(DWORD);
	if (*ptr != 8)
		fatal("%s: invalid scale dialog font size %u", dlgId, (unsigned) *ptr);
#endif  // RANGECHECK

	static const wchar_t
		msss_source[] = L"MS Sans Serif",
		msss_target[] = L"Microsoft Sans Serif",
		mono_source[] = L"Courier New",
		mono_target[] = L"Lucida Console";

	const void *source_font = ptr + 1;
	dlgMsbmp = !memcmp(source_font, msss_source, sizeof msss_source);
	size_t sizeR1 = size + size % sizeof(DWORD);
	long fontScale = scale ? (FONT_SCALE == 1 ? 8 : FONT_SCALE) : (FONT_SCALE ? 8 : 0);
#if 20170242
	if (scale == -1)
	{
		fontScale = round(fontScale * (CHROME_PERCENT_SCALE * 0.01));

		if (fontScale < 8)
			fontScale = 8;
	}
#endif  // 20170242

	const wchar_t *target_font = NULL;
	size_t source_size, target_size;

	if (dlgMsbmp)
	{
		dlgScale = FONT_NORMAL;

		if (fontScale)
		{
			target_font = msss_target;
			source_size = sizeof msss_source;
			target_size = sizeof msss_target;
		}
	}
	else if (!memcmp(source_font, mono_source, sizeof mono_source))
	{
		dlgScale = FONT_MONO;

		if (CHROME_LUCIDACONSOLE)
		{
			target_font = mono_target;
			source_size = sizeof mono_source;
			target_size = sizeof mono_target;
		}
	}
	else if (!memcmp(source_font, mono_target, sizeof mono_target))
		dlgScale = FONT_NORMAL;
	else
		dlgScale.fsOrig = 0;

	if (target_font)
	{
		size_t header = (BYTE *) ptr - (BYTE *) dt;
		size_t old_lead = header + source_size + sizeof(WORD);
		size_t new_lead = header + target_size + sizeof(WORD);
		old_lead += old_lead % sizeof(DWORD);
		new_lead += new_lead % sizeof(DWORD);

		sizeR1 += new_lead - old_lead;
		dlgTempl = (DLGTEMPLATE *) new BYTE[sizeR1 + sizeof qs_res];
		memcpy(dlgTempl, dt, header);
		*(WORD *) ((BYTE *) dlgTempl + header) = (WORD) fontScale;
		memcpy((BYTE *) dlgTempl + header + sizeof(WORD), target_font, target_size);
		memcpy((BYTE *) dlgTempl + new_lead, (BYTE *) dt + old_lead, size - old_lead);
	}
	else
	{
		dlgTempl = (DLGTEMPLATE *) new BYTE[sizeR1 + sizeof qs_res];
		memcpy(dlgTempl, dt, size);

		if (dlgScale.fsOrig)
			*((WORD *) dlgTempl + (ptr - (WORD *) dt)) = (WORD) fontScale;
	}

	// 2016:166 LPR: for focusing; 2016:271 unhacked
//	if (hide)
//		dlgTempl->style &= ~WS_VISIBLE;

	dlgTempl->cdit += (WORD) 3;
	memcpy((BYTE *) dlgTempl + sizeR1, qs_res, sizeof qs_res);
}

TResourceDialog::~TResourceDialog()
{
	delete[] dlgTempl;
}

void TResourceDialog::SetDefault(TButton *unset, TButton *set)
{
	SetDefaultId(set->GetId());
	set->SetStyle(set->GetStyle() | BS_DEFPUSHBUTTON, true);
	unset->SetStyle(unset->GetStyle() & ~BS_DEFPUSHBUTTON, true);
}

HWND TResourceDialog::DoCreate()
{
	return dlgTempl ? ::CreateDialogIndirectParam(*GetModule(), dlgTempl, Parent ? Parent->GetHandle() : NULL,
		(DLGPROC) StdDlgProc, DialogAttr.Param) : TNTDialog::DoCreate();
}

int TResourceDialog::DoExecute()
{
	return dlgTempl ? ::DialogBoxIndirectParam(*GetModule(), dlgTempl, Parent ? Parent->GetHandle() : NULL,
			(DLGPROC) StdDlgProc, DialogAttr.Param) : TNTDialog::DoExecute();
}

#define scale ((TFontScale *) lParam)

static BOOL CALLBACK FixHWScale(HWND hWnd, LPARAM lParam)
{
	RECT rect;
	RECT client;
	POINT point;

	if (!SendMessage(hWnd, CB_GETDROPPEDCONTROLRECT, 0, (LPARAM) &rect))
	{
		GetWindowRect(hWnd, &rect);
		GetClientRect(hWnd, &client);
		int height = rect.bottom - rect.top;

		char name[20];
		int n = GetClassName(hWnd, name, sizeof name);

		if (n < 1 || n > 17)
			*name = '\0';

		if ((height == scale->fsOrig && height > client.bottom - client.top) || !strcmpi(name, "msctls_updown32"))
		{
			point.x = rect.left;
			point.y = rect.top;
			ScreenToClient(GetParent(hWnd), &point);
			MoveWindow(hWnd, point.x, point.y, rect.right - rect.left, scale->fsOver, FALSE);
		}
	}

	return TRUE;
}

#undef scale

void TResourceDialog::SetupWindow()
{
	TNTDialog::SetupWindow();

	if (dlgScale.fsOrig)
		EnumChildWindows(Handle, FixHWScale, (LPARAM) &dlgScale);
}
