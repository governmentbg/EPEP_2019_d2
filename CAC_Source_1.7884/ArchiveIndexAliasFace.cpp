//{{TArchiveIndexAliasFace Implementation}}

TArchiveIndexAliasFace::TArchiveIndexAliasFace(TWindow* parent, int resourceId, const char *name, TExternContainer *tLawsuit,
	long flags)
:
	TStringAliasFace(parent, resourceId, name, ArchiveSubject, NULL, flags), lawsuit(tLawsuit)
{
}

void TArchiveIndexAliasFace::Criteria(msql &m, TGroup *group)
{
	ArchiveSubject->subject = lawsuit->subject;
	ArchiveSubject->kind = lawsuit->kind;
	TStringAliasFace::Criteria(m, group);
}
