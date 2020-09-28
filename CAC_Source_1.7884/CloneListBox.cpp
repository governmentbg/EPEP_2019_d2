#include "cac.h"

//{{TCloneListBox Implementation}}

TCloneListBox::TCloneListBox(TWindow* parent, int resourceId, TGroupArray *array, TGroup *(*tClone)())
:
	TGroupListBox(parent, resourceId, array), clone(tClone)
{
}

TGroup *TCloneListBox::NewGroup() const
{
	return clone();
}
