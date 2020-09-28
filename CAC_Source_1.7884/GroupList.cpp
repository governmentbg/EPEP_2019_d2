#include "cac.h"

TGroupList::TGroupList(TWindow *parent, TGroupArray *tArray)
#if 20180213
	: TQuickList(parent, QUICK_SEARCH_NONE), array(tArray), initialRefresh(true), sortcol(-1)
{
	if (!array)
		array = new TGroupArray;

	loadData = dynamic = !array->Count();
}
#else  // 20180213
	: TQuickList(parent, QUICK_SEARCH_NONE), array(tArray), dynamic(true), loadData(true), initialRefresh(true), sortcol(-1)
{
	if (array)
		loadData = dynamic = !array->Count();
	else
		array = new TGroupArray;
}
#endif  // 20180213

TGroupList::TGroupList(TWindow *parent, TGroupArray &tArray)
	: TQuickList(parent, QUICK_SEARCH_NONE), array(&tArray), dynamic(false), loadData(false), initialRefresh(true),
		sortcol(-1)
{
}

TGroupList::~TGroupList()
{
	if (dynamic)
		delete array;
}

TGroup *TGroupList::Group()
{
	int index = GetVisualIndex();
	return index >= 0 ? GroupAt(index) : NULL;
}

void TGroupList::RedrawVisual(int index)
{
	RemoveVisual(index);
	InsertVisual(index);
}

void TGroupList::Criteria(msql &m, TGroup *group)
{
	group->FloatCriteria(m);
}

void TGroupList::LoadData(const char *s)
{
	TGroup *const templ = NewGroup();
	TVirtPtr VP(templ);
	TQuery q(s, templ->DataBase);

	while (q.Read())
	{
		TGroup *group = NewGroup();
		*group << q;
		InsertGroup(group);
	}
}

void TGroupList::Refresh()
{
	QuickRefresh();

#if RANGECHECK
	if (sortcol >= 0 && array->Sorted())
		fatal("GroupList: sortcol and sorted array");
#endif  // RANGECHECK

	FlushVisuals();

	if (loadData)
	{
		TGroup *group = NewGroup();
		TVirtPtr VP(group);
		msql m;

		array->Flush();
		Criteria(m, group);
		LoadData(str(m));
	}
	else
	{
		for (int index = 0; index < array->Count(); index++)
			InsertVisual(index);
	}

	UpdateVisual();
}

int TGroupList::GetVisualCount()
{
	return array->Count();
}

int TGroupList::InsertGroup(TGroup *group)
{
	int index;

	if (sortcol >= 0)
	{
		mstr m;

		GetGroupText(m, group, sortcol);
		index = FindABCIndex(str(m));
		array->Insert(group, index);
	}
	else
		index = array->Add(group);

	InsertVisual(index);
	return index;
}

void TGroupList::InsertEmpty()
{
	TGroup *group = NewGroup();

	if (sortcol >= 0 || array->Sorted())
		InsertGroup(group);
	else
	{
		array->Insert(group, 0);
		InsertVisual(0);
	}
}

void TGroupList::ResortGroup()
{
	int index = GetVisualIndex();

#if RANGECHECK
	if (index < 0)
		fatal("TGroupList::RedrawGroup");
#endif  // RANGECHECK

	if (sortcol >= 0 || array->Sorted())
	{
		TGroup *group = GroupAt(index);

		RemoveVisual(index);
		array->Detach(index);
		index = InsertGroup(group);
	}
	else
		RedrawVisual(index);

	SetVisualIndex(index);
}

void TGroupList::RemoveGroup()
{
	int index = GetVisualIndex();

#if RANGECHECK
	if (index < 0)
		fatal("TGroupListWindow::RemoveGroup");
#endif  // RANGECHECK

	RemoveVisual(index);
	array->Remove(index);
}

void TGroupList::GetGroupText(mstr &m, TGroup *group, int column)
{
	group->Print(m);
	m.delead(column);
	m.cutell(255);
}

void TGroupList::GetSearchText(mstr &m, int index)
{
	char *s = GetVisualText(index, 0);
	TCharPtr CP(s);
	m.cpy(ABCSkip(s, true));
}

int TGroupList::FindABCIndex(const char *str)
{
	int l = 0, h = array->Count() - 1;

	str = ABCSkip(str, false);

	while (l < h)
	{
		int i = (l + h) / 2;
		char *s = GetVisualText(i, sortcol);
		TCharPtr CP(s);

		if (strcmpi(ABCSkip(s, false), str) <= 0)
			l = i + 1;
		else
			h = i;
	}

	if (l < array->Count())
	{
		char *s = GetVisualText(l, sortcol);
		TCharPtr CP(s);

		if (strcmpi(ABCSkip(s, false), str) <= 0)
			l++;
	}

	return l;
}
