//{{TFindReplaceListBox Implementation}}

TFindReplaceListBox::TFindReplaceListBox(TWindow* parent, int resourceId, TIArray<TReplacePair> *tPairs)
:
	TStringListBox(parent, resourceId), pairs(tPairs)
{
	initialRefresh = true;
}

TReplacePair *TFindReplaceListBox::Pair()
{
	int sel = GetVisualIndex();
	return (sel >= 0) ? (*pairs)[sel] : NULL;
}

bool TFindReplaceListBox::AddPair(TReplacePair *pair)
{
	mstr m;

	for (int i = 0; i < pairs->Count(); i++)
		if (!strcmp(pair->find, (*pairs)[i]->find) && !strcmp(pair->replace, (*pairs)[i]->replace))
			return false;

	pairs->Add(pair);
	pair->Print(m);
	AddString(str(m));

	return true;
}


void TFindReplaceListBox::DeletePair()
{
	int sel = GetVisualIndex();
	pairs->Remove(sel);
	DeleteString(sel);
}

void TFindReplaceListBox::Refresh()
{
	mstr m;

	FlushVisuals();

	for (int i = 0; i < pairs->Count(); i++)
	{
		(*pairs)[i]->Print(m);
		AddString(str(m));
	}
}
