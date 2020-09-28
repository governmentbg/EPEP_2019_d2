//{{TStringListBox Implementation}}

TStringListBox::TStringListBox(TWindow* parent, int resourceId)
:
	TGroupListBox(parent, resourceId, NULL)
{
	initialRefresh = false;
}

TStringListBox::~TStringListBox()
{
	array->Clear();
}

int TStringListBox::AddString(const char far* str)
{
	array->Add(NULL);
	return TGroupListBox::AddString(str);
}

int TStringListBox::InsertString(const char far *str, int index)
{
	array->Insert(NULL, index);
	return TGroupListBox::InsertString(str, index);
}

int TStringListBox::DeleteString(int index)
{
	array->Remove(index);
	return TGroupListBox::DeleteString(index);
}

TGroup *TStringListBox::NewGroup() const
{
	fatal("TStringListBox::NewGroup");
	return NULL;
}
