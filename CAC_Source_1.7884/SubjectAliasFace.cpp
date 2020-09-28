//{{TSubjectAliasFace Implementation}}

TSubjectAliasFace::TSubjectAliasFace(TWindow* parent, int resourceId, const char *name, char kind, long tInclude)
	: TLongAliasFace(parent, resourceId, name, Subject, NULL, tInclude ? ALIAS_EMPTY : FLAG_ACTIVE | ALIAS_EMPTY),
		include(tInclude)
{
#if RANGECHECK
	if (!strchr(KIND_LAWSUITS, kind))
		fatal("TSubjectAliasFace: %d is not a law kind", kind);
#endif  // RANGECHECK

	xCrit = C2S[kind];
	qsMode = QUICK_SEARCH_ABLE;
}

void TSubjectAliasFace::SetKind(char kind)
{
#if RANGECHECK
	if (!strchr(KIND_LAWSUITS, kind))
		fatal("TSubjectAliasFace: %d is not a law kind", kind);
#endif  // RANGECHECK

	if (strcmp(C2S[kind], xCrit))
	{
		xCrit = C2S[kind];
		Refresh();
	}
}

bool TSubjectAliasFace::QsStart(const char *text)
{
	return isdigit(*text);
}

void TSubjectAliasFace::Criteria(msql &m, TGroup *group)
{
	long si = TSubject::SI(*xCrit);

	m.Begin(group->Name);
	m.AddChar(TSubject::FNI("F_KIND", si), *xCrit);

	if (flags & FLAG_ACTIVE)
	{
		m.Add(TSubject::FNI("F_FLAGS", si));
		Subject->DataBase->CondActive(m, TSubject::FNI("F_FLAGS", si));
	}

	TSubject::AddVisible(m);
}

void TSubjectAliasFace::LoadData(const char *s)
{
	bool tInclude = include != 0;
	TQuery q(s);

	while (q.Read())
	{
		*Subject << q;

		// rare, and inconvinient for Criteria
		if (strchr(Subject->Attrib(*xCrit), POST_LIMITED) && !can_func(RIGHT_LIMITED))
			continue;

		if (Subject->subject == include)
			tInclude = false;

		TGroup *group = NewGroup();
		*group << Subject;
		InsertGroup(group);
	}

	if (tInclude)
	{
		Subject->type = kind2type(*xCrit);
		Subject->subject = include;

		if (Subject->Try())
		{
			TGroup *group = NewGroup();
			*group << Subject;
			InsertGroup(group);
		}
	}

	if (InsertEmptyOption())
		InsertEmpty();
}

void TSubjectAliasFace::GetGroupText(mstr &m, TGroup *group, int)
{
	TSubject *subject = TYPECHECK_PTR_CAST(TSubject, group);

	if (subject->subject)
		m.printf("%s %s", TSubject::Code(subject->subject, *xCrit), subject->name);
}
