#include "cac.h"

#include "EditRealityTypeDialog.h"
#include "EditRealityTypeDialog.cpp"

static bool edit_reality(TWindow* parent, TRealityType *realityType, bool edit, int resId)
{
	return TEditRealityTypeDialog(parent, realityType, edit, resId).Execute() == IDOK;
}

static void subject_reality(TWindow *parent, TRealityType *realityType, const char *realitiesName)
{
	TGArray<TSubject> subjects, tSubjects;
	msql m(Subject->Name);

	m.AddOrder("F_TYPE, F_SUBJECT");
	TQuery q(str(m));
	while (q.Read())
	{
		*Subject << q;

		for (long si = 0; si <= 1; si++)
		{
			TString *field = (TString *) Subject->Find(TSubject::FNI(realitiesName, si));

			if (strchr(field->S(), realityType->realityType))
			{
				add_saas_Subject(subjects, si);
				add_saas_Subject(tSubjects, si);
			}
		}
	}

	if (select_subjects(parent, &tSubjects, KIND_LAWSUITS, true, true))
	{
		int i, j;
		char *pos;

		for (i = 0; i < subjects.Count(); i++)
		{
			TSubject *subject = subjects[i];

			for (j = 0; j < tSubjects.Count(); j++)
			{
				TSubject *tSubject = tSubjects[j];

				if (subject->subject == tSubject->subject && subject->SaasD().kind == tSubject->SaasD().kind)
					break;
			}

			TString *field = (TString *) subject->Find(subject->FNI(realitiesName));

			if (j == tSubjects.Count() && (pos = (char *) strchr(field->S(), realityType->realityType)) != NULL)
			{
				strmove(pos, pos + 1);
				subject->Update(field->Name);
			}
		}

		for (i = 0; i < tSubjects.Count(); i++)
		{
			TSubject *tSubject = tSubjects[i];

			for (j = 0; j < subjects.Count(); j++)
			{
				TSubject *subject = subjects[j];

				if (tSubject->subject == subject->subject && tSubject->SaasD().kind == subject->SaasD().kind)
					break;
			}

			TString *field = (TString *) tSubject->Find(tSubject->FNI(realitiesName));

			if (j == subjects.Count() && (pos = (char *) strchr(field->S(), realityType->realityType)) == NULL)
			{
				strcach((char *) field->S(), (char) realityType->realityType);
				tSubject->Update(field->Name);
			}
		}
	}
}

#include "PenalityListWindow.h"
#include "PenalityListWindow.cpp"

#include "PenalityListDialog.h"
#include "PenalityListDialog.cpp"

void edit_penalities(TWindow *parent)
{
	TPenalityListDialog(parent).Execute();
	PenalityType->ReloadCache();
}

#include "ReasonListWindow.h"
#include "ReasonListWindow.cpp"

#include "ReasonListDialog.h"
#include "ReasonListDialog.cpp"

void edit_reasons(TWindow *parent)
{
	TReasonListDialog(parent).Execute();
	ReasonType->ReloadCache();
}
