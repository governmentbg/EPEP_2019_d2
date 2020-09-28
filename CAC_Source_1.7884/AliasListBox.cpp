#include "cac.h"

//{{TAliasListBox Implementation}}

TAliasListBox::TAliasListBox(TWindow* parent, int resourceId, TAliasGroup *tAlias, const char *tXCrit, long tFlags)
:
	TGroupListBox(parent, resourceId, NULL), alias(tAlias), xCrit(tXCrit), flags(tFlags)
{
}

TGroup *TAliasListBox::NewGroup() const
{
	return alias->clone();
}

void TAliasListBox::LoadData(const char *s)
{
	if (!alias->LoadCache(this, xCrit, flags))
		TGroupList::LoadData(s);
}

void TAliasListBox::SetupWindow()
{
	if (sortcol == -1 && !array->Sorted())
		sortcol = 0;

	TGroupListBox::SetupWindow();
}
