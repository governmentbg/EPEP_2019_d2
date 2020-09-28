//{{TLongAliasFace Implementation}}

TLongAliasFace::TLongAliasFace(TWindow* parent, int resourceId, const char *name, TLongAliasGroup *alias, const char *xCrit, long flags)
:
	TAliasFace(parent, resourceId, name, alias, xCrit, flags)
{
}

long TLongAliasFace::L()
{
	int index = GetVisualIndex();
	return index >= 0 ? LongAt(index) : 0;
}

void TLongAliasFace::SetL(long l)
{
	int index;

	for (index = 0; index < array->Count(); index++)
		if (LongAt(index) == l)
			break;

	SetDefaultIndex(index);
}

void TLongAliasFace::FlushList()
{
	FlushVisuals();
	array->Flush();
	UpdateVisual();
}

void TLongAliasFace::Refresh()
{
	long l = L();
	TAliasFace::Refresh();
	SetL(l);
}

void TLongAliasFace::SetupWindow()
{
	if (sortcol == -1)
	{
		delete array;
		array = new TLongAliasArray;
	}

	TAliasFace::SetupWindow();
}
