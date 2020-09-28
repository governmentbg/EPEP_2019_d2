//{{TPersonDistribAliasFace Implementation}}

TPersonDistribAliasFace::TPersonDistribAliasFace(TWindow* parent, int resourceId, const char *name, TPersonAliasGroup *alias, char tLawKind)
:
	TPersonAliasFace(parent, resourceId, name, alias, POST_VISIBLE_JUDGES, FLAG_ACTIVE | ALIAS_EMPTY), lawKind(tLawKind), persons(NULL)
{
}

TPersonDistribAliasFace::TPersonDistribAliasFace(TWindow* parent, int resourceId, const char *name, TPersonAliasGroup *alias, TGroupArray *tPersons, long flags)
:
	TPersonAliasFace(parent, resourceId, name, alias, NULL, flags), lawKind(KIND_NULL), persons(tPersons)
{
}

TPersonDistribAliasFace::~TPersonDistribAliasFace()
{
	delete alias;
}

void TPersonDistribAliasFace::SetFlags(long tFlags)
{
	if (flags != tFlags)
	{
		flags = tFlags;
		Refresh();
	}
}

void TPersonDistribAliasFace::LoadData(const char *s)
{
	if (lawKind)	// 2014:339 FIX: was TPersonAliasFace::LoadData(s)
		TGroupList::LoadData(s);
	else if (persons)
	{
		// ACTIVE must be pre-applied to persons
		for (int i = 0; i < persons->Count(); i++)
		{
			TPersonAliasGroup *person = (TPersonAliasGroup *) (*persons)[i];
			// 2010:274 LPR: pedagog support
			if (!(flags & JUROR_PEDAGOG) || (person->flags & JUROR_PEDAGOG))
			{
				TGroup *group = NewGroup();
				*group << person;
				InsertGroup(group);
			}
		}

		// 2010:292 LPR: required, copied from alias face
		// 2011:322 LPR: persons only, otherwise derived
		if (InsertEmptyOption())
			InsertEmpty();
	}
}

void TPersonDistribAliasFace::Criteria(msql &m, TGroup *group)
{
	TPersonAliasFace::Criteria(m, group);
	m.AddLike("F_LAW_KINDS", C2S[lawKind], true);
}
