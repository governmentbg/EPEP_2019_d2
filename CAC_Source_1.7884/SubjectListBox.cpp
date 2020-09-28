#include "cac.h"

//{{TSubjectListBox Implementation}}

TSubjectListBox::TSubjectListBox(TWindow* parent, int resourceId, const char *tKinds)
:
	TCloneListBox(parent, resourceId, new TGSArray<TSubject>, TSubject::Clone), kinds(tKinds ? tKinds : Default->xKinds),
		hidden(false)
{
#if CUTCITILAWS
	if (!kinds || strequal(kinds, KIND_LAWSUITS))
		kinds = KIND_PUNISHMENT_LAWS;
#endif  // CUTCITILAWS
}

void TSubjectListBox::SetMark(TGArray<TSubject> *subjects)
{
	// 2009:076 LPR: only mark by type + subject, not subject only
	// 2016:089 JRQ: clear unmarked subjects
	for (int i = 0; i < array->Count(); i++)
	{
		TSubject *subject = SubjectAt(i);
		int n;

		for (n = 0; n < subjects->Count(); n++)
			if ((*subjects)[n]->subject == subject->subject && (*subjects)[n]->type == subject->type)
				break;

		SetSel(i, n < subjects->Count());
	}
}

void TSubjectListBox::SetKinds(const char *tKinds)
{
	if (strcmp(tKinds, kinds))
	{
		kinds = tKinds;
		Refresh();
	}
}

void TSubjectListBox::LoadData(const char *s)
{
	TCloneListBox::LoadData(s);
}

void TSubjectListBox::Criteria(msql &m, TGroup *group)
{
	m.Begin(group->Name);

	if (kinds)
	{
		m.AddChars("(F_KIND_I", kinds);
		m.NextOr();
		m.AddChars("F_KIND_II", kinds);
		m.cat(")");
	}

	if (!hidden)
		TSubject::AddVisible(m);
}

void TSubjectListBox::GetGroupText(mstr &m, TGroup *group, int)
{
	TSubject *subject = (TSubject *) group;

	// 2016:074 leave blank for empty subject
	if (subject->type || subject->subject)
	{
		if (strlen(kinds) >= 2)
			m.printf("%c ", *Type->Seek(subject->type));

		subject->Print(m);
	}
}
