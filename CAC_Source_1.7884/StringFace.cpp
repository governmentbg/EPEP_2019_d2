#include "cac.h"

DEFINE_RESPONSE_TABLE1(TStringFace, TEdit)
//{{TStringFaceRSP_TBL_BEGIN}}
	EV_MESSAGE(WM_FACE_VALID, EvFaceValid),
	EV_MESSAGE(WM_FACE_NAME, EvFaceName),
//{{TStringFaceRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TStringFace Implementation}}

TStringFace::TStringFace(TWindow* parent, int resourceId, const char *tName, uint textLimit)
:
	TEdit(parent, resourceId, textLimit), name(tName)
{
	buffer = new char[textLimit];
	*buffer = '\0';
}

TStringFace::~TStringFace()
{
	delete[] buffer;
}

const char *TStringFace::S() const
{
	((TStringFace *const) this)->Transfer(buffer, tdGetData);
	return buffer;
}

void TStringFace::SetS(const char *s)
{
	if (s != buffer)
		strcpy(buffer, s);

	Transfer(buffer, tdSetData);
}

void TStringFace::CleanupWindow()
{
	if (Attr.Style & ES_PASSWORD)
		memzero_s(buffer, TextLimit);

	TEdit::CleanupWindow();
}

TResult TStringFace::WindowProc(uint msg, TParam1 p1, TParam2 p2)
{
	if (msg == WM_CHAR)
	{
		if ((p2 & EV_KEY_RELEASE_MASK) == EV_KEY_RELEASE_ALT)
		{
			if (p1 >= 0x20 && p1 <= 0xFF)
				Insert(C2S[p1]);

			return 0;
		}
	}
	else if (msg == WM_PASTE && CHROME_UNICODE_CONVERT)
	{
		char *s = cliptext(this);
		TCharPtr CP(s);

		if (s)
		{
			Insert(s);
			return 0;
		}
	}

	TResult result = TEdit::WindowProc(msg, p1, p2);

	if (msg == WM_COPY && CHROME_UNICODE_CONVERT && !(Attr.Style & ES_PASSWORD))
	{
		TClipboard clipboard(Handle, false);
		uint startPos, finalPos;

		GetSelection(startPos, finalPos);

		if (clipboard && startPos < finalPos)
		{
			HANDLE hLangId = GlobalAlloc(GMEM_MOVEABLE, sizeof(LCID));

			if (hLangId)
			{
				LCID *lpLangId = (LCID *) GlobalLock(hLangId);

				if (!lpLangId)
				{
					GlobalUnlock(hLangId);
					GlobalFree(hLangId);
				}
				else
				{
					*lpLangId = MAKELCID(MAKELANGID(LANG_BULGARIAN, SUBLANG_DEFAULT), SORT_DEFAULT);
					GlobalUnlock(hLangId);
					clipboard.SetClipboardData(CF_LOCALE, hLangId);
				}
			}
		}
	}

	return result;
}

long TStringFace::EvFaceValid(TParam1, TParam2)
{
	return IsValid(true) ? 0 : MW_INVALID_FACE;
}

long TStringFace::EvFaceName(TParam1, TParam2 lParam)
{
	*(const char **) lParam = name;
	return MW_NAMED_FACE;
}
