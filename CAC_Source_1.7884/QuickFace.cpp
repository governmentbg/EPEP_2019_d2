#include "cac.h"

//{{TQuickFace Implementation}}

TQuickFace::TQuickFace(TWindow* parent, int resourceId, const char *name, TGroupBox *group)
:
	TCheckFace(parent, resourceId, name, group)
{
}

long TQuickFace::L() const
{
	if (GetCheck() == BF_UNCHECKED)
		return 0;

	return QUICK_QUICK | (GetCheck() == BF_GRAYED ? QUICK_IMMEDIATE : 0);
}

void TQuickFace::SetL(long l)
{
	if (l & QUICK_QUICK)
		SetCheck(l & QUICK_IMMEDIATE ? BF_GRAYED : BF_CHECKED);
	else
		SetCheck(BF_UNCHECKED);
}

void TQuickFace::SetKind(char kind)
{
	if (strchr(KIND_QUICKABLES, kind))
	{
		EnableWindow(true);
		if (strchr(KIND_IMMEDIATABLES, kind))
			SetStyle(BS_AUTO3STATE, false);
		else
		{
			if (GetCheck() == BF_GRAYED)
				SetCheck(BF_CHECKED);
			SetStyle(BS_AUTOCHECKBOX, false);
		}
	}
	else
	{
		EnableWindow(false);
		SetL(strchr(KIND_QUICKONLYS, kind) ? QUICK_QUICK : QUICK_NULL);
	}
}

void TQuickFace::AlterText()
{
	SetCaption(GetCheck() == BF_GRAYED ? "Бързо (незаб.) производство" : "&Бързо производство");
}
