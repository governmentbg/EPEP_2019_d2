//{{TEisAliasFace Implementation}}

TEisAliasFace::TEisAliasFace(TWindow* parent, int resourceId, const char *name, long tNom, TXMLObject *object, long tFlags)
	: TLongAliasFace(parent, resourceId, name,
		(tNom == EISPP_CRIME ? (TEisAliasGroup *) EisCrime :
		tNom == EISPP_DEPART ? (TEisAliasGroup *) EisDepart :
		EISPP_DEPARTS.Find(tNom) != INT_MAX ? (TEisAliasGroup *) EisDepart :
		(TEisAliasGroup *) EisVal), NULL, tFlags | ALIAS_ENABLE | EISA_LIMIT), nom(tNom)
{
	if (tNom == nmk_dlosprvid)
	{
		delete array;
		array = new TGSArray<TEisVal>;
	}

	attrib = TYPECHECK_PTR_CAST(TXMLInt, object->FindAttrib(name));
	initialRefresh = attrib && (attrib->flags & XML_EDITABLE);

	if (initialRefresh)
		for (int i = 0; i < attrib->lset.Count(); i++)
			lset.Add(attrib->lset[i]);

	if (alias != EisVal)
		qsMode = QUICK_SEARCH_AUTO;

	if (!nom || !attrib || !(attrib->flags & XML_REQUIRED))
		flags |= ALIAS_EMPTY;
}

TEisAliasFace::~TEisAliasFace()
{
}

void TEisAliasFace::SetL(long l)
{
	if (!initialRefresh)
	{
	#if RANGECHECK
		if (!l && attrib && (attrib->flags & XML_REQUIRED))
			fatal("%s: неопределена стойност на задължително списъчно поле заключено за редактиране.", name);
	#endif  // RANGECHECK
		lset.Clear();
		lset.Add(l);
		Refresh();
	}

	TLongAliasFace::SetL(l);
}

void TEisAliasFace::Criteria(msql &m, TGroup *group)
{
	m.Begin(group->Name);

	if (nom > 0)
		m.AddLong("F_NOM", nom);

	m.AddLongs("F_CODE", lset);

	if (flags & FLAG_ACTIVE)
		m.AddLong("F_FLAGS", FLAG_ACTIVE);
}

void TEisAliasFace::LoadData(const char *s)
{
	TLongAlias *tNom = (TLongAlias *) alias->Find("F_NOM");

	if (tNom)
		((TLongAlias *) alias->Find("F_NOM"))->SetL(nom);

	((TEisAliasGroup *) alias)->lset = &lset;
	TAliasFace::LoadData(s);
}

void TEisAliasFace::GetGroupText(mstr &m, TGroup *group, int column)
{
	TLongAliasFace::GetGroupText(m, group, column);

	TEisAliasGroup *alias = TYPECHECK_PTR_CAST(TEisAliasGroup, group);

	if (alias->ValueField()->L() && !(alias->flags & FLAG_ACTIVE))
		m.cat(" (неакт)");
}

bool TEisAliasFace::IsValid()
{
	bool isValid = TLongAliasFace::IsValid();

	if (isValid)
	{
		if (attrib)
		{
			mstr m;

			if (!attrib->IsValid(m, L()))
				isValid = error("%s.", str(m));
		}
	}

	return isValid;
}

void TEisAliasFace::SetupWindow()
{
	TAliasFace::SetupWindow();

	if (!initialRefresh)
		autoEnable = false;

	if (!nom || !attrib)
		SetL(0);
}
