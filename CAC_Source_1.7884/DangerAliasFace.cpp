//{{TDangerAliasFace Implementation}}

TDangerAliasFace::TDangerAliasFace(TWindow* parent, int resourceId, const char *name, long tSubject, long flags)
:
	TLongAliasFace(parent, resourceId, name, DangerSubject, NULL, flags), subject(tSubject)
{
}

void TDangerAliasFace::Criteria(msql &m, TGroup *group)
{
	DangerSubject->subject = subject;
	TLongAliasFace::Criteria(m, group);
}
