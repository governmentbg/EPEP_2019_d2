DEFINE_RESPONSE_TABLE1(TMigrateSubjectsDialog, TQuickDialog)
	EV_CBN_SELCHANGE(IDC_MIGRATE_SUBJECTS_KIND, KindCBNSelChange),
	EV_COMMAND_ENABLE(IDCONVERT, MigrateBNEnable),
	EV_BN_CLICKED(IDCONVERT, MigrateBNClicked),
END_RESPONSE_TABLE;

TMigrateSubjectsDialog::TMigrateSubjectsDialog(TWindow* parent, TUpdateSubjects *tUpdate)
	: TQuickDialog(parent, IDD_MIGRATE_SUBJECTS), update(tUpdate)
{
	kind = new TCharAliasFace(this, IDC_MIGRATE_SUBJECTS_KIND, "F_KIND", Kind, KIND_LAWSUITS);
	sources = new TSelectSubjectsListBox(this, IDC_MIGRATE_SUBJECTS_SOURCES, C2S[*KIND_LAWSUITS], true, true);
	targets = new TSelectSubjectsListBox(this, IDC_MIGRATE_SUBJECTS_TARGETS, C2S[*KIND_LAWSUITS], true, false);
}

void TMigrateSubjectsDialog::CheckTargets()
{
	if (!targets->GetCount())
		targets->Extend();
}

void TMigrateSubjectsDialog::SetupWindow()
{
	TQuickDialog::SetupWindow();
	CheckTargets();
}

void TMigrateSubjectsDialog::KindCBNSelChange()
{
	sources->SetKinds(C2S[kind->C()]);
	targets->SetKinds(C2S[kind->C()]);
	CheckTargets();
}

void TMigrateSubjectsDialog::MigrateBNEnable(TCommandEnabler &tce)
{
	tce.Enable(sources->Group() && targets->Group());
}

void TMigrateSubjectsDialog::MigrateBNClicked()
{
	TMigrateSubject ms;
	TSubject *oldSubject = ((TSubject *) sources->Group());
	TSubject *newSubject = ((TSubject *) targets->Group());

	ms.kind = kind->C();
	ms.oldSubject = oldSubject->subject;
	ms.newSubject = newSubject->subject;
	ms.oldCode = oldSubject->Code(oldSubject->saas, true);
	ms.newCode = newSubject->Code(newSubject->saas, true);

	msql m;
	unsigned flags;

	if (oldSubject->flagsX & SUBJECT_X_HIDDEN)
	{
		m.printf("Желаете ли да промените предмет %s", ms.oldCode);
		flags = MB_ASK;
	}
	else
	{
		m.printf("Предмет %s по %s не е отпаднал.\n\nЖелаете ли да го промените", ms.oldCode, Kind->Seek(ms.kind));
		flags = MB_WARN;
	}

	if (message(flags, "%s в делата и преписките на %s?", str(m), ms.newCode))
	{
		if (update->MigrateManual(this, &ms))
			message("Предмет %s е променен на %s.", ms.oldCode, ms.newCode);
		else
			error("Възникнаха грешки при промяната на предмет %s.", ms.oldCode);
	}
}
