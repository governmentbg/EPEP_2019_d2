//{{THallListBox Implementation}}

THallListBox::THallListBox(TWindow* parent, int resourceId, long flags)
:
	TAliasListBox(parent, resourceId, Hall, NULL, flags)
{
}

void THallListBox::GetMarks(TIntegerArray *selected, TIntegerArray *unselected)
{
	for (int index = 0; index < array->Count(); index++)
	{
		long hall = TYPECHECK_PTR_CAST(THall, GroupAt(index))->hall;
		(GetSel(index) ? selected : unselected)->Add(hall);
	}
}
