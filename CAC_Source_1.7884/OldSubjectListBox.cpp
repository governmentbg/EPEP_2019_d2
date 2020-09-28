#include "cac.h"

//{{TOldSubjectListBox Implementation}}

TOldSubjectListBox::TOldSubjectListBox(TWindow* parent, int resourceId, const char *tKinds)
:
	TCloneListBox(parent, resourceId, new TGSArray<TOldSubject>, TOldSubject::Clone),
		kinds(tKinds ? tKinds : Default->xKinds)
{
#if CUTCITILAWS
	if (!kinds || strequal(kinds, KIND_LAWSUITS))
		kinds = KIND_PUNISHMENT_LAWS;
#endif  // CUTCITILAWS
}

void TOldSubjectListBox::SetMark(TGArray<TOldSubject> *oldSubjects)
{
	// 2009:076 LPR: only mark by type + subject, not subject only
	// 2016:089 JRQ: clear unmarked subjects
	for (int i = 0; i < array->Count(); i++)
	{
		TOldSubject *oldSubject = OldSubjectAt(i);
		int n;

		for (n = 0; n < oldSubjects->Count(); n++)
			if ((*oldSubjects)[n]->subject == oldSubject->subject && (*oldSubjects)[n]->type == oldSubject->type)
				break;

		SetSel(i, n < oldSubjects->Count());
	}
}

void TOldSubjectListBox::Criteria(msql &m, TGroup *group)
{
	m.Begin(group->Name);

	if (kinds)
		m.AddChars("F_KIND", kinds);
}

void TOldSubjectListBox::GetGroupText(mstr &m, TGroup *group, int)
{
	TOldSubject *oldSubject = (TOldSubject *) group;

	if (strlen(kinds) >= 2)
		m.printf("%c ", *Type->Seek(oldSubject->type));

	oldSubject->Print(m);
}
