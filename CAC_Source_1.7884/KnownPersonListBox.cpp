#include "cac.h"

//{{TKnownPersonListBox Implementation}}

TKnownPersonListBox::TKnownPersonListBox(TWindow* parent, int resourceId, char tInvolvement)
:
	TGroupListBox(parent, resourceId, NULL), involvement(tInvolvement)
{
	abcskip = ABC_SKIP_LOWER;
	sortcol = 0;
}

TGroup *TKnownPersonListBox::NewGroup() const
{
	TKnownPerson *person = new TKnownPerson;
	person->involvement = involvement;
	return person;
}
