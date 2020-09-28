#include "cac.h"

// ----- archive index -------------------------------------------------------
#include "EditArchiveIndexDialog.h"
#include "EditArchiveIndexDialog.cpp"

void add_saas_Subject(TGArray<TSubject> &subjects, long saas)
{
	TSubject *tSubject = new TSubject;

	*tSubject << Subject;
	tSubject->saas = saas;
	subjects.Add(tSubject);
}

static void subject_archive(TWindow *parent, TArchiveIndex *archiveIndex)
{
	TGArray<TSubject> subjects, tSubjects;
	TArchiveSubject tArchiveSubject;
	msql m(tArchiveSubject.Name);
	TGArray<TArchiveSubject> archiveSubjects;

	m.AddString("F_ARCHIVE_INDEX", archiveIndex->archiveIndex);
	tArchiveSubject.LoadArray(&archiveSubjects, str(m));

	m.Begin(Subject->Name);
	TQuery q(str(m));
	while (q.Read())
	{
		*Subject << q;

		for (int i = 0; i < archiveSubjects.Count(); i++)
		{
			TArchiveSubject *archiveSubject = archiveSubjects[i];
			long saas = TSubject::SI(archiveSubject->kind);

			if (archiveSubject->subject == Subject->subject && archiveSubject->kind == Subject->sd[saas].kind)
			{
				add_saas_Subject(subjects, saas);
				add_saas_Subject(tSubjects, saas);
			}
		}
	}

	if (select_subjects(parent, &tSubjects, KIND_LAWSUITS, true, true))
	{
		int i = 0;

		while (i < subjects.Count())
		{
			TSubject *subject = subjects[i];
			int j;

			for (j = 0; j < tSubjects.Count(); j++)
			{
				TSubject *tSubject = tSubjects[j];

				if (subject->subject == tSubject->subject && subject->SaasD().kind == tSubject->SaasD().kind)
					break;
			}

			if (j < tSubjects.Count())
			{
				subjects.Remove(i);
				tSubjects.Remove(j);
			}
			else
				i++;
		}

		strcpy(ArchiveSubject->archiveIndex, archiveIndex->archiveIndex);

		for (i = 0; i < subjects.Count(); i++)
		{
			TSubject *subject = subjects[i];

			ArchiveSubject->subject = subject->subject;
			ArchiveSubject->kind = subject->SaasD().kind;
			ArchiveSubject->Delete(true);
		}

		for (i = 0; i < tSubjects.Count(); i++)
		{
			TSubject *tSubject = tSubjects[i];

			ArchiveSubject->subject = tSubject->subject;
			ArchiveSubject->kind = tSubject->SaasD().kind;
			ArchiveSubject->Insert();
		}
	}
}

// ----- print indexes -------------------------------------------------------
#define index OFFICE_GROUP_CAST(TArchiveIndex)

static void index_keep(mstr &m, TGroup *group)
{
	m.printf("%ld%s", index->keep, index->flags & ARCHIVE_INDEX_EC ? " ЕК" : "");
}

#undef index

static const offield index_fields[] =
{
	{ "F_ARCHIVE_INDEX",	NULL },
	{ "F_NAME",			NULL },
	{ "F_KEEP",			index_keep },
	{ "F_ACTIVE", 		group_active },
	{ NULL, NULL }
};

static void print_archive_indexes(TGroupArray *indexes)
{
	TOffice office;
	
	if (office.Open("ArchivInd.htm"))
	{
		constant tr1start = office.Find("tr1 start");
		constant tr1final = office.Find("tr1 final");

		office.Emit(NULL, tr1start, null_fields, NULL);

		for (int i = 0; i < indexes->Count(); i++)
			office.Emit(tr1start, tr1final, index_fields, (*indexes)[i]);

		office.Emit(tr1final, NULL, null_fields, NULL);
		office.Show(NULL);
	}
}

#include "ArchiveIndexListWindow.h"
#include "ArchiveIndexListWindow.cpp"

#include "ArchiveIndexListDialog.h"
#include "ArchiveIndexListDialog.cpp"

void edit_archive_indexes(TWindow *parent)
{
	TArchiveIndexListDialog(parent).Execute();
	ArchiveIndex->ReloadCache();
	ArchiveSubject->ReloadCache();
}

// ----- archive / destroy ---------------------------------------------------
#include "ArchiveIndexAliasFace.h"
#include "ArchiveIndexAliasFace.cpp"

#include "ArchiveDialog.h"
#include "ArchiveDialog.cpp"

void new_archive(TWindow *parent, TExternContainer *lawsuit)
{
	TArchive archive;
	TGroupArray archiveSubjects;
	mstr m;

	archive.key = lawsuit->key;
	archive.excess = lawsuit->Excess();
	*ArchiveSubject << lawsuit;
	ArchiveSubject->LoadData(&archiveSubjects, NULL, FLAG_ACTIVE);

	if (!archiveSubjects.Count())
		error("За %s предмет %s няма дефинирани архивни индекси.", Type->Seek(lawsuit->key.type), lawsuit->Code());
	else if (TArchiveDialog(parent, &archive, lawsuit, false).Execute() == IDOK)
	{
		// 2009:023 LPR: +log
		archive.Signal(ELECTRIC_EDIT, lawsuit);
		log(LOG_ARCHIVE, CM_ARCH_NEW_ARCHIVE, lawsuit);
	}
}

void edit_archive(TWindow *parent, TArchive *archive, TExternContainer *lawsuit)
{
	// 2009:023 LPR: +log
	if (TArchiveDialog(parent, archive, lawsuit, true).Execute() == IDOK)
		log(LOG_EDIT, CM_ARCH_EDIT_ARCHIVE, &archive->key, "на данните за архивиране на дело");
}

#include "KillLawDialog.h"
#include "KillLawDialog.cpp"

void new_destroy(TWindow *parent, TArchive *archive, TLawsuit *lawsuit)
{
	if (TKillLawDialog(parent, archive, lawsuit, false).Execute() == IDOK)
	{
		archive->Signal(ELECTRIC_EDIT, lawsuit);
		log(LOG_KILL, CM_ARCH_NEW_DESTROY, &archive->key);
	}
}

void edit_destroy(TWindow *parent, TArchive *archive, TLawsuit *lawsuit)
{
	if (TKillLawDialog(parent, archive, lawsuit, true).Execute() == IDOK)
		log(LOG_EDIT, CM_ARCH_EDIT_DESTROY, &archive->key, "на данните за унищожаване на дело");
}
