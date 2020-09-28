//{{TTranscriptComboBox Implementation}}

TTranscriptComboBox::TTranscriptComboBox(TWindow* parent, int resourceId, TPrintSubpoena *tPrintSub)
:
	TGroupComboBox(parent, resourceId, NULL, 0x100), printsub(tPrintSub), plus_req(NULL)
{
}

TRCDKeyContainer *TTranscriptComboBox::GetContainer()
{
	TRCDKeyContainer *container = (TRCDKeyContainer *) Group();
	TRCDKeyContainer *tContainer = NULL;

	if (container && container->key.no)
	{
		tContainer = kind2RCDKeyContainer(container->kind);
		*tContainer << container;
	}

	return tContainer;
}

TGroup *TTranscriptComboBox::NewGroup() const
{
	return new TSurround;
}

void TTranscriptComboBox::Criteria(msql &m, TGroup *group)
{
	char surroundKinds[SIZE_OF_KINDS];

	SubpoenaKind->Seek(printsub->subpoenaKind);
	strcpy(surroundKinds, SubpoenaKind->surroundKinds);

	if (any(surroundKinds))
	{
		plus_req = strchr(surroundKinds, '+');
		if (plus_req)
			strmove(plus_req, plus_req + 1);
	}

	if (any(surroundKinds))
	{
		m.Begin(group->Name);

		m.AddLong("F_LAWSUIT_NO", printsub->origin.no);
		m.AddLong("F_LAWSUIT_YEAR", printsub->origin.year);
		m.AddChar("F_LAWSUIT_TYPE", printsub->origin.type);
		m.AddChars("F_KIND", surroundKinds);
		m.AddOrder("F_DATE, F_NO");
	}
	else	// invalid, checked by LoadData()
		m.clear();
}

void TTranscriptComboBox::LoadData(const char *s)
{
	if (any(s))
		TGroupComboBox::LoadData(s);

	if (plus_req && printsub->requestKind)
	{
		TRequest *request = new TRequest;

		request->key = printsub->requestKey;
		request->kind = printsub->requestKind;
		request->date = printsub->requestDate;
		InsertGroup(request);
	}
}

void TTranscriptComboBox::Refresh()
{
	TGroupComboBox::Refresh();

	if (array->Count())
		SetVisualIndex(0);
}

void TTranscriptComboBox::GetGroupText(mstr &m, TGroup *group, int)
{
	print_sfor(m, TYPECHECK_PTR_CAST(const TRCDKeyContainer, group));
}
