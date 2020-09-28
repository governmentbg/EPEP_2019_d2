//{{TQueryConnectListBox Implementation}}

TQueryConnectListBox::TQueryConnectListBox(TWindow* parent, int resourceId, TConnect *tConnect)
:
	TGroupListBox(parent, resourceId, new TGArray<TConnect>), connect(tConnect)
{
}

TQueryConnectListBox::~TQueryConnectListBox()
{
}

void TQueryConnectListBox::Print()
{
	mstr m("Дела/документи по ");
	connect->Print(m);
	query_con(this, (TGArray<TConnect> *) array, str(m));
}

void TQueryConnectListBox::GetGroupText(mstr &m, TGroup *group, int)
{
	TDummyContainer container;
	container << group;
	m.printf("%-25s  %5ld/%4ld", Type->Seek(container.key.type), container.key.no, container.key.year);
}

TGroup *TQueryConnectListBox::NewGroup() const
{
	return new TConnect;
}

void TQueryConnectListBox::Criteria(msql &m, TGroup *)
{
	m.Begin(connect->Name);
	m.Add(connect, "F_CONNECT_NO, F_CONNECT_YEAR, F_UCN");

	if (connect->connectType == CONNECT_TYPE_JURISDICTION)
		m.AddChars("F_CONNECT_TYPE", CONNECT_TYPE_DUPLISDICTIONS);
	else
		m.AddChar("F_CONNECT_TYPE", connect->connectType);

	if (connect->connectKind)
		m.AddChar("F_CONNECT_KIND", connect->connectKind);
}
