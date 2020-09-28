#include "cac.h"

TQuickList::TQuickList(TWindow *parent, QUICK_SEARCH_MODE tQSMode)
	: TWindow(parent), qs(NULL), qsMode(tQSMode), abcskip(ABC_SKIP_NOTHING)
{
}

void TQuickList::SetQuickSearch(TQuickSearch *tQS)
{
	qs = tQS;

	if (qs)
		QuickPosition();
}

const char *TQuickList::ABCSkip(const char *s, bool spaces)
{
	const char *ts = s;

	if (!spaces)
	{
		if (abcskip == ABC_SKIP_LOWER)
		{
			while (*ts && !isupper(*ts))
				ts++;

			spaces = ts > s;
		}
		else if (abcskip == ABC_SKIP_PREFIX)
		{
			ts = strchr(s, '.');

			if (ts)
			{
				ts++;
				spaces = true;
			}
			else
				ts = s;
		}
	}

	if (spaces)
		ts = skiplead(ts);

	return any(ts) ? ts : s;
}

bool TQuickList::QsStart(const char *)
{
	return false;
}

void TQuickList::QuickRefresh()
{
	if (qs)
		QuickDestroy();
}

void TQuickList::QuickPosition()
{
	TRect list = GetWindowRect();
	TPoint origin(list.right, list.top);
	Parent->ScreenToClient(origin);

	for (int i = QUICK_SEARCH_CONTROLS - 1; i >= 0; i--)
	{
		TRect move = qs->controls[i]->GetWindowRect();
		origin.x -= move.Width();
		qs->controls[i]->MoveWindow(origin.x, origin.y - move.Height(), move.Width(), move.Height());
	}
}

bool TQuickList::QuickSearch(int step, bool jump, const char *text)
{
	int count = GetVisualCount();
	int index = GetVisualIndex();

	if (index < 0)
	{
		index = step > 0 ? 0 : count - 1;
		jump = false;
	}

	for (int i = 0; i < count - jump; i++)
	{
		mstr m;

		if (i || jump)
		{
			index += step;

			if (index < 0)
				index = count - 1;
			else if (index >= count)
				index = 0;
		}

		//FILE *f = fopen("c:\\tmp\\srch.txt", "a");
		//fprintf(f, "i = %d, c = %d, t = %s\n", index, count, text);
		//fclose(f);

		GetSearchText(m, index);

		if (!text)
			text = qs->text;

		if (qwfindi(str(m), text, QsStart(text)))
		{
			SetVisualIndex(index);
			return true;
		}
	}

	return false;
}

void TQuickList::QuickDestroy(bool force)
{
	if (force || qsMode != QUICK_SEARCH_AUTO)
		Parent->SendMessage(WM_QUICK_SEARCH);
	else
	{
		*qs->text = '\0';
		qs->SetText();
	}
}

bool TQuickList::QuickChar(uint key)
{
	if (qsMode != QUICK_SEARCH_NONE)
	{
		// 2011:307 LPR: ' ' instead of isspace() to avoid CR, LF etc.
		if ((isgraph(key) || (key == ' ' && qs && *qs->text)) && GetVisualCount() > 0 &&
			(qs || Parent->SendMessage(WM_QUICK_SEARCH, 0, (LPARAM) this) == MW_SEARCH_QUICK))
		{
			if (strlen(qs->text) + 1 < sizeof qs->text)
			{
				strcach(qs->text, (char) tolower(key));

				if (QuickSearch(1, false))
					qs->SetText();
				else
				{
					int len = strlen(qs->text);
					bool research = len >= 2 && qs->text[len - 1] == qs->text[len - 2];

					qs->BackChar();

					if (research)
						QuickSearch(1, true);
				}
			}

			return true;
		}
	}
	else if (isgraph(key))
	{
		int index = GetVisualIndex();
		int count = GetVisualCount();

		if (index < 0)
			index = -1;
		else
			count--;

		for (int i = 0; i < count; i++)
		{
			mstr m;

			index = (index + 1) % GetVisualCount();
			GetSearchText(m, index);

			if (toupper(*str(m)) == toupper(key))
			{
				SetVisualIndex(index);
				break;
			}
		}

		return true;
	}

	return false;
}

bool TQuickList::QuickKey(uint key)
{
	bool quick = qs != NULL;

	if (quick)
	{
		switch (key)
		{
			case VK_BACK :
			{
				if (any(qs->text))
				{
					qs->BackChar();
					qs->SetText();
				}
				else
					QuickDestroy();

				break;
			}
			case VK_LEFT :
			{
				if (any(qs->text))
					qs->controls[QUICK_SEARCH_PREV]->PostClicked();
				break;
			}
			case VK_RIGHT :
			{
				if (any(qs->text))
					qs->controls[QUICK_SEARCH_NEXT]->PostClicked();
				break;
			}
			case VK_DELETE : QuickDestroy(); break;
			case VK_SPACE : break;
			default : quick = false;
		}
	}

	return quick;
}

void TQuickList::QuickSetFocus()
{
	if (qsMode == QUICK_SEARCH_AUTO)
		Parent->SendMessage(WM_QUICK_SEARCH, 0, (LPARAM) this);
}

void TQuickList::QuickKillFocus()
{
	if (qs && !qs->Focused())
		QuickDestroy(true);
}
