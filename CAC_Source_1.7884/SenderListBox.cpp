#include "cac.h"

//{{TSenderListBox Implementation}}

TSenderListBox::TSenderListBox(TWindow* parent, int resourceId, const char *tUCNTypes)
:
	TCloneListBox(parent, resourceId, NULL, TSender::Clone), ucnTypes(tUCNTypes)
{
	sortcol = 0;
}

void TSenderListBox::Criteria(msql &m, TGroup *group)
{
	m.Begin(group->Name);
	m.AddChars("F_UCN_TYPE", ucnTypes);
	// ignored by sortcol?..
	m.AddOrder(strlen(ucnTypes) < 2 ? "F_NAME" : "F_UCN");
}

void TSenderListBox::GetGroupText(mstr &m, TGroup *group, int)
{
	TSender *sender = (TSender *) group;

	if (strlen(ucnTypes) >= 2)
	{
		switch (sender->ucnType)
		{
			case UCN_INSTANCE_HIGHER : m.cach('Â'); break;
			case UCN_INSTANCE_LOWER : m.cat("I "); break;
			case UCN_INSTANCE_EQUAL : m.cach('Ï'); break;
			case UCN_INSTANCE_PUNISHER : m.cach('À'); break;
			case UCN_INSTANCE_CONFLICT :
			case UCN_INSTANCE_COMPETENT : m.cach('Ê'); break;
			case UCN_EXECUTOR_UCN :
			case UCN_EXECUTOR_CODE : m.cach('×'); break;
			default : m.cat("   ");
		}
		m.cach(' ');
	}

	m.cat(sender->name);
}
