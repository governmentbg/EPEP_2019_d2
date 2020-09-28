#include "cac.h"

#include "SubjectRealityTypesDialog.h"
#include "SubjectRealityTypesDialog.cpp"

#include "SubjectEisppDialog.h"
#include "SubjectEisppDialog.cpp"

#include "SubjectArchiveIndexDialog.h"
#include "SubjectArchiveIndexDialog.cpp"

#include "EditSubjectDialog.h"
#include "EditSubjectDialog.cpp"

static bool edit_subject(TWindow *parent, TSubject *subject, sign edit)
{
	msql m;

	// archive indexes
	TGArray<TArchiveIndex> indexes, tIndexes;
	TGArray<TArchiveSubject> archiveSubjects;

	ArchiveSubject->subject = subject->subject;
	ArchiveSubject->kind = subject->SaasD().kind;
	ArchiveSubject->LoadFloat(archiveSubjects);

	m.Begin(ArchiveIndex->Name, "F_ARCHIVE_INDEX");
	TQuery p(str(m));
	while (p.Read())
	{
		*ArchiveIndex << p;

		for (int i = 0; i < archiveSubjects.Count(); i++)
		{
			if (!strcmp(archiveSubjects[i]->archiveIndex, ArchiveIndex->archiveIndex))
			{
				TArchiveIndex *tArchiveIndex = new TArchiveIndex;
				*tArchiveIndex << ArchiveIndex;
				indexes.Add(tArchiveIndex);

				tArchiveIndex = new TArchiveIndex;
				*tArchiveIndex << ArchiveIndex;
				tIndexes.Add(tArchiveIndex);
				break;
			}
		}
	}

	if (edit == -1)
	{
		subject->subject = 0;
		*subject->name = '\0';
		*subject->alias = '\0';
		indexes.Flush();
	}

	bool result = TEditSubjectDialog(parent, subject, edit == true, &tIndexes).Execute() == IDOK;

	if (result)
	{
		m.cpy("предмет ");
		Type->Seek(subject->type);
		m.printf("%s %ld-%ld: %s достъп", Type->alias, subject->subject, subject->saas,
			strchr(subject->SaasD().attrib, POST_LIMITED) ? "ограничен" : "нормален");
		log(edit == true ? LOG_EDIT : LOG_ADD, CM_ADMIN_SUBJECTS, str(m));

		int i;

		// archive indexes
		i = 0;

		while (i < indexes.Count())
		{
			int j;

			for (j = 0; j < tIndexes.Count(); j++)
				if (!strcmp(indexes[i]->archiveIndex, tIndexes[j]->archiveIndex))
					break;

			if (j < tIndexes.Count())
			{
				indexes.Remove(i);
				tIndexes.Remove(j);
			}
			else
				i++;
		}

		ArchiveSubject->subject = subject->subject;
		ArchiveSubject->kind = subject->SaasD().kind;

		for (int i = 0; i < indexes.Count(); i++)
		{
			strcpy(ArchiveSubject->archiveIndex, indexes[i]->archiveIndex);
			ArchiveSubject->Delete(true);
		}

		for (int j = 0; j < tIndexes.Count(); j++)
		{
			strcpy(ArchiveSubject->archiveIndex, tIndexes[j]->archiveIndex);
			ArchiveSubject->Insert();
		}
	}

	return result;
}

#include "AddSubjectDialog.h"
#include "AddSubjectDialog.cpp"

static bool add_subject(TWindow *parent, TSubject *subject)
{
	char type = kind2type(subject->SaasD().kind);
	mstr kinds;

	for (const char *k = KIND_LAWSUITS; *k; k++)
		if (kind2type(*k) == type && TSubject::SI(*k) != subject->saas)
			kinds.cach(*k);

	if (!TSubject::Twin(type) || !any(kinds))
		return edit_subject(parent, subject, false);

	if (TAddSubjectDialog(parent, subject, str(kinds)).Execute() != IDOK)
		return false;

	if (edit_subject(parent, subject, true))
		return true;

	if (subject->Delete(true))
		return false;

	return error("%s: грешка при изтриване.", subject->Name);
}

#define subject OFFICE_GROUP_CAST(TSubject)

static void subject_inactive(mstr &m, TGroup *group) { m.cat(subject->SaasD().flags & FLAG_ACTIVE ? "" : "н"); }
static void subject_code(mstr &m, TGroup *group) { m.cat(subject->Code(subject->saas, true)); }

#undef subject

#include "SubjectListWindow.h"
#include "SubjectListWindow.cpp"

#include "SubjectListDialog.h"
#include "SubjectListDialog.cpp"

void edit_subjects(TWindow *parent)
{
	log(LOG_NOM, CM_ADMIN_SUBJECTS, "предмети");
	TSubjectListDialog(parent).Execute();
}
