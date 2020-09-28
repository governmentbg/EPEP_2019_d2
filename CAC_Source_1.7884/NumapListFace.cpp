//{{TNumapListFace Implementation}}

TNumapListFace::TNumapListFace(TWindow* parent, int resourceId, const char *name, int size)
:
	TCharArrayListFace(parent, resourceId, name, NULL, NULL, size)
{
}

void TNumapListFace::Advanced(bool enable)
{
	// 2010:026 LPR: rewritten to apply mark instead of hard-setting it
	TNumapMarkGroup group;

	// 2007:199 LPR: there was no IDOK check, just unconditional mark
	if (TGetNumapMarkDialog(this, &group).Execute() == IDOK)
	{
		for (int i = group.start; i <= group.final; i++)
			if (group.mark == NUMAP_MARK_ALL || i % 2 == group.mark)
				SetSel(i - 1, enable);
	}
}

const char *TNumapListFace::S()
{
	*s = '\0';

	for (int i = 0; i < GetCount(); i++)
	{
		if (GetSel(i))
		{
			strsetlen(s, i);
			strcach(s, NUMAP_ACTIVE);
		}
	}

	return s;
}

void TNumapListFace::SetS(const char *ts)
{
	for (int i = 0; i < GetCount(); i++)
		SetSel(i, i < strlen(ts) && ts[i] == NUMAP_ACTIVE);
}

void TNumapListFace::Refresh()
{
	ClearList();

	for (int i = 1; i < size; i++)
	{
		char s[SIZE_OF_ONE_ADDR_NO];
		sprintf(s, "%3d", i);
		AddString(s);
	}

	SetS(s);
}
