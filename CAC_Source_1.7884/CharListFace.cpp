//{{TCharListFace Implementation}}

TCharListFace::TCharListFace(TWindow* parent, int resourceId, const char *name, TCharAliasGroup *alias, const char *xCrit, int size, long flags)
:
	TCharArrayListFace(parent, resourceId, name, alias, xCrit, size, flags)
{
}

const char *TCharListFace::S()
{
	*s = '\0';

	for (int index = 0; index < GetCount(); index++)
		if (GetSel(index))
			strcach(s, CharAt(index));

	return s;
}

void TCharListFace::SetS(const char *s)
{
	for (int index = 0; index < GetCount(); index++)
		SetSel(index, strchr(s, CharAt(index)) != NULL);
}

bool TCharListFace::Empty()
{
	S();
	return !any(s);
}

void TCharListFace::Refresh()
{
	S();
	TCharArrayListFace::Refresh();
	SetS(s);
}

void TCharListFace::SetupWindow()
{
	if (sortcol == -1 && ((TCharAliasGroup *) alias)->OrderField())
	{
		delete array;
		array = new TCharAliasArray;
	}

	TCharArrayListFace::SetupWindow();
}

void TCharListFace::SetC(char c)
{
	char s[2] = { c, '\0' };
	SetS(s);
}
