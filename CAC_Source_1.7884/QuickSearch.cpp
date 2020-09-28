#include "cac.h"

// subset of globcmpi(), and ' ' is different
static const char *findcmpi(const char *text, const char *patt)
{
	while (*patt)
	{
		bool match;

		switch (*patt)
		{
			case ' ' :
			{
				const char *beg = text;

				while (isspace(*text))
					text++;

				if (text == beg)
					return NULL;

				patt++;
				continue;
			}
			case '\'' :
			case '"' : match = any(text) && strchr(FILTER_QUOTES, *text); break;
			case '-' : match = any(text) && strchr(FILTER_MINUSES, *text); break;
			case '#' :
			{
				static constant Nos[] = { "¹", "No.", "No", "#", NULL };
				constant *no;

				for (no = Nos; *no; no++)
					if (!strncmpi(text, *no, strlen(*no)))
						break;

				if (*no)
				{
					text += strlen(*no);
					patt++;
					continue;
				}

				return NULL;
			}
			default : match = toupper(*text) == toupper(*patt);
		}

		if (!match)
			return NULL;

		text++;
		patt++;
	}

	return text;
}

// 2015:127 LPR: removed the (disabled) metachars ^ and * support
const char *qwfindi(const char *text, const char *patt, bool start)
{
	bool alnum = isalnum(*patt);
	char prevc = '\0';

	while (*text)
	{
		if ((!alnum || !(isalpha(*patt) ? isalpha(prevc) : isdigit(prevc))) && findcmpi(text, patt))
			break;

		if (start)
			return false;

		prevc = *text++;
	}

	return *text ? text : NULL;
}

// ----- TQuickSearch --------------------------------------------------------
void TQuickSearch::Enable()
{
	*text = '\0';
	edit->SetCaption(text);
	Show(SW_SHOW);
}

bool TQuickSearch::Focused()
{
	HWND hWnd = GetFocus();

	for (int i = 0; i < QUICK_SEARCH_CONTROLS; i++)
		if (hWnd == controls[i]->HWindow)
			return true;

	return false;
}

void TQuickSearch::SetText()
{
	int len = strlen(text);
	edit->SetCaption(text);
	edit->SetSelection(len, len);
}

void TQuickSearch::Show(int nCmdShow)
{
	for (int i = 0; i < QUICK_SEARCH_CONTROLS; i++)
		controls[i]->ShowWindow(nCmdShow);
}
