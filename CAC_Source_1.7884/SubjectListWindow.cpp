//{{TSubjectListWindow Implementation}}

TSubjectListWindow::TSubjectListWindow(TWindow* parent, int resourceId)
:
	TGroupListWindow(parent, resourceId, new TGSArray<TSubject>, 264), kind('\0')
{
	Add(
		new TGroupListColumn("", subject_inactive, 12, LVCFMT_CENTER | LVCFMT_NO_SEARCH),
		new TGroupListColumn("Код", subject_code, 40, LVCFMT_RIGHT),
		new TGroupListColumn("Име", "F_NAME", 200),
		NULL
	);

	initialRefresh = false;
}

void TSubjectListWindow::SetKind(char tKind)
{
	if (kind != tKind)
	{
		kind = tKind;
		Refresh();
	}
}

void TSubjectListWindow::Copy()
{
	TSubject *subject = (TSubject *) NewGroup();
	*subject << Subject();

	if (edit_subject(this, subject, -1))
	{
		SetVisualIndex(InsertGroup(subject));
		UpdateVisual();
	}
	else
		delete subject;
}

void TSubjectListWindow::Print()
{
	print_subjects((TGSArray<TSubject> *) array, kind);
}

void TSubjectListWindow::PrintOld()
{
	print_old_subjects(kind);
}

TGroup *TSubjectListWindow::NewGroup() const
{
	TSubject *subject = new TSubject;

	subject->saas = TSubject::SI(kind);
	subject->SaasD().kind = kind;
	subject->type = kind2type(kind);		// 2007:200 required
	return subject;
}

void TSubjectListWindow::Criteria(msql &m, TGroup *group)
{
	m.Begin(group->Name);
	m.AddChar(TSubject::FNI("F_KIND", TSubject::SI(kind)), kind);
	TSubject::AddVisible(m);
}

void TSubjectListWindow::Refresh()
{
	TGroupListWindow::Refresh();
	long saas = TSubject::SI(kind);

	for (int index = 0; index < array->Count(); index++)
		((TSubject *) (*array)[index])->saas = saas;
}

bool TSubjectListWindow::AddGroup(TGroup *group)
{
	TSubject *subject = (TSubject *) group;
	subject->flagsX = SUBJECT_X_ADDED;
	return add_subject(this, subject);
}

bool TSubjectListWindow::EditGroup(TGroup *group)
{
	return edit_subject(this, (TSubject *) group, true);
}
