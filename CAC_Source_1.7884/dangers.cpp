#include "cac.h"

#if COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL

#include "EditDangerDialog.h"
#include "EditDangerDialog.cpp"

static void subject_danger(TWindow *parent, TDanger *danger)
{
	TGArray<TDangerSubject> dangerSubjects;
	TGArray<TSubject> subjects, tSubjects;
	TSubject *tSubject;
	msql m;

	m.Begin(DangerSubject->Name);
	m.AddLong("F_DANGER", danger->danger);
	DangerSubject->LoadArray(dangerSubjects, str(m));

	// 2007:149 not a FloatCriteria(), so add order
	m.Begin(Subject->Name);
	m.AddChars("F_KIND_I", KIND_DANGER_LAWS);
	m.AddOrder("F_SUBJECT");
	TQuery q(str(m));

	while (q.Read())
	{
		*Subject << q;

		for (int i = 0; i < dangerSubjects.Count(); i++)
		{
			if (dangerSubjects[i]->subject == Subject->subject)
			{
				tSubject = new TSubject;
				*tSubject << Subject;
				subjects.Add(tSubject);

				tSubject = new TSubject;
				*tSubject << Subject;
				tSubjects.Add(tSubject);
				break;
			}
		}
	}

	if (select_subjects(parent, &tSubjects, KIND_DANGER_LAWS, false, true))
	{
		int i = 0;

		while (i < subjects.Count())
		{
			int j;

			for (j = 0; j < tSubjects.Count(); j++)
				if (subjects[i]->subject == tSubjects[j]->subject)
					break;

			if (j < tSubjects.Count())
			{
				subjects.Remove(i);
				tSubjects.Remove(j);
			}
			else
				i++;
		}

		DangerSubject->danger = danger->danger;

		for (int i = 0; i < subjects.Count(); i++)
		{
			DangerSubject->subject = subjects[i]->subject;
			DangerSubject->Delete(true);
		}

		for (int j = 0; j < tSubjects.Count(); j++)
		{
			DangerSubject->subject = tSubjects[j]->subject;
			DangerSubject->Insert();
		}
	}
}

#define tDanger OFFICE_GROUP_CAST(TDanger)

static void danger_indent_name(mstr &m, TGroup *group)
{
	if (tDanger->danger % DANGER_MASK_SECTION)
	{
		m.cat("  ");
		if (tDanger->danger % DANGER_MASK_ARTICLE)
			m.cat("  ");
	}

	tDanger->Print(m);
}

#undef danger

#include "DangerListWindow.h"
#include "DangerListWindow.cpp"

#include "DangerListDialog.h"
#include "DangerListDialog.cpp"

void edit_dangers(TWindow *parent)
{
	log(LOG_NOM, CM_ADMIN_DANGERS, "основания - завишена опасност");
	TDangerListDialog(parent).Execute();
	Danger->ReloadCache();
	DangerSubject->ReloadCache();
}
#endif  // AREAL || MILITARY || REGIONAL
