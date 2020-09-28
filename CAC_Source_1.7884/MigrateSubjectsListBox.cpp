//{{TMigrateSubjectsListBox Implementation}}

TMigrateSubjectsListBox::TMigrateSubjectsListBox(TWindow* parent, int resourceId)
:
	TCloneListBox(parent, resourceId, new TGSArray<TOldSubject>, TOldSubject::Clone), kind(*KIND_LAWSUITS)
{
	type = kind2type(kind);
}

void TMigrateSubjectsListBox::SetKind(char tKind)
{
	if (tKind != kind)
	{
		kind = tKind;
		type = kind2type(kind);
		Refresh();
	}
}

TGroup *TMigrateSubjectsListBox::NewGroup() const
{
	TOldSubject *oldSubject = new TOldSubject;

	oldSubject->kind = kind;
	oldSubject->type = type;
	return oldSubject;
}

void TMigrateSubjectsListBox::InsertVisual(int index)
{
	TOldSubject *oldSubject = (TOldSubject *) GroupAt(index);

	// 2016:104 LPR: don't complain, see migration PASS 0
	if (!oldSubject->Try("F_NAME"))
		strcpy(oldSubject->name, "??");

	TCloneListBox::InsertVisual(index);
}

void TMigrateSubjectsListBox::Criteria(msql &m, TGroup *)
{
	msql w("T_REQUEST JOIN T_KIND ON T_KIND.F_KIND = T_REQUEST.F_KIND", "DISTINCT F_OLD_SUBJECT AS F_SUBJECT");

	w.AddIsNull("F_SUBJECT", true);
	w.AddIsNull("F_OLD_SUBJECT", false);
	w.AddChar("T_KIND.F_LAW_KIND", kind);

	m.Begin("T_LAWSUIT", "DISTINCT F_OLD_SUBJECT AS F_SUBJECT");
	m.AddChar("F_KIND", kind);
	m.AddIsNull("F_SUBJECT", true);
	m.printf(" UNION %s", str(w));
}

void TMigrateSubjectsListBox::GetGroupText(mstr &m, TGroup *group, int)
{
	TOldSubject *oldSubject = (TOldSubject *) group;
	m.printf("%5ld %s", oldSubject->subject, oldSubject->name);
}
