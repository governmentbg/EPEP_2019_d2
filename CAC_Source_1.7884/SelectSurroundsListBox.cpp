//{{TSelectSurroundsListBox Implementation}}

TSelectSurroundsListBox::TSelectSurroundsListBox(TWindow* parent, int resourceId, TGArray<TSurround> *possibles)
:
	TGroupListBox(parent, resourceId, possibles)
{
	loadData = dynamic = false;
}

void TSelectSurroundsListBox::SetMark(TGArray<TSurround> *surrounds)
{
	for (int i = 0; i < surrounds->Count(); i++)
	{
		TSurround *surround = (*surrounds)[i];
		int n;

		for (n = 0; n < array->Count(); n++)
			if (surround->key == SurroundAt(n)->key)
				break;

		if (n < array->Count())
			SetSel(n, true);
	}
}

int TSelectSurroundsListBox::MarkedsMax()
{
	return SURROUNDMENTS_MAX;
}

TGroup *TSelectSurroundsListBox::NewGroup() const
{
	return new TSurround;
}

void TSelectSurroundsListBox::GetGroupText(mstr &m, TGroup *group, int)
{
	print_sfor(m, (TSurround *) group);
}
