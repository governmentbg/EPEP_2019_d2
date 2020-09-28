bool TSubjectTKS::operator<=(const TSubjectTKS &that)
{
	return subject != that.subject ? subject < that.subject : tork <= that.tork;
}

// 2016:358 old-to-new -> new-to-new: large changes

struct TMigrateSubject
{
	char kind;
	long oldSubject;
	long newSubject;
	const char *oldCode;
	const char *newCode;
};

class TUpdateSubjects : public TUpdate
{
public:
	TUpdateSubjects(TWindow *parent);

	virtual void Execute();
	bool MigrateManual(TWindow *parent, const TMigrateSubject *ms);

protected:
	TGArray<TOldSubject> oldSubjects;
	TGArray<TSubject> subjects;

	void MigrateOrigins(msql &m, TOriginContainer &origin, const TMigrateSubject *ms);
	void MigrateSubject(const TMigrateSubject *ms);
};

#include "MigrateSubjectsListBox.h"
#include "MigrateSubjectsListBox.cpp"

#include "MigrateSubjectsDialog.h"
#include "MigrateSubjectsDialog.cpp"

TUpdateSubjects::TUpdateSubjects(TWindow *parent)
	: TUpdate(parent, CM_ADMIN_UPDATE_SUBJECTS, "предмети", NULL, NULL)
{
	wait = new TWaitWindow(parent, "Зареждане на предмети...");
	msql m(OldSubject->Name);

	OldSubject->LoadArray(oldSubjects, str(m));
	m.Begin(Subject->Name);
	Subject->LoadArray(subjects, str(m));
	delete wait;
	wait = NULL;
}

void TUpdateSubjects::Execute()
{
	// 2016:358 new-to-new: removes passes 0, 1

	// PASS 2: manual
	{
		action = "Промяна";
		UpdateStart();
		TMigrateSubjectsDialog(parent, this).Execute();
		dprintf("\n%d грешки при промените", wrong);
		UpdateFinal();
		UpdateShow();
	}
}

void TUpdateSubjects::MigrateOrigins(msql &m, TOriginContainer &origin, const TMigrateSubject *ms)
{
	TQuery q(str(m));
	while (q.Read())
	{
		origin << q;
		dprintf("%s %ld/%ld: %s -> %s", Type->Seek(origin.key.type), origin.key.no, origin.key.year, ms->oldCode,
			ms->newCode);

		m.Begin(origin.Name, MQ_UPDATE);
		m.printf("F_SUBJECT = %ld", ms->newSubject);
		m.Add(&origin, "F_NO, F_YEAR, F_TYPE");

		if (!AppName->Execute(str(m), ET_SILENT))
		{
			dprintf(": грешка при запис");
			wrong++;
		}

		dprintf("\n");
		wait->TickCount(++count, "обработени записа");
	}
}

void TUpdateSubjects::MigrateSubject(const TMigrateSubject *ms)
{
	TLawsuit lawsuit;
	msql m(lawsuit.Name, "F_NO, F_YEAR, F_TYPE");

	m.AddLong("F_SUBJECT", ms->oldSubject);
	m.AddChar("F_KIND", ms->kind);
	MigrateOrigins(m, lawsuit, ms);

	TRequest request;
	m.Begin("T_REQUEST R JOIN T_KIND K ON K.F_KIND = R.F_KIND", "F_NO, F_YEAR");

	m.AddLong("R.F_SUBJECT", ms->oldSubject);
	m.AddChar("K.F_LAW_KIND", ms->kind);
	MigrateOrigins(m, request, ms);
}

bool TUpdateSubjects::MigrateManual(TWindow *parent, const TMigrateSubject *ms)
{
	int saved = wrong;

	wait = new TWaitWindow(parent, WAIT);
	MigrateSubject(ms);
	delete wait;
	wait = NULL;
	return wrong == saved;
}

void update_subjects(TWindow *parent) { TUpdateSubjects(parent).Execute(); }
