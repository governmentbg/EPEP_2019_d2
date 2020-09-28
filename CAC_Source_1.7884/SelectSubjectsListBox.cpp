#include "cac.h"

//{{TSelectSubjectsListBox Implementation}}

TSelectSubjectsListBox::TSelectSubjectsListBox(TWindow* parent, int resourceId, const char *kinds, bool tSaas, bool tHidden)
:
	TSubjectListBox(parent, resourceId, kinds), saas(tSaas)
{
	hidden = tHidden;
	qsMode = QUICK_SEARCH_ABLE;
}

void TSelectSubjectsListBox::Extend()
{
	char type = kind2type(*kinds);
	long si = TSubject::SI(*kinds);

	extended.clear();

	for (const char *k = KIND_LAWSUITS; *k; k++)
		if (kind2type(*k) == type && TSubject::SI(*k) == si)
			extended.cach(*k);

	SetKinds(str(extended));
}

void TSelectSubjectsListBox::SetMark(TGArray<TSubject> *subjects)
{
	for (int i = 0; i < subjects->Count(); i++)
	{
		TSubject *subject = (*subjects)[i];

		for (int n = 0; n < array->Count(); n++)
		{
			TSubject *tSubject = SubjectAt(n);

			if (tSubject->subject == subject->subject &&
				(saas ? tSubject->SaasD().kind == subject->SaasD().kind : tSubject->type == subject->type))
			{
				SetSel(n, true);
				break;
			}
		}
	}
}

void TSelectSubjectsListBox::LoadData(const char *s)
{
	TQuery q(s, Subject->DataBase);

	while (q.Read())
	{
		*Subject << q;

		for (long si = 0; si <= 1; si++)
		{
			if (Subject->sd[si].kind && strchr(kinds, Subject->sd[si].kind))
			{
				TSubject *subject = new TSubject;

				*subject << Subject;
				subject->saas = si;
				InsertGroup(subject);

				if (!saas)
					break;
			}
		}
	}
}

void TSelectSubjectsListBox::GetGroupText(mstr &m, TGroup *group, int)
{
	TSubject *subject = (TSubject *) group;

	m.printf("%c %s %s", *Type->Seek(subject->type), subject->Code(subject->saas, saas), subject->name);
}
